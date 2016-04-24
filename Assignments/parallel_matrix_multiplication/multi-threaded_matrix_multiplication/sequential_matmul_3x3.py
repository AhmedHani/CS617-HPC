__author__ = 'Ahmed Hani Ibrahim'

import time

matrix_a = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
matrix_b = [[9, 8, 7], [6, 5, 4], [3, 2, 1]]
matrix_c = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]

start = int(round(time.time() * 1000))

for i in range(0, len(matrix_a)):
        for j in range(0, len(matrix_b[0])):
            for k in range(0, len(matrix_b)):
                 matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j]

print("Time --->", (int(round(time.time() * 1000)) - start))
print(matrix_c)
