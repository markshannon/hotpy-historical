f = open('version.txt', 'r')
num = int(f.readline())
f.close()

f = open('version.h', 'w')
print >> f, "#define MAGIC_NUMBER %d" % ((ord('P') << 8) + ord('y'))
print >> f, "#define VERSION_NUMBER %d" % num
f.close()

