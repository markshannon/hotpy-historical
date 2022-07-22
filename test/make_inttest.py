# Create a test for large integer arithmetic.
# Run this with a reliable version of python to test an unreliable one.



def print_test_un(i, op, opname):
    result = op(i)
    print("if (%s %s) != %s:" % (opname, i, result))
    print("    print('%s %s should be %s but is %%s' %% (%s %s))" %
          (opname, i, result, opname, i))
    print("    sys.exit(1)")

def make_test_bin(i1, i2, op, opname):
    print_test_bin(i1, i2, op, opname)
    print_test_bin(i1, -i2, op, opname)
    print_test_bin(-i1, i2, op, opname)
    print_test_bin(-i1, -i2, op, opname)
    
def print_test_bin(i1, i2, op, opname):
    result = op(i1, i2)
    expr = '%s %s %s' % (i1, opname, i2)
    print("test('%s', %s, %s)" % (expr, result, expr))

from random import random
import operator

print('import sys')
print("error_msg = '%s should be %s but is %s'")
print("def test(text, expected, actual):")
print("    if expected != actual:")
print("        print(error_msg % (text, expected, actual))")
print("        sys.exit(1)")
print()

for i in range(10,100,2):
    base = 2 ** i
    make_test_bin(int(base*random()), int(base*random()), operator.eq, '==')
    make_test_bin(int(base*random()), int(base*random()), operator.ne, '!=')
    make_test_bin(int(base*random()), int(base*random()), operator.lt, '<')
    make_test_bin(int(base*random()), int(base*random()), operator.gt, '>')
    make_test_bin(int(base*random()), int(base*random()), operator.le, '<=')
    make_test_bin(int(base*random()), int(base*random()), operator.ge, '>=')
for i in range(10,100,2):
    base = 2 ** i
    make_test_bin(int(base*random()), int(base*random()), operator.and_, '&')
    make_test_bin(int(base*random()), int(base*random()), operator.or_, '|')
for i in range(10,100,2):
    base = 2 ** i
    make_test_bin(int(base*random()), int(base*random()), operator.add, '+')
    make_test_bin(int(base*random()), int(base*random()), operator.sub, '-')
    make_test_bin(int(base*random()), int(base*random()), operator.mul, '*')
    make_test_bin(int(base*random()), int(base*random()), operator.floordiv, '//')



