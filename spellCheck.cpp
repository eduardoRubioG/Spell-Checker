//**********************************************************//
//                 C L I E N T . F I L E                    //
//                   (spellChecker.cpp)                     //
//                Written by Eduardo Rubio                  //
//**********************************************************//
#include <stdio.h> 
#include <iostream> 
#include <fstream> 
#include <string> 
#include <sstream> 
#include <cstdlib>
using namespace std; 
#include "dictionary.h"
#include "BinarySearchTree.h" 

//Binary Node Struct: 
//	Will be used to for adding new words 
//	into the client Dictionary  
struct BNode
{ 
	string word;
	int lineNum; 
	bool operator < (BNode otherStruct) const 
	{ 
		return ( word < otherStruct.word); 
	}
}; 
typedef BinarySearchTree<BNode> BST;
typedef Dictionary<string> Dict;  
//End Struct definition 

ostream& operator << (ostream & out, BNode & temp) 
{ 
	out << temp.word << " " << temp.lineNum; 
	return out; 
}

//Start prototype declarations 
void tolower(string &s); //.......................................................Changes string to all lowercase 
string checkPerm(string s, Dict d, Dict cD, const int ver);//.....................Checks all permutations of string
void fillDict(Dict &d);//.........................................................Fill hashTable with dictionary words 
bool check(Dict d, string word);//................................................Return t/f on whether word is in dictionary 
void checkFile(Dict d, Dict &cD, Dict &ignD, BST &t, string file);//..............Main function to check user file. Details included within the function 
bool isChar(char c);//............................................................A function to check to see if character is a letter 
string cleanup(string &s);//......................................................This function strips a string from anything other than letters and " ' " symbols 
bool isWord(string s);//..........................................................Returns t/f on whether string is considered a word 
void pHeader();//.................................................................Quick function to print the header of the program 
void pOptions();//................................................................Quick function to print the options available to the user 
int runMenu(BST &t, Dict &clientD, Dict d, Dict &iD, string word, int lineNum, string f);//.Code hub that will be the menu for the client to run all functionalities 
void printTreeOut(BST t);//.......................................................Print tree into external file  
void next();//....................................................................Prompts the user to hit 'enter' to continue program   
void replaceW(string &line, string wrongWord, string replacement);//..............Replaces misspelled words with suggested word 
void correctFile(string filename, Dict d, Dict clientD, Dict ignore_d);//.........Replaces file with spelling errors with correct words  
//End prototype declarations  

int main( int argc,char** argv ) 
{
	if( argc != 2)
	{ 
	  cout << "User has failed to specify filename. Please include filename after calling the program" << endl; 
	  exit(1); 
	} 

	/////////////////////////////////////// O B J E C T   D E C L A R A T I O N /////////////////////////////////////////////////// 
	Dict mainD("ZZZ", 1000);//........................Dictionary loaded with words to check file spelling 
	Dict clientD("ZZZ", 500);//......................Dictionary that user might want to add onto main Dictionary.   
	Dict ignore_list("ZZZ", 200);//..................Dictionary that will keep track of words to ignore within the program 
	BNode NULL_BNode = {"null", -1};//................BNode that will be used to initialize the BST   
	BST tree( NULL_BNode );//.........................BinarySearchTree that holds BNodes (Used to add words to the user dictionary)  
	ofstream outfile("notfound.txt");//...............Stream into the external writing file 
	string filename = argv[1]; 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Clear the screen to start program! 
	system ("clear"); 

	
	//Fills a hash table with dictionary words 
	fillDict(mainD); 
	

	//Main part of the program; Spell checks the file 
	checkFile(mainD, clientD, ignore_list, tree, filename ); 
	

	//Prints the tree to notfound.txt 
	tree.printTree( outfile ); 
	

	return 0; 
}

