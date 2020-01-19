#include <iostream>
#ifndef NODE_H
#include "B+ Tree.h"


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

void BPTree::seqDisplay(Node* cursor) {
    Node* firstLeft = firstLeftNode(cursor);

    if (firstLeft == NULL) {
        cout << "No Data in the Database yet!" << endl;
        return;
    }
    while (firstLeft != NULL) {
        for (int i = 0; i < firstLeft->keys.size(); i++) {
            cout << firstLeft->keys[i] << " ";
        }

        firstLeft = firstLeft->ptr2next;
    }
    cout << endl;
}

#endif 