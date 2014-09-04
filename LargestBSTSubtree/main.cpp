#include <iostream>
#include <iomanip>
#include <stack>

using namespace std;


/***************************************************************************************************************

1. Document your assumptions
	- Leaves of a Tree are considered as BSTs.
	- empty tree is considered BST.
	- BST node can have duplicate child node in the tree either to its left or right.
2. Explain your approach and how you intend to solve the problem 
	- I started with top-down approach to check if tree is BST and bottom-up to pass up largest BST tree info, but realized that my code would not walk through subtrees if node itself does not meet BST condition. Then switched to bottom-up.
	- General approach is to use recursion to walk down the tree, and gather information bottom up for min/max values for each node, whether subtrees under it are  BST and their respective children count if so.
	- Once we reach the bottom, we send up min/max values up the tree for parent node to evaluate its BST condition, start counting the children in the BST and pass it up to its parent.
		- If the parent is also satisfing the BST condition along with its left and right subtree, then it becomes the largest BST and is passed up.
		- else we compare left and right subtrees to see which has passed on the larger BST and then pass that up. Eventually, the root will have information on the largest BST and its children.
3. Provide code comments where applicable 
   - See below
4. Explain the big-O run time complexity of your solution. Justify your answer.
   - This algorithm has to visit every node in the tree since at each node it has to find if there is a subtree in either of its children.
   - That means complexity is O(N) where N is number of nodes in the tree.

   
**************************************************************************************************************/

struct NODE
{
	int Value;
	NODE *Left, *Right;

	NODE(int val) :
		Value(val),
		Left(nullptr),
		Right(nullptr)
	{}
};





void DisplayTree(NODE *node, int indent = 0, char *LR = "") //can specify default values for optional parameters.
{

	//do in-order traversal in reverse to show the tree from left to right (instead of top down) and right (top) to left (bottom). 
	if (node == nullptr) return;
	DisplayTree(node->Right, indent + 6, "R:");

	std::cout << setw(indent) << " ";   //Specifies the width of the display field for the next element in the stream.
	cout << LR << node->Value << endl;

	DisplayTree(node->Left, indent + 6, "L:");


}

/*
minValue, maxValue:  Boundary value limits for current node's value. It has be within it. This info is propagated bottom-up as we walk up the tree.
					 For each node, the minValue is the maxValue of its left subtree and maxValue is the minValue of its right subtree.


IsBST	          : This is bubbled up from children subtrees and used by current node to decide if itself is a BST.
LargestBSTChildrenCount: This is bubbled up from children subtrees and used to track largest BST under current node.
LargestBSTSubtreeRoot: This is information that is bubbled up from the subtrees and used to track largest BST under current node.
*/
void FindLargestBSTSubtreeRecursive(_In_ NODE *node, _In_ int *minValue, _In_ int *maxValue, _Out_ bool *IsBST, _Out_ int *LargestBSTChildrenCount, _Outptr_result_maybenull_ NODE **LargestBSTSubtreeRoot)
{
	bool isLeftBST = true, isRightBST = true;
	int leftLargestBSTChildrenCount = 0, rightLargestBSTChildrenCount = 0;
	NODE *leftLargestBSTSubtreeRoot = nullptr, *rightLargestBSTSubtreeRoot = nullptr;
	int nodeMinValue, nodeMaxValue;

	if (node == nullptr)
	{
		*IsBST = true;  // empty tree is considered a BST.
		*LargestBSTChildrenCount = 0;
		*LargestBSTSubtreeRoot = nullptr;
		return;
	}

	if (node->Left == nullptr)
	{
		*minValue = nodeMinValue = node->Value;
	}
	else
	{
		FindLargestBSTSubtreeRecursive(node->Left, minValue, maxValue, IsBST, LargestBSTChildrenCount, LargestBSTSubtreeRoot);
		isLeftBST = *IsBST;
		leftLargestBSTChildrenCount = *LargestBSTChildrenCount;
		leftLargestBSTSubtreeRoot = *LargestBSTSubtreeRoot;

		// minValue for current node is the maxValue of its left subtree.
		nodeMinValue = *maxValue;
	}

	if (node->Right == nullptr)
	{
		*maxValue = nodeMaxValue = node->Value;
	}
	else
	{
		FindLargestBSTSubtreeRecursive(node->Right, minValue, maxValue, IsBST, LargestBSTChildrenCount, LargestBSTSubtreeRoot);
		isRightBST = *IsBST;
		rightLargestBSTChildrenCount = *LargestBSTChildrenCount;
		rightLargestBSTSubtreeRoot = *LargestBSTSubtreeRoot;

		//maxValue for current node is the minValue of its right subtree.
		nodeMaxValue = *minValue;
	}

	//A node meets BST condition if both its subtrees are BST and it is within its min/max bounds.
	if (!isLeftBST || !isRightBST || (node->Value < nodeMinValue || node->Value > nodeMaxValue))
	{
		*IsBST = false;

		if (leftLargestBSTChildrenCount > rightLargestBSTChildrenCount)
		{
			*LargestBSTChildrenCount = leftLargestBSTChildrenCount;
			*LargestBSTSubtreeRoot = leftLargestBSTSubtreeRoot;
		}
		else
		{
			*LargestBSTChildrenCount = rightLargestBSTChildrenCount;
			*LargestBSTSubtreeRoot = rightLargestBSTSubtreeRoot;
		}
	}
	else
	{
		*IsBST = true;
		*LargestBSTChildrenCount = 1 + leftLargestBSTChildrenCount + rightLargestBSTChildrenCount;
		*LargestBSTSubtreeRoot = node;

		*minValue = nodeMinValue;
		*maxValue = nodeMaxValue;
	}
	
}

