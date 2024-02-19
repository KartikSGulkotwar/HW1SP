#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structure for storing data
struct listing {
    int id, host_id, minimum_nights, number_of_reviews, calculated_host_listings_count, availability_365;
    char *host_name, *neighbourhood_group, *neighbourhood, *room_type;
    float latitude, longitude, price;
};

// Function to parse each line and return the structure
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

// Comparison function for qsort to sort by host_name
int compareByName(const void *a, const void *b) {
    return strcmp(((struct listing*)a)->host_name, ((struct listing*)b)->host_name);
}

// Comparison function for qsort to sort by price
int compareByPrice(const void *a, const void *b) {
    return (((struct listing*)a)->price > ((struct listing*)b)->price) - (((struct listing*)a)->price < ((struct listing*)b)->price);
}

int main() {
    // Open the input file
    FILE *inputFile = fopen("listings.csv", "r");
    if (inputFile == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read and ignore the header line
    char header[256];
    fgets(header, sizeof(header), inputFile);

    // Read the data into an array of structures
    struct listing listings[100]; // Assuming a maximum of 100 listings
    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), inputFile) != NULL && count < 100) {
        listings[count++] = getfields(line);
    }

    // Close the input file
    fclose(inputFile);

    // Sort by host_name
    qsort(listings, count, sizeof(struct listing), compareByName);

    // Open the output file for sorted by host_name
    FILE *outputByName = fopen("sorted_by_name.csv", "w");
    if (outputByName == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Write the sorted data to the output file
    for (int i = 0; i < count; i++) {
        fprintf(outputByName, "%d,%d,%s,%s,%s,%.6f,%.6f,%s,%.2f,%d,%d,%d,%d\n",
                listings[i].id, listings[i].host_id, listings[i].host_name, listings[i].neighbourhood_group,
                listings[i].neighbourhood, listings[i].latitude, listings[i].longitude, listings[i].room_type,
                listings[i].price, listings[i].minimum_nights, listings[i].number_of_reviews,
                listings[i].calculated_host_listings_count, listings[i].availability_365);
    }

    // Close the output file
    fclose(outputByName);

    // Sort by price
    qsort(listings, count, sizeof(struct listing), compareByPrice);

    // Open the output file for sorted by price
    FILE *outputByPrice = fopen("sorted_by_price.csv", "w");
    if (outputByPrice == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Write the sorted data to the output file
    for (int i = 0; i < count; i++) {
        fprintf(outputByPrice, "%d,%d,%s,%s,%s,%.6f,%.6f,%s,%.2f,%d,%d,%d,%d\n",
                listings[i].id, listings[i].host_id, listings[i].host_name, listings[i].neighbourhood_group,
                listings[i].neighbourhood, listings[i].latitude, listings[i].longitude, listings[i].room_type,
                listings[i].price, listings[i].minimum_nights, listings[i].number_of_reviews,
                listings[i].calculated_host_listings_count, listings[i].availability_365);
    }

    // Close the output file
    fclose(outputByPrice);

    return 0;
}
