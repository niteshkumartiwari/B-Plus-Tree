#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;

class Node {
	/*
		Generally size of the this node should be equal to the block size. Which will limit the number of disk access and increase the accesssing time.
		Intermediate nodes only hold the Tree pointers which is of considerably small size(so they can hold more Tree pointers) and only Leaf nodes hold
		the data pointer directly to the disc.

		IMPORTANT := All the data has to be present in the leaf node
	*/
private:
	bool isLeaf;
	vector<int> keys;
	//Node* ptr2parent; //Pointer to go to parent node CANNOT USE check https://stackoverflow.com/questions/57831014/why-we-are-not-saving-the-parent-pointer-in-b-tree-for-easy-upward-traversal-in
	Node* ptr2next; //Pointer to connect next node for leaf nodes
	union ptr {//to make memory efficient Node
		vector<Node*> ptr2Tree; //Array of pointers to Children sub-trees for intermediate Nodes
		vector<FILE*> dataPtr; // Data-Pointer for the leaf node

		ptr();// To remove the error !?
		~ptr();// To remove the error !?
	}ptr2TreeOrData;

	friend class BPTree;// to access private members of the Node and hold the encapsulation concept
public:
	Node();
};


Node::ptr::ptr() {

}

Node::ptr::~ptr() {

}

Node::Node() {
	this->isLeaf = false;
	this->ptr2next = NULL;
}

class BPTree {
	/*
		::For Root Node :=
			The root node has, at least two tree pointers
		::For Internal Nodes:=
			1. ceil(maxIntChildLimit/2)     <=  #of children <= maxIntChildLimit
			2. ceil(maxIntChildLimit/2)-1  <=  #of keys     <= maxIntChildLimit -1
		::For Leaf Nodes :=
			1. ceil(maxLeafNodeLimit/2)   <=  #of keys     <= maxLeafNodeLimit -1
	*/
private:
	int maxIntChildLimit;//Limiting  #of children for internal Nodes!
	int maxLeafNodeLimit;// Limiting #of nodes for leaf Nodes!!!
	Node* root; //Pointer to the B+ Tree root
	void insertInternal(int x, Node* cursor, Node* child); //Insert x from child in cursor(parent)
	Node* findParent(Node* cursor, Node* child);

public:
	BPTree();
	BPTree(int degreeInternal, int degreeLeaf);
	Node* getRoot();
	void display(Node* cursor);
	void search(int key);
	void insert(int key, FILE* filePtr);
	//void remove(int key);
};

BPTree::BPTree() {
	/*
		By Default it will take the maxIntChildLimit as 4. And
		maxLeafNodeLimit as 3.

		::REASON FOR TWO SEPERATE VARIABLES maxIntChildLimit & maxLeafNodeLimit !!
		We are keeping the two seperate Orders
		because Internal Nodes can hold more values in one disc block
		as the size of the Tree pointer is small but the size of the
		data pointer in the leaf nodes is large so we can only put less
		nodes in the leafs as compared to the internal Nodes. Thats the
		reson to reperate out these to variables.

	*/
	this->maxIntChildLimit = 4;
	this->maxLeafNodeLimit = 3;
	this->root = NULL;
}

BPTree::BPTree(int degreeInternal, int degreeLeaf) {

	this->maxIntChildLimit = degreeInternal;
	this->maxLeafNodeLimit = degreeLeaf;
	this->root = NULL;
}

Node* BPTree::getRoot() {
	return this->root;
}

void BPTree::display(Node* cursor) {
	/*
		Depth First Display
	*/
	if (cursor != NULL) {
		for (int i = 0; i < cursor->keys.size(); i++)
			cout << cursor->keys[i] << " ";
		cout << endl;
		if (cursor->isLeaf != true) {
			for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++)
				display(cursor->ptr2TreeOrData.ptr2Tree[i]);
		}
	}
}

