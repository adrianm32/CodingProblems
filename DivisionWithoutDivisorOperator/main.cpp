#include <iostream>
using namespace std;

/********************************************************************************************
1. Document your assumptions
	- Should handle positive and negative integers including INT_MIN and INT_MAX.
	
2. Explain your approach and how you intend to solve the problem
	-Basic approach is to use subtract denominator from numerator until num is < denom.
	-For negative numbers, we track if either one is negative, and then negate the quotient before returning.
	-Special case handling for INT_MIN since abs(INT_MIN) = (INT_MIN)
		- Approach here is to reduce INT_MIN by 1 so that its absolute value fits in INT_MAX and then add the 1 back as soon as the numerator is reduced by the denom.
		- Need to handle only for numerator since if denominator is INT_MIN, then there can be no numerator value (INT_MAX is 1 less than abs INT_MIN) that is larger than abs(INT_MIN)
		- we handle the special case of numerator == denominator == INT_MIN in the beginning of the code.

3. Provide code comments where applicable
4. Explain the big-O run time complexity of your solution. Justify your answer. 
	- This is bound by O(M/N).
	- Best case is O(1) if num==denom, num == 0, denom = INT_MIN.
	- Worst case is O(M) when N = 1 and M = INT_MAX. ( this can take a while to compute)
	- I was trying to think of optmizations for worst case:
		- one idea would be to subtract the denominator in increasing multiples. That would be using multiplication. 
		- another idea would be to use bit manipulations, shifting bits. That would be constant time O(32), but signed bits would be complicated to handle.
5. Write test cases.
    see below in main method.

	PASSED 24 / 4 = 6   6
	PASSED 33 / 6 = 5   5
	PASSED 0 / 3 = 0   0
	PASSED 0 / -3 = 0   0
	PASSED 3 / 3 = 1   1
	PASSED 12 / 3 = 4   4
	PASSED -3 / -3 = 1   1
	PASSED 12 / -3 = -4   -4
	PASSED -12 / 3 = -4   -4
	PASSED -12 / -3 = 4   4
	PASSED 2147483647 / 3 = 715827882   715827882
	PASSED 2147483647 / 2147483647 = 1   1
	PASSED -2147483648 / -2147483648 = 1   1
	PASSED 2147483647 / -2147483648 = 0   0
	PASSED -2147483648 / 5 = -429496729   -429496729
	PASSED -2147483648 / -5 = 429496729   429496729
	PASSED 5 / -2147483648 = 0   0
	PASSED -5 / -2147483648 = 0   0


*************************************************************************************************/
int DivideWithoutDivisor(int numerator, int denominator)
{
	int quotient = 0;
	bool isQuotientNegative = false;


	if (numerator == 0)
	{
		return 0;
	}

	if (denominator == 0)
	{
		throw std::exception("Divide By Zero");
	}

	if (numerator == denominator)
	{
		return 1;
	}

	//special case for INT_MIN,  where there is no numerator value possible that is greater than abs(INT_MIN) denominator.
	// Largest int is 2147483647 which is still smaller than 2147483648.
	if (denominator == INT_MIN)
	{
		return 0;
	}


	bool isNumeratorINT_MIN = false;  // dont have to worry about Denominator being INT_MIN since that will always result in 0 quotient.

	//Check for negative numbers.
	if (numerator < 0 || denominator < 0)
	{
		if (!(numerator < 0 && denominator < 0))
		{
			//Set only if either input is negative.
			isQuotientNegative = true;
		}

		//convert to positive.
		if (numerator < 0)
		{
			if (numerator == INT_MIN)
			{
				numerator += 1; //increase by 1 so we can store its absolute value in INT_MAX.
				isNumeratorINT_MIN = (numerator == INT_MIN);
			}
			numerator *= (numerator < 0) ? -1 : 1;
		}

		if (denominator < 0)
		{
			denominator *= (denominator < 0) ? -1 : 1;
		}
	}

	while (numerator >= denominator)
	{
		numerator -= denominator;
		quotient++;

		if (isNumeratorINT_MIN)
		{
			numerator += 1; //add back the 1 that we reduced when converting INT_MIN to absolute value.
			isNumeratorINT_MIN = false;
		}
	}


	if (isQuotientNegative)
	{
		quotient *= -1;
	}

	return quotient;
}


void Test(int numerator, int denominator)
{
	int resultUsingDivisor = numerator / denominator;
	int resultWithoutDivisor = DivideWithoutDivisor(numerator, denominator);

	_ASSERT(resultUsingDivisor == resultWithoutDivisor);
}


int main()
{

	Test(24, 4);
	Test(33, 6);

	//0 a, positive b
	Test(0,3);

	//0 a, negative b
	Test(0, -3);

	//divide by 0
	//Test(3, 0);  //should throw.

	//same a b
	Test(3, 3);

	// positive a b
	Test(12, 3);

	//same neg a b
	Test(-3, -3);

	//positive a , neg b
	Test(12, -3);

	//neg a , positive b
	Test(-12, 3);

	//neg a , neg b
	Test(-12, -3);

	//INT_MAX a , positive b
	Test(INT_MAX, 3);

	//INT_MAX a , INT_MAX b
	Test(INT_MAX, INT_MAX);
	
	//INT_MIN a , INT_MIN b
	Test(INT_MIN, INT_MIN);

	//INT_MAX a , INT_MIN b
	Test(INT_MAX, INT_MIN);

	//INT_MIN a , positive b
	Test(INT_MIN, 5);

	//INT_MIN a , negative b
	Test(INT_MIN, -5);

	//positve a , INT_MIN b
	Test(5, INT_MIN);

	//neg a , INT_MIN b
	Test(-5, INT_MIN);


	printf_s("\n\nAll Tests Passed!");
	int n;
	cin >> n;
}