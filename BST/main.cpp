#include <iostream>
#include <iomanip>

using namespace std;

//BST cannot have duplicate nodes.


/*
struct vs class
not much difference other than struct defaults to public members whereas class defaults to private members. You can change scope by
using private/public modifiers.

structs are typically used for POD (plain old data) types.

*/

template<typename T>
struct NODE
{
	T Value;
	NODE * Left, *Right;

	NODE(T val) : 
		Value(val), 
		Left(nullptr), 
		Right(nullptr) 
	{}
};

#define TRAVERSE_PREORDER 0
#define TRAVERSE_INORDER 1
#define TRAVERSE_POSTORDER 2


/*
//Build a BST class with templates

Search
Insert
Delete
Balance
Display
IsBST
IsBalanced
GetDepth

*/



template<typename T>   // you can use <typename T> or <class T> , they both mean same. Typename is more clear since it can mean primitive types as well as classes.
class  BST
{
private:
	NODE<T> * Root;

	//When defining template methods, you have to define them inline.
	NODE<T>* FindTreeItemImpl(NODE<T>* rootNode, T item)
	{
		if (rootNode == nullptr) return rootNode;

		if (rootNode->Value == item)
			return rootNode;

		if (item < rootNode->Value)
			return FindTreeItemImpl(rootNode->Left, item);
		else
			return FindTreeItemImpl(rootNode->Right, item);
	}

	void InsertImpl(_Inout_ NODE<T>** node, _In_ T item)   //could use NODE<T>*& instead of NODE<T>** ie. pointer by ref instead of pointer to pointer. No difference, just depends on preference.
	{
		if (*node == nullptr)
		{
			*node = new NODE<T>(item);
			return;
		}

		if (item < (*node)->Value)
			InsertImpl(&(*node)->Left, item);
		else if (item >(*node)->Value)
			InsertImpl(&(*node)->Right, item);
		//ignore duplicates.
	}

	//By default traverse postorder
	void DisplayTreeImpl(_In_ NODE<T>* node, _In_ int indent = 0, _In_ char* LR = "") //can specify default values for optional parameters.
	{
		
		//do post-order traversal to show the tree from left to right (instead of top down) and right (top) to left (bottom). 
		if (node == nullptr) return;
		DisplayTreeImpl(node->Right, indent + 6, "R:");

		std::cout << setw(indent) << " ";   //Specifies the width of the display field for the next element in the stream.
		cout << LR << node->Value << endl;

		DisplayTreeImpl(node->Left, indent + 6, "L:");


	}

	void DeleteTreeImpl(_In_ NODE<T>* node)
	{
		DeleteTreeImpl(node->Left);
		DeleteTreeImpl(node->Right);
		delete(node);
	}


	bool IsBSTImpl(_In_ NODE<T> * node, T minValue, T maxValue)
	{
		if (node == nullptr) return true;
		if (node->Value > maxValue || node->Value < minValue)     //if node violates limits passed down then it is not BST
		{
			cout << "Not a BST: Violating node :" << node->Value << "\n";
			return false;
		}

		return IsBSTImpl(node->Left, minValue, node->Value)   // the left subtree has a max limit
			&& IsBSTImpl(node->Right, node->Value, maxValue);   //right subtree has min limit
	}

	NODE<T>* FindMin(NODE<T> * node)
	{
		while (node->Left)
		{
			node = node->Left;
		}
		return node;
	}

	void DeleteItemImpl(_Inout_ NODE<T> ** node, T item)
	{
		if (*node == nullptr) return;

		if (item < (*node)->Value)
			DeleteItemImpl(&(*node)->Left, item);
		else if (item >(*node)->Value)
			DeleteItemImpl(&(*node)->Right, item);
		else  
		{
			//found node to delete

			if ((*node)->Left && (*node)->Right)
			{
				//find min in right subtree or find max in left subtree.
				NODE<T>* minNodeInRightSubtree = FindMin((*node)->Right);

				//Replace values 
				(*node)->Value = minNodeInRightSubtree->Value;
				
				//now delete the min node from the right subtree
				DeleteItemImpl(&(*node)->Right, minNodeInRightSubtree->Value);
			}
			else 
			{
				NODE<T> * temp = *node;
				if ((*node)->Left)
				{
					//only left child. replace with left child.
					*node = (*node)->Left;
					delete(temp);
					temp = nullptr;
				}
				else if ((*node)->Right)
				{
					//only right child then replace with right child.
					*node = (*node)->Right;
					delete(temp);
					temp = nullptr;
				}
				else
				{
					// no children , delete node itself and set to null.
					delete (*node);
					*node = nullptr;
				}
			}
		}
	}
	

public:
	BST() : Root(nullptr) {};

