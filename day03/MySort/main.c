#include <stdio.h>
#include "sort.h"
/*
void selectionSort(int *array,int len)
{
	int min = 0;
	for(int i=0;i < len - 1;i++)
	{
		min = i;
		for(int j = i + 1;j < len;j++)
		{
			if(array[min] > array[j])
			{
				min = j;
			}
		}
		//¿¿¿¿¿¿¿
		if(min != i)
		{
			int tmp = array[min];
			array[min] = array[i];
			array[i] = tmp;
		}
	}
}

void insertionSort(int *array,int len)
{
	int index = 0;
	int tmp = 0;
	for(int i=1;i < len;i++)
	{
		index = i;
		tmp = array[i];
		for(int j = i - 1;j >= 0;j--)
		{
			if(tmp < array[j])
			{
				array[j + 1] = array[j];
				index = j;
			}
			else
			{
				break;
			}	
		}
		array[index] = tmp;
	}
}
*/
void main()
{
	int i;
	//¶¨ÒåÕûÐÍÊý×é
	int array[] = { 12, 5, 33, 6, 10, 35, 67, 89, 87, 65, 54, 24, 58, 92, 100, 24, 46, 78, 99, 200, 55, 44, 33, 22, 11, 71, 2, 4, 86, 8, 9 };
	int array2[] = { 12, 5, 33, 6, 10, 35, 67, 89, 87, 65, 54, 24, 58, 92, 100, 24, 46, 78, 99, 200, 55, 44, 33, 22, 11, 71, 2, 4, 86, 8, 9 };

	//¼ÆËãÊý×é³¤¶È
	int len = sizeof(array) / sizeof(int);
	//±éÀúÊý×é
	printf("Sort Array:\n");
	for (i = 0; i < len; ++i)
	{
		printf("%d\t", array[i]);
	}
	printf("\n");

	// selectionSort
	selectionSort(array, len);
	// printf
	printf("Selection Sort:\n");
	for (i = 0; i < len; ++i)
	{
		printf("%d  ", array[i]);
	}
	
	// insertionSort
	insertionSort(array2, len);
	// printf
	printf("\n==========Gorgeous Split Line==========\n");
	printf("Insertion Sort:	\n");
	for (i = 0; i < len; ++i)
	{
		printf("%d  ", array2[i]);
	}
	printf("\n");
}
