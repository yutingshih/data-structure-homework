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
void insertionSort(item list[], uint32_t numItems) {
	for (uint32_t i = 1; i < numItems; i++) {
		_insert(list[i+1], list, i);
	}
}

// sort a[left:right] in ascending order, and let pivot = median(left, middle, right)
void _quickSort(item a[], uint32_t left, uint32_t right) {
	if (right <= left) return;

	uint32_t middle = left + (right - left) / 2;
	if (a[right].key < a[left].key) SWAP_ITEM(a[right], a[left]); 
	if (a[right].key < a[middle].key) SWAP_ITEM(a[right], a[middle]); 
	if (a[left].key < a[middle].key) SWAP_ITEM(a[left], a[middle]); 
	
	uint32_t pivot = a[left].key;
	uint32_t i = left, j = right + 1;

	do {
		while (a[++i].key < pivot);
		while (a[--j].key > pivot);
		if (i < j) SWAP_ITEM(a[i], a[j]);
	} while (i < j);
	
	SWAP_ITEM(a[left], a[j]);

	_quickSort(a, left, j-1);
	_quickSort(a, j+1, right);
}

// sort list[1:size] in ascending order by quick sort => O(n*log(n))
void quickSort(item list[], uint32_t numItems) {
	_quickSort(list, 1, numItems);
}

// merge a[i:m] and a[m+1:n] into b[i:n]
void _merge(item a[], item b[], uint32_t i, uint32_t m, uint32_t n) {
	uint32_t j = m + 1;	// index of the second tile of list a
	uint32_t k = i;	// index of the merged list b
	while (i <= m && j <= n) {
		if (a[i].key <= a[j].key)
			b[k++] = a[i++];
		else
			b[k++] = a[j++];
	}
	while (i <= m) b[k++] = a[i++];
	while (j <= n) b[k++] = a[j++];
}

void _mergePass(item orig[], item dest[], uint32_t numItems, uint32_t tileSize) {
	uint32_t i = 1;
	uint32_t m = i + tileSize - 1; 		// end index of the first tile of original list
	uint32_t n = i + 2 * tileSize - 1;	// end index of the second tile of original list

	while (n <= numItems) {
		_merge(orig, dest, i, m, n);

		i += 2 * tileSize;
		m = i + tileSize - 1;
		n = i + 2 * tileSize - 1;
	}

	if (m < numItems)
		_merge(orig, dest, i, m, numItems);
	else 
		for (; i <= numItems; i++)
			dest[i] = orig[i];
}

// sort list[1:size] in ascending order by merge sort => O(n*log(n))
void mergeSort(item list[], uint32_t numItems) {
	uint32_t tileSize = 1;
	item* temp = malloc((numItems + 1) * sizeof(item));
	while (tileSize < numItems) {
		_mergePass(list, temp, numItems, tileSize);
		tileSize *= 2;
		_mergePass(temp, list, numItems, tileSize);
		tileSize *= 2;
	}
	free(temp); temp = NULL;
}

// trickle down the root node of a subtree to make it be a max binary heap
void _adjust(item list[], uint32_t numItems, uint32_t root) {
	item rootNode = list[root];
	uint32_t rootKey = list[root].key;
	uint32_t child = 2 * root;

	while (child <= numItems) {
		if (child < numItems && list[child].key < list[child+1].key) child++;

		if (rootKey <= list[child].key) {
			list[child/2] = list[child];
			child *= 2;
		} else break;
	}

	list[child/2] = rootNode;
}

// sort list[1:size] in ascending order by heap sort => O(n*log(n))
void heapSort(item list[], uint32_t numItems) {
	for (uint32_t i = numItems / 2; i > 0; i--) {
		_adjust(list, numItems, i);
	}
	for (uint32_t i = numItems - 1; i > 0; i--) {
		SWAP_ITEM(list[1], list[i+1]);
		_adjust(list, i, 1);
	}
}

// sorting functions
void (*sort[])(item list[], uint32_t numItems) = {
	insertionSort,
	quickSort,
	mergeSort,
	heapSort,
};

// increasing sequence generator
void increasingData(item list[], uint32_t size) {
	for (uint32_t i = 1; i <= size; i++) {
		list[i].key = i;
	}
}

// decreasing sequence generator
void decreasingData(item list[], uint32_t size) {
	for (uint32_t i = 1; i <= size; i++) {
		list[i].key = size - i;
	}
}