NODE *FindLargestBSTSubtree(NODE *Root)
{
	NODE *LargestBSTSubtree = nullptr;
	int largestBSTChildrenCount = 0;
	bool isBST = false;
	int minValue = 0, maxValue = 0;

	FindLargestBSTSubtreeRecursive(Root, &minValue, &maxValue, &isBST, &largestBSTChildrenCount, &LargestBSTSubtree);

	return LargestBSTSubtree;
}

NODE *CreateTree1()
{
	NODE *root = new NODE(9);
	NODE *node6, *node13, *node15;
	root->Left = node6 = new NODE(6);
	root->Right = node13 = new NODE(13);

	node6->Left = new NODE(5);
	node6->Right = new NODE(11);

	node13->Left = new NODE(12);
	node13->Right = node15 = new NODE(15);

	node15->Left = new NODE(14);
	node15->Right = new NODE(20);

	return root;
}

NODE *CreateTreeWithOnlyRoot()
{
	return new NODE(5);
}

NODE *CreateTreeWithOnlyLeftChildren()
{
	NODE *node3;
	NODE *root = new NODE(5);
	root->Left = node3 = new NODE(3);
	node3->Left = new NODE(2);

	return root;
}

NODE *CreateTreeWithOnlyRightChildren()
{
	NODE *node6;
	NODE *root = new NODE(5);
	root->Right = node6 = new NODE(6);
	node6->Right = new NODE(7);

	return root;
}

//This will return a leaf node since leaves are considered BST of size 1.
NODE *CreateTreeWithNoBST()
{
	NODE *root = new NODE(9);
	NODE *node6, *node13, *node15;
	root->Left = node6 = new NODE(6);
	root->Right = node13 = new NODE(13);

	node6->Left = new NODE(11);
	node6->Right = new NODE(5);

	node13->Left = new NODE(15);
	node13->Right = node15 = new NODE(15);

	node15->Left = new NODE(17);
	node15->Right = new NODE(20);

	return root;
}

NODE *CreateFullTreeAsBST()
{
	NODE *root = new NODE(9);
	NODE *node6, *node13, *node15;
	root->Left = node6 = new NODE(6);
	root->Right = node13 = new NODE(13);

	node6->Left = new NODE(5);
	node6->Right = new NODE(8);

	node13->Left = new NODE(12);
	node13->Right = node15 = new NODE(15);

	node15->Left = new NODE(14);
	node15->Right = new NODE(20);

	return root;
}


