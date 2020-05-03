#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
//#include <filesystem>
#include "B+ Tree.h"

#define _CRT_SECURE_NO_DEPRECATE  //for VS 2019

void insertionMethod(BPTree** bPTree) {
    int rollNo;
    int age, marks;
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
    cout << "Insertion of roll No: " << rollNo << " Successful"<<endl;
}

void searchMethod(BPTree* bPTree) {
    int rollNo;
    cout << "What's the RollNo to Search? ";
    cin >> rollNo;

    bPTree->search(rollNo);
}

void printMethod(BPTree* bPTree) {
    int opt;
    cout << "Press \n\t1.Hierarical-Display \n\t2.Sequential-Display\n";
    cin >> opt;

    cout << "\nHere is your File Structure" << endl;
    if (opt == 1)
        bPTree->display(bPTree->getRoot());
    else
        bPTree->seqDisplay(bPTree->getRoot());
}

void deleteMethod(BPTree* bPTree) {
    cout << "Showing you the Tree, Choose a key from here: " << endl;
    bPTree->display(bPTree->getRoot());
 
    int tmp;
    cout << "Enter a key to delete: " << endl;
    cin >> tmp;
    bPTree->removeKey(tmp);

    //Displaying
    bPTree->display(bPTree->getRoot());
}

int main() {
    /*
		Please have a look at the default schema to get to know about the table
		Reference - img/database.jpg
	*/

    cout << "\n***Welcome to DATABASE SERVER**\n"
         << endl;

    bool flag = true;
    int option;

    int maxChildInt = 4, maxNodeLeaf = 3;
    cout << "Please provide the value to limit maximum child Internal Nodes can have: ";
    cin >> maxChildInt;
    cout << "\nAnd Now Limit the value to limit maximum Nodes Leaf Nodes can have: ";
    cin >> maxNodeLeaf;

    BPTree* bPTree = new BPTree(maxChildInt, maxNodeLeaf);

    do {
        cout << "\nPlease provide the queries with respective keys : " << endl;
        cout << "\tPress 1: Insertion \n\tPress 2: Search \n\tPress 3: Print Tree\n\tPress 4: Delete Key In Tree\n\tPress 5: ABORT!" << endl;
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
            case 4:
                deleteMethod(bPTree);
                break;
            default:
                flag = false;
                break;
        }
    }while (flag);

    return 0;
}
