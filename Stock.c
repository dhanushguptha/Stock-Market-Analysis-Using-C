#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_RECORDS 1000

// Struct to hold stock data
typedef struct {
    char date[20];
    double open;
    double high;
    double low;
    double close;
    long volume;
} StockData;

// Function prototypes
int loadStockData(const char *filePath, StockData *stockData, int maxRecords);
void displayStockData(const StockData *stockData, int count);
void calculateMovingAverage(const StockData *stockData, int count, int window);
StockData findHighestHigh(const StockData *stockData, int count);
StockData findLowestLow(const StockData *stockData, int count);

// Main program
int main() {
    StockData stockData[MAX_RECORDS];
    int recordCount = 0;
    char filePath[] = "stock_data.csv"; // Path to the CSV file

    // Load stock data
    recordCount = loadStockData(filePath, stockData, MAX_RECORDS);
    if (recordCount == 0) {
        printf("Failed to load stock data. Check the file path and format.\n");
        return 1;
    }

    int choice;
    do {
        printf("\n--- Stock Market Analyzer ---\n");
        printf("1. Display Stock Data\n");
        printf("2. Calculate Moving Average\n");
        printf("3. Find Highest High\n");
        printf("4. Find Lowest Low\n");
        printf("5. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayStockData(stockData, recordCount);
                break;
            case 2: {
                int window;
                printf("Enter window size for moving average: ");
                scanf("%d", &window);
                if (window > 0 && window <= recordCount) {
                    calculateMovingAverage(stockData, recordCount, window);
                } else {
                    printf("Invalid window size.\n");
                }
                break;
            }
            case 3: {
                StockData highest = findHighestHigh(stockData, recordCount);
                printf("Highest High:\n");
                printf("%s\t%.2f\t%.2f\t%.2f\t%.2f\t%ld\n",
                       highest.date, highest.open, highest.high, highest.low, highest.close, highest.volume);
                break;
            }
            case 4: {
                StockData lowest = findLowestLow(stockData, recordCount);
                printf("Lowest Low:\n");
                printf("%s\t%.2f\t%.2f\t%.2f\t%.2f\t%ld\n",
                       lowest.date, lowest.open, lowest.high, lowest.low, lowest.close, lowest.volume);
                break;
            }
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}

// Function to load stock data from a CSV file
int loadStockData(const char *filePath, StockData *stockData, int maxRecords) {
    FILE *file = fopen(filePath, "r");
    if (!file) {
        perror("Error opening file");
        return 0;
    }

    char line[MAX_LINE];
    int count = 0;

    // Skip the header line
    fgets(line, MAX_LINE, file);

    // Read each line and parse the stock data
    while (fgets(line, MAX_LINE, file) && count < maxRecords) {
        StockData *record = &stockData[count];
        sscanf(line, "%[^,],%lf,%lf,%lf,%lf,%ld",
               record->date, &record->open, &record->high, &record->low, &record->close, &record->volume);
        count++;
    }

    fclose(file);
    return count;
}

// Function to display all stock data
void displayStockData(const StockData *stockData, int count) {
    printf("\nDate\t\tOpen\tHigh\tLow\tClose\tVolume\n");
    printf("---------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%s\t%.2f\t%.2f\t%.2f\t%.2f\t%ld\n",
               stockData[i].date, stockData[i].open, stockData[i].high, stockData[i].low, stockData[i].close, stockData[i].volume);
    }
}

// Function to calculate and display moving averages
void calculateMovingAverage(const StockData *stockData, int count, int window) {
    printf("\nMoving Averages (Window size: %d):\n", window);
    for (int i = 0; i <= count - window; i++) {
        double sum = 0.0;
        for (int j = i; j < i + window; j++) {
            sum += stockData[j].close;
        }
        double average = sum / window;
        printf("%s: %.2f\n", stockData[i + window - 1].date, average);
    }
}

// Function to find the day with the highest high
StockData findHighestHigh(const StockData *stockData, int count) {
    StockData highest = stockData[0];
    for (int i = 1; i < count; i++) {
        if (stockData[i].high > highest.high) {
            highest = stockData[i];
        }
    }
    return highest;
}

// Function to find the day with the lowest low
StockData findLowestLow(const StockData *stockData, int count) {
    StockData lowest = stockData[0];
    for (int i = 1; i < count; i++) {
        if (stockData[i].low < lowest.low) {
            lowest = stockData[i];
        }
    }
    return lowest;
}
