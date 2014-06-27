#include <iostream>


/*

Replace a char with a multi-character inside a string in place 
eg. replace '.' with '::' eg. "A.B.C" to "A::B::C"

Hint: count number of dots till the end and calculate based on that where the last character will eventually end up in the buffer. 
Then start backwards down to 0, putting characters where they eventually will end up based on preceding dots, and  keep replacing dots with :: as you go.

*/

void SimpleReplaceInPlace(char * buff, int len, int capacity)
{
	//count of preceding dots helps calculate where current character will eventually end up in the buffer.
	int counfOfDots = 0;
	for (int i = 0; i < len; i++)
	{
		if (buff[i] == '.') ++counfOfDots;
	}

	//start from end and place characters in their calculated spots.
	for (int i = len - 1; i >= 0; i--)
	{
		if (buff[i] == '.')
		{
			//add 2 colons and reduce count by 1
			buff[i + counfOfDots] = ':';
			buff[i + counfOfDots - 1] = ':';
			counfOfDots--;
		}
		else
		{
			buff[i + counfOfDots] = buff[i];
		}
	}
}

void GenericReplaceInPlace(char * buff, int len, int capacity, char * replacementString)
{
	int counfOfDots = 0;
	int replacementStringLength = strlen(replacementString);

	for (int i = 0; i < len; i++)
	{
		if (buff[i] == '.') ++counfOfDots;
	}

	for (int i = len - 1; i >= 0; i--)
	{
		if (buff[i] == '.')
		{
			//calculated spot is f(numberofDots * (strlen(replacementString) - 1))
			for (int j = replacementStringLength, k = 0; j > 0; j--, k++)
			{
				buff[i + counfOfDots * (replacementStringLength - 1) - k] = replacementString[j - 1];
			}
			counfOfDots--;
		}
		else
		{
			buff[i + counfOfDots * (replacementStringLength - 1)] = buff[i];
		}
	}
}


int main()
{
	int x;
	char string1[100] = "A.BB.CCC.DDD.";
	char string2[100] = "A.BB.CCC..DDD.";

	SimpleReplaceInPlace(string1, strlen(string1), 100);
	printf("%s\n\n", string1);

	GenericReplaceInPlace(string2, strlen(string2), 100, "::::");
	printf("%s\n\n", string2);

	scanf_s("%d", &x );


}