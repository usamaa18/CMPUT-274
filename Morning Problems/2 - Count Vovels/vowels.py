# read the input
text = input()
# solve the problem

vovels = "aeiou"
count = 0

for i in text:
	if i.lower() in vovels:
		count += 1

# output the result

print(count)