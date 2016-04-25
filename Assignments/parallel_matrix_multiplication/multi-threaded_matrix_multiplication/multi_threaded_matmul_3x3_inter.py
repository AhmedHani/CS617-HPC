__author__ = 'Ahmed Hani Ibrahim'
import threading
import time

matrix_a = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
matrix_b = [[9, 8, 7], [6, 5, 4], [3, 2, 1]]
matrix_c = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]

matrix_c_aux1 = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
matrix_c_aux2 = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
matrix_c_aux3 = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]


def sum_matrices(first, second, third):
    res = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
    for i in range(len(first)):
        for j in range(len(first)):
            res[i][j] = first[i][j] + second[i][j] + third[i][j]

    return res


class Thread1(threading.Thread):
    def __init__(self, id, name):
        threading.Thread.__init__(self)
        self.id = id
        self.name = name

    def run(self):
        print "Starting " + self.name + "\n"

        vector_3x1a = [matrix_a[i][0] for i in range(len(matrix_a))]
        vector_1x3b = [matrix_b[0][j] for j in range(len(matrix_b[0]))]

        matrix_c_aux1[0][0] = vector_3x1a[0] * vector_1x3b[0]
        matrix_c_aux1[0][1] = vector_3x1a[0] * vector_1x3b[1]
        matrix_c_aux1[0][2] = vector_3x1a[0] * vector_1x3b[2]

        matrix_c_aux1[1][0] = vector_3x1a[1] * vector_1x3b[0]
        matrix_c_aux1[1][1] = vector_3x1a[1] * vector_1x3b[1]
        matrix_c_aux1[1][2] = vector_3x1a[1] * vector_1x3b[2]

        matrix_c_aux1[2][0] = vector_3x1a[2] * vector_1x3b[0]
        matrix_c_aux1[2][1] = vector_3x1a[2] * vector_1x3b[1]
        matrix_c_aux1[2][2] = vector_3x1a[2] * vector_1x3b[2]

        print "End " + self.name + "\n"


class Thread2(threading.Thread):
    def __init__(self, id, name):
        threading.Thread.__init__(self)
        self.id = id
        self.name = name

    def run(self):
        print "Starting " + self.name + "\n"

        vector_3x1a = [matrix_a[i][1] for i in range(len(matrix_a))]
        vector_1x3b = [matrix_b[1][j] for j in range(len(matrix_b[1]))]

        matrix_c_aux2[0][0] = vector_3x1a[0] * vector_1x3b[0]
        matrix_c_aux2[0][1] = vector_3x1a[0] * vector_1x3b[1]
        matrix_c_aux2[0][2] = vector_3x1a[0] * vector_1x3b[2]

        matrix_c_aux2[1][0] = vector_3x1a[1] * vector_1x3b[0]
        matrix_c_aux2[1][1] = vector_3x1a[1] * vector_1x3b[1]
        matrix_c_aux2[1][2] = vector_3x1a[1] * vector_1x3b[2]

        matrix_c_aux2[2][0] = vector_3x1a[2] * vector_1x3b[0]
        matrix_c_aux2[2][1] = vector_3x1a[2] * vector_1x3b[1]
        matrix_c_aux2[2][2] = vector_3x1a[2] * vector_1x3b[2]

        print "End " + self.name + "\n"


class Thread3(threading.Thread):
    def __init__(self, id, name):
        threading.Thread.__init__(self)
        self.id = id
        self.name = name

    def run(self):
        print "Starting " + self.name + "\n"

        vector_3x1a = [matrix_a[i][2] for i in range(len(matrix_a))]
        vector_1x3b = [matrix_b[2][j] for j in range(len(matrix_b[2]))]

        matrix_c_aux3[0][0] += vector_3x1a[0] * vector_1x3b[0]
        matrix_c_aux3[0][1] += vector_3x1a[0] * vector_1x3b[1]
        matrix_c_aux3[0][2] += vector_3x1a[0] * vector_1x3b[2]

        matrix_c_aux3[1][0] += vector_3x1a[1] * vector_1x3b[0]
        matrix_c_aux3[1][1] += vector_3x1a[1] * vector_1x3b[1]
        matrix_c_aux3[1][2] += vector_3x1a[1] * vector_1x3b[2]

        matrix_c_aux3[2][0] += vector_3x1a[2] * vector_1x3b[0]
        matrix_c_aux3[2][1] += vector_3x1a[2] * vector_1x3b[1]
        matrix_c_aux3[2][2] += vector_3x1a[2] * vector_1x3b[2]

        print "End " + self.name + "\n"

start = int(round(time.time() * 1000))

thread1 = Thread1(1, "Thread 1")
thread2 = Thread2(2, "Thread 2")
thread3 = Thread3(3, "Thread 3")

thread1.start()
thread2.start()
thread3.start()
thread1.join()
thread2.join()
thread3.join()

matrix_c = sum_matrices(matrix_c_aux1, matrix_c_aux2, matrix_c_aux3)

print("Execution Time --->", (int(round(time.time() * 1000)) - start))

print(matrix_c)