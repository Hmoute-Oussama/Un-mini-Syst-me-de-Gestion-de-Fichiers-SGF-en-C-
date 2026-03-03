#pragma once
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <string>

using namespace std;

class FileHandle {
    // RAII wrapper for file handling
    // Constructor opens the file, destructor releases it
private:
    FILE* handle_;
    string path_;

public: 
    FileHandle(const string& path, const string& mode);
    
    ~FileHandle();
    
    // Delete copy constructor and copy assignment operator
    FileHandle(const FileHandle& other) = delete;
    FileHandle& operator=(const FileHandle& other) = delete;
    
    // Move constructor and move assignment operator
    FileHandle(FileHandle&& other) noexcept;
    FileHandle& operator=(FileHandle&& other) noexcept;
    
    FILE* get() const;
    bool isOpen() const;
    const string& getPath() const;
};
