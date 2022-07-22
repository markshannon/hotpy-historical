for i in range(100):
    print(hex(1 << i))
k = 1 << 100
for i in range(100):
    print(hex(k >> i))

