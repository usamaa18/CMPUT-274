# read in the input
x1, y1 = map(int, input().split())
x2, y2 = map(int, input().split())

# solve the problem

length = abs(x1-x2)
width = abs(y1-y2)
area = length*width

# output the result

print(area)
