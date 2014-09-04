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
	void DisplayInfix();

	NODE *Left, *Right;

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

void NODE::DisplayInfix()
{
	if (ContentType == 0)  //operator
	{
		cout << '(';
	}
	
	if (Left) Left->DisplayInfix();
	DisplayValue();
	if (Right) Right->DisplayInfix();


	if (ContentType == 0)
	{
		cout << ')';
	}

}

int NODE::Value()
{
	if (ContentType == 0)  //opcode
	{
		switch (Content.op)
		{
			case '+':
				return Left->Value() + Right->Value();  //no need to check to see if left and right are not null
			case '-':
				return Left->Value() - Right->Value();
			case '*':
				return Left->Value() *Right->Value();
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

	return Content.num;
}


_Check_return_ bool HasHigherPrecedence(_In_ char op1, _In_ char op2)
{
	std::string operators = "(+-*/";

	int precedence[] = {0 /*(*/, 1 /*+*/, 1 /*-*/, 2 /***/, 2 /*/*/};

	return precedence[operators.find(op1)] >= precedence[operators.find(op2)];
}


// 1 + 2 *3 + 5   =>  1 2 3 *5 + +
void InfixToPostfix(_In_z_ char *infix, _Out_ char *postfix)
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

				//check for precedence. Can push onto stack only if higher precedece than topmost operator of stack.
				while (!opStack.empty() && HasHigherPrecedence(opStack.top(), *infix))
				{
					*postfix++ = ' ';
					*postfix++ = opStack.top(); opStack.pop();
				}

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


_Maybenull_ NODE *PostfixToTree(_In_ std::string postfix)
{
	istringstream istr(postfix);
	char token;

	stack<NODE *> stack;
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

void TraverseTree(_In_opt_ NODE *node, _In_ TraverseOrder traverseOrder)
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



int EvaluatePostfix(_In_ string postfix)
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
						stack.push(left *right);

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



/************************************************************************************************

               INFIX TO PREFIX  / PREFIX TO INFIX

			   always start scanning from end of string and build stack(Reverse)

			   http://scanftree.com/Data_Structure/prefix-to-infix

			   http://scanftree.com/Data_Structure/infix-to-prefix





********************************************************************************************/



/*
// 1 + ( 2 *3)  =>        + 1 *2 3
// 1 + 2 *3 + 5 =>   + 1 + *2 3 5
//http://www.slideshare.net/khateeb321/infix-to-prefix

start from reverse end of string moving backwards.

*/

void InfixToPrefix(_In_z_ char *infix, _Out_ char *prefix)
{

	stack<char> prefixStack, operatorStack;

	int len = strlen(infix);

	//start iterating the infix string from reverse direction and push operators from operator stack to prefix stack on precedence violation
	for (int i = len -1; i >= 0; i--)
	{
		switch (infix[i])
		{
			case '+':
			case '-':
			case '*':
			case '/':
			{
						//check of precedence. cannot add operator on top of higher precedence operator.
						while (!operatorStack.empty() && HasHigherPrecedence(operatorStack.top(), infix[i]))
						{
							prefixStack.push(operatorStack.top());  //transfer operator from operator stack to prefix stack.
							operatorStack.pop();
						}
						operatorStack.push(infix[i]);
			}
				break;
			case '(':
			{

						prefixStack.push(operatorStack.top());  //transfer operator from operator stack to prefix stack.
						operatorStack.pop();
			}
				
				break;
			case ')': // ignore
				break;
			default:  // number;
				{
						  prefixStack.push(infix[i]);
		
				}
				break;
		}



	}

	while (!operatorStack.empty())
	{
		prefixStack.push(operatorStack.top());  //transfer operator from operator stack to prefix stack.
		operatorStack.pop();
	}

	while (!prefixStack.empty())
	{
		*prefix++ = prefixStack.top(); prefixStack.pop();
	}

	*prefix = '\0';

}




void PrefixToInfix()
{

}


int main()
{

	char postfix [_MAX_PATH], prefix[_MAX_PATH];
	NODE *Root = nullptr;

	char *infix = "1+(23*10)/5";

	InfixToPostfix(infix, postfix);
	Root = PostfixToTree(postfix);
	printf("(Infix) %s  => (Postfix) %s  => (value) %d %d\n", infix, postfix, Root->Value(), EvaluatePostfix(postfix));
	TraverseTree(Root, PreOrder);
	printf("\nBack to Infix: ");
	Root->DisplayInfix();

	InfixToPostfix("1 + 2 *3 + 5", postfix);
	Root = PostfixToTree(postfix);
	printf("\n(Infix) %s  => (Postfix) %s \n", "1 + 2 *3 + 5", postfix);
	printf("\nBack to Infix: ");
	Root->DisplayInfix();
	


	InfixToPrefix("1 + 2 *3 + 5", prefix);
	printf("\n(Infix) %s  => (Prefix) %s \n", "1 + 2 *3 + 5", prefix);

	InfixToPrefix("1 + (2 *3) + 5", prefix);
	printf("\n(Infix) %s  => (Prefix) %s \n", "1 + (2 *3) + 5", prefix);



	int n;
	cin >> n;


}