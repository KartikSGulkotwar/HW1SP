/*
Name: Kartik Gulkotwar
BlazerId: kgulkotw
Project #: HW3
To compile: <instructions for compiling the
program> To run: <instructions to run the
program>
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <sys/wait.h>
#include <limits.h>

#define ARG_MAX 4096  


typedef struct {
    int verbose;          // -v
    long minSize;         // -L
    char *searchString;   // -s
    int searchDepth;      // depth for -s
    char fileType;        // -t, 'f' for files, 'd' for directories
    char *execCommand;    // -e
    char *execCommandAll; // -E
} Options;

Options opts = {0, -1, NULL, -1, 0, NULL, NULL};

void parse_Options_(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "vL:s:t:e:E:")) != -1) {
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
                // Checking for the next argument to be a depth, if it exists and is a number
                if (optind < argc && argv[optind] != NULL && isdigit(argv[optind][0])) {
                    opts.searchDepth = atoi(argv[optind]);
                    optind++;
                }
                break;
            case 't':
                opts.fileType = optarg[0];
                break;
            case 'e':
                opts.execCommand = optarg;
                break;
            case 'E':
                opts.execCommandAll = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s [-v] [-L minSize] [-s searchString [depth]] [-t f|d] [-e \"<command>\"] [-E \"<command>\"] [directory]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
}

void executeCommand(const char *filePath) {
    if (opts.execCommand) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            char command[PATH_MAX + 50];  
            snprintf(command, sizeof(command), "%s %s", opts.execCommand, filePath);
            execl("/bin/sh", "sh", "-c", command, (char *)NULL);

            
            perror("execl");
            exit(EXIT_FAILURE);
        } else {
            
            waitpid(pid, NULL, 0);
        }
    }
}

void executeCommandAll(char *fileList[]) {
    if (opts.execCommandAll) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            
            char command[PATH_MAX + 50];  
            snprintf(command, sizeof(command), "%s", opts.execCommandAll);

            
            for (int i = 0; fileList[i] != NULL; i++) {
                char absolutePath[PATH_MAX];
                if (realpath(fileList[i], absolutePath) != NULL) {
                    snprintf(command + strlen(command), sizeof(command) - strlen(command), " %s", absolutePath);
                }
            }

            execl("/bin/sh", "sh", "-c", command, (char *)NULL);

            
            perror("execl");
            exit(EXIT_FAILURE);
        } else {
            
            waitpid(pid, NULL, 0);
        }
    }
}


void printFileInfo_(struct stat *statbuf, const char *name, const char *linkTarget, int depth, char isDirectory) {
    
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

void traverse(const char *dirPath, int currentDepth, char *fileList[]) {
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
        strcat(indent, "\t"); 
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        snprintf(fullPath, sizeof(fullPath), "%s/%s", dirPath, entry->d_name);
        lstat(fullPath, &statbuf);

        
        if (opts.minSize != -1 && statbuf.st_size < opts.minSize) continue;

        
        if (opts.searchString != NULL && (strstr(entry->d_name, opts.searchString) == NULL || currentDepth > opts.searchDepth)) continue;

        
        int isDirectory = S_ISDIR(statbuf.st_mode);

        
        if ((opts.fileType == 'f' && !S_ISREG(statbuf.st_mode)) || (opts.fileType == 'd' && !isDirectory)) continue;

        
        if (isDirectory) {
            if (opts.verbose || opts.fileType == 'd') {
                printFileInfo_(&statbuf, entry->d_name, NULL, currentDepth, 1);
            }
            if (currentDepth < opts.searchDepth || opts.searchDepth == -1) {
                traverse(fullPath, currentDepth + 1, fileList); // Recurse into the directory
            }
        } else {
            
            if (opts.fileType != 'd') {
                printFileInfo_(&statbuf, entry->d_name, NULL, currentDepth, 0);
                
                if (opts.execCommandAll) {
                    int i;
                    for (i = 0; fileList[i] != NULL; i++)
                        ;
                    fileList[i] = strdup(fullPath);
                } else {
                    
                    executeCommand(fullPath);
                }
            }
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    parse_Options_(argc, argv);

    char *startDir = ".";
    if (argc > optind) {
        startDir = argv[optind];
    }

    char *fileList[ARG_MAX] = {0}; // File list for -E option
    traverse(startDir, 0, fileList);

    // Execute the command for -E option
    executeCommandAll(fileList);

    // Free allocated memory for file list
    for (int i = 0; fileList[i] != NULL; i++) {
        free(fileList[i]);
    }

    return 0;
}
