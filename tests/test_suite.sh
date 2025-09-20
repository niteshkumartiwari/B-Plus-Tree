#!/bin/bash

# Comprehensive B+ Tree Test Script
# This script sets up, tests, and cleans up the B+ Tree implementation

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test configuration
TEST_DIR="test_temp"
BACKUP_DIR="backup_dbfiles"
ORIGINAL_DBFILES="DBFiles"

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to setup test environment
setup_test_environment() {
    print_status "Setting up test environment..."
    
    # Backup existing DBFiles if they exist
    if [ -d "$ORIGINAL_DBFILES" ]; then
        print_status "Backing up existing DBFiles..."
        cp -r "$ORIGINAL_DBFILES" "$BACKUP_DIR" 2>/dev/null || true
        rm -rf "$ORIGINAL_DBFILES"/*
    fi
    
    # Create fresh DBFiles directory
    mkdir -p "$ORIGINAL_DBFILES"
    
    # Build the project using Makefile (unless SKIP_BUILD is set)
    if [ "${SKIP_BUILD:-0}" = "1" ]; then
        print_status "Skipping build (SKIP_BUILD=1)"
    else
        print_status "Building B+ Tree project with Makefile..."
        
        if make clean > /dev/null 2>&1 && make > /dev/null 2>&1; then
            print_success "Build completed successfully"
        else
            print_error "Build failed"
            exit 1
        fi
    fi
    
    # Check if executable exists
    if [ ! -f "./bptree_demo" ]; then
        print_error "Executable 'bptree_demo' not found"
        print_error "Available files: $(ls -la *.exe bptree* basic_usage* 2>/dev/null || echo 'none found')"
        exit 1
    fi
    
    print_success "Test environment setup complete"
}

# Function to run a test case
run_test_case() {
    local test_name="$1"
    local test_input="$2"
    local expected_patterns="$3"
    
    print_status "Running test: $test_name"
    
    # Run the test and capture output
    local output
    output=$(echo "$test_input" | ./bptree_demo 2>&1)
    local exit_code=$?
    
    # Check if the program ran successfully
    if [ $exit_code -ne 0 ]; then
        print_error "Test '$test_name' failed with exit code $exit_code"
        return 1
    fi
    
    # Check for expected patterns in output
    local pattern_found=true
    while IFS= read -r pattern; do
        if [ -n "$pattern" ] && ! echo "$output" | grep -q "$pattern"; then
            print_warning "Expected pattern '$pattern' not found in test '$test_name'"
            pattern_found=false
        fi
    done <<< "$expected_patterns"
    
    if $pattern_found; then
        print_success "Test '$test_name' passed"
        return 0
    else
        print_error "Test '$test_name' failed - missing expected patterns"
        return 1
    fi
}

# Function to count files in DBFiles directory
count_db_files() {
    if [ -d "$ORIGINAL_DBFILES" ]; then
        find "$ORIGINAL_DBFILES" -name "*.txt" -not -name "dummy*" | wc -l
    else
        echo "0"
    fi
}

# Function to run all test cases
run_all_tests() {
    print_status "Starting comprehensive test suite..."
    local total_tests=0
    local passed_tests=0
    
    # Test 1: Basic Insert and Search Operations
    total_tests=$((total_tests + 1))
    local test1_input="4
3
1
101
Alice 20 85
1
102
Bob 21 90
2
101
2
102
2
999
5"
    local test1_expected="I AM ROOT
Inserted successfully
Key FOUND
Key FOUND
Key NOT FOUND"
    
    if run_test_case "Basic Insert and Search" "$test1_input" "$test1_expected"; then
        passed_tests=$((passed_tests + 1))
    fi
    
    # Verify files were created
    local file_count=$(count_db_files)
    if [ "$file_count" -ge 2 ]; then
        print_success "File creation verified: $file_count files created"
    else
        print_warning "Expected at least 2 files, found $file_count"
    fi
    
    # Test 2: Tree Splitting Operations
    total_tests=$((total_tests + 1))
    local test2_input="4
3
1
201
Data1 20 80
1
202
Data2 21 81
1
203
Data3 22 82
1
204
Data4 23 83
3
1
5"
    local test2_expected="I AM ROOT
Inserted successfully
Inserted successfully
Created new Root"
    
    if run_test_case "Tree Splitting" "$test2_input" "$test2_expected"; then
        passed_tests=$((passed_tests + 1))
    fi
    
    # Test 3: Delete Operations
    total_tests=$((total_tests + 1))
    local test3_input="4
3
1
301
Test1 20 80
1
302
Test2 21 81
1
303
Test3 22 82
4
302
3
1
5"
    local test3_expected="I AM ROOT
Inserted successfully
Inserted successfully
Successfully Deleted file"
    
    if run_test_case "Delete Operations" "$test3_input" "$test3_expected"; then
        passed_tests=$((passed_tests + 1))
    fi
    
    # Test 4: Complex Delete with Underflow
    total_tests=$((total_tests + 1))
    local test4_input="4
3
1
401
A 20 80
1
402
B 21 81
1
403
C 22 82
1
404
D 23 83
4
402
3
1
5"
    local test4_expected="I AM ROOT
Created new Root
Successfully Deleted file
UnderFlow"
    
    if run_test_case "Complex Delete with Underflow" "$test4_input" "$test4_expected"; then
        passed_tests=$((passed_tests + 1))
    fi
    
    # Test 5: Edge Cases - Empty Tree Operations
    total_tests=$((total_tests + 1))
    local test5_input="4
3
4
999
2
888
3
1
5"
    local test5_expected="B+ Tree is Empty
NO Tuples Inserted yet"
    
    if run_test_case "Edge Cases - Empty Tree" "$test5_input" "$test5_expected"; then
        passed_tests=$((passed_tests + 1))
    fi
    
    # Test 6: Single Node Tree Operations
    total_tests=$((total_tests + 1))
    local test6_input="4
3
1
501
Single 25 95
4
501
3
1
5"
    local test6_expected="I AM ROOT
Successfully Deleted file
Tree is Empty Now"
    
    if run_test_case "Single Node Tree" "$test6_input" "$test6_expected"; then
        passed_tests=$((passed_tests + 1))
    fi
    
    # Test 7: Large Tree Operations
    total_tests=$((total_tests + 1))
    local test7_input="5
4
1
601
A 20 80
1
602
B 21 81
1
603
C 22 82
1
604
D 23 83
1
605
E 24 84
1
606
F 25 85
1
607
G 26 86
1
608
H 27 87
3
1
2
605
5"
    local test7_expected="I AM ROOT
Created new Root
Key FOUND"
    
    if run_test_case "Large Tree Operations" "$test7_input" "$test7_expected"; then
        passed_tests=$((passed_tests + 1))
    fi
    
    # Test 8: Use-After-Free Bug Test (Segmentation Fault Regression Test)
    # This test specifically triggers the use-after-free bug that was fixed
    # It creates a scenario where node merging occurs and tests the fix
    total_tests=$((total_tests + 1))
    local test8_input="3
2
1
801
TestA 20 80
1
802
TestB 21 81
1
803
TestC 22 82
4
802
4
801
3
1
5"
    local test8_expected="I AM ROOT
Successfully Deleted file
Successfully Deleted file"
    
    if run_test_case "Use-After-Free Regression Test" "$test8_input" "$test8_expected"; then
        passed_tests=$((passed_tests + 1))
    fi
    
    # Test 9: NULL Pointer Dereference Regression Test
    # This test creates a scenario that could trigger findParent returning NULL
    total_tests=$((total_tests + 1))
    local test9_input="2
2
1
901
Root 30 90
4
901
3
1
5"
    local test9_expected="I AM ROOT
Successfully Deleted file
Tree is Empty Now"
    
    if run_test_case "NULL Pointer Regression Test" "$test9_input" "$test9_expected"; then
        passed_tests=$((passed_tests + 1))
    fi
    
    # Test 10: Array Bounds Regression Test
    # This test creates a scenario with sibling operations that could cause out-of-bounds access
    total_tests=$((total_tests + 1))
    local test10_input="4
3
1
1001
BoundsA 25 85
1
1002
BoundsB 26 86
1
1003
BoundsC 27 87
1
1004
BoundsD 28 88
4
1002
4
1003
3
1
5"
    local test10_expected="I AM ROOT
Successfully Deleted file
Successfully Deleted file"
    
    if run_test_case "Array Bounds Regression Test" "$test10_input" "$test10_expected"; then
        passed_tests=$((passed_tests + 1))
    fi
    
    # Test 11: Sequential Display Test
    total_tests=$((total_tests + 1))
    local test11_input="4
3
1
701
X 20 80
1
702
Y 21 81
1
703
Z 22 82
3
2
5"
    local test11_expected="I AM ROOT
Inserted successfully
Inserted successfully
701 702 703"
    
    if run_test_case "Sequential Display" "$test11_input" "$test11_expected"; then
        passed_tests=$((passed_tests + 1))
    fi
    
    # Print test summary
    echo ""
    print_status "=== TEST SUMMARY ==="
    print_status "Total Tests: $total_tests"
    print_success "Passed Tests: $passed_tests"
    
    if [ $passed_tests -eq $total_tests ]; then
        print_success "ALL TESTS PASSED! 🎉"
        return 0
    else
        local failed_tests=$((total_tests - passed_tests))
        print_error "Failed Tests: $failed_tests"
        return 1
    fi
}

# Function to verify file operations
verify_file_operations() {
    print_status "Verifying file operations..."
    
    # Check if DBFiles directory exists
    if [ ! -d "$ORIGINAL_DBFILES" ]; then
        print_error "DBFiles directory not found"
        return 1
    fi
    
    # Count files created during tests
    local file_count=$(count_db_files)
    print_status "Total data files created: $file_count"
    
    # Verify some files have content
    local files_with_content=0
    for file in "$ORIGINAL_DBFILES"/*.txt; do
        if [ -f "$file" ] && [ -s "$file" ]; then
            files_with_content=$((files_with_content + 1))
        fi
    done
    
    print_status "Files with content: $files_with_content"
    
    if [ $files_with_content -gt 0 ]; then
        print_success "File operations verified successfully"
        return 0
    else
        print_error "No files with content found"
        return 1
    fi
}

# Function to cleanup test environment
cleanup_test_environment() {
    print_status "Cleaning up test environment..."
    
    # Remove test files
    if [ -d "$ORIGINAL_DBFILES" ]; then
        print_status "Removing test data files..."
        rm -rf "$ORIGINAL_DBFILES"/*
    fi
    
    # Restore original DBFiles if backup exists
    if [ -d "$BACKUP_DIR" ]; then
        print_status "Restoring original DBFiles..."
        cp -r "$BACKUP_DIR"/* "$ORIGINAL_DBFILES"/ 2>/dev/null || true
        rm -rf "$BACKUP_DIR"
    fi
    
    # Clean build artifacts if requested
    if [ "$1" = "--clean-build" ]; then
        print_status "Cleaning build artifacts..."
        make clean > /dev/null 2>&1 || true
    fi
    
    print_success "Cleanup completed"
}

# Function to show usage
show_usage() {
    echo "Usage: $0 [OPTIONS]"
    echo "Options:"
    echo "  --clean-build    Also clean build artifacts after testing"
    echo "  --help          Show this help message"
    echo ""
    echo "This script will:"
    echo "1. Setup test environment and build the project"
    echo "2. Run comprehensive test suite"
    echo "3. Verify file operations"
    echo "4. Clean up temporary files"
}

# Main execution
main() {
    local clean_build=false
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            --clean-build)
                clean_build=true
                shift
                ;;
            --help)
                show_usage
                exit 0
                ;;
            *)
                print_error "Unknown option: $1"
                show_usage
                exit 1
                ;;
        esac
    done
    
    echo "========================================"
    echo "    B+ Tree Comprehensive Test Suite"
    echo "========================================"
    echo ""
    
    # Setup test environment
    setup_test_environment
    
    # Run all tests
    local test_result=0
    if ! run_all_tests; then
        test_result=1
    fi
    
    # Verify file operations
    if ! verify_file_operations; then
        test_result=1
    fi
    
    # Cleanup
    if $clean_build; then
        cleanup_test_environment --clean-build
    else
        cleanup_test_environment
    fi
    
    echo ""
    echo "========================================"
    if [ $test_result -eq 0 ]; then
        print_success "🎉 ALL TESTS COMPLETED SUCCESSFULLY! 🎉"
        echo "The B+ Tree implementation is working correctly."
    else
        print_error "❌ SOME TESTS FAILED!"
        echo "Please review the test output above."
    fi
    echo "========================================"
    
    exit $test_result
}

# Run main function with all arguments
main "$@"