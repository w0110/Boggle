#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_GRID_SIZE 10
char originalGrid[MAX_GRID_SIZE][MAX_GRID_SIZE];

void displayGrid(char grid[MAX_GRID_SIZE][MAX_GRID_SIZE], int size) {
    for (int i = 0; i < size * 2 + 3; i++) {
        printf("+");
    }
    printf("\n");
    
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (j % size == 0) {
                printf("| ");
            }
            printf("%c ", grid[i][j]);
        }
        printf("|\n");
    }

    for (int i = 0; i < size * 2 + 3; i++) {
        printf("+");
    }
    printf("\n");
}

// Function to perform a depth-first search for the word in the grid
int dfsSearch(char grid[MAX_GRID_SIZE][MAX_GRID_SIZE], char originalGrid[MAX_GRID_SIZE][MAX_GRID_SIZE], 
              int size, char* word, int row, int col, int wordIndex) {
    if (row < 0 || row >= size || col < 0 || col >= size) {
        return 0;
    }

    // Check if the current letter in the grid matches the current letter in the word
    if (grid[row][col] != word[wordIndex]) {
        return 0;
    }

    // Mark the current letter as used
    char temp = grid[row][col];
    grid[row][col] = '\0';

    // Check if the entire word has been found
    if (word[wordIndex + 1] == '\0') {
        // Convert the found word in the grid to uppercase
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (grid[i][j] == '\0') {
                    originalGrid[i][j] = toupper(originalGrid[i][j]);
                }
            }
        }
        return 1;
    }

    // Recursively search
    if (dfsSearch(grid, originalGrid, size, word, row - 1, col, wordIndex + 1) ||
        dfsSearch(grid, originalGrid, size, word, row + 1, col, wordIndex + 1) ||
        dfsSearch(grid, originalGrid, size, word, row, col - 1, wordIndex + 1) ||
        dfsSearch(grid, originalGrid, size, word, row, col + 1, wordIndex + 1) ||
        dfsSearch(grid, originalGrid, size, word, row - 1, col - 1, wordIndex + 1) ||
        dfsSearch(grid, originalGrid, size, word, row - 1, col + 1, wordIndex + 1) ||
        dfsSearch(grid, originalGrid, size, word, row + 1, col - 1, wordIndex + 1) ||
        dfsSearch(grid, originalGrid, size, word, row + 1, col + 1, wordIndex + 1)) {

        return 1;
    }

    grid[row][col] = temp;
    return 0;
}

// Function to search for a word in the grid
int searchWord(char grid[MAX_GRID_SIZE][MAX_GRID_SIZE], int size, char* word) {
    // Copy original grid
    memcpy(originalGrid, grid, sizeof(originalGrid)); 

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (dfsSearch(grid, originalGrid, size, word, row, col, 0)) {
                displayGrid(originalGrid, size);
                return 1; // Found
            }
        }
    }
    return 0; // Not found
}

int main(int argc, char* argv[]) {
    int size = 4; // Default grid size = 4
    char grid[MAX_GRID_SIZE][MAX_GRID_SIZE] = {0};
    char* word = NULL;

    // Find out size first or just use size as 4 
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0) {
            i++;
            if (i < argc) {
                size = atoi(argv[i]);

                if (size < 1 || size > 10) {
                    fprintf(stderr, "Invalid grid size. It should be between 1 and 10.\n");
                    return 84;
                }
            } else {
                fprintf(stderr, "Error: Missing grid size argument.\n");
                return 84;
            }
        }
    }

    // Process command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-g") == 0) {
            i++;
            if (i < argc && strlen(argv[i]) == size * size) {
                for (int j = 0; j < size; j++) {
                    for (int k = 0; k < size; k++) {
                        grid[j][k] = argv[i][j * size + k];
                    }
                }
            } else {
                fprintf(stderr, "Error: Invalid grid format.\n");
                return 84;
            }
        } else if (strcmp(argv[i], "-s") == 0) {
            i++;
            continue;
        } else if (strcmp(argv[i], "-w") == 0) {
            i++;
            if (i < argc) {
                word = argv[i];
            } else {
                fprintf(stderr, "Error: Missing word argument.\n");
                return 84;
            }
        } else {
            fprintf(stderr, "Error: Invalid option: %s\n", argv[i]);
            return 84;
        }
    }

    // Check if there is input from standard input
    if (word || isatty(fileno(stdin)) == 0) {
        if (word) {
            int found = searchWord(grid, size, word);
            if (found) {
                exit(0);
            } else {
                printf("The word \"%s\" is not in the grid.\n", word);
            }
        } else {
            // Read input from standard input
            char userInput[100];
            if (fgets(userInput, sizeof(userInput), stdin) != NULL) {
                userInput[strcspn(userInput, "\n")] = '\0';
                int found = searchWord(grid, size, userInput);
                if (found) {
                    exit(0);
                } else {
                    displayGrid(grid, size);
                    printf("> The word \"%s\" is not in the grid.\n> ", userInput);
                }
            }
        }
    } else {
        // If no word provided, enter interactive mode
        char userInput[100];
        displayGrid(grid, size);
        printf("> ");
        while (1) {
            if (fgets(userInput, sizeof(userInput), stdin) != NULL) {
                userInput[strcspn(userInput, "\n")] = '\0';
                int found = searchWord(grid, size, userInput);
                if (found) {
                    exit(0);
                } else {
                    printf("The word \"%s\" is not in the grid.\n", userInput);
                }
            }
            printf("> ");
        }
    }

    return 0;
}