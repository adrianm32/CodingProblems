#include <iostream>


typedef struct NODE
{
	int value;
	NODE *  next = nullptr;

	NODE(int val)
	{
		value = val;
	}
} NODEITEM;


NODE * CreateList(int count)
{
	if (count == 0) return nullptr;

	NODE * Head = new NODE(0);
	NODE * t = Head;
	for (int i = 1; i < count; i++)
	{
		t = t->next = new NODE(i);
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

int main()
{
	NODE * Head = CreateList(10);
	DisplayList(Head);

	Head = ReverseList(Head);
	DisplayList(Head);


	int n;
	scanf_s("%d", &n);

}

