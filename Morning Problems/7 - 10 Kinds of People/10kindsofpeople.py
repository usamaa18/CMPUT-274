# read the input

n, m = [int(x) for x in input().split()]

seq = input()

s = e = []

for i in range(m):
	st, et = [int(x) for x in input().split()]
	s.append(st)
	e.append(et)

for i in range(m):
	one = False
	zero = False
	for j in range(s.pop(0), e.pop(0)+1, 1):
		if seq[j-1] == '1':
			#print(seq[j-1],end='')
			one = True
		if seq[j-1] == '0':
			#print(seq[j-1],end='')
			zero = True
	#print()
	if (zero and one) is True:
		print("both")
	elif one is True:
		print("one")
	else:
		print("zero")




# solve the problem

# output the answer
