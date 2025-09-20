/**
 * @file basic_usage.cpp
 * @brief Basic usage example for B+ Tree implementation
 * 
 * This example demonstrates the fundamental operations of the B+ Tree:
 * - Creating a tree with custom parameters
 * - Inserting data with file-based storage
 * - Searching for keys
 * - Displaying tree structure
 * - Deleting keys
 */

#include <bptree/bptree.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace bptree;

int main() {
    std::cout << "=== B+ Tree Basic Usage Example ===\n\n";
    
    // Create B+ tree with internal node limit 4, leaf node limit 3
    BPTree tree(4, 3);
    std::cout << "Created B+ Tree with internal limit 4, leaf limit 3\n\n";
    
    // Sample student data
    struct Student {
        int id;
        std::string name;
        int age;
        int marks;
    };
    
    std::vector<Student> students = {
        {101, "Alice", 20, 85},
        {102, "Bob", 21, 90},
        {103, "Charlie", 19, 88},
        {104, "Diana", 22, 92},
        {105, "Eve", 20, 87}
    };
    
    // Insert student data
    std::cout << "Inserting student data:\n";
    for (const auto& student : students) {
        std::string filename = "DBFiles/" + std::to_string(student.id) + ".txt";
        FILE* file = fopen(filename.c_str(), "w");
        
        if (file != nullptr) {
            fprintf(file, "%s %d %d\n", student.name.c_str(), student.age, student.marks);
            tree.insert(student.id, file);
            fclose(file);
            std::cout << "  Inserted: ID=" << student.id << ", Name=" << student.name << "\n";
        } else {
            std::cerr << "  Error: Could not create file for ID " << student.id << "\n";
        }
    }
    
    std::cout << "\n=== Tree Structure (Hierarchical) ===\n";
    tree.display(tree.getRoot());
    
    std::cout << "\n=== Tree Structure (Sequential) ===\n";
    tree.seqDisplay(tree.getRoot());
    
    // Search for specific students
    std::cout << "\n=== Search Operations ===\n";
    std::vector<int> searchIds = {101, 103, 999};  // 999 doesn't exist
    
    for (int id : searchIds) {
        std::cout << "Searching for ID " << id << ": ";
        tree.search(id);
    }
    
    // Delete a student
    std::cout << "\n=== Delete Operation ===\n";
    int deleteId = 102;
    std::cout << "Deleting student with ID " << deleteId << "\n";
    tree.removeKey(deleteId);
    
    std::cout << "\n=== Tree Structure After Deletion ===\n";
    tree.display(tree.getRoot());
    
    std::cout << "\n=== Sequential View After Deletion ===\n";
    tree.seqDisplay(tree.getRoot());
    
    std::cout << "\n=== Example completed successfully! ===\n";
    
    return 0;
}