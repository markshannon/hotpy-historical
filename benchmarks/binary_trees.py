# The Computer Language Shootout Benchmarks
# http://shootout.alioth.debian.org/
#
# contributed by Antoine Pitrou
# modified by Dominique Wahli

from sys import argv

def make_tree(item, depth):
    if depth > 0:
        item2 = 2 * item
        depth -= 1
        return (item, make_tree(item2 - 1, depth), make_tree(item2, depth))
    else:
        return (item, None, None)

def check_tree(node):
    item, left, right = node
    if left is not None:
        return item + check_tree(left) - check_tree(right)
    else:
        return item

def main():
    min_depth = 4
    max_depth =  int(argv[1]) # max(min_depth + 2, int(argv[1]))
    stretch_depth = max_depth + 1

    print ("stretch tree of depth %d\t check: %d" %
            (stretch_depth, check_tree(make_tree(0, stretch_depth))))

    long_lived_tree = make_tree(0, max_depth)

    for depth in range(min_depth, stretch_depth, 2):
        iterations = 2**(max_depth - depth + min_depth)

        check = 0
        for i in range(1, iterations + 1):
            check += check_tree(make_tree(i, depth)) + check_tree(make_tree(-i, depth))

        print ("%d\t trees of depth %d\t check: %d" %
               (iterations * 2, depth, check))

    print ("long lived tree of depth %d\t check: %d" %
           (max_depth, check_tree(long_lived_tree)))

if __name__ == '__main__':
    main()


