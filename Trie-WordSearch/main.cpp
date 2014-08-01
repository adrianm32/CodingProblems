#include <iostream>

using namespace std;

typedef struct NODE
{
private :
	unsigned IsWordBitFlag: 26; 

public:
	NODE* Next[26];

	NODE()
	{
		for (int i = 0; i < 26; i++)
			Next[i] = nullptr;
	}

	bool IsWordBitFlagSet(int bitPosition)
	{
		return (IsWordBitFlag &= (1 << (sizeof(int)* 8 - 1)) >> bitPosition);

	}

	void SetWordBitFlag(int bitPosition)
	{
		IsWordBitFlag |= (1 << (sizeof(int)* 8 - 1)) >> bitPosition;
	}
} NODE;

class Trie
{
private:
	NODE* Root;

public:
	Trie() : Root(nullptr) {}
	virtual void Insert(_In_ char* word);
	virtual bool IsWord(_In_ char* word);
};

bool IsWordValid(_In_ char * word)
{
	//could also use isAlpha from ctype.h
	if (!word) return false;

	while (word && *word != '\0')
	{
		if (tolower(*word) < 'a' || tolower(*word) > 'z')
			return false;
		else
			word++;
	}

	return true;
}

void Trie::Insert(_In_ char* word)
{
	NODE * curr = this->Root;  //set curr to Root
	NODE * temp = nullptr;
	
	if (!word) return;

	if (!IsWordValid(word)) return; //bail out if not valid word.
	
	if (this->Root == nullptr)   //initialize root if not already and set curr to Root
	{
		this->Root = curr = new NODE();
	}

	char * pStr = word;
	while (*pStr != '\0')
	{
		temp = curr->Next[tolower(*pStr) - 'a'];
		if (temp == nullptr)
		{
			curr->Next[tolower(*pStr) - 'a'] = temp = new NODE();
		}
		curr = temp;

		pStr++;
	}

	//Set IsWordBitFlag
	curr->SetWordBitFlag(tolower(word[strlen(word) - 1] - 'a'));

}

bool Trie::IsWord(_In_ char* word)
{
	NODE* curr = this->Root;
	char * str = word;
	bool IsWord = false;

	if (!word) return false;

	char * pStr = word;
	while (curr && *pStr != '\0')
	{
		curr = curr->Next[tolower(*pStr) - 'a'];
		pStr++;
	}

	if (curr == nullptr || !(curr && curr->IsWordBitFlagSet(tolower(word[strlen(word) - 1] - 'a'))))
	{
		printf("\n%s is not a word in the dictionary", str);
		return false;
	}
	else
	{
		printf("\n%s is a word in the dictionary", str);
		return true;
	}
}


class TrieWithBitFlags : public Trie
{
private:

public:
	virtual void Insert(_In_ char* word) override;
	virtual bool IsWord(_In_ char* word) override;

};



int main()
{
	Trie* Dictionary = new Trie();

	Dictionary->Insert("Adrian");
	Dictionary->Insert("AdrianMa");
	Dictionary->IsWord("Adrian");
	Dictionary->IsWord("adrianMa");
	Dictionary->IsWord("a");
	Dictionary->IsWord("adria");
	Dictionary->IsWord("Hello");
	


	int n;
	cin >> n;

}