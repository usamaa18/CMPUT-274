import math

n = int(input())

x, y, r = [], [], []
# x, y, r are empty lists

for i in range(n):
    x_in, y_in, r_in = map(float, input().split())
    x.append(x_in)
    y.append(y_in)
    r.append(r_in)

m = int(input())

for j in range(m):
	x0, y0 = map(float, input().split())
	check = False
	for i in range(n):
		dist = math.sqrt((x0 - x[i])**2 + (y0 - y[i])**2)
		if dist < r[i]:
			check = True
	if check == True:
		print("Large")
	else:
		print("Small")


# Now read in the number of queries and then the
# queries themselves.
# You do not have to store all queries, you can
# output the answer for one query before reading
# in the next query point.
