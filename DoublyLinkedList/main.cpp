#include <iostream>

typedef struct NODE
{
	int value;
	NODE * next = nullptr;
	NODE *prev = nullptr;

	NODE(int val)
	{
		value = val;
	}
} NODEITEM;


void CreateList(_In_ int count, _Outptr_ NODE **Head, _Outptr_  NODE **Tail)
{
	if (count == 0) *Head = *Tail = nullptr;

	*Head = *Tail = new NODE(0);

	NODE *Curr = *Head;

	for (int i = 1; i < count; i++)
	{
		Curr->next = (*Tail = new NODE(i));
		(*Tail)->prev = Curr;

		Curr = Curr->next;
	}

}

void DisplayList(NODE *Head, NODE *Tail)
{
	NODE *t = Head;
	printf("Head->");
	while (t != nullptr)
	{
		printf("%d->", t->value);
		t = t->next;
	}
	printf("null\n");

	t = Tail;
	printf("Tail->");
	while (t != nullptr)
	{
		printf("%d->", t->value);
		t = t->prev;
	}
	printf("null\n\n\n");
}


void ReverseDoublyLinkedList(_Inout_ NODE **Head, _Inout_ NODE **Tail)
{
	NODE *prevNode = nullptr;
	NODE *currNode = *Head;
	NODE *nextNode = currNode->next;

	*Tail = *Head; //set tail to head here itself. Head will be set at end of loop.

	while (currNode->next != nullptr)
	{
		currNode->next = prevNode;
		currNode->prev = nextNode;

		prevNode = currNode;
		currNode = nextNode;
		nextNode = nextNode->next;
	}

	//reached last node
	currNode->next = prevNode;
	currNode->prev = nullptr;
	*Head = currNode;
}



int main()
{
	NODE *Head, *Tail;

	CreateList(10, &Head, &Tail);
	DisplayList(Head, Tail);

	ReverseDoublyLinkedList(&Head, &Tail);
	DisplayList(Head, Tail);

	ReverseDoublyLinkedList(&Head, &Tail);
	DisplayList(Head, Tail);


	int n;
	scanf_s("%d", &n);

}