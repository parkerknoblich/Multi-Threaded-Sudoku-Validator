#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int validFlag = 1;

typedef struct {
    int row;
    int column;
} parameters;

int grid[9][9] = {
        {6, 2, 4, 5, 3, 9, 1, 8, 7},
        {5, 1, 9, 7, 2, 8, 6, 3, 4},
        {8, 3, 7, 6, 1, 4, 2, 9, 5},
        {1, 4, 3, 8, 6, 5, 7, 2, 9},
        {9, 5, 8, 2, 4, 7, 3, 6, 1},
        {7, 6, 2, 3, 9, 1, 4, 5, 8},
        {3, 7, 1, 9, 5, 6, 8, 4, 2},
        {4, 9, 6, 1, 8, 2, 5, 7, 3},
        {2, 8, 5, 4, 7, 3, 9, 1, 6}
};

void* checkColumn(void* columnArgs) {
    int allValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i <= ((parameters*) columnArgs)->row; i++) {
        int currValue = grid[i][((parameters*) columnArgs)->column];
        if (allValues[currValue] == 0) {
            allValues[currValue] = 1;
        } else {
            validFlag = 0;
            pthread_exit(NULL);
        }
    }
    pthread_exit(NULL);
}

void* checkRow(void* rowArgs) {
    int allValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i <= ((parameters*) rowArgs)->column; i++) {
        int currValue = grid[((parameters*) rowArgs)->row][i];
        if (allValues[currValue] == 0) {
            allValues[currValue] = 1;
        } else {
            validFlag = 0;
            pthread_exit(NULL);
        }
    }
    pthread_exit(NULL);
}

void* checkSubGrid(void* subGridArgs) {
    int allValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = ((parameters*) subGridArgs)->row - 2; i <= ((parameters*) subGridArgs)->row; i++) {
        for (int j = ((parameters*) subGridArgs)->column - 2; j <= ((parameters*) subGridArgs)->column; j++) {
            int currValue = grid[i][j];
            if (allValues[currValue] == 0) {
                allValues[currValue] = 1;
            } else {
                validFlag = 0;
                pthread_exit(NULL);
            }
        }
    }
    pthread_exit(NULL);
}

int main() {
    for (int i = 0; i <= 8; i++) {                          // create column threads
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        parameters* data = (parameters*)malloc(sizeof(parameters));
        data->row = 8;
        data->column = i;
        pthread_create(&tid, &attr, checkColumn, (void*) data);
        pthread_join(tid, NULL);
    }
    for (int i = 0; i <= 8; i++) {                          // create row threads
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        parameters* data = (parameters*)malloc(sizeof(parameters));
        data->row = i;
        data->column = 8;
        pthread_create(&tid, &attr, checkRow, (void*) data);
        pthread_join(tid, NULL);
    }
    for (int i = 0; i <= 8; i += 3) {                      // create subgrid threads
        for (int j = 0; j <= 8; j += 3) {
            pthread_t tid;
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            parameters* data = (parameters*)malloc(sizeof(parameters));
            data->row = j + 2;
            data->column = i + 2;
            pthread_create(&tid, &attr, checkSubGrid, (void*) data);
            pthread_join(tid, NULL);
        }
    }
    printf("Sudoku Puzzle is: ");
    if (validFlag == 0) {
        printf("INVALID");
    } else {
        printf("VALID");
    }
    printf("\n");
}