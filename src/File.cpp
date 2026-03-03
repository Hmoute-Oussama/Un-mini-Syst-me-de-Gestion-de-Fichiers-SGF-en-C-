#include "File.hpp"
#include <cstdio>

void File::loadMetaData() {
    try {
        FileHandle filehandle(path_, "r");
        fseek(filehandle.get(), 0, SEEK_END);
        double s = ftell(filehandle.get());
        size_ = (s > 0) ? static_cast<size_t>(s) : 0;
    } catch (...) {
        size_ = 0;  // file inaccessible : size = 0
    }
}

File::File(const string& fullPath) : size_(0), path_(fullPath) {
    //   file name
    size_t lastSlash = fullPath.find_last_of("/\\");
    name_ = (lastSlash == string::npos) ? fullPath : fullPath.substr(lastSlash + 1);
    
    //  file extension
    size_t lastDot = name_.find_last_of(".");
    extension_ = (lastDot == string::npos) ? "" : name_.substr(lastDot + 1);
    
    loadMetaData();
}

string File::read() const {
    FileHandle filehandle(path_, "r");
    // We want to know the file size to allocate a buffer of the right size
    fseek(filehandle.get(), 0, SEEK_END);
    double fileSize = ftell(filehandle.get());
    fseek(filehandle.get(), 0, SEEK_SET);
    
    if (fileSize <= 0) {
        return "";
    }
    
    string content(fileSize, '\0');
    fread(&content[0], 1, fileSize, filehandle.get());
    return content;
}

void File::write(const string& content) {
    FileHandle filehandle(path_, "w");
    fwrite(content.c_str(), 1, content.size(), filehandle.get());
    size_ = content.size();
}

void File::append(const string& content) {
    FileHandle filehandle(path_, "a");
    fwrite(content.c_str(), 1, content.size(), filehandle.get());
    size_ += content.size();
}

void File::display() const {
    std::string sizeStr;
    if (size_ < 1024)
        sizeStr = std::to_string(size_) + " B";
    else if (size_ < 1024 * 1024)
        sizeStr = std::to_string(size_ / 1024) + " KB";
    else
        sizeStr = std::to_string(size_ / (1024 * 1024)) + " MB";

    printf("  %-30s %-8s %-10s\n",
        name_.c_str(),
        extension_.empty() ? "-" : extension_.c_str(),
        sizeStr.c_str());
}

const string& File::getName() const {
    return name_;
}

const string& File::getPath() const {
    return path_;
}

const string& File::getExtension() const {
    return extension_;
}

time_t File::getCreationDate() const {
    return creationDate_;
}

size_t File::getSize() const {
    return size_;
}
