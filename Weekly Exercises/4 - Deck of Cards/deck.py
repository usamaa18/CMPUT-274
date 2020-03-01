"""
	Name: Usama Ali
	ID: 1582217
	CMPUT 274, Fall 2019

	Weekly Exercise 4: Deck of Cards
"""

import copy
import sys


class Deck:
    """
    Stores playing cards
    """

    # char1 stores the 13 ranks (low to high value) and char2 stores the 4 suites
    char1 = "23456789TJQKA"
    char2 = "SHCD"

    def __init__(self, cards):
        '''Initializes attribute for deck of cards

        Arguments:
            cards: list of two-character strings representing cards
        '''

        # deepcopy used to ensure a true independent copy is made
        self.__deck = copy.deepcopy(cards)

    def deal(self):
        '''Deals one card from top of deck.

        Returns:
            Either the top card (first element in the deck list), or
            False if there are no cards left in the deck
        '''

        if len(self.__deck) < 1:
        	return False
        else:
        	return self.__deck.pop(0)

    def checkCard(i):
    	'''Checks if card has a valid rank and suite.

    	Returns:
    		False if card string is not length 2, or contains invalid chars.
    		Otherwise true.
    	'''

    	if len(i) != 2:
    		return False
    	elif not ((i[0] in Deck.char1) and (i[1] in Deck.char2)):
    		return False
    	return True

    def validate(self):
        '''Checks whether the deck is a valid 52 card deck or not.
 
        Returns:
           (is_valid, msg): a tuple containing a Boolean value indicating whether
                            the deck is valid (True) or not (False), and a string
                            that is either empty (when deck is valid) or contains
                            information about why the deck is no valid
        '''

        # deck should not have less than 52 cards
        if len(self.__deck) < 52:
        	return False, "Incomplete deck"

        # loop through each card
        for i in self.__deck:
        	# check if card is not valid
        	if Deck.checkCard(i) is False:
        		invalid = "Card " + i + " is not a valid card"
        		return False, invalid
        	# check if card exists more than once in the deck
        	if self.__deck.count(i) != 1:
        		return False, "Deck contains duplicate cards"

        # if we reach till here, deck is good to go
        return True, ""

    def __str__(self):
        '''Creates custom string to represent deck object

        Returns:
           String representation of deck object
        '''

        # prints all cards except the last seperating them with "-"
        for i in range(len(self.__deck)-1):
        	print(self.__deck[i], end='-')

        # prints the last card and end with newline
        print(self.__deck[i+1])


def readfile(filename):
    '''Reads the file and returns a list of strings representing the deck of cards.
	Each line in the file represents one card.

	Arguements:
		filename: name of the file to be inputted

	Returns:
		list_deck: list of strings representing the inputted deck of cards

	'''
    fin = open(filename, "r")

    list_deck = []

    # go through each line in file, strip any '\n' any white spaces
    for line in fin:
    	line = line.strip('\n').strip()

    	# append the card to the list
    	list_deck.append(line.upper())

    return list_deck

    fin.close()


def play(deck):
	'''Function that plays rounds of High Card Draw till all cards in the deck are over

	Arguements:
		deck: object of type Deck, that stores the deck of cards
	'''
	check = True
	round = 1

	# keep going till all cards in deck are over
	while check:

		# deal the player the top-most card, and the dealer the second top-most card
		player = deck.deal()
		dealer = deck.deal()

		# assign False to "check" when cards in the deck are over, or only one card is left
		if (player is False) or (player is False):
			check = False

		else:

			# assign rank to player and dealer based on position of their ranks in char1
			player_rank = Deck.char1.find(player[0])
			dealer_rank = Deck.char1.find(dealer[0])

			# give result of round based of higher rank
			if player_rank > dealer_rank:
				print("Round {}: Player wins!".format(round))
			elif player_rank == dealer_rank:
				print("Round {}: Tie!".format(round))
			else:
				print("Round {}: Dealer wins!".format(round))

			round += 1


if __name__ == "__main__":

	# string showing correct syntax of program
    correct_usage = "USAGE: python3 deck.py <input_file_name>"

    # print error and exit if user does not use correct syntax
    if len(sys.argv) < 2:
    	print("ERROR: Too few command-line arguements")
    	print(correct_usage)
    elif len(sys.argv) > 2:
    	print("ERROR: Too many command-line arguements")
    	print(correct_usage)

    else:
    	# create object of deck of cards using readfile()
    	deck = Deck(readfile(sys.argv[1]))

    	# check if deck is valid
    	valid, error = deck.validate()

    	# print the appropriate error msg if deck is invalid, else play game
    	if valid is False:
    		print(error)
    	else:
    		play(deck)