void BPTree::search(int key) {
	if (root == NULL) {
		cout << "NO Tuples Inserted yet" << endl;
		return;
	}
	else {
		Node* cursor = root;
		while (cursor->isLeaf == false) {
			/*
				upper_bound returns an iterator pointing to the first element in the range
				[first,last) which has a value greater than �val�.(Because we are storing the
				same value in the right node;(STL is doing Binary search at back end)
			*/
			int idx = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
			cursor = cursor->ptr2TreeOrData.ptr2Tree[idx]; //upper_bound takes care of all the edge cases
		}

		int idx = std::lower_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin(); //Binary search

		if (idx == cursor->keys.size() || cursor->keys[idx] != key) {
			cout << "HUH!! Key NOT FOUND" << endl;
			return;
		}

		/*
			We can fetch the data from the disc in main memory using data-ptr
			using cursor->dataPtr[idx]
		*/

		string fileName = "DBFiles/";
		string data;
		fileName += to_string(key) + ".txt";
		FILE* filePtr = fopen(fileName.c_str(), "r");
		cout << "Hurray!! Key FOUND" << endl;
		cout << "Corresponding Tuple Data is: ";
		char ch = fgetc(filePtr);
		while (ch != EOF)
		{
			printf("%c", ch);
			ch = fgetc(filePtr);
		}
		fclose(filePtr);
		cout << endl;
	}
}

void BPTree::insert(int key, FILE * filePtr) {//in Leaf Node
	/*
		1. If the node has an empty space, insert the key/reference pair into the node.
		2. If the node is already full, split it into two nodes, distributing the keys
		evenly between the two nodes. If the node is a leaf, take a copy of the minimum
		value in the second of these two nodes and repeat this insertion algorithm to
		insert it into the parent node. If the node is a non-leaf, exclude the middle
		value during the split and repeat this insertion algorithm to insert this excluded
		value into the parent node.
	*/

	if (root == NULL) {
		root = new Node;
		root->isLeaf = true;
		root->keys.push_back(key);
		root->ptr2TreeOrData.dataPtr.push_back(filePtr);

		cout << key << ": I AM ROOT!!" << endl;
		return;
	}
	else {
		Node* cursor = root;
		Node* parent=NULL;
		//searching for the possible position for the given key by doing the same procedure we did in search
		while (cursor->isLeaf == false) {
			parent = cursor;
			int idx = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
			cursor = cursor->ptr2TreeOrData.ptr2Tree[idx];
		}

		//now cursor is the leaf node in which we'll insert the new key
		if (cursor->keys.size() < maxLeafNodeLimit) {
			/*
				If current leaf Node is not FULL, find the correct position for the new key and insert!
			*/
			int i = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
			cursor->keys.push_back(key);
			cursor->ptr2TreeOrData.dataPtr.push_back(filePtr);

			if (i != cursor->keys.size() - 1) {
				for (int j = cursor->keys.size() - 1; j > i; j--) {// shifting the position for keys and datapointer
					cursor->keys[j] = cursor->keys[j - 1];
					cursor->ptr2TreeOrData.dataPtr[j] = cursor->ptr2TreeOrData.dataPtr[j - 1];
				}

				//since earlier step was just to inc. the size of vectors and making space, now we are simplying inserting 
				cursor->keys[i] = key;
				cursor->ptr2TreeOrData.dataPtr[i] = filePtr;
			}
			cout << "Inserted successfully: " << key << endl;
		}
		else {
			/*
				DAMN!! Node Overflowed :(
				HAIYYA! Splitting the Node .
			*/
			vector<int> virtualNode(cursor->keys);
			vector<FILE*> virtualDataNode(cursor->ptr2TreeOrData.dataPtr);

			//finding the probable place to insert the key
			int i = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();

			virtualNode.push_back(key);// to create space
			virtualDataNode.push_back(filePtr);// to create space

			if (i != virtualNode.size() - 1) {
				for (int j = virtualNode.size() - 1; j > i; j--) {// shifting the position for keys and datapointer
					virtualNode[j] = virtualNode[j - 1];
					virtualDataNode[j] = virtualDataNode[j - 1];
				}

				//inserting
				virtualNode[i] = key;
				virtualDataNode[i] = filePtr;
			}
			/*
				BAZINGA! I have the power to create new Leaf :)
			*/

			Node* newLeaf = new Node;
			newLeaf->isLeaf = true;

			//swapping the next ptr
			Node* temp = cursor->ptr2next;
			cursor->ptr2next = newLeaf;
			newLeaf->ptr2next = temp;

			//resizing and copying the keys & dataPtr to OldNode
			cursor->keys.resize((maxLeafNodeLimit) / 2);
			cursor->ptr2TreeOrData.dataPtr.resize((maxLeafNodeLimit) / 2);
			for (int i = 0; i < (maxLeafNodeLimit) / 2; i++) {
				cursor->keys[i] = virtualNode[i];
				cursor->ptr2TreeOrData.dataPtr[i] = virtualDataNode[i];
			}

			//Pushing new keys & dataPtr to NewNode
			for (int i = (maxLeafNodeLimit) / 2; i < virtualNode.size(); i++)
			{
				newLeaf->keys.push_back(virtualNode[i]);
				newLeaf->ptr2TreeOrData.dataPtr.push_back(virtualDataNode[i]);
			}

			if (cursor == root) {
				/*
					If cursor is root node we create new node
				*/

				Node* newRoot = new Node;
				newRoot->keys.push_back(newLeaf->keys[0]);
				newRoot->ptr2TreeOrData.ptr2Tree.push_back(cursor);
				newRoot->ptr2TreeOrData.ptr2Tree.push_back(newLeaf);
				root = newRoot;
				cout << "Created new Root!" << endl;
			}
			else {
				// Insert new key in the parent
				insertInternal(newLeaf->keys[0], parent, newLeaf);
			}
		}
	}

}

