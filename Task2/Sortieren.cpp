#include <iostream>
#include "unit.h"

using namespace std;


/*
Help-function: determine median of the first, middle and last element of an rarray that start at left and ends at right.
 The median element is automatically placed at the very right of the array.
*/
int median3(unsigned int *&array, int left, int right)
{
	// Determine the middle elements by sorting these three values manually
	int center = (left + right)/2;
	if (array[center] < array[left]) 
		tausche(array, left, center);
	if (array[right] < array[left])
		tausche(array, left, right);
	if (array[right]< array[center])
		tausche(array, center, right);

	// Swap the median to be placed at the very right.
	tausche(array, center, right);

	return array[right];
}

/*
Helper function for quickSort: Returns index of pivot element
	low	: Starting index,
	high: Ending index 
	
	
	Places all smaller elements in front of the pivot element.
	Places all larger elements behind the pivot element.
*/
int partition (unsigned int *&array, int low, int high, bool useMedian = false)
{
	int pivot, i, j;
	
	if (useMedian == true) {
		pivot = median3(array, low, high); 
		// Be careful, if there are only 2 elements: Having called median3() they are already sorted.   
 		if (low == high - 1) 
			return 0; // Return value doens't matter here, because 1 element is always sorted
	
	} else {
		pivot = array[high];    
	}
	
	
	// Index of where the pivot is going to be located
	i = (low - 1); 
    // Pivot is placed at the very right, so just go until high-1
	for (j = low; j <= high-1; j++) {
		// If current element is smaller than or equal to pivot element
		if (array[j] <= pivot) {
			// increment index of pivot, because we just found a smaller element
			i++;
            // Swap j (which is <= pivot) with i (which is > pivot or which is i == j)
			tausche(array, i, j);
		}
	}

    //Swap with the pivot element, which is located at the very right
	tausche(array, i + 1, high);

	return (i + 1);
}


/*
Quick-Sort:
	low	: Starting index,
	high: Ending index 
*/
void quickSort(unsigned int *&array, int low, int high, bool useMedian = false)
{
	int pi;
	if (low < high) {
		// pi is the partitioning index, array[pi] is automatically at the right place by calling this
		pi = partition(array, low, high, useMedian);

		// Separately sort elements before
		// partition and after partition
		quickSort(array, low, pi - 1);
		quickSort(array, pi + 1, high);
	}
}




/*
Halper function for Heap-Sort:
Heapifies a subtree rooted with node i which is an index in array[]
*/
void heapify(unsigned int *&array, size_t length, int i)
{
	int largest = i;  // Initialize largest as root
	int l = 2*i + 1;  // left = 2*i + 1  is the left child
	int r = 2*i + 2;  // right = 2*i + 2 is the right child
 
	// If left child is larger than root
	if (l < length && array[l] > array[largest])
		largest = l;
 
	// If right child is larger than largest so far
	if (r < length && array[r] > array[largest])
		largest = r;
 
	// If largest is not root
	if (largest != i) {
		tausche(array, i, largest);

		// Recursively heapify the affected sub-tree
		heapify(array, length, largest);
	}
}
 


/*
Heap-Sort
*/
void heapSort(unsigned int *&array, int n)
{
	// Build heap. Only iterate over the first half - 1, because we must access the children properly in heapify()
    // Heapify from bottom of the tree to the top
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(array, n, i);
 
	// One by one extract an element from heap
	for (int i=n-1; i>=0; i--) {
		// Move current root to end.
        // Swap 0 (largest element) to i (the end)
		tausche(array, 0, i);
 
		// call max heapify on the reduced heap.
        // Don't touch the last element, cause that was alredy brought into the right position
		heapify(array, i, 0);
	}
}



/*
Helper function for Merge-Sort:
	Merges two sorted sub-arrays into a combined sorted array
	First subarray is arr[l..m]
	Second subarray is arr[m+1..r]
*/
void merge(unsigned int *&array, int l, int m, int r)
{
	int i, j, k;
	
	int n1 = m - l + 1;
	int n2 =  r - m;
 
	// Temporary Arrays for storing the left and right arrays (each of them is already sorted)
	int L[n1], R[n2];
 
	// Copy data 
	for (i = 0; i < n1; i++)
		L[i] = array[l + i];
	for (j = 0; j < n2; j++)
		R[j] = array[m + 1+ j];
 
	// Start the merging process of L and R back into the real
	i = 0; // Index for the left sub-array
	j = 0; // Index for the right sub-array
	k = l; // Merging starts at index l of the original array
	while (i < n1 && j < n2) {
		// Compare the first elements of L and R subarrays and copy the smaller one to the original array
		if (L[i] <= R[j]) {
			array[k] = L[i];
			i++; // Advance the index of the left-subarray cause we have just taken an element from it
		} else {
			array[k] = R[j];
			j++; // Advance the index of the right-subarray cause we have just taken an element from it
		}
		// Whichever sub-array we took an element from, advance the index of the original array
		k++;
	}
 
	// Copy the remaining elements of L[], if there are any. 
	while (i < n1) {
		array[k] = L[i];
		i++;
		k++;
	}
 
	// Copy the remaining elements of R[], if there  are any. This may be the case, when the array is already completely sorted [1,2],[3,4]
	while (j < n2) {
		array[k] = R[j];
		j++;
		k++;
	}
	
}



