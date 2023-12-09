#ifndef Sudoku_Solver
#define Sudoku_Solver

int solve_sudo(char *filpath, int oldarr[9][9]);

void printgrid(int arr[9][9]);

int SSudo(int arr[9][9], int row, int col);

int isOk(int arr[9][9], int row, int col, int num);

#endif
