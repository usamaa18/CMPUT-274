num_matches = int(input())

countAlice=countBob=0

for i in range(num_matches):
	rounds = [str(x) for x in input().split()]
	winAlice=winBob=0
	for i in rounds:
		if i[0]=='R' and i[1]=='S':
			winAlice +=1
		elif i[0]=='S' and i[1]=='P':
			winAlice +=1
		elif i[0]=='P' and i[1]=='R':
			winAlice +=1
		elif i[0]=='S' and i[1]=='R':
			winBob +=1
		elif i[0]=='P' and i[1]=='S':
			winBob +=1
		elif i[0]=='R' and i[1]=='P':
			winBob +=1
	if winAlice>winBob:
		countAlice+=1
	elif winBob>winAlice:
		countBob+=1

if countAlice>=countBob:
	print("Alice",countAlice)
elif countBob>countAlice:
	print("Bob",countBob)

    # now you do the rest!
    # read in the rounds in this match
    # example: if the line of input was "RR RP SR" then
    # rounds == ["RR", "RP", "SR"]
    

# print here whoever is the overall winner of all the matches and
# how many matches the winner won
