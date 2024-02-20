#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defining the structure for listing
struct listing {
    int id, host_id, minimum_nights, number_of_reviews, calculated_host_listings_count, availability_365;
    char *host_name, *neighbourhood_group, *neighbourhood, *room_type;
    float latitude, longitude, price;
};

// Function to parse each line for returning a listing structure
struct listing getfields(char* line) {
    struct listing item;
    item.id = atoi(strtok(line, ","));
    item.host_id = atoi(strtok(NULL, ","));
    item.host_name = strdup(strtok(NULL, ","));
    item.neighbourhood_group = strdup(strtok(NULL, ","));
    item.neighbourhood = strdup(strtok(NULL, ","));
    item.latitude = atof(strtok(NULL, ","));
    item.longitude = atof(strtok(NULL, ","));
    item.room_type = strdup(strtok(NULL, ","));
    item.price = atof(strtok(NULL, ","));
    item.minimum_nights = atoi(strtok(NULL, ","));
    item.number_of_reviews = atoi(strtok(NULL, ","));
    item.calculated_host_listings_count = atoi(strtok(NULL, ","));
    item.availability_365 = atoi(strtok(NULL, ","));
    return item;
}

// Comparing function for qsort based on host_name
static int compareByHostName(const void *a, const void *b) {
    return strcmp(((struct listing *)a)->host_name, ((struct listing *)b)->host_name);
}

// Comparison function for qsort based on the price
static int compareByPrice(const void *a, const void *b) {
    return (((struct listing *)a)->price > ((struct listing *)b)->price) - (((struct listing *)a)->price < ((struct listing *)b)->price);
}

int main() {
    FILE *inputFile, *sortedByHostFile, *sortedByPriceFile;
    char line[1024];
    int numberOfListings = 0;

    // Opening the input file
    inputFile = fopen("listings.csv", "r");
    if (inputFile == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Counting the number of lines in the file
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        numberOfListings++;
    }
    rewind(inputFile); 

   
    struct listing *listings = malloc(numberOfListings * sizeof(struct listing));

    
    for (int i = 0; i < numberOfListings; i++) {
        fgets(line, sizeof(line), inputFile);
        listings[i] = getfields(line);
    }

    
    fclose(inputFile);

    
    qsort(listings, numberOfListings, sizeof(struct listing), compareByHostName);

    // Open a new file for sorted by host_name
    sortedByHostFile = fopen("sortedByHost.csv", "w");
    if (sortedByHostFile == NULL) {
        perror("Error opening sortedByHost file");
        free(listings);
        return EXIT_FAILURE;
    }

    // Write sorted data to the new file
    for (int i = 0; i < numberOfListings; i++) {
        fprintf(sortedByHostFile, "%d,%d,%s,%s,%s,%.6f,%.6f,%s,%.2f,%d,%d,%d,%d\n",
            listings[i].id, listings[i].host_id, listings[i].host_name, listings[i].neighbourhood_group,
            listings[i].neighbourhood, listings[i].latitude, listings[i].longitude, listings[i].room_type,
            listings[i].price, listings[i].minimum_nights, listings[i].number_of_reviews,
            listings[i].calculated_host_listings_count, listings[i].availability_365);
    }

    // Close the sortedByHost file
    fclose(sortedByHostFile);

    // Sort by price using qsort
    qsort(listings, numberOfListings, sizeof(struct listing), compareByPrice);

    // Open a new file for sorted by price
    sortedByPriceFile = fopen("sortedByPrice.csv", "w");
    if (sortedByPriceFile == NULL) {
        perror("Error opening sortedByPrice file");
        free(listings);
        return EXIT_FAILURE;
    }

    // Write sorted data to the new file
    for (int i = 0; i < numberOfListings; i++) {
        fprintf(sortedByPriceFile, "%d,%d,%s,%s,%s,%.6f,%.6f,%s,%.2f,%d,%d,%d,%d\n",
            listings[i].id, listings[i].host_id, listings[i].host_name, listings[i].neighbourhood_group,
            listings[i].neighbourhood, listings[i].latitude, listings[i].longitude, listings[i].room_type,
            listings[i].price, listings[i].minimum_nights, listings[i].number_of_reviews,
            listings[i].calculated_host_listings_count, listings[i].availability_365);
    }

    // Close the sortedByPrice file
    fclose(sortedByPriceFile);

    // Free allocated memory for the array
    free(listings);

    return EXIT_SUCCESS;
}
