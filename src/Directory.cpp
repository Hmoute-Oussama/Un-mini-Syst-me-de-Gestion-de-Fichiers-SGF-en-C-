#include "Directory.hpp"

Directory::Directory(const string& path) : path_(path) {
    size_t lastSlash = path.find_last_of("/\\");
    name_ = (lastSlash == string::npos || lastSlash == path.size() - 1)
                ? path
                : path.substr(lastSlash + 1);
    load();
}

void Directory::load() {
    files_.clear();
    subDirectories_.clear();

    DIR* dir = opendir(path_.c_str());
    if (!dir) return;   // Directory inaccessible

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        string entryName = entry->d_name;

        // Ignore "." and ".."
        if (entryName == "." || entryName == "..") continue;

        string fullPath = path_ + "/" + entryName;

        if (entry->d_type == DT_REG) {
            // Regular file
            try { files_.emplace_back(fullPath); } catch (...) {}
        }
        else if (entry->d_type == DT_DIR) {
            // Subdirectory : store only the path
            subDirectories_.push_back(fullPath);
        }
        else if (entry->d_type == DT_UNKNOWN) {
            // Some filesystems don't fill d_type
            // → fallback with stat()
            struct stat st;
            if (stat(fullPath.c_str(), &st) == 0) {
                if (S_ISREG(st.st_mode))
                    try { files_.emplace_back(fullPath); } catch (...) {}
                else if (S_ISDIR(st.st_mode))
                    subDirectories_.push_back(fullPath);
            }
        }
    }
    closedir(dir);
}

vector<File> Directory::filter(function<bool(const File&)> predicate) const {
    vector<File> result;
    for (const File& f : files_)
        if (predicate(f))
            result.push_back(f);
    return result;
}

void Directory::sort(function<bool(const File&, const File&)> comparator) {
    std::sort(files_.begin(), files_.end(), comparator);
}

vector<File> Directory::search(const string& name) const {
    auto matchName = [&name](const File& f) {
        return f.getName().find(name) != string::npos;
    };
    return filter(matchName);
}

void Directory::list() const {
    printf("\n📁  %s/\n", path_.c_str());
    printf("  %-30s %-8s %-10s\n", "Nom", "Ext", "Taille");
    printf("  %s\n", string(50, '-').c_str());

    if (files_.empty() && subDirectories_.empty()) {
        printf("  (dossier vide)\n");
        return;
    }

    for (const string& sub : subDirectories_)
        printf("  📁  %s/\n", sub.c_str());

    for (const File& f : files_)
        f.display();

    printf("\n  %zu fichier(s), %zu sous-dossier(s)\n",
           files_.size(), subDirectories_.size());
}

const string& Directory::getPath() const {
    return path_;
}

const string& Directory::getName() const {
    return name_;
}

const vector<File>& Directory::getFiles() const {
    return files_;
}

vector<File>& Directory::getFiles() {
    return files_;
}

const vector<string>& Directory::getSubDirs() const {
    return subDirectories_;
}
