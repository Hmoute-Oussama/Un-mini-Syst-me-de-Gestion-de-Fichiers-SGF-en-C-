#pragma once
#include <ctime>
#include <string>
#include "FileHandle.hpp"

using namespace std;

class File {
private:
    string name_;
    string path_;
    string extension_;
    time_t creationDate_;
    size_t size_;
    
    void loadMetaData();

public:
    // Constructor
    File(const string& fullPath);
    
    // Read function to read the content of the file and return it as a string
    string read() const;
    
    // Write function to write a string to the file
    void write(const string& content);
    
    // Append function to append a string to the file
    void append(const string& content);
    
    // Display function
    void display() const;
    
    // Getters
    const string& getName() const;
    const string& getPath() const;
    const string& getExtension() const;
    time_t getCreationDate() const;
    size_t getSize() const;
};