NODE *CreateLargerBSTTreeOnRight()
{
	NODE *root = new NODE(9);
	NODE *node10, *node13, *node15;
	root->Left = node10 = new NODE(10);
	root->Right = node13 = new NODE(13);

	node10->Left = new NODE(5);
	node10->Right = new NODE(11);

	node13->Left = new NODE(12);
	node13->Right = node15 = new NODE(15);

	node15->Left = new NODE(14);
	node15->Right = new NODE(20);

	return root;
}


NODE *CreateLargerBSTTreeOnLeft()
{
	NODE *root = new NODE(9);
	NODE *node6, *node13, *node11, *node5;
	root->Left = node6 = new NODE(6);
	root->Right = node13 = new NODE(13);

	node6->Left = node5 = new NODE(5);
	node6->Right = new NODE(8);

	node5->Left = new NODE(4);

	node13->Left = new NODE(12);
	node13->Right = node11 = new NODE(11);

	node11->Left = new NODE(10);
	node11->Right = new NODE(20);

	return root;
}


//algorithm will be pick right BST if there are two BSTs of same size.
NODE *CreateEqualSizedBSTsOnLeftAndRight()
{
	NODE *root = new NODE(9);
	NODE *node6, *node8, *node15;
	root->Left = node6 = new NODE(6);
	root->Right = node8 = new NODE(8); // invalide BST, but children under it are.

	node6->Left = new NODE(5);
	node6->Right = new NODE(10);

	node8->Left = new NODE(14);
	node8->Right = node15 = new NODE(15);

	node15->Left = new NODE(14);
	node15->Right = new NODE(20);

	return root;
}

void TestGivenSample()
{
	printf("\nInput tree (Given Sample): \n");
	NODE *root = CreateTree1();
	DisplayTree(root);

	printf("\nLargest BST Subtree tree: \n");
	NODE *largestBSTSubtree = FindLargestBSTSubtree(root);
	DisplayTree(largestBSTSubtree);

	_ASSERT(largestBSTSubtree->Value == 13);
}

void TestNullRoot()
{
	printf("\nInput tree (Null Root): \n");
	DisplayTree(nullptr);

	printf("\nLargest BST Subtree tree: \n");
	NODE *largestBSTSubtree = FindLargestBSTSubtree(nullptr);
	DisplayTree(largestBSTSubtree);

	_ASSERT(largestBSTSubtree == nullptr);

}


void TestTreeWithOnlyRoot()
{
	printf("\nInput tree (Only Root): \n");
	NODE *root = CreateTreeWithOnlyRoot();
	DisplayTree(root);

	printf("\nLargest BST Subtree tree: \n");
	NODE *largestBSTSubtree = FindLargestBSTSubtree(root);
	DisplayTree(largestBSTSubtree);

	_ASSERT(largestBSTSubtree->Value == 5);

}

void TestTreeWithOnlyLeftChildren()
{
	printf("\nInput tree (Tree with only Left children): \n");
	NODE *root = CreateTreeWithOnlyLeftChildren();
	DisplayTree(root);

	printf("\nLargest BST Subtree tree: \n");
	NODE *largestBSTSubtree = FindLargestBSTSubtree(root);
	DisplayTree(largestBSTSubtree);

	_ASSERT(largestBSTSubtree->Value == 5);

}

void TestTreeWithOnlyRightChildren()
{
	printf("\nInput tree (Tree with only Right children): \n");
	NODE *root = CreateTreeWithOnlyRightChildren();
	DisplayTree(root);

	printf("\nLargest BST Subtree tree: \n");
	NODE *largestBSTSubtree = FindLargestBSTSubtree(root);
	DisplayTree(largestBSTSubtree);

	_ASSERT(largestBSTSubtree->Value == 5);

}
void TestLeafBSTTreeFound()
{
	printf("\nInput tree (No BST Subtree, only leaves are BST): \n");
	NODE *root = CreateTreeWithNoBST();
	DisplayTree(root);

	printf("\nLargest BST Subtree tree: \n");
	NODE *largestBSTSubtree = FindLargestBSTSubtree(root);
	DisplayTree(largestBSTSubtree);

	_ASSERT(largestBSTSubtree->Value == 20);
}