void checkFile(Dict d, Dict &cD, Dict &ignoreD, BST &t, string filename ) 
{ 
	//1.) Open file stream to user file (the one that will be checked for checking) 
	ifstream file( filename.c_str() ); 
	string input;
	int lineNum = 0; 
	int quit_mark = 1; //This will serve as an integer to determine whether the program should continue to analize text file 
	
	//2.) Open while loop that iterates through every word in the file
	while( getline(file, input) && quit_mark != 0) 
	{ 
		istringstream iss(input); 
		lineNum++; 
		do { 
		string word; 
		iss >> word;
		//2a.) Check to see if string is a word 
		if ( isWord( word ) ) 
		{
			//2b.) Check to see if word is dictionary 
			//	If not found: 	Do nothing 				
			//	If found:     	Check permutations of word 
			string original = word; 
			cleanup(word); //Strip string of all non-letter characters 
			tolower(word); //Makes word all lowercase for checking the hash table 
			if( !check(d, word) && !check(cD, word) && !check( ignoreD, word ) )  //Word is not found in the main or client dictionary  
			{ 
				pHeader(); 
 				cout << "On line " << lineNum << ", \"" << original << "\" was not found in dictionary... \n" << endl; 
				pOptions(); 
				quit_mark = runMenu(t, cD, d,ignoreD, word, lineNum, filename);//Returns 0 if user chooses to quit program 
			}//........................End inner if statement 
		}//................................End outermost if statement  
	    } while(iss && quit_mark != 0); //.....End do-while loop 
	}//........................................End outermost while loop 
}//end checkFile() 

void printTreeOut(BST t) 
{ //Prints tree elements into the outfile.txt 
	ofstream outfile("notfound.txt"); 
	t.printTree( outfile ); 
	outfile.close(); 
}//end printTreeOut()~  

string cleanup(string &s) 
{ 
	for(int x = 0; x < s.size(); x++) 			//Check through all parts of string 
	{ 
		if( !isChar(s[x]) ) 				//If character is not a letter 
		{ 
			string first_half, second_half; 	// concatonate string without the non-letter character 
			first_half = s.substr(0, x); 
			second_half = s.substr(x+1, s.size() - x); 
			s = first_half + second_half; 
		}
	}
	return s;						//Return stripped string  
}//end cleanup()~ 

bool check(Dict d, string word) 
{ 
	if( d.lookup(word) == word) 
		return true; 
	else 
		return false; 
}//End check() ~ 

void fillDict(Dict &d)					
{ 
	//This function will fill a hash table with all the strings in a file 
	//1.) Open the file stream 
	ifstream file("dict.txt"); 
	string input; 
	//2.) fill the table 
	while( getline(file, input) ) 
	{
	  d.insert( input ); 
	}
}//end fillHash()~

string checkPerm(string s, Dict d, Dict clientD, Dict ignore_d, const int ver) 
{//Checks all the permutations of the string put into the function 
 //	Will additionally search each of the permuations found 
 //	Permutations will only be found through adjacent characters
 //	If a permutation is found within either of the Dictionaries: 
 //		Found word will be offered as a suggested spelling 
 //	Else 
 //		Original misspelled word will be returned as indication 
 //		no suggested spelling can be offered  
	char tmp; 
	int posOne, posTwo; 
	string original = s; 

	for(int x = 0; x < s.size() - 1; x++) 	//For all the perms that a string shoud have 
	{ 
		posOne = x; 
		posTwo = x+1; 

		tmp = s[posOne]; 		//Swaps adjacent characters 
		s[posOne] = s[posTwo]; 
		s[posTwo] = tmp; 

		if( check( d, s) || check( clientD, s) || check( ignore_d, s) ) 
		  return s; 
		
		s = original; 			//Returns the checked string back to original 
	} 
	if(ver == 0)  cout << "\nNo suggested spelling found... \n" << endl; 
	return original; 			//No suggested spelling found; return original word 
}//end checkPerm()~

bool isWord(string s)
{ 
//Checks to see if string is a word 
	int numLetters = 0; 
	for ( int x = 0; x < s.size(); x++ ) 
		if( isChar(s[x]) ) 
			numLetters++; 
	return ( numLetters >= 1 ? true : false ); 
}//end isWord()~  

bool isChar(char c) 
{ 
	if( 		c == ' ' || 
			c == ',' || 
			c == '!' ||
			c == '-' ||
			c == '_' ||
			c == '.' ||
			c == '?' || 
			c == ':' || 
			isdigit(c)) 
		return false; 
	else 
		return true; 
} //end isChar()~

void tolower(string &s) 
{ //Returns a string in all lowercase 
	for(int x = 0; x < s.size(); x++) 
	{ 
		s[x] = tolower(s[x]); 
	}
}//end tolower() ~

