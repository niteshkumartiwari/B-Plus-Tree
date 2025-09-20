#include <iostream>
#include "bptree/bptree.hpp"

using namespace std;
using namespace bptree;

Node* parent = NULL;

Node::ptr::ptr() {
}

Node::ptr::~ptr() {
}

Node::Node() {
    this->isLeaf = false;
    this->ptr2next = NULL;
}

Node::~Node() {
    // Clean up file pointers if this is a leaf node
    if (isLeaf) {
        for (size_t i = 0; i < ptr2TreeOrData.dataPtr.size(); i++) {
            if (ptr2TreeOrData.dataPtr[i] != NULL) {
                fclose(ptr2TreeOrData.dataPtr[i]);
                ptr2TreeOrData.dataPtr[i] = NULL;
            }
        }
        ptr2TreeOrData.dataPtr.~vector<FILE*>();
    } else {
        // Clean up child pointers for internal nodes
        ptr2TreeOrData.ptr2Tree.~vector<Node*>();
    }
}

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

BPTree::~BPTree() {
    destroyTree(root);
}

void BPTree::destroyTree(Node* node) {
    if (node == NULL) return;
    
    if (!node->isLeaf) {
        // Recursively delete all children
        for (size_t i = 0; i < node->ptr2TreeOrData.ptr2Tree.size(); i++) {
            destroyTree(node->ptr2TreeOrData.ptr2Tree[i]);
        }
    }
    
    delete node;
}

int BPTree::getMaxIntChildLimit() {
    return maxIntChildLimit;
}

int BPTree::getMaxLeafNodeLimit() {
    return maxLeafNodeLimit;
}

Node* BPTree::getRoot() {
    return this->root;
}

void BPTree::setRoot(Node *ptr) {
    this->root = ptr;
}

Node* BPTree::firstLeftNode(Node* cursor) {
    if (cursor->isLeaf)
        return cursor;
    for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++)
        if (cursor->ptr2TreeOrData.ptr2Tree[i] != NULL)
            return firstLeftNode(cursor->ptr2TreeOrData.ptr2Tree[i]);

    return NULL;
}

Node** BPTree::findParent(Node* cursor, Node* child) {
    /*
		Finds parent using depth first traversal and ignores leaf nodes as they cannot be parents
		also ignores second last level because we will never find parent of a leaf node during insertion using this function
	*/

    // Safety checks
    if (cursor == NULL || child == NULL) {
        return NULL;
    }

    if (cursor->isLeaf) {
        return NULL;
    }

    // Check if cursor's children are empty or if first child is leaf
    if (cursor->ptr2TreeOrData.ptr2Tree.empty()) {
        return NULL;
    }

    if (cursor->ptr2TreeOrData.ptr2Tree[0] == NULL || cursor->ptr2TreeOrData.ptr2Tree[0]->isLeaf) {
        return NULL;
    }

    // Check direct children first
    for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++) {
        if (cursor->ptr2TreeOrData.ptr2Tree[i] == child) {
            parent = cursor;
            return &parent;
        }
    }

    // Recursively search in children
    for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++) {
        Node* tmpCursor = cursor->ptr2TreeOrData.ptr2Tree[i];
        if (tmpCursor != NULL && !tmpCursor->isLeaf) {
            Node** result = findParent(tmpCursor, child);
            if (result != NULL) {
                return result;
            }
        }
    }

    return NULL;
}