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
		bool insert( std::string s ); // inserts a given word into hash table
		void sort(); // sorts the hash table by frequency
		void printStats( std::string file ); // prints stats to output files
		int getNumWords(); // returns num of words in file
		int getNumDistinct(); // returns num of words in hash table
		int getSize(); // returns size of hash table
        bool deleteWord( std::string ); // deletes a given word from the hash table
        int findWord( std::string );  // find a given word in the hash table
		void printHashTable();   // prints hash table to console
        int hashFunc( std::string ); // hash function to determine where to insert

        // element in the hash table
		struct tableItem 
        {
			std::string word = "";
			int freq = -1;
		};

        tableItem* resize( int newSize ); // creates a new table and rehashes all
                                            // entries into the new table

    private:		
		tableItem* table = nullptr;
        int tableSize = 0, numDistinct = 0, numWords = 0;
		int fullSize = 1352;
		friend int freqcomp( tableItem*, tableItem* );
        friend int wordcomp( tableItem*, tableItem*);
        friend int findLength( int, const hashTable* );
		int getDigits(int num);
};

#endif
