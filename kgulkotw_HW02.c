#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

typedef struct {
    int verbose;          // -v
    long minSize;         // -L
    char *searchString;   // -s
    int searchDepth;      // depth for -s
    char fileType;        // -t, 'f' for files, 'd' for directories
} Options;

Options opts = {0, -1, NULL, -1, 0};

void parseOptions(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "vL:s:t:")) != -1) {
        switch (opt) {
            case 'v':
                opts.verbose = 1;
                break;
            case 'L':
                opts.minSize = atol(optarg);
                break;
            case 's':
                opts.searchString = optarg;
                opts.searchDepth = 0; // Default depth
                // Check for the next argument to be a depth, if it exists and is a number
                if (optind < argc && argv[optind] != NULL && isdigit(argv[optind][0])) {
                    opts.searchDepth = atoi(argv[optind]);
                    optind++;
                }
                break;
            case 't':
                opts.fileType = optarg[0];
                break;
            default:
                fprintf(stderr, "Usage: %s [-v] [-L minSize] [-s searchString [depth]] [-t f|d] [directory]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
}


void printFileInfo(struct stat *statbuf, const char *name, const char *linkTarget, int depth, char isDirectory) {
    // Generate indentation based on depth
    char indent[256] = {0};
    for (int i = 0; i < depth && i < sizeof(indent) - 1; i++) {
        strcat(indent, "\t"); // Use "\t" for tabs or "  " for spaces
    }

    long size = isDirectory ? 0 : statbuf->st_size;
    if (opts.verbose) {
        printf("%s%s // Size: %ld bytes, Permissions: %o, Last Access: %s\n",
               indent, name, size, statbuf->st_mode & 0777,
               ctime(&statbuf->st_atime));
    } else {
        printf("%s%s%s\n", indent, name, linkTarget ? linkTarget : "");
    }
}

void traverse(const char *dirPath, int currentDepth) {
    DIR *dir = opendir(dirPath);
    struct dirent *entry;
    struct stat statbuf;
    char fullPath[PATH_MAX];

    if (!dir) {
        perror("opendir");
        return;
    }

    char indent[256] = {0};
    for (int i = 0; i < currentDepth && i < sizeof(indent) - 1; i++) {
        strcat(indent, "\t"); // Use "\t" for tabs or "  " for spaces
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;


        snprintf(fullPath, sizeof(fullPath), "%s/%s", dirPath, entry->d_name);
        lstat(fullPath, &statbuf);

        // Apply -L size filter
        if (opts.minSize != -1 && statbuf.st_size < opts.minSize) continue;

        // Apply -s search string and depth filter
        if (opts.searchString != NULL && (strstr(entry->d_name, opts.searchString) == NULL || currentDepth > opts.searchDepth)) continue;

        // Determine if the entry is a directory or a regular file
        int isDirectory = S_ISDIR(statbuf.st_mode);

        // Apply -t file type filter
        if ((opts.fileType == 'f' && !S_ISREG(statbuf.st_mode)) || (opts.fileType == 'd' && !isDirectory)) continue;

        // Print directories or files based on the verbose flag and fileType
        if (isDirectory) {
            if (opts.verbose || opts.fileType == 'd') {
                printFileInfo(&statbuf, entry->d_name, NULL, currentDepth, 1);
            }
            if (currentDepth < opts.searchDepth || opts.searchDepth == -1) {
                // printf("%s%s:\n", indent, entry->d_name); // Print the directory name with indentation
                traverse(fullPath, currentDepth + 1); // Recurse into the directory
            }
        } else {
            // It's a file, check if we're listing files
            if (opts.fileType != 'd') {
                printFileInfo(&statbuf, entry->d_name, NULL, currentDepth, 0);
            }
        }
    }

    closedir(dir);
}


int main(int argc, char *argv[]) {
    parseOptions(argc, argv);

    char *startDir = ".";
    if (argc > optind) {
        startDir = argv[optind];
    }

    traverse(startDir, 0);
    return 0;
}