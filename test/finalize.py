#Run this test with limited memory (~600k)

class C:
    def __init__(self):
        global count
        count += 1
    
    def __del__(self):
        global count
        count -= 1 
        print (count)
        
count = 0
        
print (count)
for i in range(1000):
    C()
    
print ("Start:", count)

for i in range(10000000):
    pass

print ("End:", count)
