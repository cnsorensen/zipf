/**
 * Includes
**/
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
//#include "hashTable.h"

using namespace std;

/**
 * Global Variables
**/
const char* valid = "abcdefghijklmnopqrstuvwxyz'";

/**
 * Prototypes
**/
void fileHandle(char* fileName);
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
  switch(argc) {
    //if 2 args, pass to file handling and continue program
    case 2:
      fileHandle(argv[1]);
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
void fileHandle(char* fileName) {
  ifstream in;
  in.open(fileName, ios::in);
  if (!in) {
	  cout << "Unable to open " << fileName << ". Exiting program.\n";
	  return;
  }

  string x;
  
  while(in >> x) {
    x = tokenize(x);
    cout << x << " ";
  }
  
}

/** 
 * tokenize, takes in an input string and makes all chars lowercase, then
 * checks for valid chars throughout the string and returns out the newly
 * modified string. This function is a modification of Dr. Weiss's tokenize2
 * code.
**/
string tokenize(string str) {
	// this will lowercase the string
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	
	// skip delimiters to start of first token
	int tokenStart = str.find_first_of(valid, 0);
	// find next delimiter (i.e., end of first token)
	int tokenEnd = str.find_first_not_of(valid, tokenStart);

	// since tokenEnd will return a -1 if no invalid char is found
	// we need to change it to the length of the string
	if (tokenEnd == -1) {
		tokenEnd = str.size();
	}
	// remove beginning ' or end ' 
	if (str[tokenEnd-1] == '\'')
		tokenEnd--;
	if (str[tokenStart] == '\'')
		tokenStart++;

	// substring our word from tokenStart to tokenEnd
	str = str.substr(tokenStart, (tokenEnd - tokenStart));

	//output our newly modified string
	return str;
}
