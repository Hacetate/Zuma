#include "MySort.h"

int partition(int* a, int p, int r)
{
	int x = a[r], y, i = p - 1, j = p;
	for (; j < r + 1; j++)
    {
		if (a[j] <= x) {
            i++;
            y = a[j];
            a[j] = a[i];
			a[i] = y;
        }
    }
    return i;
}
void quicksort(int* a, int p, int r)
{
    int  n;
    if (p < r) {
		n = partition(a, p, r);
        quicksort(a, p, n - 1);
        quicksort(a, n + 1, r);
    }
}

void Myqsort(int* array, int size)
{
    quicksort(array, 0, size - 1);
}