from sys import argv


def fannkuch(n):
   count = list(range(1,n+1))
   maxFlipsCount, m, r, check = 0, n-1, n, 0

   perm1 = list(range(n))
   perm  = list(range(n))
   while True:
      if check < 30:
         print ("".join([str(i+1) for i in perm1]))
         check += 1;

      while r != 1:
         count[r-1] = r
         r -= 1

      if perm1[0] != 0 and perm1[m] != m:
         perm[:]=perm1
         flipsCount = 0
         k = perm[0]
         while k:
            perm[:k+1] = perm[k::-1]
            flipsCount += 1
            k = perm[0]

         if flipsCount > maxFlipsCount:
            maxFlipsCount = flipsCount
            maxPerm = list(perm1)

      while True:
         if r == n: return maxFlipsCount
         perm1.insert(r,perm1.pop(0))
         count[r] -= 1
         if count[r] > 0: break
         r += 1

def main():
    n = int(argv and argv[1] or 1)
    print ("Pfannkuchen(%d) = %d\n"%(n,fannkuch(n)))

if __name__=="__main__":
    main()


