/* hashTable.cpp */
#include <stdio.h>
#include <stdlib.h>
#include "hashTable.h"

hashTable :: hashTable()
{
	std::cout << "Generating a hash table of size " << fullSize << ".\n";
	// Initialize hashtable to size tableSize and check for proper allocation
	table = new tableItem[fullSize];
	if (table == nullptr) { 
		std::cout << "Unable to allocate space for hash table.\n";
		exit(1); // exit with an error
	}
}

hashTable :: ~hashTable()
{
	delete[] table;
}

//pretty sure this method wont work, but lets try anyway..
void hashTable::resize(int newSize) {
	std::cout << "Rehashing to size " << newSize << " ... ";
	tableItem *newTable = new tableItem[newSize];
	if (newTable == nullptr) {
		std::cout << "Unable to resize hashtable to size " << newSize << ". Exiting program.\n";
		exit(1);
	}
	for (int i = 0; i < fullSize; i++) {
		int currKey = ((table[i].word[0] - 97) * (newSize / 26));
		while (newTable[currKey].word != "") {
			if (currKey == tableSize)
				currKey = -1;
			currKey++;
		}
		newTable[currKey].word = table[i].word;
		newTable[currKey].freq = table[i].freq;
	}
	fullSize = newSize;
	delete[] table;
	table = new tableItem[fullSize];
	for (int i = 0; i < fullSize; i++) {
		table[i].word = newTable[i].word;
		table[i].freq = newTable[i].freq;
	}
	delete[] newTable;
	std::cout << "finished!\n";
}

bool hashTable::insert(std::string s) {
	if (tableSize == (fullSize)) {
		std::cout << "WARNING: Hash table (size " << fullSize << ") is 75% full!\n";
		resize(fullSize * 2);
	}
	int startKey = ((s[0]-97) * (fullSize / 26)); 
	int currKey = startKey;
	while (table[currKey].word != "" && table[currKey].word != s) {
		if (currKey == tableSize)
			currKey = -1;
		currKey++;
	}
	//std::cout << "Ended word " << s << " at pos " << currKey << "\n";
	if (table[currKey].word != s) {
		table[currKey].word = s;
		tableSize++;
		distinct++;
	    table[currKey].freq++;
    }
	table[currKey].freq++;
	words++;
	return true;
}

<<<<<<< HEAD
int hashTable::compareFreq( const void* p1, const void* p2 )
{
    return( (*(tableItem**)p1)->freq - (*(tableItem**)p2)->freq );
=======
int freq_sort(const void *p1, const void *p2) {
  hashTable::tableItem *e1 = *(hashTable::tableItem**) p1;
  hashTable::tableItem *e2 = *(hashTable::tableItem**) p2;
  return (e2->freq - e1->freq);
}


void hashTable::sort() {
	// nope... nothing yet :(
	//clambda = []{ ( (const void*) a, (const void*) b ){ return( *(int*) b.freq - *(int*) a.freq ); };
  std::qsort(&table[0], fullSize, sizeof(tableItem*), freq_sort );	
>>>>>>> 8ce893cb1e03cfdbe008cbf3b369c387797d5477
}

void hashTable::sort() {
    std::qsort( table, tableSize, sizeof(tableItem*), compareFreq ); 	
}


void hashTable::printStats(std::string file) {
	std::ofstream out;
	int ext = file.find_last_of('.');
	if (ext == -1)
		ext = file.length();
	std::string wrdFile = file.substr(0, ext)+".wrd";
	out.open(wrdFile, std::ios::out | std::ios::trunc);
	out << "\nZipf's Law\n----------\nFile: " << file;
	out << "\nTotal number of words = " << words;
	out << "\n\nWord Frequencies\t\t\tRanks\tAvg Rank";
	out << "\n----------------\t\t\t-----\t--------";
	out << "\n\n";
	out.close();
}

int hashTable::getWords() {
	return words;
}

int hashTable::getDistinct() {
	return distinct;
}

int hashTable::getSize() {
	return fullSize;
}