void BPTree::insertInternal(int x, Node * cursor, Node * child) {//in Internal Nodes
	if (cursor->keys.size() < maxIntChildLimit - 1) {
		/*
			If cursor is not full find the position for the position for the new key.
		*/
		int i = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), x) - cursor->keys.begin();
		cursor->keys.push_back(x);
		cursor->ptr2TreeOrData.ptr2Tree.push_back(child);

		if (i != cursor->keys.size() - 1) {// if there are more than one element
			// Different loops because size is different for both (i.e. diff of one)

			for (int j = cursor->keys.size() - 1; j > i; j--) {// shifting the position for keys and datapointer
				cursor->keys[j] = cursor->keys[j - 1];
			}

			for (int j = cursor->ptr2TreeOrData.ptr2Tree.size() - 1; j > (i + 1); j--) {
				cursor->ptr2TreeOrData.ptr2Tree[j] = cursor->ptr2TreeOrData.ptr2Tree[j - 1];
			}

			cursor->keys[i] = x;
			cursor->ptr2TreeOrData.ptr2Tree[i + 1] = child;
		}
		cout << "Inserted key in the internal node :)" << endl;
	}
	else {//splitting
		cout << "Inserted Node in internal node successful" << endl;
		cout << "Overflow in internal:( HAIYAA! splitting internal nodes" << endl;

		vector<int> virtualKeyNode(cursor->keys);
		vector<Node*> virtualTreePtrNode(cursor->ptr2TreeOrData.ptr2Tree);


		int i = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), x) - cursor->keys.begin();//finding the position for x
		virtualKeyNode.push_back(x);// to create space
		virtualTreePtrNode.push_back(child);// to create space

		if (i != virtualKeyNode.size() - 1) {
			for (int j = virtualKeyNode.size() - 1; j > i; j--) {// shifting the position for keys and datapointer
				virtualKeyNode[j] = virtualKeyNode[j - 1];
			}

			for (int j = virtualTreePtrNode.size() - 1; j > (i + 1); j--) {
				virtualTreePtrNode[j] = virtualTreePtrNode[j - 1];
			}

			virtualKeyNode[i] = x;
			virtualTreePtrNode[i + 1] = child;
		}

		int partitionKey;//exclude middle element while splitting
		partitionKey = virtualKeyNode[(virtualKeyNode.size() / 2)];//right biased
		int partitionIdx = (virtualKeyNode.size() / 2);

		//resizing and copying the keys & TreePtr to OldNode
		cursor->keys.resize(partitionIdx);
		cursor->ptr2TreeOrData.ptr2Tree.resize(partitionIdx + 1);
		for (int i = 0; i < partitionIdx; i++) {
			cursor->keys[i] = virtualKeyNode[i];
		}

		for (int i = 0; i < partitionIdx + 1; i++) {
			cursor->ptr2TreeOrData.ptr2Tree[i] = virtualTreePtrNode[i];
		}

		Node* newInternalNode = new Node;
		//Pushing new keys & TreePtr to NewNode

		for (int i = partitionIdx + 1; i < virtualKeyNode.size(); i++) {
			newInternalNode->keys.push_back(virtualKeyNode[i]);
		}

		for (int i = partitionIdx + 1; i < virtualTreePtrNode.size(); i++) {// because only key is excluded not the pointer
			newInternalNode->ptr2TreeOrData.ptr2Tree.push_back(virtualTreePtrNode[i]);
		}

		if (cursor == root) {
			/*
				If cursor is a root we create a new Node
			*/
			Node* newRoot = new Node;
			newRoot->keys.push_back(partitionKey);
			newRoot->ptr2TreeOrData.ptr2Tree.push_back(cursor);
			newRoot->ptr2TreeOrData.ptr2Tree.push_back(newInternalNode);

			root = newRoot;
			cout << "Created new ROOT!" << endl;
		}
		else {
			/*
				::Recursion::
			*/
			insertInternal(partitionKey, findParent(root, cursor), newInternalNode);
		}
	}
}

