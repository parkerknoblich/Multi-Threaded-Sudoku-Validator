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
    for (int i = 0; i <= 8; i++) {                         // create row threads
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        parameters* data = (parameters*)malloc(sizeof(parameters));
        data->row = i;
        data->column = 8;
        pthread_create(&tid, &attr, checkRow, (void*) data);
        pthread_join(tid, NULL);
    }
    if (validFlag == 0) {
        printf("NOT VALID");
    } else {
        printf("VALID");
    }
}









//void* checkColumn(void* columnArgs) {
//    int allValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//    printf("ALL VALUES BEFORE = ");
//    for (int i = 0; i <= 9; i++) {
//        printf("%d ", allValues[i]);
//    }
//    printf("\n");
//    for (int i = 0; i <= ((parameters*) columnArgs)->row; i++) {
//        int currValue = grid[i][((parameters*) columnArgs)->column];
//        if (allValues[currValue] == 0) {
//            printf("GOOD: Row %d Column %d\n", i, ((parameters*) columnArgs)->column);
//            allValues[currValue] = 1;
//        } else {
//            printf("BAD: Row %d Column %d\n", i, ((parameters*) columnArgs)->column);
//            validFlag = 0;
//            pthread_exit(NULL);
//        }
//        printf("ALL VALUES AFTER (Row %d Column %d Value %d) = ", i, ((parameters*) columnArgs)->column, currValue);
//        for (int j = 0; j <= 9; j++) {
//            printf("%d ", allValues[j]);
//        }
//        printf("\n");
//    }
//    pthread_exit(NULL);
//}