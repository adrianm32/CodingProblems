#include <iostream>

using namespace std;

char *Encode(char *str)
{

	return nullptr;
}


char *Decode()
{

	return nullptr;
}




/*
http://www.wikihow.com/Compress-Data-Using-Huffman-Encoding
*/
int main()
{

	char *string = "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

	char *EncodedString = Encode(string);

	char *DecodedString = Decode();

	int sizeOriginalString = strlen(string) *sizeof(char)*8;
	int sizeEncodedString = strlen(EncodedString) *sizeof(char)*8;

	printf("Sizeof(original string) =  %d, Sizeof(Encoded String)=%d \n", sizeOriginalString, sizeEncodedString);
	if (strncmp(string, DecodedString, strlen(string)) == 0)
	{
		printf("\nThe original and decoded strings match");
	}
	else
	{
		printf("\nThe original and decoded strings do not match");
	}

	int n;
	cin >> n;
}