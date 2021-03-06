#include <iostream>

using namespace std;

typedef long HRESULT;
#define S_OK  0x1L
#define E_FAIL   0x0L


_Check_return_ bool isValidNumber(_In_ char ch)
{
	if (ch < '0' || ch > '9')
	{
		return false;
	}
	return true;
}

bool CheckForIntegerOverflowMultiplication(_In_ int a, _In_ int b)
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

HRESULT AtoI(_In_ char *buff, _Out_ int& num)   //passing by ref
{
	bool isNegative = false;
	int result = 0;
	num = -1;  // return initialized memory even in case of failure. This value should be discarded if HRESULT is E_FAIL.

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

		if (result > 0 && CheckForIntegerOverflowMultiplication(result, 10) || CheckForIntegerOverflowAddition(result *10, (*buff - '0'))) { printf("\n Integer Overflow");  return E_FAIL; }
		result = result *10 + (*buff - '0');

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
HRESULT IToA(_In_ int num, _In_range_(12, _MAX_PATH)  int buffLen, _Inout_z_count_(buffLen) char **buff)
{
	bool isNegative = false;
	bool isOverflowing = false;

	if (buff == nullptr || *buff == nullptr) return E_FAIL;

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


char * IToA2(_Inout_z_count_(12) char *pBuff, _In_ int num)
{
	bool isNegative = num < 0;

	pBuff += 12; //-1 000 000 000 \0  = 12 chars
	*--pBuff = '\0';   //using decrement first it is easier to keep track of pBuff ptr at end of loop since it will always be on valid curr position. Else we will need a if loop for IsNegative to increment pBuff if it is not negative and return that.

	int digit = abs(num % 10);  //do abs to get rid of minus sign if neg and avoid havin to call abs each time in the loop.
	num = abs(num / 10);   //in case of INTMIN, reduce number first and then take abs. the abs then takes care of the minus sign.
	*--pBuff = digit + '0';
	

	while (num > 0)
	{
		digit = num % 10;   
		num = num / 10;

		*--pBuff = digit + '0';
	} 

	if (isNegative)
	{
		*--pBuff = '-';
	}

	return pBuff;
}

void TestAToI(_In_ char *pStr)
{
	int num;

	if (AtoI(pStr, num) == S_OK) //passing by ref
	{
		printf("\n%s => %d", pStr, num);
		_ASSERT(atoi(pStr) == num);
	}
	else
		printf("\n%s => Invalid Number", pStr);
}

void TestIToA(_In_ int num, _In_opt_ bool flag = false)
{
	char buff[12] = {};  //INT_MAX has 10 characters + 1 more for sign + 1 for '\0' = 12 characters max is needed for buffer size.

	char *pBuff = buff;

	if (!flag)
	{
		if (IToA(num, 12, &pBuff) == S_OK) //passing by ref
		{
			printf("\n%d => %s", num, pBuff);
			char tempBuff[12] = {};
			_itoa_s(num, tempBuff, 10);
			_ASSERT(strncmp(tempBuff, pBuff, strlen(pBuff)) == 0);
		}
	}
	else
	{
			printf("\n%d => %s", num, IToA2(pBuff, num));
			char tempBuff[12] = {};
			_itoa_s(num, tempBuff, 10);
			_ASSERT(strncmp(tempBuff, pBuff, strlen(pBuff)) == 0);

	}
	
}


int main()
{
	int num;

	//AtoI
	TestAToI("123");
	TestAToI("-123");
	TestAToI("--123");
	TestAToI("+123");
	TestAToI("		+123");
	TestAToI("000");
	TestAToI("+000123");
	TestAToI("-000123");
	TestAToI("2147483648");


	//ItoA
	TestIToA(123);
	TestIToA(-123);
	TestIToA(INT_MIN); //special case to handle overflow
	TestIToA(INT_MAX);


	//ItoA2
	TestIToA(123, true);
	TestIToA(-123, true);
	TestIToA(INT_MIN, true); //special case to handle overflow
	TestIToA(INT_MAX, true);


	printf_s("\n\nAll Tests Passed!");
	cin >> num;

}