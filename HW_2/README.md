# File Search

In this program it traverses directories starting from a specified path and lists files and directories based on the specified criteria. It supports various command-line options for detailed listings, file size filtering, search strings with depth, and file type filtering.

## Features

- **Verbose Listing (`-v`)**: Provides detailed information about each file, including size, permissions, and last access time.
- **Size Filter (`-L`)**: Lists files larger than a specified size.
- **Search String and Depth (`-s`)**: Lists files that contain the specified substring. Optionally, a depth can be specified to limit the search within the directory hierarchy.
- **File Type Filter (`-t`)**: Lists either files (`-t f`) or directories (`-t d`) exclusively.

## Compilation

To compile the program, enter the following command:
gcc -o search kgulkotw_HW02.c

## Link for Repository: 
 