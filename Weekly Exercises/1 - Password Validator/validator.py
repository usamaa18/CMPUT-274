"""
	Name: Usama Ali
	ID: 1582217
	CMPUT 274, Fall 2019

	Weekly Exercise 1: Password Validator
"""

import random

# strings with each type of required characters
upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
lower = "abcdefghijklmnopqrstuvwxyz"
dec = "0123456789"
chars = "!#$%&'()*+,./:;<=>?@[]^`{|}~"

# string with forbideen characters
forbidden = " -_"


def validate(password):
	""" Tests validity and security of given password

	Arguements:
		password (string): the password inputted by user

	Returns:
		Either "Invalid", "Insecure", or "Secure"
	"""

	# assume that password does not contain any of the required character types
	existsUpper = existsLower = existsDec = existsChars = False

	# password invalid if it is less than 8 characters
	if len(password) < 8:
		return("Invalid")

	# go through each character
	for char in password:

		# return "Invalid" if any forbiddden character exists
		if char in forbidden:
			return("Invalid")

		# test each required character type
		if char in upper:
			existsUpper = True
		if char in lower:
			existsLower = True
		if char in dec:
			existsDec = True
		if char in chars:
			existsChars = True

	# if all required character types exist, it is "Secure", else "Insecure"
	if existsUpper and existsLower and existsDec and existsChars:
		return("Secure")
	else:
		return("Insecure")


def generate(n):
	""" Generates random "Secure" password of length n

	Arguements:
		n (int): length of password requested by user

	Returns:
		Either an ERROR (if n<8) or a "Secure" password
	"""

	if n < 8:
		return("ERROR: NUMBER MUST BE GREATER THAN 7")
	else:
		password = ""
		# infinite loop until a "Secure" password is generated and returned to main
		# it is highly unlikely that it will run more than once
		while True:
			password = ""  # resets the password string every iteration
			for i in range(n):
				charlist = random.choice([upper, lower, dec, chars])  # randomly chooses one of the required character types
				char = random.choice(charlist)  # randomly chooses one character from that type
				password += char  # append that character to password

			# to ensure that all required character types are fulfilled before returning it
			if validate(password) == "Secure":
				return(password)


if __name__ == "__main__":
	password = input("Enter a password: ")
	print(validate(password))
	n = int(input("Enter a number: "))
	print(generate(n))
