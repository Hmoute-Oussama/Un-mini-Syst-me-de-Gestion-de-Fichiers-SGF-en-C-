#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "Directory.hpp"

using namespace std;

class FileSystem {
private:
    Directory current_;
    string rootPath_;

public:
    // Constructor
    explicit FileSystem(const string& rootPath);

    // Navigation
    // Change current directory
    void navigate(const string& path);

    // List current directory
    void list() const;

    // Search
    void search(const string& name) const;

    // Copy a file
    void copyFile(const string& srcName, const string& dstName);

    // Delete a file
    void deleteFile(const string& fileName);

    // Create an empty file
    void createFile(const string& fileName);

    // Print file contents
    void printFile(const string& fileName) const;

    // Filter by extension
    void filterByExtension(const string& ext) const;

    // Sort by size
    void sortBySize();

    // Sort by name
    void sortByName();

    // Getters
    const Directory& getCurrentDir() const;
    const string& getRootPath() const;
    const string& getCurrentPath() const;
};
