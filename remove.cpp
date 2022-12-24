#include <iostream>
#include <cstring>
#include "B+ Tree.h"

void BPTree::removeKey(int x) {
	Node* root = getRoot();

	// If tree is empty
	if (root == NULL) {
		cout << "B+ Tree is Empty" << endl;
		return;
	}

	Node* cursor = root;
	Node* parent;
	int leftSibling, rightSibling;

	// Going to the Leaf Node, Which may contain the *key*
	// TO-DO : Use Binary Search to find the val
	while (cursor->isLeaf != true) {
		for (int i = 0; i < cursor->keys.size(); i++) {
			parent = cursor;
			leftSibling = i - 1;//left side of the parent node
			rightSibling = i + 1;// right side of the parent node

			if (x < cursor->keys[i]) {
				cursor = cursor->ptr2TreeOrData.ptr2Tree[i];
				break;
			}
			if (i == cursor->keys.size() - 1) {
				leftSibling = i;
				rightSibling = i + 2;// CHECK here , might need to make it negative
				cursor = cursor->ptr2TreeOrData.ptr2Tree[i+1];
				break;
			}
		}
	}

	// Check if the value exists in this leaf node
	int pos = 0;
	bool found = false;
	for (pos = 0; pos < cursor->keys.size(); pos++) {
		if (cursor->keys[pos] == x) {
			found = true;
			break;
		}
	}

	auto itr = lower_bound(cursor->keys.begin(), cursor->keys.end(), x);

	if (found == false) {
		cout << "Key Not Found in the Tree" << endl;
		return;
	}
	
	// Delete the respective File and FILE*
	string fileName = "DBFiles/" + to_string(x) + ".txt";
	char filePtr[256];
	strcpy(filePtr, fileName.c_str());

	//delete cursor->ptr2TreeOrData.dataPtr[pos];//avoid memory leaks
	if (remove(filePtr) == 0)
		cout << "SuccessFully Deleted file: " << fileName << endl;
	else
		cout << "Unable to delete the file: " << fileName << endl;

	// Shifting the keys and dataPtr for the leaf Node
	for (int i = pos; i < cursor->keys.size()-1; i++) {
		cursor->keys[i] = cursor->keys[i+1];
		cursor->ptr2TreeOrData.dataPtr[i] = cursor->ptr2TreeOrData.dataPtr[i + 1];
	}
	int prev_size = cursor->keys.size();
	cursor->keys.resize(prev_size - 1);
	cursor->ptr2TreeOrData.dataPtr.resize(prev_size - 1);

	// If it is leaf as well as the root node
	if (cursor == root) {
		if (cursor->keys.size() == 0) {
			// Tree becomes empty
			setRoot(NULL);
			cout << "Ohh!! Our Tree is Empty Now :(" << endl;
		}
	}
	
	cout << "Deleted " << x << " From Leaf Node successfully" << endl;
	if ((cursor->keys.size() >= (getMaxLeafNodeLimit() + 1) / 2) || (cursor == root)) {
		//Sufficient Node available for invariant to hold
		return;
	}

	cout << "UnderFlow in the leaf Node Happended" << endl;
	cout << "Starting Redistribution..." << endl;

	//1. Try to borrow a key from leftSibling
	if (leftSibling >= 0) {
		Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];

		//Check if LeftSibling has extra Key to transfer
		if (leftNode->keys.size() >= (getMaxLeafNodeLimit()+1) / 2 +1) {

			//Transfer the maximum key from the left Sibling
			int maxIdx = leftNode->keys.size()-1;
			cursor->keys.insert(cursor->keys.begin(), leftNode->keys[maxIdx]);
			cursor->ptr2TreeOrData.dataPtr
				.insert(cursor->ptr2TreeOrData.dataPtr.begin(), leftNode->ptr2TreeOrData.dataPtr[maxIdx]);

			//resize the left Sibling Node After Tranfer
			leftNode->keys.resize(maxIdx);
			leftNode->ptr2TreeOrData.dataPtr.resize(maxIdx);

			//Update Parent
			parent->keys[leftSibling] = cursor->keys[0];
			printf("Transferred from left sibling of leaf node\n");
			return;
		}
	}

	//2. Try to borrow a key from rightSibling
	if (rightSibling < parent->ptr2TreeOrData.ptr2Tree.size()) {
		Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];

		//Check if RightSibling has extra Key to transfer
		if (rightNode->keys.size() >= (getMaxLeafNodeLimit() + 1) / 2 + 1) {

			//Transfer the minimum key from the right Sibling
			int minIdx = 0;
			cursor->keys.push_back(rightNode->keys[minIdx]);
			cursor->ptr2TreeOrData.dataPtr
				.push_back(rightNode->ptr2TreeOrData.dataPtr[minIdx]);

			//resize the right Sibling Node After Tranfer
			rightNode->keys.erase(rightNode->keys.begin());
			rightNode->ptr2TreeOrData.dataPtr.erase(rightNode->ptr2TreeOrData.dataPtr.begin());

			//Update Parent
			parent->keys[rightSibling-1] = rightNode->keys[0];
			printf("Transferred from right sibling of leaf node\n");
			return;
		}
	}

	// Merge and Delete Node
	if (leftSibling >= 0) {// If left sibling exists
		Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];
		//Transfer Key and dataPtr to leftSibling and connect ptr2next
		for (int i = 0; i < cursor->keys.size(); i++) {
			leftNode->keys.push_back(cursor->keys[i]);
			leftNode->ptr2TreeOrData.dataPtr
				.push_back(cursor->ptr2TreeOrData.dataPtr[i]);
		}
		leftNode->ptr2next = cursor->ptr2next;
		cout << "Merging two leaf Nodes" << endl;
		removeInternal(parent->keys[leftSibling], parent, cursor);//delete parent Node Key
		//delete cursor;
	}
	else if (rightSibling <= parent->keys.size()) {
		Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];
		//Transfer Key and dataPtr to rightSibling and connect ptr2next
		for (int i = 0; i < rightNode->keys.size(); i++) {
			cursor->keys.push_back(rightNode->keys[i]);
			cursor->ptr2TreeOrData.dataPtr
				.push_back(rightNode->ptr2TreeOrData.dataPtr[i]);
		}
		cursor->ptr2next = rightNode->ptr2next;
		cout << "Merging two leaf Nodes" << endl;
		removeInternal(parent->keys[rightSibling-1], parent, rightNode);//delete parent Node Key
		//delete rightNode;
	}

}

