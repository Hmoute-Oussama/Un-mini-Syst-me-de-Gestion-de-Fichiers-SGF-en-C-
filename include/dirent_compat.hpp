#pragma once

#ifdef _WIN32
    #include <windows.h>
    #include <cstring>
    
    struct dirent {
        char d_name[MAX_PATH];
        int d_type;
    };

    #define DT_REG  0
    #define DT_DIR  1
    #define DT_UNKNOWN 2

    #define S_ISREG(mode) ((mode) == DT_REG)
    #define S_ISDIR(mode) ((mode) == DT_DIR)

    typedef struct {
        HANDLE handle;
        WIN32_FIND_DATAA data;
        struct dirent ent;
        int first;
    } _DIR;

    typedef _DIR DIR;

    inline DIR* opendir(const char* path) {
        _DIR* dir = (_DIR*)malloc(sizeof(_DIR));
        if (!dir) return NULL;
        
        char searchPath[MAX_PATH];
        strcpy_s(searchPath, sizeof(searchPath), path);
        strcat_s(searchPath, sizeof(searchPath), "\\*");
        
        dir->handle = FindFirstFileA(searchPath, &dir->data);
        if (dir->handle == INVALID_HANDLE_VALUE) {
            free(dir);
            return NULL;
        }
        
        dir->first = 1;
        return dir;
    }

    inline struct dirent* readdir(DIR* dirp) {
        _DIR* dir = (_DIR*)dirp;
        if (!dir) return NULL;
        
        if (dir->first) {
            dir->first = 0;
        } else {
            if (!FindNextFileA(dir->handle, &dir->data))
                return NULL;
        }
        
        strcpy_s(dir->ent.d_name, sizeof(dir->ent.d_name), 
                 dir->data.cFileName);
        
        dir->ent.d_type = (dir->data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
                         ? DT_DIR : DT_REG;
        
        return &dir->ent;
    }

    inline int closedir(DIR* dirp) {
        _DIR* dir = (_DIR*)dirp;
        if (!dir) return -1;
        
        FindClose(dir->handle);
        free(dir);
        return 0;
    }

#else
    #include <dirent.h>
    #include <sys/stat.h>
#endif
