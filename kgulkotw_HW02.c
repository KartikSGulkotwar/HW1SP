#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

// Defining a struct here to hold program options
typedef struct {
    int verbose_;
    long file_Size;
    char *search_String;
    int depth;
    char fileType; // 'f' for files, 'd' for directories, 0 if they are not set
} Options;

Options opts = {0, -1, NULL, -1, 0}; // Initializing all the options to default values
void parse_Options(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "vL:s:t:")) != -1) {
        switch (opt) {
            case 'v':
                opts.verbose_ = 1;
                break;
            case 'L':
                opts.file_Size = atol(optarg);
                break;
            case 's':
                opts.search_String = optarg;
                if (optind < argc && *argv[optind] != '-') {
                    opts.depth = atoi(argv[optind]);
                    optind++;
                }
                break;
            case 't':
                opts.fileType = optarg[0];
                break;
            default: /* '?' */
                fprintf(stderr, "Usage: %s [-v] [-L size] [-s string depth] [-t f|d] [directory]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
}
void list_Files(const char *startDir, int currentDepth) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char path[1024];

    if (!(dir = opendir(startDir))) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(path, sizeof(path), "%s/%s", startDir, entry->d_name);
        lstat(path, &statbuf);

        

        printf("%s\n", path); 

        if (S_ISDIR(statbuf.st_mode)) {
            list_Files(path, currentDepth + 1); 
        }
    }
    closedir(dir);
}
int main(int argc, char *argv[]) {
    parse_Options(argc, argv);

    const char *startDir = ".";
    if (argc > optind) {
        startDir = argv[optind];
    }

    list_Files(startDir, 0);
    return 0;
}
