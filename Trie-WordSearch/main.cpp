#include <iostream>

using namespace std;

typedef struct NODE
{
	NODE* Next[26];

	NODE()
	{
		for (int i = 0; i < 26; i++)
			Next[i] = nullptr;
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

	if (!IsWordValid(word)) return; //bail out if not valid word.
	
	if (this->Root == nullptr)   //initialize root if not already and set curr to Root
	{
		this->Root = curr = new NODE();
	}

	while (word &&  *word != '\0')
	{
		temp = curr->Next[tolower(*word) - 'a'];
		if (temp == nullptr)
		{
			curr->Next[tolower(*word) - 'a'] = temp = new NODE();
		}
		curr = temp;

		word++;
	}
}

bool Trie::IsWord(_In_ char* word)
{
	NODE* curr = this->Root;
	char * str = word;
	bool IsWord = false;

	while (curr && word && *word != '\0')
	{
		curr = curr->Next[tolower(*word) - 'a'];
		word++;
	}

	if (curr == nullptr && word && *word != '\0')
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
	Dictionary->IsWord("Hello");
	


	int n;
	cin >> n;

}