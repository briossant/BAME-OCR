#include "Sudoku_Solver.h"
#include <complex.h>
#include <stdlib.h>

int isOk(int arr[9][9], int row, int col, int num)
{
    for (int i = 0; i <= 8; i++)
        if (arr[row][i] == num)
            return 0;

    for (int j = 0; j <= 8; j++)
        if (arr[j][col] == num)
            return 0;

    int sRow = row - row % 3, sCol = col - col % 3;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (arr[i + sRow][j + sCol] == num)
                return 0;

    return 1;
}

int SSudo(int arr[9][9], int row, int col)
{

	if (row == 8 && col ==9)
		return 1;
	
	if (col==9) 
	{
		row++;
		col=0;
	}
	if (arr[row][col]>0) 
	{
		return SSudo(arr, row, col+1);
	}

	for (int ind = 1; ind<=9; ind++) 
	{
		if (isOk(arr, row, col, ind)==1) 
		{
			arr[row][col] = ind;
			
			if (SSudo(arr,row,col+1)==1)
				return 1;

			arr[row][col] = 0;
		}
	}
	return 0;
}
