#include <iostream>
#include <stack>
#include <sstream>
#include<xstring>   //std::string

using namespace std;


typedef struct NODE
{
	union
	{
		char op;
		int num;
	} Content;

	bool ContentType : 1;   //0 is opcode , 1 is number.

	int Value();
	void DisplayValue();

	NODE* Left, *Right;

	NODE() : Left(nullptr), Right(nullptr)
	{}
} NODE;

void NODE::DisplayValue()
{
	if (ContentType == 0)
	{
		cout << Content.op << ' ';
	}
	else
		cout << Content.num << ' ';

}

int NODE::Value()
{
	if (ContentType == 0)  //opcode
	{
		switch (Content.op)
		{
			case '+':
				return Left->Value() + Right->Value();
			case '-':
				return Left->Value() - Right->Value();
			case '*':
				return Left->Value() * Right->Value();
			case '/':
				return Left->Value() / Right->Value();
			default:
				//should not reach here
				break;
		}
	}
	else  //number
	{
		return Content.num;
	}
}

void InfixToPostfix(_In_ char * infix, _Out_ char * postfix)
{
	stack<char> opStack;
	bool addspace = false;

	while (infix && *infix != '\0')
	{
		switch (*infix)
		{
		case '(':   
			break;  //skip '('
			
		case '+':
		case '-':
		case '*':
		case '/':
			{
				opStack.push(*infix);
				addspace = true;
			}
			break;

		case ')':    // on ')' pop operator from stack
			{
					*postfix++ = ' ';
					*postfix++ = opStack.top(); opStack.pop();
			}
			break;

		default:   //keep adding numbers to postfix unless you see ) in which case you pop operator
			{
				if (addspace)
				{
					*postfix++ = ' ';
					addspace = false;
				}

				//should check to see if numeric only
				*postfix++ = *infix;
			}
			break;
		}

		infix++;
	}


	//add remaining operators to end of string.
	while (!opStack.empty())
	{
		*postfix++ = ' ';
		*postfix++ = opStack.top(); opStack.pop();
	}
	*postfix = '\0';


}


NODE* PostfixToTree(std::string postfix)
{
	istringstream istr(postfix);
	char token;

	stack<NODE*> stack;
	NODE *node = nullptr;

	while (istr >> token)    //this will pop one char at a time from the stream
	{
		switch (token)
		{
			case '+':
			case '-':
			case '*':
			case '/':
			{
						// pop last two nodes from stack to form children of operator node and then push operator node back on stack.
						node = new NODE();
						node->Content.op = token;
						node->ContentType = 0;  //operator bit flag.

						node->Right = stack.top(); stack.pop();
						node->Left = stack.top(); stack.pop();

						stack.push(node);

			}
				break;
			default:   //means it is a number, keep pushing operand nodes to the stack.
			{
						   istr.putback(token);
						   int num;
						   istr >> num;

						   node = new NODE();
						   node->Content.num = num;
						   node->ContentType = 1; // number bit flag

						   stack.push(node);
			}
				break;
		}

	}

	
	//at this point there should be only 1 node remaining which is the root.
	return stack.top();

}

enum TraverseOrder
{
	InOrder,
	PreOrder,
	PostOrder
} ;

void TraverseTree(NODE* node, TraverseOrder traverseOrder)
{
	switch (traverseOrder)
	{
		case InOrder:
		{
						if (node != nullptr)
						{
							TraverseTree(node->Left, traverseOrder);
							node->DisplayValue();
							TraverseTree(node->Right, traverseOrder);


						}
		}
			break;
		case PreOrder:
		{
						 if (node != nullptr)
						 {
							 node->DisplayValue();
							 TraverseTree(node->Left, traverseOrder);
							 TraverseTree(node->Right, traverseOrder);
						 }

		}
			break; 
		case PostOrder:
		{
						  if (node != nullptr)
						  {
							  TraverseTree(node->Left, traverseOrder);
							  TraverseTree(node->Right, traverseOrder);
							  node->DisplayValue();
						  }
		}
			break;
		default:
			break;
	}
	

}



int EvaluatePostfix(string postfix)
{
	istringstream istr(postfix);

	stack<int> stack;
	char token;
	int right, left, num;

	while (istr >> token)
	{
		switch (token)
		{
			case '+':
			{
						right = stack.top(); stack.pop();
						left = stack.top(); stack.pop();
						stack.push(left + right);
			}
				break;
			case '-':
			{
						right = stack.top(); stack.pop();
						left = stack.top(); stack.pop();
						stack.push(left - right);


			}
				break;
			case '/':
			{
						right = stack.top(); stack.pop();
						left = stack.top(); stack.pop();
						stack.push(left / right);

			}
				break;
			case '*':
			{
						right = stack.top(); stack.pop();
						left = stack.top(); stack.pop();
						stack.push(left * right);

			}
				break;
			default:  //means it is a number
			{
						  istr.putback(token);
						  istr >> num;
						  stack.push(num);
			}
				break;
		}

	}

	return stack.top();

}


int main()
{

	char postfix [_MAX_PATH];
	NODE* Root = nullptr;

	char * infix = "1+(23*10)/5";

	InfixToPostfix(infix, postfix);
	Root = PostfixToTree(postfix);
	printf("(Infix) %s  => (Postfix) %s  => (value) %d %d\n", infix, postfix, Root->Value(), EvaluatePostfix(postfix));
	TraverseTree(Root, PreOrder);


	int n;
	cin >> n;


}