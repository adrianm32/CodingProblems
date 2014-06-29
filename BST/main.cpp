#include <iostream>
#include <iomanip>

using namespace std;


/*
struct vs class
not much difference other than struct defaults to public members whereas class defaults to private members. You can change scope by
using private/public modifiers.

structs are typically used for POD (plain old data) types.


template <typename T>
struct NODE
{
	T item;
	NODE * left, * right;
};

*/


/*

Search
Insert
Delete
Balance
Display
IsBST
IsBalanced
GetDepth


*/


/*
Build a BST class with templates

template<typename T>   // you can use <typename T> or <class T> , they both mean same. Typename is more clear since it can mean primitive types as well as classes.
struct  BST
{
	NODE * Root;

	BST();
	void Insert(NODE * node);
	void Insert(T items []);
	void Display();

};

*/


template<typename T>
struct NODE
{
	T Value;
	NODE * Left = nullptr, *Right = nullptr;

	NODE(int val) : Value(val) {}
};


template<typename T>
void Insert(_Inout_ NODE<T>** Root, _In_ NODE<T> * item)
{
	if (*Root == nullptr)
	{
		*Root = item;
		return;
	}

	if (item->Value < (*Root)->Value)
		Insert(&(*Root)->Left, item);
	else
		Insert(&(*Root)->Right, item);
}

// have to pass in count since when we pass in int[] , it gets converted to int * (and it loses array count info) and _countof does not like int *. Also sizeof loses the information of array count.
template<typename T>
void Insert(_Inout_ NODE<T> ** Root, _In_ T items[], int countOfItems)
{

	for (int i = 0; i < countOfItems; i++)
	{
		Insert(Root, new NODE<T>(items[i]));
	}

}

template<typename T>
void DisplayTree(NODE<T>* Root, int indent = 0, char* LR = "")
{
	//do post-order traversal to show the tree from left to right (instead of top down) and right (top) to left (bottom). 
	if (Root == nullptr) return;
	DisplayTree(Root->Right, indent + 6, "R:");
	std::cout << setw(indent) << " ";   //Specifies the width of the display field for the next element in the stream.
	cout << LR << Root->Value << endl;
	DisplayTree(Root->Left, indent + 6, "L:");
}

template<typename T>
void DeleteTree(NODE<T> * Root)
{
	if (Root == nullptr) return;
	DeleteTree(Root->Left);
	DeleteTree(Root->Right);
	delete(Root);
}

int main()
{
	NODE<int>* RootInt = nullptr;

	int items[7] = { 4, 2, 6, 1, 3, 5, 7 };
	Insert<int>(&RootInt, items, _countof(items)); // have to pass in count since when we pass in int[] , it gets converted to int * and _countof does not like int *
	DisplayTree<int>(RootInt);
	printf("\n\n\n\n");

	int items2[4] = { 18, 5, 13, 17 };
	Insert<int>(&RootInt, items2, _countof(items2));
	DisplayTree<int>(RootInt);
	printf("\n\n\n\n");

	NODE<char> * RootChar = nullptr;

	char itemsChar[7] = {'F', 'D', 'J', 'A', 'K', 'E', 'C'};
	Insert<char>(&RootChar, itemsChar, _countof(itemsChar)); // have to pass in count since when we pass in int[] , it gets converted to int * and _countof does not like int *
	DisplayTree<char>(RootChar);
	printf("\n\n\n\n");



	int n;
	cin >> n;

}