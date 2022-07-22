import sys
from itertools import islice, count

def gen_x():
    return map(lambda k: (k, 4*k + 2, 0, 2*k + 1), count(1))

def compose(xxx_todo_changeme, xxx_todo_changeme1):
    (aq, ar, as_, at) = xxx_todo_changeme
    (bq, br, bs, bt) = xxx_todo_changeme1
    return (aq * bq,
            aq * br + ar * bt,
            as_ * bq + at * bs,
            as_ * br + at * bt)

def extract(xxx_todo_changeme2, j):
#    print(xxx_todo_changeme2)
    (q, r, s, t) = xxx_todo_changeme2
    return (q*j + r) // (s*j + t)

def pi_digits():
    z = (1, 0, 0, 1)
    x = gen_x()
    while 1:
        y = extract(z, 3)
        while y != extract(z, 4):
            z = compose(z, next(x))
            y = extract(z, 3)
        z = compose((10, -10*y, 0, 1), z)
        yield y

def main():
    n = int(sys.argv[1])
    digits = pi_digits()
    width = 10
    for i in range(width, n+1, width):
        print("%s\t:%d" % ("".join(map(str, islice(digits, width))), i))
    if n % width > 0:
        sl = islice(digits, n % width)
        print("%s\t:%d" % ("".join(map(str, sl)).ljust(width), n))


main()

