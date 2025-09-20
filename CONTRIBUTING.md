# Contributing to B+ Tree Implementation

Thank you for your interest in contributing to this B+ Tree implementation! This document provides guidelines and information for contributors.

## 🤝 How to Contribute

### Reporting Issues

1. **Search existing issues** first to avoid duplicates
2. **Use the issue template** when creating new issues
3. **Provide detailed information** including:
   - Operating system and version
   - Compiler and version
   - Steps to reproduce the issue
   - Expected vs actual behavior
   - Relevant code snippets or error messages

### Submitting Pull Requests

1. **Fork the repository** and create a feature branch
2. **Follow the coding standards** (see below)
3. **Write tests** for new functionality
4. **Update documentation** as needed
5. **Ensure all tests pass** before submitting (`make test`)
6. **Use our [PR template](.github/PULL_REQUEST_TEMPLATE.md)** when submitting
7. **Write clear commit messages** following conventional commits

## 🏗️ Development Setup

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.15 or higher
- Git

### Setup Instructions

```bash
# Fork and clone the repository
git clone <your-forked-repo-url>
cd bplus-tree

# Create development build
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)

# Run tests to ensure everything works
./test_suite.sh
```

## 📝 Coding Standards

### Code Style

We use `.clang-format` for consistent code formatting:

```bash
# Format all source files
find src include -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i
```

### Naming Conventions

- **Classes**: PascalCase (`BPTree`, `Node`)
- **Functions**: camelCase (`insertInternal`, `findParent`)
- **Variables**: camelCase (`maxIntChildLimit`, `leftSibling`)
- **Constants**: UPPER_SNAKE_CASE (`MAX_KEYS`)
- **Files**: snake_case (`bptree.hpp`, `insertion.cpp`)

### Code Guidelines

1. **Use modern C++17 features** where appropriate
2. **Prefer RAII** for resource management
3. **Use smart pointers** when ownership is unclear
4. **Write self-documenting code** with clear variable names
5. **Add comments** for complex algorithms
6. **Keep functions focused** and single-purpose
7. **Use const correctness** throughout

### Example Code Style

```cpp
namespace bptree {

class BPTree {
  private:
    int maxIntChildLimit_;
    std::unique_ptr<Node> root_;
    
    void insertInternal(int key, Node** cursor, Node** child);
    
  public:
    explicit BPTree(int degreeInternal, int degreeLeaf);
    ~BPTree() = default;
    
    // Non-copyable, movable
    BPTree(const BPTree&) = delete;
    BPTree& operator=(const BPTree&) = delete;
    BPTree(BPTree&&) = default;
    BPTree& operator=(BPTree&&) = default;
    
    void insert(int key, const std::string& data);
    bool search(int key) const;
    void remove(int key);
};

}  // namespace bptree
```

## 🧪 Testing

### Running Tests

```bash
# Run all tests
./test_suite.sh

# Run with verbose output
./test_suite.sh --verbose

# Run specific test categories
ctest -R "unit_tests"
```

### Writing Tests

1. **Add unit tests** for new functions in `tests/unit/`
2. **Add integration tests** for new features in `tests/integration/`
3. **Update test_suite.sh** for comprehensive scenarios
4. **Ensure tests are deterministic** and don't depend on external state

### Test Structure

```cpp
// tests/unit/test_insertion.cpp
#include <gtest/gtest.h>
#include "bptree/bptree.hpp"

namespace bptree {
namespace test {

class InsertionTest : public ::testing::Test {
  protected:
    void SetUp() override {
        tree_ = std::make_unique<BPTree>(4, 3);
    }
    
    std::unique_ptr<BPTree> tree_;
};

TEST_F(InsertionTest, BasicInsertion) {
    EXPECT_NO_THROW(tree_->insert(1, "test_data"));
    EXPECT_TRUE(tree_->search(1));
}

}  // namespace test
}  // namespace bptree
```

## 📋 Pull Request Process

### Before Submitting

1. **Rebase your branch** on the latest main
2. **Run all tests** and ensure they pass (`make test`)
3. **Format your code** using clang-format (if available)
4. **Update documentation** if needed
5. **Fill out our [PR template](.github/PULL_REQUEST_TEMPLATE.md)**

### Commit Message Format

We follow [Conventional Commits](https://www.conventionalcommits.org/):

```
type(scope): description
```

Examples:
```
feat(insertion): add support for duplicate key handling
fix(deletion): resolve underflow in leaf nodes
docs(readme): update build instructions
test(search): add edge case tests for empty tree
```

## 🎯 Areas for Contribution

### High Priority

- [ ] **Performance optimizations** in search/insert operations
- [ ] **Thread safety** improvements
- [ ] **Memory usage** optimizations
- [ ] **Error handling** enhancements

### Medium Priority

- [ ] **Template-based** generic implementation
- [ ] **Serialization/deserialization** support
- [ ] **Benchmarking suite** expansion
- [ ] **Documentation** improvements

### Low Priority

- [ ] **Python bindings** using pybind11
- [ ] **Visualization tools** for tree structure
- [ ] **Fuzzing tests** for robustness
- [ ] **Alternative storage backends**

## 📞 Getting Help

- **GitHub Discussions**: For questions and general discussion
- **GitHub Issues**: For bug reports and feature requests
- **Code Review**: For feedback on your contributions

## 🏆 Recognition

Contributors will be recognized in:
- **README.md** acknowledgments section
- **CHANGELOG.md** for significant contributions
- **GitHub contributors** page

Thank you for contributing to make this B+ Tree implementation better! 🎉