Node* BPTree::findParent(Node* cursor, Node* child) {
	/*
		Finds parent using depth first traversal and ignores leaf nodes as they cannot be parents
		also ignores second last level because we will never find parent of a leaf node during insertion using this function
	*/

	Node* parent=NULL;

	if (cursor->isLeaf || cursor->ptr2TreeOrData.ptr2Tree[0]->isLeaf)
		return NULL;

	for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++) {
		if (cursor->ptr2TreeOrData.ptr2Tree[i] == child) {
			parent = cursor;
			return parent;
		}
		else {
			parent = findParent(cursor->ptr2TreeOrData.ptr2Tree[i], child);
		}
	}

	return parent;
}

void insertionMethod(BPTree * *bPTree) {
	int rollNo, age, marks;
	string name;

	cout << "Please provide the rollNo: ";
	cin >> rollNo;
	cout << "\nWhat's the Name, Age and Marks acquired?: ";
	cin >> name >> age >> marks;

	string fileName = "DBFiles/";
	fileName += to_string(rollNo) + ".txt";
	FILE* filePtr = fopen(fileName.c_str(), "w");
	string userTuple = name + " " + to_string(age) + " " + to_string(marks) + "\n";
	fprintf(filePtr, userTuple.c_str());
	//fclose(filePtr);

	(*bPTree)->insert(rollNo, filePtr);
	fclose(filePtr);
}

void searchMethod(BPTree * bPTree) {
	int rollNo;
	cout << "What's the RollNo to Search? ";
	cin >> rollNo;

	bPTree->search(rollNo);
}

void printMethod(BPTree * bPTree) {
	cout << "Here is your File Structure" << endl;
	bPTree->display(bPTree->getRoot());
}

int main() {
	/*
		Please have a look at the default schema to get to know about the table
		Reference - img/database.jpg
	*/

	cout << "\n***Welcome to DATABASE SERVER**\n" << endl;

	bool flag = true;
	int option;

	int maxChildInt, maxNodeLeaf;
	cout << "Please provide the value to limit maximum child Internal Nodes can have: ";
	cin >> maxChildInt;
	cout << "\nAnd Now Limit the value to limit maximum Nodes Leaf Nodes can have: ";
	cin >> maxNodeLeaf;

	BPTree* bPTree = new BPTree(maxChildInt, maxNodeLeaf);

	do {
		cout << "\nPlease provide the queries with respective keys : " << endl;
		cout << "\tPress 1: Insertion \n\tPress 2: Search \n\tPress 3: Print Tree\n\tPress 4: ABORT!" << endl;
		cin >> option;

		switch (option) {
		case 1:
			insertionMethod(&bPTree);
			break;
		case 2:
			searchMethod(bPTree);
			break;
		case 3:
			printMethod(bPTree);
			break;
		default:
			flag = false;
			break;
		}

	} while (flag);

	return 0;
}
