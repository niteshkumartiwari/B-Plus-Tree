# B+ Tree Implementation

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://github.com/user/bplus-tree)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Version](https://img.shields.io/badge/version-1.0.0-green.svg)](CHANGELOG.md)

A high-performance, modern C++17 implementation of B+ Tree data structure with comprehensive testing, memory safety, and professional documentation.

## 🚀 Features

- ✅ **Complete B+ Tree Operations**: Insert, Search, Delete with proper tree balancing
- ✅ **Modern C++17**: Clean, type-safe implementation with proper namespacing
- ✅ **Memory Safe**: RAII principles, proper destructors, and leak prevention
- ✅ **File-Based Storage**: Simulates disk-based database operations
- ✅ **Comprehensive Testing**: Full test suite with 8+ scenarios
- ✅ **Cross-Platform**: Works on Linux, macOS, and Windows
- ✅ **Professional Structure**: Industry-standard project organization

## 📋 Table of Contents

- [Demo](#demo)
- [Quick Start](#quick-start)
- [Building](#building)
- [Usage](#usage)
- [B+ Tree Theory](#b-tree-theory)
- [API Reference](#api-reference)
- [Testing](#testing)
- [Contributing](#contributing)
- [License](#license)

## 🎬 Demo

![Demo](img/demo.gif)

## 🏃 Quick Start

```cpp
#include <bptree/bptree.hpp>
#include <iostream>

int main() {
    // Create B+ tree with internal node limit 4, leaf node limit 3
    bptree::BPTree tree(4, 3);
    
    // Insert student data
    FILE* file = fopen("DBFiles/101.txt", "w");
    fprintf(file, "Alice 20 85\n");
    tree.insert(101, file);
    fclose(file);
    
    // Search for data
    tree.search(101);  // Output: "Hurray!! Key FOUND"
    
    // Display tree structure
    tree.display(tree.getRoot());
    
    return 0;
}
```

## 🔨 Building

### Prerequisites

- **C++17 compatible compiler** (GCC 7+, Clang 5+, MSVC 2017+)
- **Make** or **CMake 3.15+** (optional)

### Build Instructions

#### Option 1: Using Makefile (Recommended)
```bash
# Clone the repository
git clone https://github.com/user/bplus-tree.git
cd bplus-tree

# Build the project
make

# Run the demo
./bptree_demo

# Run example program
./basic_usage

# Run comprehensive tests
make test
```

#### Option 2: Using CMake
```bash
# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
cmake --build . --parallel

# Run tests
ctest --verbose
```

### Build Targets

- `make` - Build demo and example programs
- `make run-demo` - Build and run the main demo
- `make run-example` - Build and run the basic usage example
- `make test` - Run comprehensive test suite
- `make clean` - Clean build artifacts

## 📖 Usage

### Database Schema

Our database schema simulates a student management system:

![Database Schema](img/database.jpg)

Each record contains:
- **Roll Number** (Primary Key): Integer identifier
- **Name**: Student name
- **Age**: Student age
- **Marks**: Academic score

### Basic Operations

```cpp
#include <bptree/bptree.hpp>

// Create tree with custom parameters
bptree::BPTree tree(4, 3);  // internal_limit=4, leaf_limit=3

// Insert student record
FILE* studentFile = fopen("DBFiles/12345.txt", "w");
fprintf(studentFile, "John Doe 20 85\n");
tree.insert(12345, studentFile);
fclose(studentFile);

// Search for student
tree.search(12345);  // Displays: "Hurray!! Key FOUND"

// Delete student record
tree.removeKey(12345);

// Display tree structures
tree.display(tree.getRoot());     // Hierarchical view
tree.seqDisplay(tree.getRoot());  // Sequential leaf traversal
```

### Advanced Usage

```cpp
// Create multiple trees for different tables
bptree::BPTree studentsTree(4, 3);
bptree::BPTree coursesTree(6, 5);  // Different capacity

// Batch operations
std::vector<int> rollNumbers = {101, 102, 103, 104, 105};
for (int rollNo : rollNumbers) {
    std::string filename = "DBFiles/" + std::to_string(rollNo) + ".txt";
    FILE* file = fopen(filename.c_str(), "w");
    fprintf(file, "Student_%d 20 85\n", rollNo);
    studentsTree.insert(rollNo, file);
    fclose(file);
}
```

## 🌳 B+ Tree Theory

### Core Properties

B+ Trees are balanced tree data structures optimized for systems that read and write large blocks of data. Unlike B-Trees, **all data is stored in leaf nodes**, making sequential access efficient.

### Key Characteristics

1. **Dual Order Values**: Separate limits for internal and leaf nodes
2. **Right-Biased Splitting**: When splitting even-sized nodes, right sibling gets one extra element
3. **Primary Key Based**: No duplicate keys allowed (maintains primary key constraints)
4. **Sequential Access**: Leaf nodes are linked for efficient range queries
5. **Memory Efficient**: Union-based node structure separates internal/leaf node data

### Implementation Details

#### Node Structure
```cpp
class Node {
    bool isLeaf;
    std::vector<int> keys;
    Node* ptr2next;  // Links leaf nodes for sequential access
    
    union ptr {
        std::vector<Node*> ptr2Tree;    // Internal node children
        std::vector<FILE*> dataPtr;     // Leaf node data pointers
    } ptr2TreeOrData;
};
```

#### Design Decisions

1. **No Parent Pointers**: Avoids complexity during deletion operations
2. **Explicit ptr2next**: Enables efficient sequential traversal
3. **File-Based Storage**: Simulates disk block access patterns
4. **Union Memory Layout**: Optimizes memory usage for different node types

### B+ Tree Properties

#### Node Capacity Constraints

**Internal (Non-Leaf) Nodes:**
- `ceil(maxInternalLimit/2) ≤ #children ≤ maxInternalLimit`
- `ceil(maxInternalLimit/2)-1 ≤ #keys ≤ maxInternalLimit-1`

**Leaf Nodes:**
- `ceil(maxLeafLimit/2) ≤ #keys ≤ maxLeafLimit`
- Each key has corresponding data pointer of same size

**Minimum 50% Occupancy Rule:**
All nodes (except root) must maintain at least 50% capacity to ensure balanced tree performance.

![B+ Tree Properties](img/prop_1.png)
![B+ Tree Structure](img/prop_2.png)
![B+ Tree Constraints](img/prop_3.png)



### Search Algorithm

The search operation efficiently navigates from root to leaf using the tree's ordered structure:

1. **Internal Node Navigation**: 
   - Find first key ≥ search key
   - Follow corresponding child pointer
   - If all keys < search key, follow rightmost pointer

2. **Leaf Node Search**:
   - Perform binary search within leaf node
   - Return success/failure with data location

3. **Time Complexity**: O(log n) for all operations

```cpp
void search(int key) {
    Node* cursor = root;
    
    // Navigate to leaf node
    while (!cursor->isLeaf) {
        int idx = upper_bound(cursor->keys.begin(), 
                             cursor->keys.end(), key) 
                 - cursor->keys.begin();
        cursor = cursor->ptr2TreeOrData.ptr2Tree[idx];
    }
    
    // Search in leaf node
    int idx = lower_bound(cursor->keys.begin(), 
                         cursor->keys.end(), key) 
             - cursor->keys.begin();
    
    if (idx < cursor->keys.size() && cursor->keys[idx] == key) {
        // Key found - display data
        displayData(cursor->ptr2TreeOrData.dataPtr[idx]);
    }
}
```

![B+ Tree Search](img/search_1.png)



### Insertion Algorithm

Our implementation uses the **split-only approach** for simplicity and performance:

#### Insertion Strategies Comparison

| Strategy | Approach | Pros | Cons |
|----------|----------|------|------|
| **Redistribution** | Try borrowing from siblings first | Better space utilization | Increased I/O operations |
| **Split-Only** ✅ | Immediately split full nodes | Simpler implementation, fewer I/O | Potentially more nodes |

#### Algorithm Steps

1. **Navigate to Leaf**: Traverse tree to find insertion point
2. **Check Capacity**: 
   - If space available → Insert directly
   - If full → Split node
3. **Handle Splits**:
   - **Leaf Split**: Copy minimum key of new node to parent
   - **Internal Split**: Promote middle key to parent
4. **Propagate Upward**: Repeat splitting up the tree if necessary

#### Insertion Example

Inserting key `8` into the tree:

**Step 1: Initial State**
![Insertion Step 1](img/insert_1.png)

**Step 2: Node Split**
![Insertion Step 2](img/insert_2.png)

**Step 3: Final State**
![Insertion Step 3](img/insert_3.png)

```cpp
void insert(int key, FILE* filePtr) {
    if (root == nullptr) {
        // Create root node
        root = new Node();
        root->isLeaf = true;
        root->keys.push_back(key);
        root->ptr2TreeOrData.dataPtr.push_back(filePtr);
        return;
    }
    
    // Find leaf node for insertion
    Node* leaf = findLeafNode(key);
    
    if (leaf->keys.size() < maxLeafNodeLimit) {
        // Simple insertion
        insertIntoLeaf(leaf, key, filePtr);
    } else {
        // Split required
        splitLeafAndInsert(leaf, key, filePtr);
    }
}
```

## 📚 API Reference

### BPTree Class

#### Constructors
```cpp
BPTree();                                    // Default: internal=4, leaf=3
BPTree(int degreeInternal, int degreeLeaf);  // Custom configuration
```

#### Core Operations
```cpp
void insert(int key, FILE* filePtr);        // Insert key-data pair
void search(int key);                       // Search and display data
void removeKey(int key);                    // Delete key from tree
```

#### Display Operations
```cpp
void display(Node* cursor);                 // Hierarchical tree view
void seqDisplay(Node* cursor);              // Sequential leaf traversal
```

#### Utility Methods
```cpp
Node* getRoot();                            // Get root node
int getMaxIntChildLimit();                  // Get internal node capacity
int getMaxLeafNodeLimit();                  // Get leaf node capacity
void setRoot(Node* ptr);                    // Set root node
```

### Node Structure

```cpp
class Node {
public:
    bool isLeaf;                            // Node type flag
    std::vector<int> keys;                  // Sorted keys
    Node* ptr2next;                         // Next leaf node (for leaves)
    
    union ptr {
        std::vector<Node*> ptr2Tree;        // Child pointers (internal)
        std::vector<FILE*> dataPtr;         // Data pointers (leaf)
    } ptr2TreeOrData;
    
    Node();                                 // Constructor
    ~Node();                                // Destructor with cleanup
};
```

## 🧪 Testing

### Running Tests

```bash
# Run comprehensive test suite
make test

# Or run directly
./tests/test_suite.sh

# Run with verbose output
./tests/test_suite.sh --verbose

# Clean build and test
./tests/test_suite.sh --clean-build
```

### Test Coverage

Our test suite includes **8 comprehensive scenarios**:

| Test Case | Description | Validates |
|-----------|-------------|-----------|
| **Basic Operations** | Insert, search, display | Core functionality |
| **Tree Splitting** | Node overflow handling | Tree growth |
| **Delete Operations** | Simple key removal | Basic deletion |
| **Complex Deletion** | Underflow and merging | Advanced deletion |
| **Empty Tree** | Edge case operations | Error handling |
| **Single Node** | Minimal tree operations | Boundary conditions |
| **Large Tree** | Multi-level operations | Scalability |
| **Sequential Display** | Leaf traversal | Sequential access |

### Test Results
```
Total Tests: 8
Passed Tests: 8
ALL TESTS PASSED! 🎉

File Operations:
- Total data files created: 22+
- Files with content: 22+
- File operations verified successfully
```

## 🤝 Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for detailed guidelines.

### Quick Contribution Steps

1. **Fork** the repository
2. **Create** a feature branch (`git checkout -b feature/amazing-feature`)
3. **Commit** your changes (`git commit -m 'Add amazing feature'`)
4. **Push** to the branch (`git push origin feature/amazing-feature`)
5. **Open** a Pull Request

### Development Setup

```bash
# Clone your fork
git clone https://github.com/your-username/bplus-tree.git
cd bplus-tree

# Build and test
make
make test

# Format code (if clang-format available)
find src include -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i
```

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

**Made with ❤️ for the database and algorithms community**

*Happy Coding! 🚀*
