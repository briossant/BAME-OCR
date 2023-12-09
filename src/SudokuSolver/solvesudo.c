#include "Sudoku_Solver.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>

int solve_sudo(char *filpath, int oldarr[9][9]) {
    int arr[9][9];
    int row = 0;
    int col = 0;

    FILE *f;
    char c;
    f = fopen(filpath, "r");
    if (f == NULL) {
        errx(1, "Can't open the provided file");
    }
    while (row < 9 && col < 9) {
        c = fgetc(f);
        if (c == EOF) {
            errx(1, "Pointer err in the solver");
        }
        if (c == '.' || c == '0') {
            arr[col][row] = 0;
            row++;
        }
        if (c >= '1' && c <= '9') {
            arr[col][row] = c - '0';
            row++;
        }
        if (row >= 9) {
            row = 0;
            col++;
        }
    }
    fclose(f);

    // printgrid(arr);
    // printf("\n\n\n");

    if (SSudo(arr, 0, 0) != 1) {
        printf("Please enter a right grid\n");
        return 0;
    } else {
        for (int x = 0; x < 9; x++) {
            for (int y = 0; y < 9; y++) {
                oldarr[x][y] = arr[x][y];
            }
        }
        printgrid(oldarr);
    }
    return 1;
}

/*int arr[9][9];



for (size_t i =0; i<strlen(argv[1]); i++)
        {
                if (argv[1][i]=='}')
                {
                        row=0;
                        col++;
                        continue;
                }
                if (argv[1][i]>='0' && argv[1][i]<='9')
                {
                        arr[row][col] = argv[1][i]-'0';
                        row++;
                }
        }
*/
