#include "Sudoku_Solver.h"
#include <stdio.h>

#define N 9

void printgrid(int arr[N][N])
{
	for (int i=0; i<N; i++) 
	{
		if (i%3==0 && i!=0) 
		{
			printf("\n");
		}
		for (int j=0; j<N; j++) 
		{
			if (j%3==0 && j!=0) 
			{
				printf(" ");
			}
			printf("%d ",arr[i][j]);
		}
		printf("\n");
		
	}
}
