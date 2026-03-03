#include "FileSystem.hpp"

FileSystem::FileSystem(const string& rootPath)
    : current_(rootPath), rootPath_(rootPath)
{}

void FileSystem::navigate(const string& path) {
    string targetPath;

    // Absolute path
    if (path[0] == '/')
        targetPath = path;
    // Go up one level ".."
    else if (path == "..") {
        size_t lastSlash = current_.getPath().find_last_of("/\\");
        if (lastSlash == string::npos || current_.getPath() == rootPath_) {
            cout << " Already at root.\n";
            return;
        }
        targetPath = current_.getPath().substr(0, lastSlash);
    }
    // Relative path
    else {
        targetPath = current_.getPath() + "/" + path;
    }

    struct stat st;
    if (stat(targetPath.c_str(), &st) != 0 || !S_ISDIR(st.st_mode)) {
        cout << " Directory not found : " << targetPath << "\n";
        return;
    }

    current_ = Directory(targetPath);
    cout << " Navigated to : " << targetPath << "\n";
}

void FileSystem::list() const {
    current_.list();
}

void FileSystem::search(const string& name) const {
    cout << "\n Searching for \"" << name << "\" in " << current_.getPath() << "\n";

    vector<File> results = current_.search(name);

    if (results.empty()) {
        cout << "  No file found.\n";
        return;
    }

    cout << "  " << results.size() << " result(s) :\n";
    for (const File& f : results)
        f.display();
}

void FileSystem::copyFile(const string& srcName, const string& dstName) {
    string srcPath = current_.getPath() + "/" + srcName;
    string dstPath = current_.getPath() + "/" + dstName;

     
    FileHandle src(srcPath, "rb");
    
    FileHandle dst(dstPath, "wb");

   
    char buffer[4096];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), src.get())) > 0)
        fwrite(buffer, 1, bytesRead, dst.get());

    cout << " Copied : " << srcName << " → " << dstName << "\n";
    current_.load();  
}

void FileSystem::deleteFile(const string& fileName) {
    string fullPath = current_.getPath() + "/" + fileName;

    if (remove(fullPath.c_str()) == 0) {
        cout << "  Deleted : " << fileName << "\n";
        current_.load();    
    } else {
        cout << " Unable to delete : " << fileName << "\n";
    }
}

void FileSystem::createFile(const string& fileName) {
    string fullPath = current_.getPath() + "/" + fileName;

    FileHandle fh(fullPath, "w");   
    cout << " File created : " << fileName << "\n";
    current_.load();
}

void FileSystem::printFile(const string& fileName) const {
    string fullPath = current_.getPath() + "/" + fileName;

    try {
        File f(fullPath);
        cout << "\n── Content of " << fileName << " ──\n";
        cout << f.read();
        cout << "\n────────────────────────────\n";
    } catch (const exception& e) {
        cout << " " << e.what() << "\n";
    }
}

void FileSystem::filterByExtension(const string& ext) const {
    // ankhdm b Lambda by value : 
    auto byExt = [ext](const File& f) {
        return f.getExtension() == ext;
    };

    vector<File> results = current_.filter(byExt);

    cout << "\n Files with extension \"" << ext << "\" :\n";
    if (results.empty()) {
        cout << "  No file found.\n";
        return;
    }
    for (const File& f : results)
        f.display();
}

void FileSystem::sortBySize() {
    current_.sort([](const File& a, const File& b) {
        return a.getSize() < b.getSize();
    });
    cout << " Files sorted by size.\n";
    current_.list();
}

void FileSystem::sortByName() {
    current_.sort([](const File& a, const File& b) {
        return a.getName() < b.getName();
    });
    cout << " Files sorted by name.\n";
    current_.list();
}

const Directory& FileSystem::getCurrentDir() const {
    return current_;
}

const string& FileSystem::getRootPath() const {
    return rootPath_;
}

const string& FileSystem::getCurrentPath() const {
    return current_.getPath();
}
