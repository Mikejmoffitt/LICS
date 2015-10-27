# Simple GCC error / warning colorization script
# Michael Moffitt 2015

import fileinput
import sys
import re

def line_parse(words):
	# Print color control characters if a key word was identified
	for word in words:
		# Default to "bright white"
		print ('\033[1m', end = "");
		if ("error" in word):
			# Red color
			print ('\033[91m', end = "")
		elif ("warning" in word):
			# Yellow color
			print ('\033[93m', end = "")
		else:
			# No color
			print ('\033[0m', end = "")

		# reinsert the removed space if necessary
		if ('\n' not in word):
			print (word, end=" ")
		else:
			print (word, end="")

def main():	
	# Reset color at start, just in case
	print ('\033[0m', end="");

	for line in fileinput.input():
		line_parse(re.split(' ', line));
			
if __name__ == "__main__":
	main()
