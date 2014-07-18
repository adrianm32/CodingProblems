#include <iostream>

using namespace std;


bool IsSorted(int * items, int length)
{
	for (int i = 0; i < length - 1; i++)
	{
		if (items[i] > items[i+1])
		{
			return false;
		}
	}

	return true;
}

void CopyArray(int items1[], int items2[], int count)
{
	for (int i = 0; i < count; i++)
	{
		items1[i] = items2[i];
	}
}

void DisplayItems(int items[], int length)
{
	for (int i = 0; i < length; i++)
	{
		cout << items[i] << " ";

	}

	printf("\n");
}


/*
Compare adjacent elements until the largest ends up on the end.
*/
void BubbleSort(int * items, int length)
{
	int len = length;
	while (len > 1)  //len reduces as elements keep moving to right end.
	{
		for (int i = 0; i < len - 1; i++)
		{
			if (items[i] > items[i + 1])     //compare every adjacent elements until the smallest reaches the end.
			{
				//swap without usign temp variable.
				items[i] = items[i] + items[i + 1];   // a = a + b
				items[i + 1] = items[i] - items[i + 1];   // b = a - b
				items[i] = items[i] - items[i + 1];   // a = a - b 
			}
		}

		len--;  //at this point smallest has reached the end, so reduce end of unsorted by 1.
	}

	if (IsSorted(items, length))
	{
		printf("\nBubble Sort Sorted items: ");
	}
	else
	{
		printf("\n Bubble Sort Incorrect sorting : ");
	}
	DisplayItems(items, length);

}

/*
keep inserting each element in its rightful place to the left.
*/
void InsertionSort(int * items, int length)
{
	int len = length;
	int temp,j;

		for (int i = 1; i < len - 1; i++)   // start from 1 and not 0 and keep moving right.
		{
			j = i; 
			temp = items[i];
			while (j > 0 && items[j-1] > temp)
			{
				//keeping moving elements to the right to make room for insertion of ith element.
				items[j] = items[j - 1];
				j--;
			}

			items[j] = temp;  //finally insert the ith element in its rightful place.
		}


	if (IsSorted(items, length))
	{
		printf("\n Insertion Sort Sorted items: ");
	}
	else
	{
		printf("\n Insertion Sort Incorrect sorting : ");
	}
	DisplayItems(items, length);


}


/*
select the ith min element from the unsorted and move it to the sorted list.
*/
void SelectionSort(int * items, int length)
{
	int min = INT_MIN;
	int minPosition = -1;

	for (int i = 0; i < length - 1; i++)   // one less than j, at end-1
	{
		minPosition = i;
		for (int j = i + 1; j < length; j++)  // one more than i all the way to the end
		{
			if (items[j] < items[minPosition])
			{
				minPosition = j;
			}
		}

		//swap i and minposition
		if (i != minPosition)
		{
			items[i] = items[i] + items[minPosition];
			items[minPosition] = items[i] - items[minPosition];
			items[i] = items[i] - items[minPosition];
		}
	}

	if (IsSorted(items, length))
	{
		printf("\nSelection Sort - Sorted items: ");
	}
	else
	{
		printf("\nSelection Sort - Incorrect sorting : ");
	}
	DisplayItems(items, length);


}




void SomeSort(int * items, int length)
{
	for (int i = 0; i < length - 1; i++)   // one less than j, at end-1
	{
		for (int j = i + 1; j < length; j++)  // one more than i all the way to the end
		{
			if (items[i] > items[j])
			{
				 //swap without usign temp variable.
				items[i] = items[i] + items[j];   // a = a + b
				items[j] = items[i] - items[j];   // b = a - b
				items[i] = items[i] - items[j];   // a = a - b 
			}
		}

	}

	if (IsSorted(items, length))
	{
		printf("\nSorted items: ");
	}
	else
	{
		printf("\n Incorrect sorting : ");
	}
	DisplayItems(items, length);


}


void Heapify(int *items, int length)
{

}


void HeapSort(int * items, int length)
{
	Heapify(items, length);

	for (int i = 0; i < length - 2; i++)
	{
		//swap items[i] and items[length - 1];
		length--;

	}



	if (IsSorted(items, length))
	{
		printf("\nHeap Sort - Sorted items: ");
	}
	else
	{
		printf("\n Heap Sort- Incorrect sorting : ");
	}
	DisplayItems(items, length);


}




int main()
{
	int  items[50], items2[50];
	int length = _countof(items);

	for (int i = 0; i < length; i++)
	{
		items[i] = items2[i]= rand();
	}

	printf("\n Unsorted List: ");
	DisplayItems(items, length);
	BubbleSort(items, length);

	printf("\n Unsorted List: ");
	CopyArray(items, items2, length);
	DisplayItems(items, length);
	InsertionSort(items, length);

	printf("\n Unsorted List: ");
	CopyArray(items, items2, length);
	DisplayItems(items, length);
	SelectionSort(items, length);


	int n;
	cin >> n;


}