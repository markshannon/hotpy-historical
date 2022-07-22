assert [1,2,1,2,1,2 ] == [1,2,1,2,1,2 ]

l = [1,2,3]
l *= 2

assert l == [1,2,3,1,2,3]

l = [1,2,3,4,5,6]

assert l[3::] == [4,5,6]

assert l[3:] == [4,5,6]

assert l[:3] == [1,2,3]

assert l[3::-1] == [4,3,2,1]

l.append(l)

print (l)

