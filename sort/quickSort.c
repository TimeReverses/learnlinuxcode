#include <stdio.h>
#include <stdlib.h>

void quicksort(int *arr,int start,int end)
{
	//首先使用临时变量保存数据
	int i = start;
	int j = end;
	if(i < j)
	{
		//设置基准数
		int temp = arr[i];
		while(i< j)
		{
			//int temp = arr[i];
			//从右向左查,如果小于基准数停止
			while(i < j && temp < arr[j])
			{
				j--;
			}
			//填左边的坑
			if(i < j)
			{
				arr[i] = arr[j];
				i++;
			}
			//从左边向右查,如果大于基准数停止
			while(i < j && temp > arr[i])
			{
				i++;
			}
			//填右边的坑
			if(i < j)
			{
				arr[j] = arr[i];
				j--;
			}
		}
		//左右分治
		quicksort(arr,start,i-1);
		quicksort(arr,i+1,end);
		//将基准数填入
		arr[i] = temp;
	}	
}

void printArr(int arr[],int len)
{
	for(int i=0;i<len;i++)
	{
		printf("%d ",arr[i]);
	}
	printf("\n");
}

int main(void)
{
	int arr[] = {4,1,9,2,10,48,23,94,100,0,12,36};
	printArr(arr,sizeof(arr)/sizeof(int));
	quicksort(arr,0,sizeof(arr)/sizeof(int) - 1);
	printArr(arr,sizeof(arr)/sizeof(int));	
	return 0;
}
