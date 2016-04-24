__author__ = 'Ahmed Hani Ibrahim'
import threading
import time

matrix_a = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
matrix_b = [[9, 8, 7], [6, 5, 4], [3, 2, 1]]
matrix_c = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]


class Thread1(threading.Thread):
    def __init__(self, id, name):
        threading.Thread.__init__(self)
        self.id = id
        self.name = name

    def run(self):
        print "Starting " + self.name + "\n"

        vector_1a = matrix_a[0]
        matrix_c[0][0] = vector_1a[0] * matrix_b[0][0] + vector_1a[1] * matrix_b[1][0] + vector_1a[2] * matrix_b[2][0]
        matrix_c[0][1] = vector_1a[0] * matrix_b[0][1] + vector_1a[1] * matrix_b[1][1] + vector_1a[2] * matrix_b[2][1]
        matrix_c[0][2] = vector_1a[0] * matrix_b[0][2] + vector_1a[1] * matrix_b[1][2] + vector_1a[2] * matrix_b[2][2]

        print "End " + self.name + "\n"


class Thread2(threading.Thread):
    def __init__(self, id, name):
        threading.Thread.__init__(self)
        self.id = id
        self.name = name

    def run(self):
        print "Starting " + self.name + "\n"

        vector_2a = matrix_a[1]
        matrix_c[1][0] = vector_2a[0] * matrix_b[0][0] + vector_2a[1] * matrix_b[1][0] + vector_2a[2] * matrix_b[2][0]
        matrix_c[1][1] = vector_2a[0] * matrix_b[0][1] + vector_2a[1] * matrix_b[1][1] + vector_2a[2] * matrix_b[2][1]
        matrix_c[1][2] = vector_2a[0] * matrix_b[0][2] + vector_2a[1] * matrix_b[1][2] + vector_2a[2] * matrix_b[2][2]

        print "End " + self.name + "\n"


class Thread3(threading.Thread):
    def __init__(self, id, name):
        threading.Thread.__init__(self)
        self.id = id
        self.name = name

    def run(self):
        print "Starting " + self.name + "\n"

        vector_3a = matrix_a[2]
        matrix_c[2][0] = vector_3a[0] * matrix_b[0][0] + vector_3a[1] * matrix_b[1][0] + vector_3a[2] * matrix_b[2][0]
        matrix_c[2][1] = vector_3a[0] * matrix_b[0][1] + vector_3a[1] * matrix_b[1][1] + vector_3a[2] * matrix_b[2][1]
        matrix_c[2][2] = vector_3a[0] * matrix_b[0][2] + vector_3a[1] * matrix_b[1][2] + vector_3a[2] * matrix_b[2][2]

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

print("Execution Time --->", (int(round(time.time() * 1000)) - start))

print(matrix_c)
