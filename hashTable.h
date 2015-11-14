/* hashTable.h */
//guard against multiple inclusions
#ifndef __HASHTABLE_H_
#define __HASHTABLE_H_

// inlcudes
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

//class declaration
class hashTable
{
    public:
        hashTable();  // constructor
        ~hashTable(); // destructor
		void resize(int newSize); // resizes the hash table
		bool insert(std::string s); // inserts string into hash table
		void sort(); // sorts the hash table by frequency
		void printStats(std::string file); // prints stats to output files
		int getNumWords(); // returns num of words in file
		int getNumDistinct(); // returns num of words in hash table
		int getSize(); // returns size of hash table
        bool deleteWord(std::string);
        int findWord(std::string);  
		void printHashTable();    
	
		struct tableItem {
			std::string word = "";
			int freq = -1;
		};

    private:		
		tableItem *table = nullptr;
        int tableSize = 0, numDistinct = 0, numWords = 0;
		int fullSize = 1352;
		friend int freqcomp(tableItem *e1, tableItem *e2);
        friend int wordcomp(tableItem *e1, tableItem *e2);
        friend int findLength( int, const hashTable* );
		int getDigits(int num);
};

#endif
