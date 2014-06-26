#include <iostream>

typedef struct NODE
{
	int value;
	NODE * next;

	NODE(int val, NODE* node)
	{
		value = val;
		next = node;
	}
} NODEITEM, *PNODEITEM;


/*
Josephus problem: 
Josephus style election where group of N people stand in a circle, then count around the circle, eliminating every Mth person, until only one is left who is then the leader.
*/

int main(int* argc, char * argv[])  //argc is count of parameters which is always (1 + count of argv[]) since program name is prepended as argv[0]. So start reading values from argv[1]
{
	int i, N = atoi(argv[1]), M = atoi(argv[2]);

	//create circular list 
	PNODEITEM start = new NODEITEM(1, nullptr); //first node
	start->next = start;

	PNODEITEM t = start;

	for (i = 2; i <= N; i++)
	{
		t -> next = new NODEITEM(i, start);
		t = t->next;  // or we can do  t = t -> next = new NODEITEM(i, start);
	}


	//start the election process by eleminating every Mth person
	PNODEITEM temp;

	while (t != t->next)
	{
		for (i = 1; i < M; i++)
		{
			t = t->next;
		}

		t->next = (temp = t->next)->next;
		delete temp;
	}


	printf("Josephus leader is %d\n", t->value);
	scanf_s("%d", &i);


}