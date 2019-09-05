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
		FILE* dataPtr; // Data-Pointer for the leaf node
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
		::For Internal Nodes:=
			1. ceil(minInternalNodes/2)     <=  #of children <= minInternalNodes
			2. ceil(minInternalNodes/2) -1  <=  #of keys     <= minInternalNodes -1
		::For Leaf Nodes :=
			1. ceil(minLeafNodes/2) -1  <=  #of keys     <= minLeafNodes -1
	*/
private:
	int minInternalNodes, minLeafNodes;
	Node* root; //Pointer to the B+ Tree root

public:
	BPTree();
	BPTree(int degreeInternal, int degreeLeaf);
	Node* getRoot();
	void search(int key);
	void insert(int key);
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

	}
}

int main() {

	return 0;
}

