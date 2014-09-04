#include <iostream>
#include <stack>

using namespace std;

class Trie;
typedef struct NODE
{
	friend class Trie;
private :
	unsigned int IsWordBitFlag; 

public:
	NODE *Next[26];

	NODE()
	{
		for (int i = 0; i < 26; i++)
			Next[i] = nullptr;

		IsWordBitFlag = 0;
	}

	bool IsWordBitFlagSet(int bitPosition)
	{
		return (IsWordBitFlag &= (1 << (sizeof(int)*8 - 1)) >> bitPosition);

	}

	void SetWordBitFlag(int bitPosition)
	{
		IsWordBitFlag |= (1 << (sizeof(int)*8 - 1)) >> bitPosition;
	}

	void UnsetWordBitFlag(int bitPosition)
	{
		IsWordBitFlag &= ~((1 << (sizeof(int)*8 - 1)) >> bitPosition);
	}

	bool HasChildren()
	{
		for (int i = 0; i < 26; i++)
		{
			if (Next[i])
			{
				return true;
			}
		}
			
		return false;
	}
} NODE;

struct Visitor
{
public:
	virtual void Visit(NODE *pNode) = 0;

};

class DeleteVisitor : public Visitor
{
private:

public:
	stack<NODE *> stack;

	void Visit(NODE *pNode) override
	{
		stack.push(pNode);
	}

};


class Trie
{
private:
	NODE *Root;

public:
	Trie() : Root(nullptr) {}
	void Insert(_In_ char *pWord);
	bool IsWord(_In_ char *pWord, _In_opt_ Visitor *pVisitor = nullptr);
	void DeleteWord(_In_ char *pWord);
};

bool IsWordValid(_In_ char *pWord)
{
	//could also use isAlpha from ctype.h
	if (!pWord) return false;

	while (pWord && *pWord != '\0')
	{
		if (tolower(*pWord) < 'a' || tolower(*pWord) > 'z')
			return false;
		else
			pWord++;
	}

	return true;
}

void Trie::Insert(_In_ char *pWord)
{
	NODE *pCurr = this->Root;  //set curr to Root
	NODE *temp = nullptr;
	
	if (!pWord || *pWord == '\0') return;

	if (!IsWordValid(pWord)) return; //bail out if not valid word.
	
	if (this->Root == nullptr)   //initialize root if not already and set curr to Root
	{
		this->Root = pCurr = new NODE();
	}

	int len = strlen(pWord);
	int prefix = tolower(pWord[0]) - 'a';
	for (int i = 1; i < len; i++)
	{
		temp = pCurr->Next[prefix];
		if (temp == nullptr)
		{
			pCurr->Next[prefix] = temp = new NODE();
		}
		pCurr = temp;
		prefix = tolower(pWord[i]) - 'a';

	}

	//Set IsWordBitFlag
	pCurr->SetWordBitFlag(prefix);

}

bool Trie::IsWord(_In_ char *pWord, _In_opt_ Visitor *pVisitor)
{
	NODE *pCurr = this->Root;
	if (pVisitor)
	{
		pVisitor->Visit(pCurr);
	}

	if (!pWord || *pWord == '\0') return false;

	int len = strlen(pWord);
	int prefix = tolower(pWord[0]) - 'a';

	for (int i = 1; i < len; i++)
	{
		pCurr = pCurr->Next[prefix];
		if (pVisitor)
		{
			pVisitor->Visit(pCurr);
		}

		prefix = tolower(pWord[i]) - 'a';

		if (pCurr == nullptr)
		{
			return false;
		}
	}

	return pCurr->IsWordBitFlagSet(prefix);
}

void Trie::DeleteWord(_In_ char *pWord)
{
	DeleteVisitor visitor;
	if (!IsWord(pWord, &visitor))
	{
		return;
	}

	stack<NODE *> stack = visitor.stack;

	NODE *pCurr = stack.top(); stack.pop();

	int length = strlen(pWord);
	pCurr->UnsetWordBitFlag(pWord[length - 1] - 'a');

	for (int i = length - 2; i >= 0; i--)
	{
		if (pCurr->HasChildren() || pCurr->IsWordBitFlag)
		{
			break;
		}
		else
		{
			delete pCurr;
			pCurr = stack.top(); stack.pop();
			pCurr->Next[pWord[i] - 'a'] = nullptr;
		}
	}

	while (!stack.empty())
	{
		stack.top(); stack.pop();
	}
}


void TestInsert()
{
	Trie *trie = new Trie();

	trie->Insert("Adrian");
	trie->Insert("Apple");
	trie->Insert("AdrianMa");
	trie->Insert("catapult");
	trie->Insert("cater");
	trie->Insert("cataract");
	trie->Insert("catamaran");
	trie->Insert("catamarans");

	_ASSERT(trie->IsWord("Adrian"));
	_ASSERT(!trie->IsWord("Ad"));
	_ASSERT(trie->IsWord("AdrianMa"));
	_ASSERT(trie->IsWord("Apple"));
	_ASSERT(trie->IsWord("catamaran"));
	_ASSERT(trie->IsWord("cater"));
	_ASSERT(!trie->IsWord("pupil"));



}

void TestDelete()
{
	Trie *trie = new Trie();

	trie->Insert("Adrian");
	trie->Insert("Apple");
	trie->Insert("AdrianMa");
	trie->Insert("catapult");
	trie->Insert("cater");
	trie->Insert("cataract");
	trie->Insert("catamaran");
	trie->Insert("catamarans");


	_ASSERT(trie->IsWord("catamaran"));
	trie->DeleteWord("catamaran");  //should only unset flag
	_ASSERT(!trie->IsWord("catamaran"));
	_ASSERT(trie->IsWord("catamarans"));

	trie->Insert("catamaran");  //add flag back.
	trie->DeleteWord("catamarans");
	_ASSERT(!trie->IsWord("catamarans"));
	_ASSERT(trie->IsWord("catamaran"));

	trie->DeleteWord("catamar"); //should not delete since catamar is not a word.
	_ASSERT(trie->IsWord("catamaran"));
	trie->DeleteWord("catermaran");
	_ASSERT(trie->IsWord("cataract"));
	_ASSERT(trie->IsWord("catapult"));

	trie->Insert("catamaran");
	trie->DeleteWord("catapult");  //delete first child
	_ASSERT(trie->IsWord("catamaran")); //next sibling becomes first child
	_ASSERT(trie->IsWord("cataract"));

	trie->DeleteWord("cataract");  //delete last sibling
	_ASSERT(trie->IsWord("catamaran"));


	trie->DeleteWord("catamaran");  //delete only child 
	_ASSERT(!trie->IsWord("catamaran"));
	_ASSERT(trie->IsWord("cater"));

	trie->DeleteWord("Adrian");
	trie->DeleteWord("AdrianMa");
	trie->DeleteWord("Apple");

	_ASSERT(trie->IsWord("cater"));

}

int main()
{
	TestInsert();

	TestDelete();

	printf_s("\n\nAll Tests Passed!");
	int n;
	cin >> n;

}