void TestFullTreeAsBST()
{
	printf("\nInput tree (Full Tree is BST): \n");
	NODE *root = CreateFullTreeAsBST();
	DisplayTree(root);

	printf("\nLargest BST Subtree tree: \n");
	NODE *largestBSTSubtree = FindLargestBSTSubtree(root);
	DisplayTree(largestBSTSubtree);

	_ASSERT(largestBSTSubtree->Value == 9);
}

void TestLargerBSTTreeOnRight()
{
	printf("\nInput tree (Larger BST is on right, left also has a BST): \n");
	NODE *root = CreateLargerBSTTreeOnRight();
	DisplayTree(root);

	printf("\nLargest BST Subtree tree: \n");
	NODE *largestBSTSubtree = FindLargestBSTSubtree(root);
	DisplayTree(largestBSTSubtree);

	_ASSERT(largestBSTSubtree->Value == 13);

}

void TestLargerBSTTreeOnLeft()
{
	printf("\nInput tree (Larger BST is on left, right also has a BST): \n");
	NODE *root = CreateLargerBSTTreeOnLeft();
	DisplayTree(root);

	printf("\nLargest BST Subtree tree: \n");
	NODE *largestBSTSubtree = FindLargestBSTSubtree(root);
	DisplayTree(largestBSTSubtree);

	_ASSERT(largestBSTSubtree->Value == 6);

}

//Algorithm will pick right BST
void TestEqualSizedBSTsOnLeftAndRight()
{
	printf("\nInput tree (Left and Right have equal BSTs, returns right): \n");
	NODE *root = CreateEqualSizedBSTsOnLeftAndRight();
	DisplayTree(root);

	printf("\nLargest BST Subtree tree: \n");
	NODE *largestBSTSubtree = FindLargestBSTSubtree(root);
	DisplayTree(largestBSTSubtree);

	_ASSERT(largestBSTSubtree->Value == 15);
}




int main()
{

	TestGivenSample();

	TestNullRoot();

	TestTreeWithOnlyRoot();

	TestTreeWithOnlyLeftChildren();

	TestTreeWithOnlyRightChildren();

	TestLeafBSTTreeFound();

	TestFullTreeAsBST();

	TestLargerBSTTreeOnRight();

	TestLargerBSTTreeOnLeft();

	TestEqualSizedBSTsOnLeftAndRight();

	printf_s("\n\nAll Tests Passed!");
	int n;
	std::cin >> n;

}



/******************************************************************************************


Input tree (Given Sample):
                  R:20
            R:15
                  L:14
      R:13
            L:12
 9
            R:11
      L:6
            L:5

Largest BST Subtree tree:
            R:20
      R:15
            L:14
 13
      L:12

Input tree (Null Root):

Largest BST Subtree tree:

Input tree (Only Root):
 5

Largest BST Subtree tree:
 5

Input tree (Tree with only Left children):
 5
      L:3
            L:2

Largest BST Subtree tree:
 5
      L:3
            L:2

Input tree (Tree with only Right children):
            R:7
      R:6
 5

Largest BST Subtree tree:
            R:7
      R:6
 5

Input tree (No BST Subtree, only leaves are BST):
                  R:20
            R:15
                  L:17
      R:13
            L:15
 9
            R:5
      L:6
            L:11

Largest BST Subtree tree:
 20

Input tree (Full Tree is BST):
                  R:20
            R:15
                  L:14
      R:13
            L:12
 9
            R:8
      L:6
            L:5

Largest BST Subtree tree:
                  R:20
            R:15
                  L:14
      R:13
            L:12
 9
            R:8
      L:6
            L:5

Input tree (Larger BST is on right, left also has a BST):
                  R:20
            R:15
                  L:14
      R:13
            L:12
 9
            R:11
      L:10
            L:5

Largest BST Subtree tree:
            R:20
      R:15
            L:14
 13
      L:12

Input tree (Larger BST is on left, right also has a BST):
                  R:20
            R:11
                  L:10
      R:13
            L:12
 9
            R:8
      L:6
            L:5
                  L:4

Largest BST Subtree tree:
      R:8
 6
      L:5
            L:4

Input tree (Left and Right have equal BSTs, returns right):
                  R:20
            R:15
                  L:14
      R:8
            L:14
 9
            R:10
      L:6
            L:5

Largest BST Subtree tree:
      R:20
 15
      L:14









*****************************************************************************************/