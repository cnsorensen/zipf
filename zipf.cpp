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
 * Prototypes
**/
void fileHandle(char* fileName);
string tokenize(string str);


/**
 * Functions
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
  
  //hashTable T;
  
  return 0;
}

void fileHandle(char* fileName) {
  ifstream in;
  in.open(fileName, ios::in);

  string x;
  
  while(in >> x) {
    x = tokenize(x);
    cout << x << " ";
  }
  
}

/** 
 * not done 
**/
string tokenize(string str) {
  transform(str.begin(), str.end(), str.begin(), ::tolower);
  for(int i = 0;i<str.length();i++) {
    if(str[i] == 
  }
  
  return str;
  
}
