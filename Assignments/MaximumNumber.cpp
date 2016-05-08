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

	vector<pair<int, int>> candidates;

	for (int i = 0; i < nproc; i++) {
		int local_max = 0;
		//... Retrieve the max value from the slaves
		MPI_Recv(&local_max, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, recieve_status);
		int source = recieve_status->MPI_SOURCE;

		//... Put all of them on a vector
		candidates.push_back(make_pair(local_max, source));
	}

	pair<int, int> max_number(0, 0);
	for (int i = 0; i < candidates.size(); i++) {
		if (max_number.first < candidates[i].first) {
			max_number = candidates[i];
		}//... Get the maximum of all maximums 
	}

	printf("\nMax number is found in the process %d\n", max_number.second);
	printf("Max number is %d\n\n", max_number.first);
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

	for (int i = 0; i < local_array_size; i++) {
		if (sub_array[i] > max_value) {
			max_value = sub_array[i];
			target_index = i;
		} //... Get the maximum of the local array
	}

	printf("\nSlave rank %d sent value %d\n", slave_rank, max_value);

	MPI_Send(&max_value, 1, MPI_INT, 0, slave_rank, MPI_COMM_WORLD); //... Send the index to the Master

	delete[] sub_array;
}

int main(int argc, char* argv[]) {
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

	MPI_Finalize();

	return 0;
}