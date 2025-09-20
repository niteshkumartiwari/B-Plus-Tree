# Testing GitHub Workflows Guide

This guide explains how to test the GitHub Actions workflows for the B+ Tree project.

## 🧪 Testing Approaches

### 1. Local Pre-Push Testing

Before pushing to GitHub, run our local CI simulation:

```bash
# Run local CI tests
./scripts/test-ci-locally.sh

# Expected output:
# ✅ All tests passed! (5/5)
# ✅ Your project is ready for GitHub Actions!
```

### 2. GitHub Actions Testing

#### **Method A: Push to Feature Branch (Recommended)**

```bash
# Create a test branch
git checkout -b test-workflows

# Add and commit changes
git add .
git commit -m "test: add GitHub workflows and CI/CD pipeline"

# Push to GitHub (triggers CI)
git push origin test-workflows
```

#### **Method B: Create Pull Request**

1. **Fork the repository** (if not owner)
2. **Create feature branch** with workflow changes
3. **Open Pull Request** to main/master branch
4. **Watch CI run** automatically on PR

#### **Method C: Direct Push (If you're confident)**

```bash
# Add all changes
git add .
git commit -m "feat: add comprehensive GitHub Actions CI/CD pipeline"

# Push to main branch (triggers CI)
git push origin master
```

### 3. Workflow-Specific Testing

#### **CI Workflow Testing**

The CI workflow will test:
- ✅ **Build on 3 platforms** (Linux, macOS, Windows)
- ✅ **Multiple compilers** (GCC, Clang, MSVC)
- ✅ **Code quality checks**
- ✅ **Documentation validation**
- ✅ **Security scanning**

**Expected Results:**
```
✅ build-and-test (ubuntu-latest, gcc)
✅ build-and-test (ubuntu-latest, clang)  
✅ build-and-test (macos-latest, gcc)
✅ build-and-test (macos-latest, clang)
✅ build-and-test (windows-latest, msvc)
✅ code-quality
✅ documentation
✅ security
```

#### **Release Workflow Testing**

To test the release workflow:

```bash
# Create and push a version tag
git tag v1.0.0
git push origin v1.0.0

# This will trigger:
# ✅ Automatic release creation
# ✅ Multi-platform binary builds
# ✅ Asset uploads (tar.gz, zip files)
# ✅ Changelog extraction
```

#### **Security Workflow Testing**

CodeQL runs automatically on:
- Every push to main/master
- Every pull request
- Weekly schedule (Sundays)

## 🔍 **What to Watch For**

### **Success Indicators:**

1. **Green Checkmarks**: All workflow jobs pass
2. **Build Artifacts**: Executables created for each platform
3. **Test Results**: All 8 comprehensive tests pass
4. **Security Scan**: No vulnerabilities detected
5. **Documentation**: All links and references valid

### **Common Issues & Solutions:**

| Issue | Cause | Solution |
|-------|-------|----------|
| **Build Fails on Windows** | Path separators, MSVC differences | Use CMake for Windows builds |
| **Tests Timeout** | Long-running operations | Optimize test suite timing |
| **Missing Dependencies** | Platform-specific packages | Update workflow dependencies |
| **Format Check Fails** | Code style inconsistencies | Run `clang-format` locally |
| **Security Alerts** | Vulnerable code patterns | Review and fix flagged code |

## 🛠️ **Local Development Tools**

### **Install Recommended Tools:**

```bash
# macOS (using Homebrew)
brew install clang-format cppcheck yq

# Ubuntu/Debian
sudo apt-get install clang-format cppcheck yq

# Check installation
clang-format --version
cppcheck --version
yq --version
```

### **Pre-Commit Hooks (Optional)**

Create `.git/hooks/pre-commit`:

```bash
#!/bin/bash
# Run local CI tests before commit
./scripts/test-ci-locally.sh
```

## 📊 **Monitoring Workflow Results**

### **GitHub Interface:**

1. **Actions Tab**: View all workflow runs
2. **Commit Status**: See checkmarks on commits
3. **PR Checks**: View results on pull requests
4. **Security Tab**: Monitor security alerts

### **Badges in README:**

The badges will automatically update:
- [![CI](https://github.com/user/repo/workflows/CI/badge.svg)](link) - Shows build status
- [![CodeQL](https://github.com/user/repo/workflows/CodeQL/badge.svg)](link) - Shows security status

## 🚀 **Testing Strategy**

### **Phase 1: Initial Testing**
```bash
# 1. Run local tests
./scripts/test-ci-locally.sh

# 2. Create test branch
git checkout -b add-github-workflows

# 3. Commit and push
git add .
git commit -m "feat: add GitHub Actions workflows"
git push origin add-github-workflows

# 4. Watch GitHub Actions run
```

### **Phase 2: Comprehensive Testing**
```bash
# 1. Test different scenarios
git checkout -b test-edge-cases

# 2. Make intentional changes to test CI
echo "// Test comment" >> src/main.cpp
git add . && git commit -m "test: trigger CI with minor change"
git push origin test-edge-cases

# 3. Create PR to see full CI pipeline
```

### **Phase 3: Release Testing**
```bash
# 1. Merge to main
git checkout master
git merge add-github-workflows

# 2. Create release tag
git tag v1.0.0
git push origin v1.0.0

# 3. Watch release workflow create binaries
```

## 🎯 **Expected Timeline**

| Workflow | Duration | Triggers |
|----------|----------|----------|
| **CI Pipeline** | 8-12 minutes | Every push/PR |
| **CodeQL Security** | 3-5 minutes | Push/PR + Weekly |
| **Release Build** | 10-15 minutes | Version tags |

## 📋 **Testing Checklist**

Before pushing workflows to GitHub:

- [ ] ✅ Local CI test script passes
- [ ] ✅ All executables build successfully  
- [ ] ✅ Test suite runs without errors
- [ ] ✅ Documentation files are complete
- [ ] ✅ Workflow YAML files are valid
- [ ] ✅ .gitignore includes CI artifacts
- [ ] ✅ README badges are configured

## 🎉 **Ready to Test!**

Your workflows are ready for testing! I recommend starting with **Method A** (feature branch) to safely test the workflows without affecting your main branch.

Would you like me to help you:
1. **Run the local CI test** to validate everything locally first?
2. **Create a test branch** and push to GitHub?
3. **Set up any additional testing tools**?

The workflows will provide immediate feedback and make your project look incredibly professional! 🚀