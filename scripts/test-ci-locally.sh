#!/bin/bash

# Local CI Testing Script
# This script simulates the GitHub Actions workflow locally

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

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

print_header() {
    echo ""
    echo "========================================"
    echo "  $1"
    echo "========================================"
    echo ""
}

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Test 1: Build System Test
test_build_system() {
    print_header "Testing Build System"
    
    print_status "Cleaning previous builds..."
    make clean > /dev/null 2>&1 || true
    
    print_status "Testing Make build..."
    if make > /dev/null 2>&1; then
        print_success "Make build successful"
    else
        print_error "Make build failed"
        return 1
    fi
    
    print_status "Testing executables..."
    if [ -f "./bptree_demo" ] && [ -f "./basic_usage" ]; then
        print_success "Executables created successfully"
    else
        print_error "Executables not found"
        return 1
    fi
    
    return 0
}

# Test 2: Code Quality Checks
test_code_quality() {
    print_header "Testing Code Quality"
    
    # Check for clang-format
    if command_exists clang-format; then
        print_status "Checking code formatting..."
        if find src include examples -name "*.cpp" -o -name "*.hpp" | xargs clang-format --dry-run --Werror > /dev/null 2>&1; then
            print_success "Code formatting check passed"
        else
            print_warning "Code formatting issues found (run clang-format to fix)"
        fi
    else
        print_warning "clang-format not available, skipping format check"
    fi
    
    # Check for cppcheck
    if command_exists cppcheck; then
        print_status "Running static analysis..."
        if cppcheck --enable=warning --error-exitcode=1 --suppress=missingIncludeSystem src/ include/ > /dev/null 2>&1; then
            print_success "Static analysis passed"
        else
            print_warning "Static analysis found issues"
        fi
    else
        print_warning "cppcheck not available, skipping static analysis"
    fi
    
    # Check for TODO/FIXME
    print_status "Checking for TODO/FIXME comments..."
    if grep -r "TODO\|FIXME" src/ include/ examples/ > /dev/null 2>&1; then
        print_warning "Found TODO/FIXME comments:"
        grep -rn "TODO\|FIXME" src/ include/ examples/ || true
    else
        print_success "No TODO/FIXME comments found"
    fi
    
    return 0
}

# Test 3: Documentation Validation
test_documentation() {
    print_header "Testing Documentation"
    
    # Check essential files
    print_status "Checking essential files..."
    local missing_files=()
    for file in README.md LICENSE CONTRIBUTING.md CHANGELOG.md SECURITY.md; do
        if [ ! -f "$file" ]; then
            missing_files+=("$file")
        fi
    done
    
    if [ ${#missing_files[@]} -eq 0 ]; then
        print_success "All essential files present"
    else
        print_error "Missing files: ${missing_files[*]}"
        return 1
    fi
    
    # Check directory structure
    print_status "Checking directory structure..."
    local missing_dirs=()
    for dir in src include tests examples .github; do
        if [ ! -d "$dir" ]; then
            missing_dirs+=("$dir")
        fi
    done
    
    if [ ${#missing_dirs[@]} -eq 0 ]; then
        print_success "All essential directories present"
    else
        print_error "Missing directories: ${missing_dirs[*]}"
        return 1
    fi
    
    # Check README images
    print_status "Checking README image references..."
    if grep -o '!\[.*\](img/[^)]*)' README.md > /dev/null 2>&1; then
        local missing_images=()
        while IFS= read -r img_path; do
            img_file=$(echo "$img_path" | sed 's/.*(//' | sed 's/).*//')
            if [ ! -f "$img_file" ]; then
                missing_images+=("$img_file")
            fi
        done < <(grep -o '!\[.*\](img/[^)]*)' README.md | sed 's/.*(//' | sed 's/).*//')
        
        if [ ${#missing_images[@]} -eq 0 ]; then
            print_success "All referenced images exist"
        else
            print_warning "Missing images: ${missing_images[*]}"
        fi
    else
        print_success "No image references to check"
    fi
    
    return 0
}

# Test 4: Workflow Validation
test_workflows() {
    print_header "Testing GitHub Workflows"
    
    # Check workflow files exist
    print_status "Checking workflow files..."
    local workflow_files=(".github/workflows/ci.yml" ".github/workflows/release.yml" ".github/workflows/codeql.yml")
    local missing_workflows=()
    
    for workflow in "${workflow_files[@]}"; do
        if [ ! -f "$workflow" ]; then
            missing_workflows+=("$workflow")
        fi
    done
    
    if [ ${#missing_workflows[@]} -eq 0 ]; then
        print_success "All workflow files present"
    else
        print_error "Missing workflows: ${missing_workflows[*]}"
        return 1
    fi
    
    # Validate YAML syntax (if yq is available)
    if command_exists yq; then
        print_status "Validating YAML syntax..."
        local yaml_errors=()
        for workflow in "${workflow_files[@]}"; do
            if ! yq eval . "$workflow" > /dev/null 2>&1; then
                yaml_errors+=("$workflow")
            fi
        done
        
        if [ ${#yaml_errors[@]} -eq 0 ]; then
            print_success "All YAML files are valid"
        else
            print_error "YAML syntax errors in: ${yaml_errors[*]}"
            return 1
        fi
    else
        print_warning "yq not available, skipping YAML validation"
    fi
    
    return 0
}

# Test 5: Comprehensive Test Suite
test_comprehensive_suite() {
    print_header "Running Comprehensive Test Suite"
    
    print_status "Running test suite..."
    if [ -f "tests/test_suite.sh" ]; then
        if ./tests/test_suite.sh > /dev/null 2>&1; then
            print_success "Comprehensive test suite passed"
        else
            print_error "Comprehensive test suite failed"
            return 1
        fi
    else
        print_error "Test suite not found"
        return 1
    fi
    
    return 0
}

# Main execution
main() {
    print_header "Local CI Testing for B+ Tree Project"
    
    local failed_tests=0
    local total_tests=5
    
    # Run all tests
    test_build_system || ((failed_tests++))
    test_code_quality || ((failed_tests++))
    test_documentation || ((failed_tests++))
    test_workflows || ((failed_tests++))
    test_comprehensive_suite || ((failed_tests++))
    
    # Summary
    print_header "Test Summary"
    local passed_tests=$((total_tests - failed_tests))
    
    if [ $failed_tests -eq 0 ]; then
        print_success "🎉 All tests passed! ($passed_tests/$total_tests)"
        print_success "Your project is ready for GitHub Actions!"
        exit 0
    else
        print_error "❌ Some tests failed ($passed_tests/$total_tests passed)"
        print_error "Please fix the issues before pushing to GitHub"
        exit 1
    fi
}

# Run main function
main "$@"