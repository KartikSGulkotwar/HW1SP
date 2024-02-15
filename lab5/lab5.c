#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void traveseDirectory_(char *dirname, int level);

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <dirname>\n", argv[0]);
        exit(-1);
    }

    traveseDirectory_(argv[1], 0);

    return 0;
}

void traveseDirectory_(char *dirname, int level) {
    DIR *dir;
    struct dirent *dirent;

    if ((dir = opendir(dirname)) == NULL) {
        printf("Error opening directory '%s'\n", dirname);
        exit(-1);
    }

    int count = 1;

    while ((dirent = readdir(dir)) != NULL) {
        if (strcmp(dirent->d_name, ".") != 0 && strcmp(dirent->d_name, "..") != 0) {
            for (int i = 0; i < level; i++) {
                printf("  "); 
            }

            printf("[%d] %s\n", count, dirent->d_name);
            count++;

            
            if (dirent->d_type == DT_DIR) {
                
                char path[PATH_MAX];
                snprintf(path, sizeof(path), "%s/%s", dirname, dirent->d_name);

                
                traveseDirectory_(path, level + 1);
            }
        }
    }

    closedir(dir);
}
