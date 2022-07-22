from math import sin, cos, sqrt
import sys
import time

NUMBER_OF_RUNS = 10

class Point(object):

    def __init__(self, i):
        self.x = x = sin(i)
        self.y = cos(i) * 3
        self.z = (x * x) / 2

    def __repr__(self):
        return "<Point: x=%s, y=%s, z=%s>" % (self.x, self.y, self.z)

    def normalize(self):
        x = self.x
        y = self.y
        z = self.z
        norm = sqrt(x * x + y * y + z * z)
        self.x /= norm
        self.y /= norm
        self.z /= norm

    def maximize(self, other):
        self.x = max(self.x, other.x)
        self.y = max(self.y, other.y)
        self.z = max(self.z, other.z)
        return self


def maximize(points):
    next = points[0]
    for p in points[1:]:
        next = next.maximize(p)
    return next

def benchmark(n):
    points = [None] * n
    for i in range(n):
        points[i] = Point(i)
    for p in points:
        p.normalize()
    return maximize(points)

def main():
    times = []
    if len(sys.argv) == 1:
        n = 100000
    else:
        n = int(sys.argv[1])
    for i in range(NUMBER_OF_RUNS):
        print("Run: %s" % i)
        t = time.time()
        print(benchmark(n))
        times.append(time.time() - t)
    print()
    print("Best time: %s" % min(times))

if __name__ == '__main__':
    main()
