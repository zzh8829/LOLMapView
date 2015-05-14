import pprint

import sys
sys.stdout = open("err.txt","w")

a = open("err.log").read().split()
print(len(a))

l = []

for i in range(len(a)//11):
	l.append(a[i*11:i*11+11])

pprint.pprint(l)