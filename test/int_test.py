print(100000000)
print(100000000 * 100000000)
print(100000000 * 100000000 * 100000000)
print(100000000 * 100000000 * 100000000 * 100000000)
assert 7<<56 == 7*2**56
assert 123456789 << 17 == 123456789 * 2**17
assert 7914601 << 179 >> 178 == 7914601 << 1

assert 7//3 == 2
assert 7%3 == 1
assert -7//3 == -3
assert -7%3 == 2
assert 7//-3 == -3
assert 7%-3 == -2
assert -7//-3 == 2
assert -7%-3 == -1

x = 10000000 * 10000000 * 10000000
assert(7 *x)//(3*x) == 2
#assert(7*x)%(3*x) == x
print('%d // %d = %d' % ((-7*x), (3*x), (-7*x)//(3*x)))
print('%d // %d = %d' % ((7*x), (-3*x), (7*x)//(-3*x)))
print('%d // %d = %d' % ((-7*x), (-3*x), (-7*x)//(-3*x)))
print('%d mod %d = %d' % ((7*x), (3*x), (7*x)%(3*x)))
print('%d mod %d = %d' % ((7*x), (-3*x), (7*x)%(-3*x)))
print('%d mod %d = %d' % ((-7*x), (3*x), (-7*x)%(3*x)))
print('%d mod %d = %d' % ((-7*x), (-3*x), (-7*x)%(-3*x)))
assert(-7 *x)//(3*x) == -3
print (2*x)
assert(-7*x)%(3*x) == 2*x
assert(7 *x)//(-3*x) == -3
assert(7*x)%(-3*x) == -2*x
assert(-7 *x)//(-3*x) == 2
assert(-7*x)%(-3*x) == -x
assert 123456789 * 87654321 * 555555 // (98765432 * 555555) == 109567900

