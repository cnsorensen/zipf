/**
 * Includes
**/
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <ctime>
#include "hashTable.h"

using namespace std;

/**
 * Global Variables
**/
const char* valid = "abcdefghijklmnopqrstuvwxyz";
const char* valid2 = "abcdefghijklmnopqrstuvwxyz'";

/**
 * Prototypes
**/
void fileHandle(char* fileName, hashTable &h);
string tokenize(string str);

/**
 * Functions
**/

/**
 * main, the starting point of our program, reads in command
 * line args and passes them off to other functions or prints
 * out a usage statement
**/
int main(int argc, char*argv[])
{
	hashTable h;

    switch(argc) 
    {
		//if 2 args, pass to file handling and continue program
		case 2:
			fileHandle(argv[1], h);
		break;

		//if more or less than 2 args print out usage statement
		default: 
			cout << "Usage: zipf filename\n";
			return 1;
	}

	return 0;
}

/**
 * fileHandle, reads in a file and passes each word of the file
 * onto a tokenize function and inserts the word into a hashtable
**/
void fileHandle(char* fileName, hashTable &h) 
{
    ifstream in;
    in.open(fileName, ios::in);
    if (!in) 
    {
	    cout << "Unable to open " << fileName << ". Exiting program.\n";
	    return;
    }

    string x;
    auto c1 = clock();
    while(in >> x) 
    {
        x = tokenize(x);

	    // we would add into a hash table here.. 
	    // should only need to add in, in the insert function for 
	    // our hash table it should be able to handle everything else
	    if (x != "" && !h.insert(x)) 
        {
		    cout << "Unable to insert " << x << " into hashtable. Exiting program\n";
		    return;
	    }
    }

    cout << "Finished generating a hash table of size " << h.getSize() << ".\n";
    cout << "Read " << h.getNumWords() << " words from the file " << fileName << ".\n";
    cout << "Inserted " << h.getNumDistinct() << " distinct words into the hash table.\n";
    cout << "Compacting and sorting the hash table ... ";
    h.sort();
    cout << "finished!\n";  
    auto c2 = clock();  
    auto totalTime = c2 - c1;
    cout << "Elapsed time = " << totalTime * 1000.0 / CLOCKS_PER_SEC <<  " msec.\n";
        
    h.printStats(fileName);
}

/** 
 * tokenize, takes in an input string and makes all chars lowercase, then
 * checks for valid chars throughout the string and returns out the newly
 * modified string. This function is a modification of Dr. Weiss's tokenize2
 * code.
**/
string tokenize(string str) 
{
	// this will lowercase the string
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	
	// skip delimiters to start of first token
	int tokenStart = str.find_first_of(valid, 0);
	// find next delimiter (i.e., end of first token)
	int tokenEnd = str.find_first_not_of(valid2, tokenStart);

	// if tokenstart == -1, no valid chars were found in the input string
	if (tokenStart == -1) 
    {
		return "";
	}

	// since tokenEnd will return a -1 if no invalid char is found
	// we need to change it to the length of the string
	if (tokenEnd == -1) 
    {
		tokenEnd = str.size();
	}

	// remove beginning ' or end ' 
	if (str[tokenEnd-1] == '\'')
		tokenEnd--;

	//if (str[tokenStart] == '\'')
		//tokenStart++;

	// substring our word from tokenStart to tokenEnd
	str = str.substr(tokenStart, (tokenEnd - tokenStart));

	//output our newly modified string
	return str;
}
