#include <iostream>

using namespace std;


void IsPalindrome(char * str)
{
	int len = strlen(str);
	for (int i = 0, j = len-1; i < j; i++, j--)
	{
		if (str[i] != str[j])
		{
			cout << str << " is not a palindrome\n";
			return;
		}
	}

	cout << str << " is a palindrome\n";
}


void ReverseString(char *str , int displayOutput = 1)
{
	int len = strlen(str);
	char temp;

	if (displayOutput) { cout << "\n " << str << "  (Reversed) "; }
	for (int i = 0, j = len -1; i < j; i++, j--)
	{
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
	}

	if (displayOutput) { cout << str << "\n"; }
}

void ReverseWords(char * str)
{
	int len = strlen(str);

	cout << "\n " << str << "  (Words Reversed) ";


	//first reverse string
	ReverseString(str, 0);

	//then reverse each letters of each word.
	int i = 0, k=0 , l=0;
	char temp;
	while (i < len)
	{
		while (str[i] != '\0' && str[i] == ' ') i++;  //skip spaces
		l = i;  //store beginning of word
		while (str[i] != '\0' && str[i] != ' ') i++;  //skip letters to reach end of word
		k = i - 1;  //store end of word

		//swap letters within word
		for (int i = l, j = k; i < j; i++, j--)
		{
			temp = str[i];
			str[i] = str[j];
			str[j] = temp;
		}
	}

	cout << str << "\n";
}



int main()
{

	IsPalindrome("ABA");
	IsPalindrome("A");
	IsPalindrome("adrian");
	IsPalindrome("");

	char str[] = "Coding is Fun";
	ReverseString(str);

	char str2[] = "Coding is Fun";
	ReverseWords(str2);

	int n;
	cin >> n;




}