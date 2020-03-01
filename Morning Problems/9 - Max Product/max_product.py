import math

n = int(input())

x = [int(a) for a in input().split()]
y = [int(a) for a in input().split()]

def dotprod(x,y):
	total = 0
	for i in range(n):
		term = x[i]*y[i]
		total += term

	return total
"""
xpos = []
xneg = []
ypos = []
yneg = [] 


for i in x:
	print(i)
	if i <= 0:
		print("neg")
		xneg.append(i)
	elif i > 0:
		print("pos")
		xpos.append(i)
for i in y:
	print(i)
	if i <= 0:
		yneg.append(i)
	elif i > 0:
		ypos.append(i)


print(xpos)
print(ypos)
print(xneg)
print(yneg)

xpos = maxvec(xpos)
ypos = maxvec(ypos)
xneg = maxvec(xneg).reverse()
yneg = maxvec(yneg).reverse()




x = list(xpos) + list(xneg)
y = list(ypos) + list(yneg)
"""

#x = maxvec(x)
#y = maxvec(y)

#print (x)
#print (y)
x.sort()
y.sort()
print(dotprod(x,y))

