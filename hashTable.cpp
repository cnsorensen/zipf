/* hashTable.cpp */
#include "hashTable.h"

hashTable :: hashTable()
{
	cout << "Generating a hash table of size " << fullSize << ".\n";
	// Initialize hashtable to size tableSize and check for proper allocation
	table = new tableItem[fullSize];
	if (table == nullptr) { 
		cout << "Unable to allocate space for hash table.\n";
		exit(1); // exit with an error
	}
}

hashTable :: ~hashTable()
{
	//delete[] table;
}

//pretty sure this method wont work, but lets try anyway..
void hashTable::resize(int newSize) {
	cout << "Rehashing to size " << newSize << " ... ";
	tableItem *newTable = new tableItem[newSize];
	if (newTable == nullptr) {
		cout << "Unable to resize hashtable to size " << newSize << ". Exiting program.\n";
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
	cout << "finished!\n";
}

bool hashTable::insert(string s) {
	if (tableSize == (fullSize)) {
		cout << "WARNING: Hash table (size " << fullSize << ") is 75% full!\n";
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
		numDistinct++;
	    table[currKey].freq++;
    }
	table[currKey].freq++;
	numWords++;
	return true;
}

/**
 * I don't know how this works now... it didnt work earlier, yet it
 * decides to work now.. little trolls fixed bugs or something,
 * unexplainable how this works now. -lucas
 *
 * It's been working for me. I even used your format for the word compare
 * (see below) I haven't seen any errors. -cs
**/
int freqcomp(const void *a, const void *b) {
	hashTable::tableItem *ia = (hashTable::tableItem *)a;
	hashTable::tableItem *ib = (hashTable::tableItem *)b;
	return (int)(ib->freq - ia->freq);
}

int wordcomp(const void *a, const void *b){
    hashTable::tableItem *ia = (hashTable::tableItem *)a;
    hashTable::tableItem *ib = (hashTable::tableItem *)b;
    return ia->word.compare(ib -> word ); 
}

int findEnd( int index, const hashTable* h )
{
    int length = 0;

    // Base case: hit the end of the line of similar frequencies
    if( h->table[index].freq != h->table[index+1].freq )
    {    
        return 0;
    }

    // Add one more to the number of words with the same frequency
    length += findEnd( index + 1, h ) + 1;
    return length;
}

void hashTable::sort()
{
    // Sort by the frequency values
	qsort(table, fullSize, sizeof(tableItem), freqcomp);
    
    // Sort alphabetically within the frequency values
    for( int i = 0; i < tableSize; i++ )
    {
        // Compare with the following word
        if( table[i].freq == table[i+1].freq )
        {
            // If they have the same frequency, find the number of words with
            // that frequency. Sort only those in the table
            int length = findEnd( i, this );
            qsort( table + i, length, sizeof( tableItem ), wordcomp );
            i += length;
        }  
    }    
}

void hashTable::printStats(string file) {
	ofstream out;
	int ext = file.find_last_of('.');
	if (ext == -1)
		ext = file.length();
	string wrdFile = file.substr(0, ext)+".wrd";
	out.open(wrdFile, ios::out | ios::trunc);
	out << "\nZipf's Law\n----------\nFile: " << file;
	out << "\nTotal number of words = " << numWords;
	out << "\nNumber of distinct words = " << numDistinct << "\n\n";
	/*out << "\n\nWord Frequencies\t\t\tRanks\tAvg Rank";
	out << "\n----------------\t\t\t-----\t--------";
	out << "\n\n";*/
	//29 spaces between s and r, 5 spaces between s and a
	out << "Word Frequencies"<< setw(47) << "Ranks     Avg Rank";
	out << "\n";
	out << "----------------"<< setw(47) << "-----     --------";
	out << "\n\n";
	int i = 0, numWords, curFreq, digits = 0, temp = 0, rank = 1;
	string *words;
	while(table[i].freq != -1) {
	  numWords = digits = temp = 0;
	  curFreq = table[i].freq;
	  words = new string[numDistinct]; // distinct becuz worst case is they all occur once or something.

	  temp = curFreq/10;
	  while(temp != 0) {
	    if(temp > 0)
	      digits++;
	    temp = temp/10;
	  }
	  cout << "Dig: " << digits << "\n";
	  out << "Words occurring " <<curFreq<< " times:" << setw((26-digits)) << rank;
	  out << "\n";
	  while(table[i].freq == curFreq) {
	    words[numWords] = table[i].word;
	    
	    numWords++;
	    i++;
	  }
	  i++;
	}
	out.close();
}

int hashTable::getNumWords() {
	return numWords;
}

int hashTable::getNumDistinct() {
	return numDistinct;
}

int hashTable::getSize() {
	return fullSize;
}

void hashTable::printHashTable()
{
    for( int i = 0; i < fullSize; i++ )
        if( table[i].freq != -1 )
            cout << table[i].word << " " << table[i].freq << "\n";
}
