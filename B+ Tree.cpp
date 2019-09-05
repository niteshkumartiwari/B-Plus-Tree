#include <iostream>
#include <vector>
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
	Node* ptr2parent; //Pointer to go to parent node
	Node* ptr2next; //Pointer to connect next node for leaf nodes
	union ptr {//to make memory efficient Node
		vector<Node*> ptr2Tree; //Array of pointers to Children sub-trees for intermediate Nodes
		vector<FILE*> dataPtr; // Data-Pointer for the leaf node
	};

	friend class BPTree;// to access private members of the Node and hold the encapsulation concept
public:
	Node();
};

Node::Node() {
	this->isLeaf = false;
	this->ptr2parent = NULL;
	this->ptr2next = NULL;
}

class BPTree {
	/*
		::For Root Node :=
			The root node has, at least two tree pointers
		::For Internal Nodes:=
			1. ceil(minInternalNodes/2)     <=  #of children <= minInternalNodes
			2. ceil(minInternalNodes/2)-1  <=  #of keys     <= minInternalNodes -1
		::For Leaf Nodes :=
			1. ceil(minLeafNodes/2)-1   <=  #of keys     <= minLeafNodes -1
	*/
private:
	int minInternalNodes, minLeafNodes;
	Node* root; //Pointer to the B+ Tree root

public:
	BPTree();
	BPTree(int degreeInternal, int degreeLeaf);
	Node* getRoot();
	void search(int key);
	void insert(int key, FILE *filePtr);
	void remove(int key);
};

BPTree::BPTree() {
	/*
		By Default it will take the minInternalNodes as 4. And
		minLeafNodes as 3.

		::REASON FOR TWO SEPERATE VARIABLES minInternalNodes & minLeafNodes !!
		We are keeping the two seperate Orders
		because Internal Nodes can hold more values in one disc block
		as the size of the Tree pointer is small but the size of the
		data pointer in the leaf nodes is large so we can only put less
		nodes in the leafs as compared to the internal Nodes. Thats the
		reson to reperate out these to variables.

	*/
	this->minInternalNodes = 4;
	this->minLeafNodes = 3;
	this->root = NULL;
}

BPTree::BPTree(int degreeInternal, int degreeLeaf) {
	this->minInternalNodes = degreeInternal;
	this->minLeafNodes = degreeLeaf;
	this->root = NULL;
}

Node* BPTree::getRoot() {
	return this->root;
}

void BPTree::search(int key) {
	if (root == NULL) {
		cout << "NO Table Inserted yet" << endl;
		return;
	}
	else {
		Node* cursor = root;
		while (cursor->isLeaf == false) {
			/*
				upper_bound returns an iterator pointing to the first element in the range 
				[first,last) which has a value greater than ‘val’.(Because we are storing the
				same value in the right node;(STL is doing Binary search at back end)
			*/
			int idx = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
			
			if (idx == cursor->keys.size())
				cursor = cursor->ptr2Tree[idx + 1];
			else
				cursor = cursor->ptr2Tree[idx];
			
		}
		
		int idx = std::lower_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
		
		if (idx == cursor->keys.size() || cursor->keys[i] != key) {
			cout << "HUH!! Key NOT FOUND" << endl;
			return;
		}

		/*
			We can fetch the data from the disc in main memory using data-ptr
			using cursor->dataPtr
		*/
		cout << "Hurray!! Key FOUND" << endl;
	}
}

void BPTree::insert(int key, FILE* filePtr) {
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
		root->isLeaf[] = true;
		root->keys[0] = key;
		root->dataPtr[0] = filePtr;

		cout << key << ": I AM ROOT!!" << endl;
		return;
	}
	else {
		Node* cursor = root;

		//searching for the possible position for the given key by doing the same procedure we did in search
		while (cursor->isLeaf == false) {
			for (int i = 0; i < cursor->keys.size(); i++) {
				if (cursor->keys[i] > key) {
					cursor = cursor->ptr2Tree[i];
					break;
				}

				if (i == cursor->keys.size() - 1) {
					cursor = cursor->ptr2Tree[i+1];
					break;
				}
			}
		}
	}

}

int main() {

	return 0;
}

