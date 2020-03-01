# Read in the input

n = int(input())

dictionary = {}

for i in range(n):
	binary, word = [str(x) for x in input().split()]
	dictionary[binary] = word

code = str(input())


start = 0
end = 1
length = len(code)

while end<=length:
	if code[start:end] in dictionary:
		print(dictionary[code[start:end]], end=" ")
		start = end
	end += 1




# Construct a dictionary mapping binary strings to English words

# Use the dictionary to decode the binary string
