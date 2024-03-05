# Custom File Explorer Utility

This is a custom command-line utility designed for exploring file systems and executing commands on files or directories based on specific criteria. It operates in Unix-like environments and provides detailed insights and control over file system navigation and manipulation.

## Features

- Verbose Output (-v):Displays detailed information about each item, including size, permissions, and last access time.
- Minimum Size (-L): Only shows files or directories larger than a specified size.
- Search String (-s): Filters results to only include files or directories containing a specified substring, with an optional search depth.
-File Type (-t): Limits the search to files or directories.
- Execute Command (-e): Executes a specified command on each matching file or directory.
- Execute Command on All (-E): Executes a specified command on all matching files or directories collectively.

## Compilation

gcc -o search kgulkotw_HW03.c

Link for Repository: 