#include "FileHandle.hpp"

FileHandle::FileHandle(const string& path, const string& mode) 
    : path_(path), handle_(nullptr) {
    handle_ = fopen(path.c_str(), mode.c_str());
    if (!handle_) {
        throw runtime_error("Failed to open file : " + path);
    }
}

FileHandle::~FileHandle() {
    if (handle_) {
        fclose(handle_);
        handle_ = nullptr;
    }
}

FileHandle::FileHandle(FileHandle&& other) noexcept 
    : handle_(other.handle_), path_(move(other.path_)) {
    other.handle_ = nullptr;
}

FileHandle& FileHandle::operator=(FileHandle&& other) noexcept {
    if (this != &other) {
        if (handle_) {
            fclose(handle_);
        }
        handle_ = other.handle_;
        path_ = move(other.path_);
        other.handle_ = nullptr;
    }
    return *this;
}

FILE* FileHandle::get() const {
    return handle_;
}

bool FileHandle::isOpen() const {
    return handle_ != nullptr;
}

const string& FileHandle::getPath() const {
    return path_;
}
