# Read the input
a, b, c = map(int, input().split())


# Solve the problem

avg = (a+b+c)/3

print("{0} {1} {2}".format(int(abs(avg-a)), int(abs(avg-b)), int(abs(avg-c))))


# Output the result