int runMenu(BST &t, Dict &clientD, Dict d, Dict &ignore_d, string word, int lineNum, string filename)//If function returns 0, then it will stop the whole program
{
    int user_choice;
    cin >> user_choice;
    
    switch ( user_choice )
    {
        case 1://
	     {
	     cout << "\nAdded '" << word << "' to client dictionary!\n" << endl;//Add word to client dictionary and move on  
             ofstream file( "dict.txt", ios::app ); 
	     file << word << "\n"; 
	     clientD.insert(word); 
	     BNode new_word = {word, lineNum};     
	       t.insert( new_word );  
	     next(); 
	     return 1;
	     } 
        case 2://
	    { 
            //In this option, we simply do nothing. Just ignore the word and move onto the next word
            cout << "\nIgnoring future instances of word\n" << endl;
	    ignore_d.insert(word);   
	    BNode new_word = {word, lineNum};     
	      t.insert( new_word );  
            next();  
	    return 1;
	    }
        case 3:
	    { //
            // Similar to case 2. However, in this option we add word to BST, and at end of program BST will be printed to the outfile 
	    BNode new_word = {word, lineNum};     
	      t.insert( new_word );  
	    next(); 
            return 1;
	    } 
        case 4:
	    { 
            cout << "\nFinding alternate spelling options... " << endl; // Do the perm and check. If one of them is found, suggest that spelling (will we need to change the text???) 
	    BNode new_word = {word, lineNum};     
	      t.insert( new_word );  
	    if( checkPerm( word, d, clientD, ignore_d, 0 ) != word ) 
		cout << "\nSuggested spelling:\t" << checkPerm( word, d, clientD, ignore_d, 0) << "\n" << endl; 
	    next(); 
            return 1;
	    } 
	case 5: 
	   { 
	   cout << "Replacing all wrong words with spelling suggestions..." << endl; 
	   correctFile(filename, d, clientD, ignore_d); 
	   next(); 
	   return 1; 
	   }
        case 6://
	    { 
            cout << "\nExiting program... " << endl; // Done 
            return 0;
	    }
	default: //
            cout << "\nInvalid entry; Please try again..." << endl;
            return runMenu(t, clientD, d, ignore_d, word, lineNum, filename); 
    }
}//end runMenu()~ 

void pHeader() //Prints the header for the program 
{
    cout << "--------------------------------------";
    cout << "       S P E L L  C H E C K E R       ";
    cout << "--------------------------------------\n\n";
}//end pHeader()~

void pOptions()//Prints the options available to the user 
{
    cout << "1.) Add word to dictionary" << endl;
    cout << "2.) Ignore word, and skip future reference" << endl; //To make this happen, create new 'code' to insert into hashtable as a 'been here, don't flag' mark
    cout << "3.) Move on to next word" << endl;
    cout << "4.) Find suggested spelling" << endl;
    cout << "5.) Replace all wrong words with suggested spellings" << endl; 
    cout << "6.) Quit Spell Checker program\n" << endl;
    cout << "Please enter your selection: "; 
}//end pOptions()~

void next() // Just a stop-go function for the user to go through program at their own pace 
{ 
	char temp = 'x'; 
	cin.ignore(); 
	cout << "Please press enter to continue... " << endl; 
	while( temp != '\n' )
	  cin.get(temp); 
	if( temp == '\n' ) 
	  system( "clear" ); 
} //end next()~ 

void stats() 
{ 


}//end stats()~ 
 
void replaceW(string &line, string wrongWord, string replacement) 
{ 
	size_t pos = line.find( wrongWord );

	if( pos != string::npos ) 
		line.replace(pos, wrongWord.length(), replacement ); 
}

void correctFile(string filename, Dict d, Dict clientD, Dict ignore_d)  
{ 
	ifstream file( filename.c_str() ); 
	string currLine; 
	ofstream newFile( "newFile.txt" ); 

	//1.) Get line and check for word 
	while( getline(file, currLine) ) 
	{ 
		istringstream iss(currLine); 

		do { 
		string word; 
		iss >> word;
		//2.) Check to see if string is a word 
		if ( isWord( word ) ) 
		{

		//	2a.) If word is found, check to see if it is spelled correctly 
			string original = word; 
			cleanup(word);  
			tolower(word);  
			if( !check(d, word) && !check(clientD, word) )  //Word is not found in the main or client dictionary  
			{ 
			//2b.) Check to see if there are spelling suggestions 
				string replacement = checkPerm( word, d, clientD, ignore_d, 1 );  
				replaceW(currLine, original, replacement); 
			}//........................End inner if statement 
		}//................................End outermost if statement  
	    } while(iss); //.......................End do-while loop 
	    newFile << currLine << endl; 
	}//........................................End outermost while loop 
        newFile.close(); 
	file.close(); 

	string command = "rm " + filename; 
	system( command.c_str() );
	string rename = "mv newFile.txt " + filename;  
	system( rename.c_str() ); 
}//end replaceW()

