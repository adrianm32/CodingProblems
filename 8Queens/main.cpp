#include <iostream>
#include <stack>

/*
depth first BackTracking algorithm: gets rid of invalid subtrees and backs up.
worst case (brute force) is 64
                C
                  8    combinations as each queen can be placed in any of the 64 positions.

*/

int *Queens; //solution array Queens[row]=col. Each value in the array is the column number of the ith row Queen. eg. Queens[4] = 5 is Queen at (4,5).
int solutionCount;

void DisplaySolution(_In_ int columns)
{
    printf("\n Solution %d : ", ++solutionCount);
    for (int i = 1; i <= columns; i++)
    {
        printf("%i ", Queens[i]);
    }
    printf("\n");
}


/*
check for diagonal:

if Q is on (row, col) = (3,4)   then diagonals are along (1,2) (2,3),(4,5) etc. on one diagonal and (4,3)(2,5)(1,6) etc. on other diagonal.
i.e.    difference of 1 between row/col on one diagonal, |3-4|=|1-2|=|2-3|
and sum of 7 on the other diagonal , 3+4=4+3=2+5

in other words   if we have (i,j) and (m,n), then
|i-j| = |m-n|  and
i + j = m + n

reaaranging above two, we get
i - m = j - n
i - m = n - j

in other words | i - m | = | j - n|


*/
_Check_return_ bool CanPlaceQueen(_In_ int currQueen, _In_ int column)
{
    for (int i = 1; i <= currQueen - 1; i++)
    {
        if (Queens[i] == column   //another queen already has this column
            || (abs(i - currQueen) == abs(Queens[i] - column))      //this is on the diagonal path of another queen.
            )
        {
            return false;
        }
    }

    return true;
}

void NQueens(_In_ int currQueen /*current queen */, _In_ int columns /*number of columns */)
{
    for (int column = 1; column <= columns; column++) //1-based , iterate through all columns for current Queen. 
    {
        if (CanPlaceQueen(currQueen, column))
        {
            Queens[currQueen] = column;
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

struct QueenPosition
{
    int CurrQueen = 0;
    int col = 0;

    QueenPosition(int queen, int col) : CurrQueen(queen), col(col)
    {}
};

void NQueensWithoutRecursion(_In_ int columns /*number of columns */)
{
    std::stack<QueenPosition> QStack;
    int currQueen;
    int currQueenCol;

    QStack.push(QueenPosition(1, 0));

    do
    {
        QueenPosition qp = QStack.top(); 
        QStack.pop();
        currQueen = qp.CurrQueen;
        currQueenCol = qp.col + 1;

        for (int column = currQueenCol; column <= columns; column++)
        {
            if (CanPlaceQueen(currQueen, column))
                {
                    Queens[currQueen] = column;
                    if (currQueen == columns)
                    {
                        DisplaySolution(columns);
                    }
                    else
                    {
                        QStack.push(QueenPosition(currQueen, column));
                        currQueen++;
                        QStack.push(QueenPosition(currQueen, 0));
                        break; //need to go back to beginning of do loop from here with next queen.
                    }
                }
        }
    }
    while (!QStack.empty());
    

}


// Prints all possible solutions to the 8 queen problem.  There are only 92 possible solutions to 8 queen.
int main()
{
    int n;
    printf("Enter the number of queens/columns: ");
    scanf_s("%d", &n);

    Queens = new int[n + 1]; //to keep it 1-based instead of 0 based, which makes it easier to read.

    printf("\n With Recursion:\n");
    NQueens(1 , n); //always start with queen on row 1. 

    printf("\n Without Recursion:\n");
    solutionCount = 0;
    Queens = new int[n + 1]; //to keep it 1-based instead of 0 based, which makes it easier to read.
    NQueensWithoutRecursion(n);

    scanf_s("%d", &n);
}