/*
Merge-Sort:
	l is for left index and r is right index of the sub-array of arr to be sorted
*/
void mergeSort(unsigned int *&array, int l, int r) {
	if (l < r) {
		// Same as (l+r)/2, but avoids overflow for
		// large l and h
		int m = l+(r-l)/2;
 
		// Sort first and second halves
		mergeSort(array, l, m);
		mergeSort(array, m+1, r);
 
		// Now merge these subarrays in the right order
		merge(array, l, m, r);
	}
}




/*
Bubble-Sort (in place)
*/
void bubbleSort(unsigned int *&array, size_t length) {

	int i, j;

    // Iterate over all elements
	for (i = 0 ; i < ( length - 1 ); i++) {
		// Iterate from the end of the array down to i + 1
		for (j = length - 1 ; j > i; j--) {
			if (array[j-1] > array[j])	{
 				// Swap the elements at i and j
				tausche(array, j-1, j);
			}
		}
	}
}


/*
Selection-Sort
*/
void selectionSort(unsigned int *&array, size_t length) {
	int i, k, t, minIndex; 
	
	// Iterate over all elements
	for (i = 0 ; i < ( length - 1 ); i++) {
		minIndex = i;
		// Determine minimum of all following elements
		for( k = i+1; k < length; k++) {
			if( array[k] < array[minIndex]) minIndex = k;
		}

		// Then swap the current element with the minimum Element
		tausche(array, i, minIndex);
	}
}


/*
Insertion-Sort
*/
void insertionSort(unsigned int *&array, size_t length) {
	int i, j;

	// Iterate through the array
	for (i = 1 ; i < length; i++) {
		// Now take a look at the sub-array that consists of all elements up to the i-th element
		j = i;
	 
		// "Insert" the i-th element at the right position in that sub-array
		while ( j > 0 && array[j-1] > array[j]) {
            // Swap bigger element to the right
			tausche(array, j-1, j);
			j--;
		}
	}


}


/*
Helper method: Determine if a string displays an integer
*/
bool isInteger(string s){
	for (int i = 0; i < s.size(); i++){
		if(!isdigit(s[i])) return false;
	}
	return true;
}


/*
Helper method: print an array of unsigned integers
*/
void printArray(unsigned int* array, size_t length) {
	int i;
	for (i = 0; i < length - 1; i++) {
		cout << array[i] << ", ";
	}
	cout << array[length - 1] << endl;
}

void waitForUserToContinue() {
	getchar();
}

int main(int argc, char *argv[]) {
	cout << "Please specify the length of the arrays you wish to sort..." << endl << "After that, each algorithm is tested with each testing mode." << endl;
	cout << "Length: ";
	string command;    
	cin >> command;

  	
	if(isInteger(command)){
		size_t length = stoi(command);
		unsigned int *array = new unsigned int[length];

		int i;
		// Go through all the testing modes
		for (i = 1; i < AnzahlBeispiele; i++) {
			// Reset length, cause it gets overwritten by start()
			length = stoi(command);

			start(i, length, array);
			bubbleSort(array, length);
			cout << "Bubble Sort: ";
			//printArray(array, length);
			ergebnis(array);
			cout << endl;
			waitForUserToContinue();
				
			start(i, length, array);
			selectionSort(array, length);
			cout << "Selection Sort: ";
			//printArray(array, length);
			ergebnis(array);
			cout << endl;
			waitForUserToContinue();


			start(i, length, array);
			insertionSort(array, length);
			cout << "Insertion Sort: ";
			//printArray(array, length);
			ergebnis(array);
			cout << endl;
			waitForUserToContinue();

			start(i, length, array);
			mergeSort(array,0,length-1);
			cout << "Merge Sort: ";
			//printArray(array, length);
			ergebnis(array);
			cout << endl;
			waitForUserToContinue();

			start(i, length, array);
			heapSort(array, length);
			cout << "Heap Sort: ";
			//printArray(array, length);
			ergebnis(array);
			cout << endl;
			waitForUserToContinue();

			start(i, length, array);
			quickSort(array, 0, length-1, false);
			cout << "Quick Sort (classic): ";
			//printArray(array, length);
			ergebnis(array);
			cout << endl;
			waitForUserToContinue();

			start(i, length, array);
			quickSort(array, 0, length-1, true);
			cout << "Quick Sort (median): ";
			//printArray(array, length);
			ergebnis(array);
			cout << endl;
			waitForUserToContinue();

		}

	}
	else {
		cout << "You did not enter a valid number. exiting.." << endl;
	}
	return 0;

}
