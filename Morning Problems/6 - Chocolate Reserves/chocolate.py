chocolates, jars = [int(x) for x in input().split()]

count = 0

for i in range(jars):
	filled, size = [int(x) for x in input().split()]
	if chocolates <= (size-filled):
		count += 1

print(count)