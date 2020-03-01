seq = [x for x in input().split()]
dictionary = {}
for i in seq:
	if i in dictionary:
		dictionary[i] += 1
	else:
		dictionary[i] = 1

dkeys=[]
dvals=[]

for i in dictionary.keys():
	dkeys.append(i)
	dvals.append(dictionary[i])

dmax = max(dvals)

final = []

for i in range(len(dvals)):
	if dvals[i] == dmax:
		final.append(dkeys[i])

final.sort()

for i in final:
	print(i)
