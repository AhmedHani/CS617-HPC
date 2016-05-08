#include <mpi.h>
#include <cstdio>
#include <iostream>

#pragma warning(disable : 4996)

static int MATRIX_SIZE = 3;

static int** matrix_a;
static int** matrix_b;
static int** matrix_c;

void print_matrix(int** matrix, int rows, int columns) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			printf("%d ", matrix[i][j]);
		}

		puts("");
	}

	puts("");
}
void print_matrix_with_message(int** matrix, int rows, int columns, char* message) {
	puts(message);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			printf("%d ", matrix[i][j]);
		}

		puts("");
	}

	puts("");
}
int** initialize_matrix(int** matrix, int rows, int columns) {
	matrix = new int*[rows];
	
	for (int i = 0; i < rows; i++) {
		matrix[i] = new int[columns];
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			matrix[i][j] = 0;
		}
	}

	return matrix;
}
void print_vector(int* vector, int size) {
	for (int i = 0; i < size; i++) {
		printf("%d ", vector[i]);
	}

	puts("");
}
void print_vector_with_message(int* vector, int size, char* message) {
	puts(message);

	for (int i = 0; i < size; i++) {
		printf("%d ", vector[i]);
	}

	puts("");
}

void input() {
	matrix_a = new int*[MATRIX_SIZE];
	matrix_b = new int*[MATRIX_SIZE];
	matrix_c = new int*[MATRIX_SIZE];
	
	for (int i = 0; i < MATRIX_SIZE; i++) {
		matrix_a[i] = new int[MATRIX_SIZE];
		matrix_b[i] = new int[MATRIX_SIZE];
		matrix_c[i] = new int[MATRIX_SIZE];
	}

	int value = 1;
	
	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			matrix_a[i][j] = value++;
		}
	}

	value = 9;

	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			matrix_b[i][j] = value--;
		}
	}

	print_matrix_with_message(matrix_a, MATRIX_SIZE, MATRIX_SIZE, "------------MATRIX A----------");
	print_matrix_with_message(matrix_b, MATRIX_SIZE, MATRIX_SIZE, "------------MATRIX B----------");
}

void master(int nproc) {
	int size = nproc;

	int** result_matrix = NULL;
	result_matrix = initialize_matrix(result_matrix, size, size);

	for (int i = 0; i < nproc; i++) {
		MPI_Send(&size, 1, MPI_INT, i + 1, i + 1, MPI_COMM_WORLD);

		int* vector_a = new int[size];
		int* vector_b = new int[size];

		for (int k = 0; k < size; k++) {
			vector_a[k] = matrix_a[k][i];
			vector_a[k] = matrix_a[k][i];
			vector_a[k] = matrix_a[k][i];
		}
		
		print_vector_with_message(vector_a, size, "-------------MASTER VECTOR A-----------");

		for (int k = 0; k < size; k++) {
			vector_b[k] = matrix_b[i][k];
			vector_b[k] = matrix_b[i][k];
			vector_b[k] = matrix_b[i][k];
		}

		print_vector_with_message(vector_b, size, "-------------MASTER VECTOR B-----------");

		MPI_Send(vector_a, size, MPI_INT, i + 1, i + 1, MPI_COMM_WORLD);
	    MPI_Send(vector_b, size, MPI_INT, i + 1, i + 1, MPI_COMM_WORLD);
	}

	for (int p = 0; p < nproc; p++) {
		int index = 0;
		int* matrix_temp_1d = new int[size * size];

		MPI_Recv(matrix_temp_1d, size * size, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				result_matrix[i][j] += matrix_temp_1d[index++];
			}
		}
	}

	print_matrix_with_message(result_matrix, size, size, "---------RESULT--------");
}

void slave(int slave_rank) {
	int slave_size = 0;

	MPI_Recv(&slave_size, 1, MPI_INT, 0, slave_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	int* vector_a = new int[slave_size];
	int* vector_b = new int[slave_size];

	MPI_Recv(vector_a, slave_size, MPI_INT, 0, slave_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Recv(vector_b, slave_size, MPI_INT, 0, slave_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	int** matrix_c_temp = NULL;
	matrix_c_temp = initialize_matrix(matrix_c_temp, slave_size, slave_size);
	
	for (int k = 0; k < slave_size; k++) {
		for (int q = 0; q < slave_size; q++) {
			matrix_c_temp[k][q] = vector_a[k] * vector_b[q];
		}
	}

	int* matrix_c_temp_1d = new int[slave_size * slave_size];
	int index = 0;

	for (int k = 0; k < slave_size; k++) {
		for (int q = 0; q < slave_size; q++) {
			matrix_c_temp_1d[index++] = matrix_c_temp[k][q];
		}
	}

	int offset = 0;
	MPI_Send(matrix_c_temp_1d + offset, index, MPI_INT, 0, slave_rank, MPI_COMM_WORLD);

	delete[] matrix_c_temp_1d;
	delete[] vector_a;
	delete[] vector_b;
}

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);
	
	int rank, nproc;

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
