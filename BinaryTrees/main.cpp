#include <iostream>

using namespace std;



struct NODE
{
	int Value;
	NODE * pLeft, *pRight;

	NODE(int val) :
		Value(val),
		pLeft(nullptr),
		pRight(nullptr)
	{}
};


#pragma region Algorithm 1 

void FindCommonAncestorImpl(_In_ NODE * pNode, _In_opt_ NODE * pN1, _In_opt_ NODE *pN2, _Out_ bool * pFoundN1, _Out_ bool * pFoundN2, _Outptr_ NODE** ppCommonAncestor)
{
	if (pNode == nullptr) return;
	
	bool bFoundN1 = *pFoundN1, bFoundN2 = *pFoundN2;
	if (pN1 == pNode)
	{
		*pFoundN1 = true;;
	}

	if (pN2 == pNode)
	{
		*pFoundN2 = true;;
	}

	//if both N1 and N2 are found in this node, then set this node as common ancestor and return. Dont walk tree any further.
	if (!bFoundN1 && !bFoundN2 && *pFoundN1 && *pFoundN2)
	{
		*ppCommonAncestor = pNode;
		return;
	}


	//Search left
	FindCommonAncestorImpl(pNode->pLeft, pN1, pN2, pFoundN1, pFoundN2, ppCommonAncestor);
	if (*ppCommonAncestor)
	{
		return;  //terminate search if common ancestor already found in left subtree.
	}
	bool bLeftFoundN1 = *pFoundN1;
	bool bLeftFoundN2 = *pFoundN2;


	//search right
	FindCommonAncestorImpl(pNode->pRight, pN1, pN2, pFoundN1, pFoundN2, ppCommonAncestor);
	if (*ppCommonAncestor)
	{
		return; //terminate search if common ancestor already found in right subtree.
	}
	bool bRightFoundN1 = *pFoundN1;
	bool bRightFoundN2 = *pFoundN2;

	
	// N1 and N2 are now found in either left or right subtree, and either one or both were not found when we visited this node earlier.
	if (!bFoundN1 && !bFoundN2 && *pFoundN1 && *pFoundN2)
	{
		*ppCommonAncestor = pNode;

	}
}


NODE * FindCommonAncestor1(_In_ NODE * pRoot, _In_opt_ NODE * pN1, _In_opt_ NODE *pN2)
{

	NODE* pCommonAncestor = nullptr;
	bool bFoundN1 = false, bFoundN2 = false;
	FindCommonAncestorImpl(pRoot, pN1, pN2, &bFoundN1, &bFoundN2, &pCommonAncestor);

	return pCommonAncestor;
}

#pragma endregion

#pragma region Algorithm 2  : Better concise algorithm but longer worst case as it searches all nodes even if it found the node in one of the subtrees.

//assumption N1 and N2 are both in the tree. This will traverse all nodes in the tree even if it found the nodes in the left subtree.

NODE* FindCommonAncestor2(_In_ NODE * pRoot, _In_opt_ NODE * pN1, _In_opt_ NODE *pN2)
{
	if (pRoot == nullptr || !pN1 || !pN2) 
	{
		return nullptr;
	}

	if (pRoot == pN1 || pRoot == pN2)
	{
		return pRoot;   //return if node matches N1 or N2.
	}

	NODE* pLeft = FindCommonAncestor2(pRoot->pLeft, pN1, pN2);

	NODE* pRight = FindCommonAncestor2(pRoot->pRight, pN1, pN2);

	if (pLeft && pRight)  // if both left and right subtrees return something (N1 or N2), then this node must be common ancestor
	{
		return pRoot;
	}

	return pLeft ? pLeft : pRight;  //else return whichever is not null (or null if both are null).
}

#pragma endregion


typedef NODE* (*FindCommonAncestor) (_In_ NODE * pRoot, _In_opt_ NODE * pN1, _In_opt_ NODE *pN2);
void TestCommonAncestor(FindCommonAncestor func)
{
	NODE * pRoot = new NODE(9);
	NODE * pNode6, *pNode13, *pNode15 , *pNode5, *pNode11, *pNode12, *pNode14, *pNode20;
	pRoot->pLeft = pNode6 = new NODE(6);
	pRoot->pRight = pNode13 = new NODE(13);

	pNode6->pLeft = pNode5 = new NODE(5);
	pNode6->pRight = pNode11 = new NODE(11);

	pNode13->pLeft = pNode12 =new NODE(12);
	pNode13->pRight = pNode15 = new NODE(15);

	pNode15->pLeft = pNode14 = new NODE(14);
	pNode15->pRight = pNode20 = new NODE(20);

	NODE *pCommonAncestor = func(pRoot, pNode14, pNode20);  // both on right subtree of Root, at same level.
	_ASSERT(pCommonAncestor->Value == 15);

	pCommonAncestor = func(pRoot, pNode12, pNode20);  //both on right subree of Root, but different levels
	_ASSERT(pCommonAncestor->Value == 13);

	pCommonAncestor = func(pRoot, pNode14, pNode5); // N1 right of root, N2 left of root
	_ASSERT(pCommonAncestor->Value == 9);

	pCommonAncestor = func(pRoot, pNode14, pNode14);  //same N1 and N2
	_ASSERT(pCommonAncestor->Value == 14);

	pCommonAncestor = func(pRoot, pRoot, pRoot);  //N1=N2=root
	_ASSERT(pCommonAncestor->Value == 9);

	pCommonAncestor = func(pRoot, pNode13, pNode15);  //N2 is right child of N1
	_ASSERT(pCommonAncestor->Value == 13);

	pCommonAncestor = func(pRoot, pNode5, pNode6);  //N1 is left child of N2
	_ASSERT(pCommonAncestor->Value == 6);

	pCommonAncestor = func(pRoot, nullptr, pNode6);  //N1 is null
	_ASSERT(!pCommonAncestor);

	pCommonAncestor = func(nullptr, pNode5, pNode6);  //Root is null
	_ASSERT(!pCommonAncestor);

	NODE * pTemp = new NODE(5);
	pCommonAncestor = func(pRoot, pTemp, pTemp);  //node not in tree.
	_ASSERT(!pCommonAncestor);


	pCommonAncestor = func(pRoot, pRoot, pNode20);  //N2 is right descendant of Root
	_ASSERT(pRoot->Value == 9);

	pCommonAncestor = func(pRoot, pRoot, pNode11);  //N2 is left descendant of N2
	_ASSERT(pRoot->Value == 9);

	if (func == FindCommonAncestor1)  //does not work for FindCommonAncestor2 as it assumes nodes are both in the tree.
	{
		pCommonAncestor = func(pRoot, pNode5, pTemp);  //node N2 not in tree.
		_ASSERT(!pCommonAncestor);
	}
	
}

int main()
{

	TestCommonAncestor(FindCommonAncestor1);

	TestCommonAncestor(FindCommonAncestor2);



	printf_s("\nAll Tests Passed!\n");
	int n;
	cin >> n;
}
