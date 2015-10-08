# Really simple GCC error output coloring script
# Michael Moffitt 2015

import fileinput
import sys
import re

def main():
	for line in fileinput.input():
		words = re.split(' ',line);
		for word in words:
			# Everything is bold		
			print ('\033[1m',end="")
			if ("error" in word):
				# Red color
				print ('\033[91m',end="")
			elif ("warning" in word):
				# Yellow color
				print ('\033[93m',end="")
			else:
				# No color
				print ('\033[0m',end="")
			# reinsert the removed space if necessary
			if ('\n' not in word):
				print (word, end=" ")
			else:
				print (word, end="")
	
if __name__ == "__main__":
	main()
