#include <iostream>

using namespace std;

struct NODE
{
	char Key;

	NODE * Child;
	NODE * Sibling;

	bool bIsWord;

	NODE(char key) : Key(key), Child(nullptr), Sibling(nullptr), bIsWord(false)
	{};

};


class Trie
{
private:
	NODE * Root;

public:
	Trie()
	{ Root = new NODE('\0'); };
	
	void Insert(_In_ char * pStr);

	bool IsWord(_In_ char * pStr);

};


void Trie::Insert(_In_ char * pStr)
{
	if (pStr == nullptr) return;

	NODE ** ppCurr = &Root;


	while (*pStr != '\0')
	{
		if ((*ppCurr)->Child == nullptr)
		{
			(*ppCurr)->Child = new NODE(*pStr);
			ppCurr = &(*ppCurr)->Child;
		}
		else
		{
			if ((*ppCurr)->Child->Key == *pStr)
			{
				ppCurr = &(*ppCurr)->Child;
			}
			else
			{
				NODE ** ppSibling = &(*ppCurr)->Child;
				while (*ppSibling != nullptr)
				{
					if ((*ppSibling)->Key == *pStr)
					{
						ppCurr = ppSibling;
						break;
					}
					else
						ppSibling = &(*ppSibling)->Sibling;

				}

				if (*ppSibling == nullptr)
				{
					*ppSibling = new NODE(*pStr);
				}

			}
		}


		pStr++;
	}
	(*ppCurr)->bIsWord = true;
}


bool Trie::IsWord(_In_ char * pStr)
{

	if (pStr == nullptr)
	{
		return false;
	}

	if (Root->Child == nullptr)
	{
		return false;
	}

	NODE *pCurr = Root;


	while (*pStr != '\0' && pCurr)
	{
		if (pCurr->Child == nullptr)
		{
			return false;
		}

		if (pCurr->Child->Key == *pStr)
		{
			pCurr = pCurr->Child;
		}
		else
		{
			NODE * pSibling = pCurr->Child;
			while (pSibling)
			{
				if (pSibling->Key == *pStr)
				{
					pCurr = pSibling;
					break;
				}
				else
				{
					pSibling = pSibling->Sibling;
				}
			}

			if (pSibling == nullptr)
			{
				return false;
			}
		}

		pStr++;
	}

	if (pCurr == nullptr)
	{
		return false;
	}

	if (*pStr == '\0' && pCurr->bIsWord)
	{
		return true;
	}
	else
		return false;

}


int main()
{

	Trie* trie = new Trie();

	trie->Insert("Adrian");
	trie->Insert("Apple");
	trie->Insert("AdrianMa");

	printf("Adrian is a word : %s \n", trie->IsWord("Adrian") ? "true": "false");
	printf("Ad is a word : %s \n", trie->IsWord("Ad") ? "true" : "false");
	printf("AdrianMa is a word : %s \n ", trie->IsWord("AdrianMa") ? "true" : "false");


	



	int n;
	cin >> n;

}