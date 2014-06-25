#include <iostream>

int * X; //solution array X[row]=col. Each value in the array is the column number of the ith row Queen. eg. X[4] = 5 is Queen at (4,5).
int solutionCount;

void DisplaySolution(int columns)
{
	printf("\n Solution %d : ", ++solutionCount);
	for (int i = 1; i <= columns; i++)
	{
		printf("%i ", X[i]);
	}
	printf("\n");
}

bool CanPlaceQueen(int currQueen, int column)
{
	for (int i = 1; i <= currQueen - 1; i++)
	{
		if (X[i] == column   //another queen already has this column
			|| (abs(i - currQueen) == abs(X[i] - column))      //this is on the diagonal path of another queen.
			)
		{
			return false;
		}
	}

	return true;
}

void NQueens(int currQueen /* current queen */, int columns /* number of columns */)
{
	for (int column = 1; column <= columns; column++) //1-based , iterate through all columns for current Queen. 
	{
		if (CanPlaceQueen(currQueen, column))
		{
			X[currQueen] = column;
			if (currQueen == columns)
			{
				DisplaySolution(columns);
			}
			else
			{
				NQueens(currQueen + 1, columns);
			}
		}
	}
}


// Prints all possible solutions to the 8 queen problem.
int main()
{
	int n;
	printf("Enter the number of queens/columns: ");
	scanf_s("%d", &n);

	X = new int[n + 1]; //to keep it 1-based instead of 0 based, which makes it easier to read.

	NQueens(1 , n); //always start with queen on row 1. 

	scanf_s("%d");
}