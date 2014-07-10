#include <iostream>
#include <iomanip>
#include <stack>

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


enum TraverseOrder
{
	PreOrder,
	InOrder,
	PostOrder,
	PostOrderOneStack,
	PostOrderTwoStacks,
};


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

		//Recursively keep finding the element.
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
	

	void TraverseTreeRecursiveImpl(NODE<T>* node, int traverseOrder)
	{
		switch (traverseOrder)
		{
		case InOrder:
			if (node != nullptr)
			{
				TraverseTreeRecursiveImpl(node->Left, traverseOrder);
				cout << node->Value << " ";
				TraverseTreeRecursiveImpl(node->Right, traverseOrder);
			}
			break;
		case PreOrder:
			if (node != nullptr)
			{
				cout << node->Value << " ";
				TraverseTreeRecursiveImpl(node->Left, traverseOrder);
				TraverseTreeRecursiveImpl(node->Right, traverseOrder);
			}
			break;
		case PostOrder:
			if (node != nullptr)
			{
				TraverseTreeRecursiveImpl(node->Left, traverseOrder);
				TraverseTreeRecursiveImpl(node->Right, traverseOrder);
				cout << node->Value << " ";
			}
			break;
		default:
			break;
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

	void TraverseTree(_In_ int traverseOrder = InOrder)
	{
		printf("Traversing Tree :");
		TraverseTreeRecursiveImpl(this->Root, traverseOrder);
		printf("\n");
	}

	void TraverseTreeWithoutRecursion(_In_ int traverseOrder = InOrder)
	{
		stack<NODE<T>*> stack, outputStack;
		NODE<T> * curr = nullptr;


		switch (traverseOrder)
		{
			case InOrder:
			{
				//current solution is using current pointer. You could also keep track of visited nodes, but then that requires additional storage in Node struct.
  
				curr = this->Root;
				while (!stack.empty() || curr)
				{
					if (curr)     //keep iterating down the left tree pushing visited node onto the stack.
					{
						stack.push(curr);
						curr = curr->Left;
					}
					else   
					{
						//when reach leftmost leaf node, pop parent from stack, print it and visit right subtree.
						curr = stack.top(); stack.pop();
						cout << curr->Value << " ";
						curr = curr->Right;
					}
				}

			}
			break;
		case PreOrder:  //easiest
			{

				stack.push(this->Root);

				while (!stack.empty())
				{
					curr = stack.top(); stack.pop();

					if (curr)
					{
						cout << curr->Value << " ";
						stack.push(curr->Right);  //push right first so that it is processed last.
						stack.push(curr->Left);  //push left last so that it is processed before right.
					}
				}
			}
			break;

		case PostOrderOneStack:   //most difficult
			{
				stack.push(this->Root);
				curr = this->Root;
				NODE<T> * prev = nullptr;  //previously traversed node . this helps to know whether you are traversing up or down the tree based on its relation to curr.

				while (!stack.empty())
				{
					curr = stack.top();  //just peek, dont pop here.
					if (!prev || prev->Left == curr || prev->Right == curr)  //traversing down, if prev == null or curr is left/right child of prev
					{
						if (curr->Left)   //traverse  down left if there is a left node
						{
							stack.push(curr->Left);
						}
						else if (curr->Right)   //traverse  down right if there is a right node
						{
							stack.push(curr->Right);
						}
					}
					else if (curr->Left == prev)  //traversing up from left
					{
						if (curr->Right)
						{
							stack.push(curr->Right);
						}
					}
					else   //traversing up from right. Traversing up from right means you are done and need to print current node.
					{
						cout << curr->Value << " ";
						stack.pop();  //actually printing the value, so pop it off here.
					}

					prev = curr;

				}
			}
			break;

		case PostOrderTwoStacks:
			{
				/*
				An alternative solution is to use two stacks.  it is doing a reversed pre-order traversal. That is, the order of traversal is a node, then its right child followed by its left child. This yields post-order traversal in reversed order. Using a second stack, we could reverse it back to the correct order.
				*/

				stack.push(this->Root);

				while (!stack.empty())
				{
					curr = stack.top(); stack.pop();
					outputStack.push(curr);  //push to second stack.

					if (curr->Left) stack.push(curr->Left);   // in this case push left first so that it goes into outputstack last.
					if (curr->Right)  stack.push(curr->Right);
				}

				//stack has post-order in reverse order
				while (!outputStack.empty())
				{
					curr = outputStack.top(); outputStack.pop();
					cout << curr->Value << " ";
				}
			}
			break;
		}
		
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


	printf("\nPreOrder Traversal:");
	BSTInt->TraverseTree(PreOrder);
	printf("\nInOrder Traversal:");
	BSTInt->TraverseTree(InOrder);
	printf("\nPostOrder Traversal:");
	BSTInt->TraverseTree(PostOrder);


	printf("\nPreOrder Traversal Without Recursion:");
	BSTInt->TraverseTreeWithoutRecursion(PreOrder);
	printf("\nInOrder Traversal Without Recursion:");
	BSTInt->TraverseTreeWithoutRecursion(InOrder);
	printf("\nPostOrder Traversal Without Recursion- One Stack:");
	BSTInt->TraverseTreeWithoutRecursion(PostOrderOneStack);
	printf("\nPostOrder Traversal Without Recursion - Two Stacks:");
	BSTInt->TraverseTreeWithoutRecursion(PostOrderTwoStacks);


	printf("\n\n");
	BST<char>* BSTChar = new BST<char>();

	char itemsChar[7] = {'F', 'D', 'J', 'A', 'K', 'E', 'C'};
	BSTChar->Insert(itemsChar, _countof(itemsChar)); 
	BSTChar->DisplayTree();
	printf("\nThe tree is a BST : %s\n\n", BSTChar->IsBST('A' - 1, 'Z' + 1) ? "True" : "False");


	int n;
	std::cin >> n;

}