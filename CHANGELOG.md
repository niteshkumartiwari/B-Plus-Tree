# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2024-09-20

### Added
- Initial B+ Tree implementation with modern C++17
- Comprehensive insert, search, and delete operations
- Memory-safe implementation with proper destructors
- Industry-standard project structure
- CMake build system with cross-platform support
- Comprehensive test suite with 8+ test scenarios
- Professional documentation and README
- Code formatting with clang-format
- Contribution guidelines and development setup

### Security
- Fixed format string vulnerability in fprintf calls
- Added proper input validation and error handling
- Implemented secure file operations with error checking

### Performance
- Optimized node splitting and merging algorithms
- Improved memory management with RAII principles
- Enhanced deletion operations with better borrowing logic

### Documentation
- Complete API documentation
- Usage examples and quick start guide
- Development and contribution guidelines
- Professional README with badges and clear structure

## [0.1.0] - Initial Release

### Added
- Basic B+ Tree implementation
- Insert, search, delete operations
- File-based data storage simulation
- Simple test cases
- Basic documentation

### Known Issues
- Memory leaks in deletion operations
- Security vulnerability in string formatting
- Inconsistent error handling
- Limited test coverage