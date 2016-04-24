__author__ = 'Ahmed Hani Ibrahim'

import time

matrix_a = [[1, 2], [3, 4]]
matrix_b = [[2, 0], [1, 2]]
matrix_c = [[0, 0], [0, 0]]

start = int(round(time.time() * 1000))

for i in range(0, len(matrix_a)):
        for j in range(0, len(matrix_b[0])):
            for k in range(0, len(matrix_b)):
                 matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j]

print("Time --->", (int(round(time.time() * 1000)) - start))
print(matrix_c)
