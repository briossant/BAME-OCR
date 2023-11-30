#include <stdio.h>
#include <stdlib.h>
#include "../NeuralNetwork/matrices/matrices.h"
//Checks whether it will be legal to put that number in the row and column
int valid(Matrix grid, int row, size_t col,size_t num) {

    //same number on similar row return false 
    for (int x = 0; x <= 8; x++)
        if (grid.mat[row][x] == num)
            return 0;

    //same number in similar column return false 
for (int x = 0; x <= 8; x++)
        if (grid.mat[x][col] == num)
            return 0;
// Check if we find the same num in the particular 3*3 matrix return false 
    int startRow = row - row % 3,
                 startCol = col - col % 3;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid.mat[i + startRow][j +
                          startCol] == num)
                return 0;
    return 1;//valid 
}


//this function uses the backtracking method 
int Solver (Matrix Input, size_t row, size_t col)
{
    if ( row==8 && col == 9)
    {
        return 1; //to avoid further backtracking
    }
    if (col ==9)
    {
        row++;
        col=0;
    }

    if (Input.mat[row][col] > 0)
        return Solver(Input, row, col + 1);
    for (int num = 1; num <= 9; num++) 
    {
        if (valid(Input, row, col, num)==1)
        {
            Input.mat[row][col]=num;
            if (Solver (Input, row, col+1)==1)
                return 1;//posibility with next column
        }
        else{
             Input.mat[row][col] = 0;
        }
    }
    return 0;
}


Matrix SudokuSolver(Matrix grid)
{
    if (Solver( grid, 0, 0)==0)
    {
        printf("There was no solution for this sudoku");
        exit(1);
    }
    return grid;
}


int main()
{
    // 0 means unassigned cells
double gr[81] = {
    3, 0, 6, 5, 0, 8, 4, 0, 0,
    5, 2, 0, 0, 0, 0, 0, 0, 0,
    0, 8, 7, 0, 0, 0, 0, 3, 1,
    0, 0, 3, 0, 1, 0, 0, 8, 0,
    9, 0, 0, 8, 6, 3, 0, 0, 5,
    0, 5, 0, 0, 9, 0, 6, 0, 0,
    1, 3, 0, 0, 0, 0, 2, 5, 0,
    0, 0, 0, 0, 0, 0, 0, 7, 4,
    0, 0, 5, 2, 0, 6, 3, 0, 0
};

double* g = &gr[0];

Matrix grid;
{
    grid.mat = array_to_matrix (g, 9, 9);
 grid.w= 9;
    grid.h=9;
    grid.label= "Solver";
}
    MatPrint( grid);
    return 0;
}
