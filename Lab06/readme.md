

C program to to read the input file listings.csv and write two new functions that will sort the
data based on the host_name and price columns. 
## Functionalities

Parsing CSV Data: The program reads each line from the `listings.csv` file and parses the data into a structure (`struct listing`). This structure contains various fields such as `id`, `host_name`, `neighbourhood_group`, etc.

.

Sorting: The program sorts the array of listings twice: once by host name and once by price. It uses the qsort function from the C standard library for sorting.

File Output: After sorting, the program writes the sorted listings to two separate files: `sortedByHost.csv` for the listings sorted by host name, and `sortedByPrice.csv` for the listings sorted by price.


## Compilation

To compile the program, use the following command:

gcc -o lab lab6.c

Link for the Repository: https://github.com/KartikSGulkotwar/HWSP/tree/main/Lab06
