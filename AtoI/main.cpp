#include <iostream>

using namespace std;

typedef long HRESULT;
#define S_OK  0x1L
#define E_FAIL   0x0L


bool isValidNumber(char ch)
{
	if (ch < '0' || ch > '9')
	{
		return false;
	}
	return true;
}

bool CheckForIntegerOverflowMultiplication(int a, int b)
{
	if (INT_MAX / a <= b)
		return true;
	else
		return false;
}

bool CheckForIntegerOverflowAddition(int a, int b)
{
	if (a < 0 && b > 0)
	{
		return INT_MAX - b < a;
	}
	else if (b < 0 && a > 0)
	{
		return INT_MAX - a < b;
	}
	else
	{
		//check for underflow
	}

	return false;

}

HRESULT AtoI(_In_ char* buff, _Out_ int& num)   //passing by ref
{
	bool isNegative = false;
	int result = 0;

	if (!buff || *buff == '\0') return E_FAIL;

	while (*buff == ' ' || *buff == '\t') buff++;  //ignore leading whitespaces.

	//Negative sign
	if (*buff == '-')
	{
		isNegative = true;
		//check if next char is not a number
		if (!isValidNumber(*(buff + 1)))
		{
			return E_FAIL;
		}
		else
		{
			isNegative = true;
			buff++;
		}
	}

	//Positive sign
	if (*buff == '+')
	{
		buff++;
		//check if next char is not a number
		if (!isValidNumber(*(buff + 1)))
		{
			return E_FAIL;
		}
	}

	while (*buff != '\0')
	{
		if (!isValidNumber(*buff)) return E_FAIL;

		if (result > 0 && CheckForIntegerOverflowMultiplication(result, 10) || CheckForIntegerOverflowAddition(result * 10, (*buff - '0'))) { printf("\n Integer Overflow");  return E_FAIL; }
		result = result * 10 + (*buff - '0');

		buff++;

	}

	if (isNegative)
	{
		result = -result;
	}

	num = result;
	return S_OK;
}


//In place change and update buff pointer.  Else will have to calculate lenght of int or will have to use stack to pop digits in reverse.
HRESULT IToA(_In_ int num, _In_  int buffLen, _Out_ char ** buff)
{
	bool isNegative = false;
	bool isOverflowing = false;

	if (num < 0)
	{
		isNegative = true;

		if (abs(num) == num)  //abs(INT_MIN) overflows and returns INT_MIN again  
		{
			isOverflowing = true; //this means it is INT_MIN.
			num = abs(num + 1); // increase it by one so that it can fit as INT_MAX, and later add 1 to last character value.
		}
		else
			num = abs(num);
	}
	

	int i = buffLen - 1;
	(*buff)[i] = '\0';
	i--;

	while (num > 0)
	{
		(*buff)[i] = (num % 10) + '0';
		i--;
		num = num / 10;
	}

	if (isOverflowing)
	{
		(*buff)[buffLen - 2] += 1;  //add back one since we increased INT_MIN by 1 to fit into INT_MAX
	}

	if (isNegative)
	{
		(*buff)[i] = '-';

		*buff = &(*buff)[i];
	}
	else
		*buff = &(*buff)[++i];


	return S_OK;
}


void PrintNum(char * str)
{
	int num;

	if (AtoI(str, num)) //passing by ref
		printf("\n%s => %d", str, num);
	else
		printf("\n%s => Invalid Number", str);
}

void PrintString(int num)
{
	char buff[13];  //INT_MAX has 11 characters + 1 more for sign + 1 for '\0' = 13 characters max is needed for buffer size.

	char * buffPtr = buff;

	if (IToA(num, 13, &buffPtr)) //passing by ref
		printf("\n%d => %s", num, buffPtr);
}


int main()
{
	int num;

	//AtoI
	PrintNum("123");
	PrintNum("-123");
	PrintNum("--123");
	PrintNum("+123");
	PrintNum("		+123");
	PrintNum("000");
	PrintNum("+000123");
	PrintNum("-000123");
	PrintNum("2147483648");


	//ItoA
	PrintString(123);
	PrintString(-123);
	PrintString(INT_MIN); //special case to handle overflow
	PrintString(INT_MAX);


	cin >> num;

}