	//Insert 
	void Insert(_In_ T item)
	{
		InsertImpl(&Root, item);
	}

	// have to pass in count since when we pass in int[] , it gets converted to int * (and it loses array count info) and _countof does not like int *. Also sizeof loses the information of array count.
	void Insert(_In_ T items[], int countOfItems)
	{
		for (int i = 0; i < countOfItems; i++)
		{
			Insert(items[i]);
		}
	}


	void DeleteTree()
	{
		DeleteTreeImpl(this->Root);
	}

	void Delete(T item)
	{
		DeleteItemImpl(&this->Root, item);
	}

	bool IsBST(_In_ T minValue = INT_MIN , _In_ T maxValue = INT_MAX)
	{
		return IsBSTImpl(this->Root, minValue, maxValue);
	}

	NODE<T>* FindTreeItem(T item)
	{
		return FindTreeItemImpl(this->Root, item);
	}


	void DisplayTree()
	{
		DisplayTreeImpl(this->Root);
	}


};




int main()
{
	BST<int> * BSTInt = new BST<int>();

	int items[7] = { 4, 2, 6, 1, 3, 5, 7 };
	BSTInt->Insert(items, _countof(items)); // have to pass in count since when we pass in int[] , it gets converted to int * and _countof does not like int *
	BSTInt->DisplayTree();
	printf("\nThe tree is a BST : %s\n\n", BSTInt->IsBST()? "True" : "False");


	int items2[5] = { 18, 5, 13, 17, 6 };
	BSTInt->Insert(items2, _countof(items2));
	BSTInt->DisplayTree();
	printf("\nThe tree is a BST : %s\n\n", BSTInt->IsBST() ? "True" : "False");

	/* The auto keyword is a declaration specifier. However, the C++ standard defines an original and a revised meaning for this keyword.
	Before Visual C++ 2010, the auto keyword declares a variable in the automatic storage class; that is, a variable that has a local lifetime.
	Starting with Visual C++ 2010, the auto keyword declares a variable whose type is deduced from the initialization expression in its declaration. The /Zc:auto[-] compiler option controls the meaning of the auto keyword.
	*/
	auto t1 = BSTInt->FindTreeItem(3);
	auto t2 = BSTInt->FindTreeItem(5);
	t1->Value = 5; t2->Value = 3;
	BSTInt->DisplayTree();
	printf("\nThe tree is a BST : %s\n\n", BSTInt->IsBST() ? "True" : "False");
	t1->Value = 3; t2->Value = 5;


	printf("Deleting 4 \n");
	BSTInt->Delete(4);
	BSTInt->DisplayTree();
	printf("\nThe tree is a BST : %s\n\n", BSTInt->IsBST() ? "True" : "False");

	printf("Deleting 6 \n");
	BSTInt->Delete(6);
	BSTInt->DisplayTree();
	printf("\nThe tree is a BST : %s\n\n", BSTInt->IsBST() ? "True" : "False");

	printf("Deleting 2 \n");
	BSTInt->Delete(2);
	BSTInt->DisplayTree();
	printf("\nThe tree is a BST : %s\n\n", BSTInt->IsBST() ? "True" : "False");



	BST<char>* BSTChar = new BST<char>();

	char itemsChar[7] = {'F', 'D', 'J', 'A', 'K', 'E', 'C'};
	BSTChar->Insert(itemsChar, _countof(itemsChar)); 
	BSTChar->DisplayTree();
	printf("\nThe tree is a BST : %s\n\n", BSTChar->IsBST('A' - 1, 'Z' + 1) ? "True" : "False");




	int n;
	std::cin >> n;

}