#include "dir_con.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define NAME_MAX 255

int dir_read(const char* const path, char** files, int* count, const int max_count){
    DIR *dir = opendir(path);
    struct dirent *entry;
    int files_added = 0;
    char *subpath = (char*) malloc(sizeof(char) * (strlen(path) + NAME_MAX + 2));

    if (dir == NULL) {
        return -1;    
    }

    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, ".." ) == 0 || strcmp(entry->d_name, ".git" ) == 0) {
            continue;
        }
        if (entry->d_type == DT_DIR) {
            printf("Directory: %s\n", entry->d_name);
            sprintf(subpath, "%s/%s", path, entry->d_name);
            dir_read(subpath, files, count, max_count);
        } else {
            printf("File %3d: %s\n", ++files_added, entry->d_name);
            if (*count < max_count) {
                files[*count] = (char*) malloc(sizeof(char) * (strlen(path) + NAME_MAX + 2));
                sprintf(files[*count], "%s/%s", path, entry->d_name);
                (*count)++;
            }
        }
    }

    closedir(dir);
    free(subpath);

    return 0;
}