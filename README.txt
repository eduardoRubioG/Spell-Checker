README.txt file 

This is a program that will serve as a spell checker for text files. The features in this program will include: 
1.) Read text file that includes “dictionary” words and input them into a hash table
	i.) If no such file exists, default to empty hash table
2.) Read text file to check for spelling accuracy 
3.) Once file is analyzed, user will have following options: 
	i.) Add incorrect words into the dictionary 
	ii.) Ignore the word 
	iii.) Move on to the next word 
	iv.) Search for suggested spelling 
		- This will be done by switching adjacent characters in the word and then cross-checking the hash table. If 
		   if found, then suggest the word to add to the dictionary file. 
	v.) Quit 
4.) Output all words not found in the dictionary and the line number on which they occurred to an external file. 
	i.) Duplicates will be printed once with the line number being the last occurrence 
5.) Dynamically update the hash table as well as the “dictionary” file 

Program Files: 
- HashTable: A spec & implementation for a hash table 
- BinarySearchTree: A spec & implementation for a hash table 
- LinkedList: A spec & Implementation for a LinkedList  
- dict.txt: The dictionary file 
- myFile.txt: A short sample file for testing 
- spellCheck: Client file 
- not found.txt: Output file 
- dictionary.(cpp/h): Spec & Implementation to the dictionary hash (client level) 


