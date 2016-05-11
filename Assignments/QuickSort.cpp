#include <climits>
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <map>
#include <algorithm>
#include <functional>
#include <Windows.h>
#include <ctime>
#include "mpi.h"
#include <vector>
#include <list>

#define Max 50 + 5
#define INF 1000000000
#define INT_MAX 2147483647
#define FOR(i, N) for(int i = 0; i < N; i++)
#define FOR1e(i, N) for(int i = 1; i <= N; i++)
#define FORe(i, N) for(int i = 0; i <= N; i++)

#pragma warning(disable : 4996)

using namespace std;

void print_array(int* array, int size) {
	for (int i = 0; i < size; i++) {
		printf("%d, ", array[i]);
	}
	puts("");
}
void print_1d_vector(vector<int> v) {
	for (int i = 0; i < v.size(); i++) {
		printf("%d, ", v[i]);
	}

	puts("");
}
void print_2d_vector(vector<vector<int>> v) {
	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < v[i].size(); j++) {
			printf("%d, ", v[i][j]);
		}
		puts("");
	}
}

int partition(int* array, int start, int end, int pivot) {
	int left = start - 1;
	int right= end;

	while(true) {
		while(array[++left] < pivot) {

		}

		while(right > 0 && array[--right] > pivot) {

		}

		if (left >= right) {
			break;
		} else {
			swap(array[left], array[right]);
		}
	}

	swap(array[left], array[end]);

	return left;
}
void quick_sort(int* array, int start, int end) {
	if (end - start <= 0) {
		return;
	} else {
		int pivot = array[end];
		int partition_index = partition(array, start, end, pivot);

		quick_sort(array, start, partition_index - 1);
		quick_sort(array, partition_index + 1, end);
	}
}

int* input_array;
int array_size;
int sub_arrays_size;
MPI_Status* recieve_status = new MPI_Status();

void input() {
	puts("Enter array size:");
	cin >> array_size;
	input_array = new int[array_size];

	puts("Enter array elements:");

	for (int i = 0; i < array_size; i++) {
		cin >> input_array[i];
	}

	puts("");
}

void master(int nproc) {
	sub_arrays_size = array_size / nproc; //... The size of each of array size
	int remaining = array_size % nproc; //... The remaining elements size that will be added to the slaves
	int offset = 0;//... Array start index

	for (int i = 0; i < nproc; i++) {
		int array_size = sub_arrays_size;

		if (remaining) {
			array_size++;
			MPI_Send(&array_size, 1, MPI_INT, i + 1, i + 1, MPI_COMM_WORLD);
			MPI_Send(input_array + offset, array_size, MPI_INT, i + 1, i + 1, MPI_COMM_WORLD);
			remaining--;
		}//... If there are remain, add the elements one by one to the slaves from the begin

		else {
			MPI_Send(&array_size, 1, MPI_INT, i + 1, i + 1, MPI_COMM_WORLD);
			MPI_Send(input_array + offset, array_size, MPI_INT, i + 1, i + 1, MPI_COMM_WORLD);
		}//... Continue sending sub-arrays to slaves

		offset += array_size; //... Update the array offset
	}

	vector<vector<int>> sorted_lists;

	for (int i = 0; i < nproc; i++) {
		//... Get subarrays sizes from each slave
		int local_sub_array_size = 0;
		MPI_Recv(&local_sub_array_size, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, recieve_status);

		//... Get the slave's array
		int* local_sorted_sub_array = new int[local_sub_array_size];
		MPI_Recv(local_sorted_sub_array, local_sub_array_size, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, recieve_status);

		//... Convert to vector
		vector<int> v(local_sorted_sub_array, local_sorted_sub_array + local_sub_array_size);

		sorted_lists.push_back(v);
	}//... Retrieve data from slaves

	vector<int> final_sorted_array;
	for (int i = 0; i < sorted_lists.size(); i++) {
		for (int j = 0; j < sorted_lists[i].size(); j++) {
			final_sorted_array.push_back(sorted_lists[i][j]);
		}
	}//... Convert 2d vector to 1d

	sort(final_sorted_array.begin(), final_sorted_array.end());
	puts("---------------SORTED ARRAY------------");
	print_1d_vector(final_sorted_array);
}

void slave(int slave_rank) {
	int local_array_size = 0;

	//...  Get the subarray size
	MPI_Recv(&local_array_size, 1, MPI_INT, 0, slave_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 

	int* sub_array = new int[local_array_size];

	//... Get the subarray elements
	MPI_Recv(sub_array, local_array_size, MPI_INT, 0, slave_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 

	printf("\nProcess %d recieved %d element(s)\n", slave_rank, local_array_size);
	print_array(sub_array, local_array_size);

	int target_index = -1;
	int max_value = INT_MIN;

	quick_sort(sub_array, 0, local_array_size - 1);

	MPI_Send(&local_array_size, 1, MPI_INT, 0, slave_rank, MPI_COMM_WORLD);
	MPI_Send(sub_array + 0, local_array_size, MPI_INT, 0, slave_rank, MPI_COMM_WORLD);

	delete[] sub_array;
}

int main(int argc, char* argv[]) {
	clock_t start = clock();
	MPI_Init(&argc, &argv);

	int rank;
	int nproc;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);

	if (rank == 0) {
		input();
		master(nproc - 1);

	} else {
		slave(rank);
	}

	printf("\nTime taken: %.5fs\n", (double)(clock() - start)/CLOCKS_PER_SEC);

	MPI_Finalize();
	

	return 0;
}