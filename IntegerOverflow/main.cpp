#include <iostream>
#include <limits.h>  //int_max, int_min
#include <math.h>

using namespace std;


/*
In two’s-complement arithmetic (used for signed integer arithmetic by most modern CPUs), a negative number is represented by inverting all the bits of the binary number and adding 1. A 1 in the most-significant bit indicates a negative number.
Thus, for 4-byte signed integers, 0x7fffffff = 2147483647, but 0x80000000 = -2147483648


*/


int main()
{
	int num;

	printf("\nINT_MIN=%d   INT_MAX=%d", INT_MIN, INT_MAX);

	printf("\nABS(INT_MIN) = %d    it returns same number because of 2's complements", abs(INT_MIN));

	int a = INT_MAX;
	int b = 5;

	//works only if both a and b are positive.
	if (INT_MAX - a < b)  // basically  checks for a + b > INT_MAX without having to do addition that causes overflow
		printf("\n%d + %d = %d   (Integer overflow, greater than %d)", a, b, a + b, INT_MAX);

	if (INT_MAX - b < a)   // can do with b instead of a
		printf("\n%d + %d = %d   (Integer overflow, greater than %d)", a, b, a + b, INT_MAX);

	if (a + b < a)   
		printf("\n%d + %d = %d   (Integer overflow, greater than %d)", a, b, a + b, INT_MAX);
	if (a + b < b)
		printf("\n%d + %d = %d   (Integer overflow, greater than %d)", a, b, a + b, INT_MAX);


	//does not work if one of the numbers is negative. since it is a subtraction.
	b = -5;
	//this works
	if (INT_MAX - a < b)  // basically  checks for a + b > INT_MAX without having to do addition that causes overflow
		printf("\n%d + %d = %d   (Integer overflow, greater than %d)", a, b, a + b, INT_MAX);

	//this results in overflow on the left side
	if (INT_MAX - b < a)   // can do with b instead of a
		printf("\n%d + %d = %d   (Incorrect overflow -Integer overflow, greater than %d)", a, b, a + b, INT_MAX);

	if (a + b < a)
		printf("\n%d + %d = %d   (Integer overflow, greater than %d)", a, b, a + b, INT_MAX);
	//does not work
	if (a + b < b)
		printf("\n%d + %d = %d   (Incorrect overflow Integer overflow, greater than %d)", a, b, a + b, INT_MAX);


	//multiplication overflow
	a = pow(2,30);
	b = 2;  //2^32 should overflow
	if ((INT_MAX / a) <= b)
		printf("\n%d * %d = %d   (Multiplication Integer overflow, greater than %d)", a, b, a * b, INT_MAX);

	if ((INT_MAX / a) <= a)
		printf("\n%d * %d = %d   (Multiplication Integer overflow, greater than %d)", a, b, a * b, INT_MAX);



	//Check for underflow
	a = INT_MIN + 1;
	b = 2;
	if (a - b > a)
		printf("\n%d - %d = %d    (Integer underflow, less than %d)", a, b, a - b, INT_MIN);

	if (a  < INT_MIN + b)   // a - b < INT_MIN
		printf("\n%d - %d = %d    (Integer underflow, less than %d)", a, b, a - b, INT_MIN);

	
	cin >> num;

}