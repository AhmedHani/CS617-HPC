__author__ = 'Ahmed Hani Ibrahim'

import threading
import time
from multiprocessing.dummy import Pool

matrix_a = [[1, 2], [3, 4]]
matrix_b = [[2, 0], [1, 2]]
matrix_c = [[0, 0], [0, 0]]

'''
for i in range(0, len(matrix_a)):
        for j in range(0, len(matrix_b[0])):
            for k in range(0, len(matrix_b)):
                 matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j]

print(len(matrix_a[0]))

vector_a1 = matrix_a[0]
vector_a2 = matrix_a[1]

matrix_c[0][0] = vector_a1[0] * matrix_b[0][0] + vector_a1[1] * matrix_b[1][0]
matrix_c[0][1] = vector_a1[0] * matrix_b[0][1] + vector_a1[1] * matrix_b[1][1]
matrix_c[1][0] = vector_a2[0] * matrix_b[0][0] + vector_a2[1] * matrix_b[1][0]
matrix_c[1][1] = vector_a2[0] * matrix_b[0][1] + vector_a2[1] * matrix_b[1][1]

print(matrix_c)
'''

class Thread1(threading.Thread):
    def __init__(self, id, name):
        threading.Thread.__init__(self)
        self.id = id
        self.name = name

    def run(self):
        print "Starting " + self.name + "\n"

        vector_a1 = matrix_a[0]
        matrix_c[0][0] = vector_a1[0] * matrix_b[0][0] + vector_a1[1] * matrix_b[1][0]
        matrix_c[0][1] = vector_a1[0] * matrix_b[0][1] + vector_a1[1] * matrix_b[1][1]

        print "End " + self.name + "\n"


class Thread2(threading.Thread):
    def __init__(self, id, name):
        threading.Thread.__init__(self)
        self.id = id
        self.name = name

    def run(self):
        print "Starting " + self.name + "\n"

        vector_a2 = matrix_a[1]
        matrix_c[1][0] = vector_a2[0] * matrix_b[0][0] + vector_a2[1] * matrix_b[1][0]
        matrix_c[1][1] = vector_a2[0] * matrix_b[0][1] + vector_a2[1] * matrix_b[1][1]

        print "End " + self.name + "\n"

start = int(round(time.time() * 1000))
thread1 = Thread1(1, "Thread 1")
thread2 = Thread2(2, "Thread 2")

thread1.start()
thread2.start()
thread1.join()
thread2.join()

print("Execution Time --->", (int(round(time.time() * 1000)) - start))

print(matrix_c)
