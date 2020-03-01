# Get the input
leftlane = input().split()
rightlane = input().split()
# now do something similar to get the list of vehicles in the right lane

i=j=0

n = len(leftlane)
m = len(rightlane)

while i < n or j < m:
	if i < n:
		print(leftlane[i], end=' ')
		i += 1

	if j < m:
		print(rightlane[j], end=' ')
		j += 1

print()



# Solve the problem


# Print the result
