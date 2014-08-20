#include <iostream>
#include <stack>

using namespace std;

struct NODE
{
	char Key;

	NODE * Child;
	NODE * Sibling;

	bool bIsWord : 1;  //BitField

	NODE(char key) : Key(key), Child(nullptr), Sibling(nullptr), bIsWord(false)
	{};

};


struct Visitor
{
public:
	virtual void Visit(NODE * pNode) = 0;

};
class DeleteVisitor : public Visitor
{
private:

public:
	stack<NODE *> stack;

	void Visit(NODE * pNode) override
	{
		stack.push(pNode);
	}

};

class Trie
{
private:
	NODE * Root;

public:
	Trie()
	{ Root = new NODE('\0'); };
	
	void Insert(_In_ char * pStr);

	void DeleteWord(_In_ char * pStr);

	bool IsWord(_In_ char * pStrn, _In_opt_ Visitor * pVisitor = nullptr);

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
			ppCurr = &((*ppCurr)->Child);
		}
		else
		{
			if ((*ppCurr)->Child->Key == *pStr)
			{
				ppCurr = &(*ppCurr)->Child;
			}
			else
			{
				NODE ** ppSibling = &((*ppCurr)->Child);
				while (*ppSibling != nullptr)
				{
					if ((*ppSibling)->Key == *pStr)
					{
						ppCurr = ppSibling;
						break;
					}
					else
						ppSibling = &((*ppSibling)->Sibling);

				}

				if (*ppSibling == nullptr)
				{
					*ppSibling = new NODE(*pStr);
					ppCurr = ppSibling;  //becomes new current.
				}

			}
		}


		pStr++;
	}
	(*ppCurr)->bIsWord = true;
}

void Trie::DeleteWord(_In_ char * pStr)
{
	DeleteVisitor visitor;
	if (!IsWord(pStr, &visitor))
	{
		return;
	}

	stack<NODE *> stack = visitor.stack;

	NODE * pCurr = stack.top(); stack.pop();

	if (pCurr == this->Root)
	{
		return;
	}

	pCurr->bIsWord = false;  //set flag to false.

	if (pCurr->Child)
	{
		return;  // has children, so just unset flag and return.
	}

	NODE * pParent = nullptr, ** ppPrevChild = nullptr, ** ppChild = nullptr;
	
	while (pCurr != this->Root)
	{
		pParent = stack.top(); stack.pop();

		ppPrevChild = ppChild = &(pParent->Child);

		while (*ppChild != pCurr)
		{
			ppPrevChild = ppChild;
			ppChild = &((*ppChild)->Sibling);

		}

		//at this point ppChild is pointing to node to be deleted.
		*ppPrevChild = (*ppChild)->Sibling;
		delete(pCurr);

		if (pParent->Child || pParent->bIsWord)
		{
			break;  //the parent node has other children so we break the loop here.
		}
		else
		{
			pCurr = pParent; // else we walk up the stack.

		}
	}

	while (!stack.empty())
	{
		stack.top(); stack.pop();
	}
}

bool Trie::IsWord(_In_ char * pStr, _In_opt_ Visitor * pVisitor)
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
	if (pVisitor)
	{
		pVisitor->Visit(pCurr);
	}

	while (*pStr != '\0' && pCurr)
	{
		if (pCurr->Child == nullptr)
		{
			return false;
		}

		if (pCurr->Child->Key == *pStr)
		{
			pCurr = pCurr->Child;
			if (pVisitor)
			{
				pVisitor->Visit(pCurr);
			}
		}
		else
		{
			NODE * pSibling = pCurr->Child;
			while (pSibling)
			{
				if (pSibling->Key == *pStr)
				{
					pCurr = pSibling;
					if (pVisitor)
					{
						pVisitor->Visit(pCurr);
					}
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

void TestInsert()
{
	Trie* trie = new Trie();

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
	Trie* trie = new Trie();

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



	printf_s("All Tests passed!");
	int n;
	cin >> n;

}