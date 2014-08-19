#include <iostream>

using namespace std;

/********************************************************************************************

Given ascii string with chars from a-z, store each char in 5 bits instead of ascii 8 since we need only 5 bits to store 26 chars.

Algo: convert each char to a number between 1 and 26 , and store this number in succesive 5 bits of a byte array. We dont use 0 since that 
will conflict with '\0' which is also 0.













**************************************************************************/



/***************

_Out_ pCompressedLength  : Need otuput length to know when last byte of encoded byte is a valid 0 byte compared to final '\0' byte. 
****************/

void Bits8To5Compression(_In_ char * pszInput, _Outptr_opt_ char ** ppszCompressedOutput, _Out_ int * pCompressedLength)
{

	int inputLen = strnlen_s(pszInput, _MAX_PATH);

	//int compressedBits = inputLen * 5;
	//int compressedOutputLen1 = compressedBits / 8 + (((compressedBits % 8) == 0) ? 0 : 1);
	
	int compressedOutputLen = (inputLen % 8) == 0 ? (inputLen * 5 / 8) :(inputLen * 5 / 8 + 1);
	compressedOutputLen += 1; // for adding '\0' at end.

	char * pCompressedOutput = new char[compressedOutputLen]; //create storage for the compressed string.

	for (int i = 0; i < compressedOutputLen; i++)
	{
		pCompressedOutput[i] = 0;  //all values are '\0'. So, we dont have to worry about setting \0 at the end or adjusting 0 bits.
	}

	int encodedIndex = 0,  // tracks the index in the compressedOutput array.
		encodedBits = 0;  //tracks how many bits in the output char are processed. When it reaches 8, move to the next index.
	char bit = 0;
	int temp = 0;

	while (*pszInput != '\0')
	{
	
		//convert char to a-z index starting with 1-base to avoid conflict with '\0'.
		temp = (tolower(*pszInput) - 'a' + 1); // 'a' + 1 because 'a' -'a' would be '\0' which would confuse with real '\0'.

		//extract lower 5 bits and store in compressedOutput Char.
		for (size_t i = 0; i < 5; i++)
		{
			bit = temp &  (1 << (5 - i - 1)); 
			bit = (bit == 0) ? 0 : 1;  //whether bit is set or not.
			encodedBits++;


			if (bit)  // only if bit is set.
			{
				pCompressedOutput[encodedIndex] |= bit << (8 - encodedBits);
			}

			if (encodedBits == 8)
			{
				encodedIndex++; //move to next output char
				encodedBits = 0;  //start from 0 again.
			}

		}

		++pszInput;  
	}
	
	//no need to pad extra bits since it is already initialized to 0.

	*ppszCompressedOutput = pCompressedOutput;
	*pCompressedLength = compressedOutputLen - 1;  //strlen


}


/**************************

_In_ compressedLength  : Need compressed length to know when last byte of encoded byte is a valid 0 byte compared to final '\0' byte.
**********************/
void Bits5to8Decompression(_In_ char * pszCompressedInput, _In_ int compressedLength, _Outptr_ char ** ppszDecompressedOutput, _Out_ int * pDecompressedLength)
{
	int decompressedOutputLen = compressedLength * 8 / 5;
	decompressedOutputLen += 1; // for adding '\0' at end.

	char * pDecompressedOutput = new char[decompressedOutputLen];

	for (int i = 0; i < decompressedOutputLen; i++)
	{
		pDecompressedOutput[i] = 0;  //all values are '\0'.
	}

	int decodedIndex = 0, decodedBits = 0;
	char bit = 0;
	char temp = 0;

	for (int k = 0; k <= compressedLength; k++)
	{
		//extract each bit of the compressed Char and stuff the first 5 into the decompresed output char.
		for (size_t i = 0; i < 8; i++)
		{
			bit = *pszCompressedInput &  (1 << (8 - i - 1));
			bit = (bit == 0) ? 0 : 1;
			decodedBits++;


			if (bit)  // only if 1
			{
				temp |= bit << (5 - decodedBits); //skip first 3 bits of the  8 bits since they will be 0.
			}

			//if decoded 5 bits, then store it in output char and reset temp and decodedbits.
			if (decodedBits == 5)
			{
				if (temp > 0)  // skip if value is 0 since output char is already initialized to zero.
				{
					pDecompressedOutput[decodedIndex] = temp + 'a' - 1;  // -1 because we are using 1-base for a-z.
				}
				decodedIndex++;
				decodedBits = 0;
				temp = 0;
			}

		}

		++pszCompressedInput;
	}

	//no need to pad extra bits since it is already initialized to 0.

	*ppszDecompressedOutput = pDecompressedOutput;
	*pDecompressedLength = decompressedOutputLen - 1;  //strlen


}


void Test(char * pInput)
{
	int len = strnlen_s(pInput, _MAX_PATH);
	printf_s("Input String: %s,  Len: %d, Size:%d\n", pInput, len, len + 1);



	char * pCompressedOutput = nullptr;
	int compressedLength = 0;
	Bits8To5Compression(pInput, &pCompressedOutput, &compressedLength);
	printf_s("Compressed String: %s, Len: %d, Size: %d\n", pCompressedOutput, strnlen_s(pCompressedOutput, _MAX_PATH), compressedLength + 1);
	char * pDecompressedOutput = nullptr;
	int decompressedLength = 0;
	Bits5to8Decompression(pCompressedOutput, compressedLength, &pDecompressedOutput, &decompressedLength);
	printf_s("DeCompressed String: %s, Len: %d, Size: %d\n", pDecompressedOutput, strnlen_s(pDecompressedOutput, _MAX_PATH), decompressedLength + 1);

	printf_s("\n\n");

}

int main()
{
	Test("a");
	Test("aa");
	Test("aaa");
	Test("aaaa");
	Test("aaaat");
	Test("aaaatt");
	Test("aaaattt");
	Test("aaaatttt");
	Test("aaaattttb");
	Test("aaaattttbb");
	Test("aaaattttbbb");
	Test("aaaattttbbbb");
	Test("aaaattttbbbbq");
	Test("aaaattttbbbbqq");
	Test("aaaattttbbbbqqq");
	Test("aaaattttbbbbqqqq");
	Test("aaaattttbbbbqqqqr");



	int n;
	cin >> n;



}