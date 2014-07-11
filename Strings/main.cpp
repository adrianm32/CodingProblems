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

	//then reverse letters of each word.
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


void AreAnagrams(char * str1, char * str2)
{
	int chars[26];
	char * tempStr1 = str1;
	char * tempStr2 = str2;
	for (int i = 0; i < 26; i++)
	{
		chars[i] = 0;  //initialize to zero.
	}

	//increment character counts using str1
	while (str1 && *str1 != '\0')
	{
		chars[tolower(*str1) - 'a']++;
		str1++;
	}

	//decrement character counts using str2
	while (str2 && *str2 != '\0')
	{
		chars[tolower(*str2) - 'a']--;
		str2++;
	}

	//char counts should be zero
	for (int i = 0; i < 26; i++)
	{
		if (chars[i] > 0)
		{
			cout << tempStr1 << " and " << tempStr2 << " are not anagrams \n";
			return;
		}
	}
	cout << tempStr1 << " and " << tempStr2 << " are anagrams \n";
}


void Swap(char * a, char * b)
{
	char temp = *a;
	*a = *b;
	*b = temp;
}

int Partition(char str[], int startIndex, int endIndex)
{
	char partitionValue = str[endIndex];  //use end element as the partition element.
	int i = startIndex - 1;

	for (int j = startIndex; j <= endIndex - 1; j++)
	{
		if (tolower(str[j]) <= tolower(partitionValue))
		{
			i++;   //keeps track of partition point as all elements to left of it are smaller than partition Value.
			Swap(&str[i], &str[j]);

		}

	}
	
	Swap(&str[i+1], &str[endIndex]);  //swap the end partition element to the final partition point
	return (i + 1);

}

void QuickSort(char str[], int startIndex, int endIndex)
{
	int partitionIndex;
	if (startIndex < endIndex)
	{
		partitionIndex = Partition(str, startIndex, endIndex);
		QuickSort(str, startIndex, partitionIndex - 1);
		QuickSort(str, partitionIndex + 1, endIndex);
	}
}

void AreAnagramsUsingQuickSort(char * str1, char * str2)
{
	int str1Len = strlen(str1);
	int str2Len = strlen(str2);

	if (str1Len != str2Len)
	{
		cout << str1 << " and " << str2 << " are not anagrams \n";
		return;
	}

	//sort both strings
	QuickSort(str1, 0, str1Len-1);
	QuickSort(str2, 0, str2Len -1);

	//and then check for match
	for (int i = 0; i < str1Len; i++)
	{
		if (tolower(str1[i]) != tolower(str2[i]))
		{
			cout <<  " are not anagrams \n";
			return;
		}
	}

	cout <<  " are anagrams \n";
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
	char str3[] = "Coding";
	ReverseWords(str3);

	AreAnagrams("Cat", "Tac");
	AreAnagrams("Caa", "Aac");
	AreAnagrams("Caa", "Tac");
	AreAnagrams("A", "A");

	char str4[] = "Cat";
	char str5[] = "Tac";
	cout << "\nQuickSort Anagrams: " << str4 << " and " << str5;
	AreAnagramsUsingQuickSort(str4, str5);

	char str6[] = "Cat";
	char str7[] = "Taz";
	cout << "\nQuickSort Anagrams: " << str6 << " and " << str7;
	AreAnagramsUsingQuickSort(str6, str7);
	



	int n;
	cin >> n;




}