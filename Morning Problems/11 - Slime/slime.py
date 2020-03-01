n = int(input())

slimes = []

for i in range(n):
	slimes.append(1)
	cond = True
	while len(slimes)>1 and cond:
		if slimes[-1] == slimes[-2]:
			slimes.pop()
			slimes[-1] += 1
		else:
			cond = False

for i in slimes:
	print(i, end=' ')