void BPTree::removeInternal(int x, Node* cursor, Node* child) {
	Node* root = getRoot();

	// Check if key from root is to deleted
	if (cursor == root) {
		if (cursor->keys.size() == 1) {
			// If only one key is left and matches with one of the
			// child Pointers
			if (cursor->ptr2TreeOrData.ptr2Tree[1] == child) {
				setRoot(cursor->ptr2TreeOrData.ptr2Tree[0]);
				//delete cursor;
				cout << "Wow! New Changed Root" <<endl;
				return;
			}
			else if (cursor->ptr2TreeOrData.ptr2Tree[0] == child) {
				setRoot(cursor->ptr2TreeOrData.ptr2Tree[1]);
				//delete cursor;
				cout << "Wow! New Changed Root" << endl;
				return;
			}
		}
	}

	// Deleting key x from the parent
	int pos;
	for (pos = 0; pos < cursor->keys.size(); pos++) {
		if (cursor->keys[pos] == x) {
			break;
		}
	}
	for (int i = pos; i < cursor->keys.size()-1; i++) {
		cursor->keys[i] = cursor->keys[i + 1];
	}
	cursor->keys.resize(cursor->keys.size() - 1);

	// Now deleting the ptr2tree
	for (pos = 0; pos < cursor->ptr2TreeOrData.ptr2Tree.size(); pos++) {
		if (cursor->ptr2TreeOrData.ptr2Tree[pos] == child) {
			break;
		}
	}

	for (int i = pos; i < cursor->ptr2TreeOrData.ptr2Tree.size() - 1; i++) {
		cursor->ptr2TreeOrData.ptr2Tree[i] = cursor->ptr2TreeOrData.ptr2Tree[i + 1];
	}
	cursor->ptr2TreeOrData.ptr2Tree
		.resize(cursor->ptr2TreeOrData.ptr2Tree.size()-1);

	// If there is No underflow. Phew!!
	if (cursor->keys.size() >= (getMaxIntChildLimit() + 1) / 2 - 1) {
		cout << "Deleted " << x << " from internal node successfully\n";
		return;
	}

	cout << "UnderFlow in internal Node! What did you do :/" << endl;

	if (cursor == root) {
		return;
	}

	Node** p1 = findParent(root, cursor);
	Node* parent = *p1;

	int leftSibling, rightSibling;

	// Finding Left and Right Siblings as we did earlier
	for (pos = 0; pos < parent->ptr2TreeOrData.ptr2Tree.size(); pos++) {
		if (parent->ptr2TreeOrData.ptr2Tree[pos] == cursor) {
			leftSibling = pos - 1;
			rightSibling = pos + 1;
			break;
		}
	}

	// If possible transfer to leftSibling
	if (leftSibling >= 0) {
		Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];

		//Check if LeftSibling has extra Key to transfer
		if (leftNode->keys.size() >= (getMaxIntChildLimit() + 1) / 2 ) {

			//transfer key from left sibling through parent
			int maxIdxKey = leftNode->keys.size() - 1;
			cursor->keys.insert(cursor->keys.begin(), parent->keys[leftSibling]);
			parent->keys[leftSibling] = leftNode->keys[maxIdxKey];

			int maxIdxPtr = leftNode->ptr2TreeOrData.ptr2Tree.size()-1;
			cursor->ptr2TreeOrData.ptr2Tree
				.insert(cursor->ptr2TreeOrData.ptr2Tree.begin(), leftNode->ptr2TreeOrData.ptr2Tree[maxIdxPtr]);

			//resize the left Sibling Node After Tranfer
			leftNode->keys.resize(maxIdxKey);
			leftNode->ptr2TreeOrData.dataPtr.resize(maxIdxPtr);

			return;
		}
	}

	// If possible transfer to rightSibling
	if (rightSibling < parent->ptr2TreeOrData.ptr2Tree.size()) {
		Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];

		//Check if LeftSibling has extra Key to transfer
		if (rightNode->keys.size() >= (getMaxIntChildLimit() + 1) / 2) {

			//transfer key from right sibling through parent
			int maxIdxKey = rightNode->keys.size() - 1;
			cursor->keys.push_back(parent->keys[pos]);
			parent->keys[pos] = rightNode->keys[0];
			rightNode->keys.erase(rightNode->keys.begin());

			//transfer the pointer from rightSibling to cursor
			cursor->ptr2TreeOrData.ptr2Tree
				.push_back(rightNode->ptr2TreeOrData.ptr2Tree[0]);
			cursor->ptr2TreeOrData.ptr2Tree
				.erase(cursor->ptr2TreeOrData.ptr2Tree.begin());
			 
			return;
		}
	}

	//Start to Merge Now, if None of the above cases applied
	if (leftSibling >= 0) {
		//leftNode + parent key + cursor
		Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];
		leftNode->keys.push_back(parent->keys[leftSibling]);

		for (int val : cursor->keys) {
			leftNode->keys.push_back(val);
		}

		for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++) {
			leftNode->ptr2TreeOrData.ptr2Tree
				.push_back(cursor->ptr2TreeOrData.ptr2Tree[i]);
			cursor->ptr2TreeOrData.ptr2Tree[i] = NULL;
		}

		cursor->ptr2TreeOrData.ptr2Tree.resize(0);
		cursor->keys.resize(0);

		removeInternal(parent->keys[leftSibling], parent, cursor);
		cout << "Merged with left sibling"<<endl;
	}
	else if (rightSibling < parent->ptr2TreeOrData.ptr2Tree.size()) {
		//cursor + parentkey +rightNode
		Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];
		cursor->keys.push_back(parent->keys[rightSibling - 1]);

		for (int val : rightNode->keys) {
			cursor->keys.push_back(val);
		}

		for (int i = 0; i < rightNode->ptr2TreeOrData.ptr2Tree.size(); i++) {
			cursor->ptr2TreeOrData.ptr2Tree
				.push_back(rightNode->ptr2TreeOrData.ptr2Tree[i]);
			rightNode->ptr2TreeOrData.ptr2Tree[i] = NULL;
		}

		rightNode->ptr2TreeOrData.ptr2Tree.resize(0);
		rightNode->keys.resize(0);

		removeInternal(parent->keys[rightSibling - 1], parent, rightNode);
		cout << "Merged with right sibling\n";
	}
}