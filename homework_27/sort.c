#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define SWAP_ITEM(a, b) {item tmp = (a); (a) = (b); (b) = tmp;}

typedef struct item {uint32_t key;} item;

#define lt(a, b) ((a).key < (b).key)
#define le(a, b) ((a).key <= (b).key)

// insert e into a sorted list a[1:i]; the inserted list a[1:i+1] is also sorted
void _insert(item e, item a[], uint32_t i) {
	a[0] = e;
	while (e.key < a[i].key) {
		a[i+1] = a[i];
		i--;
	}
	a[i+1] = e;
}

// sort list[1:size] in ascending order by insertion sort => O(n^2)
void insertionSort(item list[], uint32_t size) {
	for (uint32_t i = 1; i < size; i++) {
		_insert(list[i+1], list, i);
	}
}

void _quickSortDK(item a[], uint32_t lo, uint32_t hi) {
	if (hi <= lo) return;

	uint32_t lt = lo, gt = hi, pivot = a[lo].key;

	uint32_t i = lo;
	while (i <= gt) {
		if (a[i].key < pivot) {
			SWAP_ITEM(a[i], a[lt]);
			lt++; i++;
		} else if (a[i].key > pivot) {
			SWAP_ITEM(a[i], a[gt]);
			gt--;
		} else {
			i++;
		}
	}

	_quickSortDK(a, lo, lt - 1);
	_quickSortDK(a, gt + 1, hi);
}

// sort a[left:right] in ascending order, and let pivot = midian(left, mid, right)
void _quickSort(item a[], uint32_t left, uint32_t right) {
	if (right <= left) return;

	uint32_t pivot = a[left].key;
	uint32_t i = left, j = right + 1;

	do {
		while (a[++i].key <= pivot);
		while (a[--j].key > pivot);
		if (i < j) SWAP_ITEM(a[i], a[j]);
	} while (i < j);
	
	SWAP_ITEM(a[left], a[j]);

	_quickSort(a, left, j-1);
	_quickSort(a, j+1, right);
}

// sort list[1:size] in ascending order by quick sort => O(n*log(n))
void quickSort(item list[], uint32_t size) {
	//_quickSort(list, 1, size);
	_quickSortDK(list, 1, size);
}

// sort list[1:size] in ascending order by merge sort => O(n*log(n))
void mergeSort(item list[], uint32_t size) {

}

// sort list[1:size] in ascending order by heap sort => O(n*log(n))
void heapSort(item list[], uint32_t size) {

}

void (*sort[])(item[], uint32_t) = {
	insertionSort,
	quickSort,
	mergeSort,
	heapSort,
};

void genData(item list[], uint32_t size) {
	if (size <= 0) return;
	for (uint32_t i = 1; i <= size; i++)
		list[i].key = 1 + rand() % (size);
}

void printList(item list[], uint32_t size) {
	if (size <= 0) return;
	for (uint32_t i = 1; i <= size; i++)
		printf("%d ", list[i].key);
	printf("\n");
}
int main(void) {
	const uint32_t size = 30;
	item* list = malloc((size + 1) * sizeof(item));
	genData(list, size);
	printList(list, size);
	quickSort(list, size);
	printList(list, size);
	free(list); list = NULL;
	return 0;
}
