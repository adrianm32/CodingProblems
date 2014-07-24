#include <iostream>


typedef struct NODE
{
	int value;
	NODE *  next = nullptr;

	NODE(int val) : value(val)  {}   //initialization list instead of initializing inside of ctor ,saves double initialization.
} NODEITEM;


NODE * CreateList(int count, bool random = false)
{
	if (count == 0) return nullptr;

	NODE * Head = new NODE(0);
	NODE * t = Head;
	for (int i = 1; i < count; i++)
	{
		t = t->next = new NODE(random ? rand() % count : i);
	}

	return Head;
}

void DisplayList(NODE* Head)
{
	NODE * t = Head;
	while (t != nullptr)
	{
		printf("%d->", t->value);
		t = t->next;
	}
	printf("null\n");
}


NODE* ReverseList(NODE * Head)
{
	NODE * currNode = Head;
	NODE * prevNode = nullptr;
	NODE * nextNode = currNode->next;

	while (currNode->next != nullptr)
	{
		currNode->next = prevNode;

		prevNode = currNode;
		currNode = nextNode;
		nextNode = nextNode->next;
	}

	currNode->next = prevNode;

	return currNode;

}

NODE * RecursiveHead;
NODE* ReverseListRecursive(NODE * node)
{
	if (node->next == nullptr)
	{
		RecursiveHead = node;
		return node;
	}

	NODE* temp = ReverseListRecursive(node->next);
	temp->next = node;
	node->next = nullptr;
	return node;
}

void SortList(NODE * node)
{
	//Bubble sorting by comparing adjacent nodes.
	for (NODE* node1 = node; node1->next != nullptr; node1= node1->next)    // i=0 to end - 1 => (node1->next != null)
	{
		for (NODE* node2 = node1->next; node2 != nullptr; node2 = node2->next)   //j= i+1 to end   end => (node2 != null)
		{
			if (node1->value > node2->value)
			{
				//swap values
				node1->value = node1->value + node2->value;  //watch out for integer overflow
				node2->value = node1->value - node2->value;
				node1->value = node1->value - node2->value;
			}
		}
	}
}

int main()
{
	NODE * Head = CreateList(10);
	DisplayList(Head);

	Head = ReverseList(Head);
	DisplayList(Head);

	Head = ReverseList(Head);
	Head = ReverseListRecursive(Head);
	DisplayList(RecursiveHead);

	Head = CreateList(10, true);
	DisplayList(Head);

	SortList(Head);
	DisplayList(Head);

	



	int n;
	scanf_s("%d", &n);

}

