/* hashTable.h */
//guard against multiple inclusions
#ifndef __HASHTABLE_H_
#define __HASHTABLE_H_

// inlcudes
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

//class declaration
class hashTable
{
    public:
        // constructor
        hashTable();
        // destructor
        ~hashTable();
    
	void resize(int newSize);
	bool insert(std::string s);
	void sort();
	void printStats(std::string file);
	int getWords();
	int getDistinct();
	int getSize();
        //delete
        //find
	
	struct tableItem {
	  std::string word = "";
	  int freq = -1;
	};

        //friend int compare( const void*, const void* );

    private:
		/**
		 * maybe if we store it as a struct, we can make an array of structs
		 * and access each index using a key.. like with a starting size of 1009
		 * the key would be  (word[0] * (size/26)), if we have a large array of 
		 * these words we can enhance the key to be (word[0] * (size/26) + word[1]
		 * or something similar, not sure yet.. will have to evaluate more, but the
		 * key can be upgraded later to see if more efficient.
		**/
		
		tableItem *table = nullptr;
        int tableSize = 0, distinct = 0, words = 0;
		int fullSize = 1352;
		friend int freqcomp(tableItem *e1, tableItem *e2); // <-- accidently did this, and it works now..
};

#endif
