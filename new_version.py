f = open('version.txt', 'r')
num = int(f.readline())
f.close()
num += 1
f = open('version.txt', 'w')
f.write(str(num))
f.close()
