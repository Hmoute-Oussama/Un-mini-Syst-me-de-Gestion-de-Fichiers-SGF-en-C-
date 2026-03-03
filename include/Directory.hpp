#pragma once
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <sys/stat.h>
#include "dirent_compat.hpp"
#include "File.hpp"

using namespace std;

class Directory {
private:
    string name_;
    string path_;
    vector<File> files_;
    vector<string> subDirectories_;

public:
    explicit Directory(const string& path);

    // Load directory contents
    void load();

    // Filter with lambda
    vector<File> filter(function<bool(const File&)> predicate) const;

    // Sort with lambda
    void sort(function<bool(const File&, const File&)> comparator);

    // Search by name
    vector<File> search(const string& name) const;

    // Display contents
    void list() const;

    // Getters
    const string& getPath() const;
    const string& getName() const;
    const vector<File>& getFiles() const;
    vector<File>& getFiles();
    const vector<string>& getSubDirs() const;
};
