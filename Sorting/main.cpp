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


/*********************************************************************************

							BUBBLE SORT


************************************************************************/





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


/*********************************************************************************

						INSERTION SORT


************************************************************************/


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


/*********************************************************************************

						SELECTION SORT


************************************************************************/


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

/*********************************************************************************

							VARITION OF SELECTION SORT


************************************************************************/


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

/*********************************************************************************

							HEAP SORT


************************************************************************/


/*
Binary tree layout in array.
For 1-Based array, :  1 2 3
Parent =  floor((i / 2)     
LeftChild = 2 * i;
RightChild = 2 * i + 1;


For 0-Based Array  :   0 1 2

Parent = floor((i-1)/ 2)     eg. (2-1)/ 2 = 0 else it would be 2/2 = 1
Left Child = 2 * i + 1
Right child = 2 * i + 2



First heapify the tree in the array such that it is a valid priority queue with max element at the top of heap at index 0.
Then move/swap root to end of array (sorted list) and restore the heap property for remaining smaller heap(unsorted list).

heapify from bottom up, by sifting down each parent down to the leaves until heap property is satisfied.


*/



enum HeapifyBuildOrder
{
	BottomUp,   //start with bottom most parent node level and move up.   0(n) since we skip all the leaf nodes which is most of the number.
	TopDown    //start with empty heap and add one element at a time. 0(n log n) since we have to iterate every leaf node which is most of the number.
};


/*
Pushes parent down to the tree until heap order is restored.
*/

void SiftDown(int * items, int startIndex, int endIndex)
{
	int childIndex, temp;
	int rootIndex = startIndex;  //start at root of heap.

	while ((2 * rootIndex + 1) <= endIndex)  //while root has at least one child, left child
	{
		int swapIndex = rootIndex;

		childIndex = 2 * rootIndex + 1;
		if (items[swapIndex] < items[childIndex])  //compare with left child.
		{
			swapIndex = childIndex;
		}
		
		if (childIndex + 1 <= endIndex && items[swapIndex] < items[childIndex + 1])  // if there is right child, compare parent/left with right child
		{
			swapIndex = childIndex + 1;
		}

		if (swapIndex != rootIndex)
		{
			//swap
			temp = items[swapIndex];
			items[swapIndex] = items[rootIndex];
			items[rootIndex] = temp;

			rootIndex = swapIndex; // make swap index the next root and sift down the tree.
		}
		else
			break;  // return since heap priority queue is in order.
	}
}


void SiftUp(int * items, int startIndex, int endIndex)
{
	int childIndex = endIndex;  //start with bottom most child and sift it up along its parent tree path.
	int parentIndex, temp;

	while (childIndex > startIndex)
	{
		parentIndex = floor((childIndex - 1)/2);  //sift up with parent.

		if (items[parentIndex] < items[childIndex])
		{
			//swap
			temp = items[parentIndex];
			items[parentIndex] = items[childIndex];
			items[childIndex] = temp;

			childIndex = parentIndex;  //move/sift up the tree.
		}
		else
			break; //return since heap is priority queeue order.
	}
}

/*
Heapify the binary tree stored in array by skipping leaf nodes and starting with the last parent node in the last but one level of the tree.
and then keep building it bottom-up all the way to the root.
*/
void Heapify(int *items, int length, HeapifyBuildOrder heapifyOrder)
{
	int endIndex = length - 1;  // 0 based.

	switch (heapifyOrder)
	{
		 
		case BottomUp: //start with bottom most parent node level and move up.   0(n) since we skip all the leaf nodes which is most of the number.
		{
						 int parentIndex = floor((endIndex - 1) / 2);  //start with the last parent in the array.

						 while (parentIndex >= 0)
						 {
							 SiftDown(items, parentIndex, endIndex);
							 parentIndex--;
						 }

		}
			break;
		case TopDown: //start with empty heap and add one element at a time. 0(n log n) since we have to iterate every leaf node which is most of the number
		{
						  int heapEndIndex = 0;
						  while (heapEndIndex <= endIndex)
						  {
							  SiftUp(items, 0, heapEndIndex);
							  heapEndIndex++;

						  }
		}
			break;
		default:
			break;
	}

	
}

void HeapSort(int * items, int length, HeapifyBuildOrder heapifyOrder = BottomUp)
{
	int temp;

	Heapify(items, length, heapifyOrder);   //after this point root of heap (max value ) is at items[0]

	int endIndex = length - 1;

	while (endIndex > 0)
	{
		//swap 0 and endIndex
		temp = items[0];
		items[0] = items[endIndex];
		items[endIndex] = temp;

		endIndex--;   //reduce end by 1 since the largest of the heap is being moved to the sorted end, thereby reducing heap size.

		SiftDown(items, 0, endIndex);   //swap ruined heap property, so restore it again by sifting the new root at 0 down the tree.

	}


	if (IsSorted(items, length))
	{
		printf("\nHeap Sort %s - Sorted items: " , heapifyOrder == BottomUp? "BottomUp" : "TopDown");
	}
	else
	{
		printf("\n Heap Sort %s- Incorrect sorting : ", heapifyOrder == BottomUp ? "BottomUp" : "TopDown");
	}
	DisplayItems(items, length);


}

/********************************************************************

				BINARY SEARCH

***************************************************************/

void BinarySearch(int * items, int length, int item)
{
	int start = 0;
	int end = length - 1;
	int mid;

	while (start <= end)   // <= in case there is just one item or if item is the last item.
	{
		mid = (start + end) / 2;

		if (item == items[mid])
		{
			printf("\n Found item %d at position %d \n ", item, mid );
			return;
		}
		else if (item < items[mid])
		{
			end = mid - 1;
		}
		else
			start = mid + 1;
	}

	printf("\n Did not find item %d \n", item);

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

	printf("\n Unsorted List: ");
	CopyArray(items, items2, length);
	DisplayItems(items, length);
	HeapSort(items, length);

	printf("\n Unsorted List: ");
	CopyArray(items, items2, length);
	DisplayItems(items, length);
	HeapSort(items, length, TopDown);


	BinarySearch(items, length, 5);
	BinarySearch(items, length, 41);
	BinarySearch(items, length, 2995);
	BinarySearch(items, length, 32391);

	
	




	int n;
	cin >> n;


}