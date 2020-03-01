# read in the input
x1, y1, x2, y2 = map(int, input().split())
x3, y3, x4, y4 = map(int, input().split())

# now solve the problem. good luck :D

def slope(x1,y1,x2,y2):
	try: 
		slope = (y2-y1)/(x2-x1)
	except ZeroDivisionError:
		slope = "infinity"
	return slope

slope1 = slope(x1, y1, x2, y2)
slope2 = slope(x3, y3, x4, y4)

if slope1 == slope2:
	print("parallel")
##elif slope1 == slope2*-1:
##	print("parallel")
else:
	print("not parallel")