// worst case data generator for merge sort
void seperate(item list[], uint32_t size) {
	if (size <= 1) return;

	const uint32_t size1 = size / 2, size2 = size - size1;
	item list1[size1], list2[size2];
	uint32_t i = 0, j = 0, k = 0;

	for (i = 0; i < size1 && k < size; k += 2) {
		list1[i++] = list[k];
	}
	k = 1;
	for (j = 0; j < size2 && k < size; k += 2) {
		list2[j++] = list[k];
	}

	seperate(list1, size1);
	seperate(list2, size2);
	
	k = 0;
	for (uint32_t i = 0; i < size1; i++) {
		list[k++] = list1[i];
	}
	for (uint32_t j = 0; j < size2; j++) {
		list[k++] = list2[j];
	}
}

// worst case data generator for merge sort
void mergeWorstData(item list[], uint32_t size) {
	increasingData(list, size);
	seperate(list + 1, size);
}

// random permutation generator
void permute(item list[], uint32_t numItems) {
	for (uint32_t i = numItems; i >= 1; i--) {
		uint32_t j = 1 + rand() % i;	// random integer in the range [1, i]
		SWAP_ITEM(list[i], list[j]);
	}
}

// random permutation generator for heap sort and quick sort
void randomData(item list[], uint32_t size) {
	increasingData(list, size);
	permute(list, size);
}

void printList(item list[], uint32_t size) {
	if (size <= 0) return;
	for (uint32_t i = 1; i <= size; i++)
		printf("%2d ", list[i].key);
	printf("\n");
}

void verify(item list[], uint32_t size) {
	for (uint32_t i = 1; i + 1 <= size; i++) {
		if (list[i].key > list[i+1].key) {
			fprintf(stderr, "FAILED: ");
			printList(list, size);
			return;
		}
	}
}

double timer(void(*func)(item[], uint32_t), item list[], uint32_t size) {
	double t = (double)clock();
	func(list, size);
	t = ((double)clock() - t) / CLOCKS_PER_SEC;
	return t;
}

int main(void) {
	srand((unsigned)time(NULL));

	const uint32_t numCases = 6;
	const uint32_t size[] = {500, 1000, 2000, 3000, 4000, 5000};
	double executionTime[4][numCases];
	double max = 0.0, t = 0.0;

	for (uint32_t i = 0; i < numCases; i++) {
		item* list = malloc((size[i] + 1) * sizeof(item));

		// insertion sort
		decreasingData(list, size[i]);
		executionTime[0][i] = timer(insertionSort, list, size[i]);
		verify(list, size[i]);

		// quick sort
		randomData(list, size[i]);
		max = t = 0.0;
		for (uint32_t j = 0; j < 50000 / size[i]; j++) {
			t = timer(quickSort, list, size[i]);
			if (t > max) max = t;
		}
		executionTime[1][i] = max;
		verify(list, size[i]);

		// merge sort
		mergeWorstData(list, size[i]);
		executionTime[2][i] = timer(mergeSort, list, size[i]);
		verify(list, size[i]);

		// heap sort
		randomData(list, size[i]);
		max = t = 0.0;
		for (uint32_t j = 0; j < 50000 / size[i]; j++) {
			t = timer(heapSort, list, size[i]);
			if (t > max) max = t;
		}
		executionTime[3][i] = max;
		verify(list, size[i]);

		free(list); list = NULL;
	}

	printf("%15s  ", "");
	for (uint32_t i = 0; i < numCases; i++)
		printf("n = %4u  ", size[i]);
	printf("\n");

	printf("%15s  ", "insertion sort");
	for (uint32_t i = 0; i < numCases; i++)
		printf("%f  ", executionTime[0][i]);
	printf("\n");

	printf("%15s  ", "quick sort");
	for (uint32_t i = 0; i < numCases; i++)
		printf("%f  ", executionTime[1][i]);
	printf("\n");

	printf("%15s  ", "merge sort");
	for (uint32_t i = 0; i < numCases; i++)
		printf("%f  ", executionTime[2][i]);
	printf("\n");

	printf("%15s  ", "heap sort");
	for (uint32_t i = 0; i < numCases; i++)
		printf("%f  ", executionTime[3][i]);
	printf("\n");

	return 0;
}
