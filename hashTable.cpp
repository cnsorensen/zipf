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
	qsort(table, fullSize, sizeof(tableItem), [](const void* a, const void *b)
	{
		hashTable::tableItem *ia = (hashTable::tableItem *)(a);
		hashTable::tableItem *ib = (hashTable::tableItem *)(b);
		return (int)(ib->freq - ia->freq);
	});
    

	/**
	 *  @christine, this worked somewhat, the end of the last 
	 * contained a random word... didn't write it so i had no idea
	 * how to go about fixing it.. i just used qsort with a lambda
	 * function below in the .wrd printing
	**/
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
	//Declare ofstreams for file output
	ofstream wrdout;
	ofstream csvout;

	//Find the file extension and give a new extension
	int ext = file.find_last_of('.');
	if (ext == -1)
		ext = file.length();
	string wrdFile = file.substr(0, ext)+".wrd";
	string csvFile = file.substr(0, ext) + ".csv";

	//Open and begin writing to the files..
	wrdout.open(wrdFile, ios::out | ios::trunc);
	csvout.open(csvFile, ios::out | ios::trunc);

	//First we write to the wrd file, then the csv file
	wrdout << "\nZipf's Law\n----------\nFile: " << file;
	wrdout << "\nTotal number of words = " << numWords;
	wrdout << "\nNumber of distinct words = " << numDistinct << "\n\n";
	wrdout << "Word Frequencies"<< setw(47) << "Ranks     Avg Rank";
	wrdout << "\n";
	wrdout << "----------------"<< setw(47) << "-----     --------";
	wrdout << "\n\n";

	csvout << "\n    Zipf's Law\n    ----------\n    File: " << file;
	csvout << "\n    Total number of words = " << numWords;
	csvout << "\n    Number of distinct words = " << numDistinct;
	csvout << "\n\n    rank    freq    rank*freq\n    ----    ----    ---------\n";
	
	// Here we handle the ranks and frequencies of our hash table
	int i = 0, count, curFreq;
	float rank = 1.0;
	string rankword;
	string *words;
	while(table[i].freq != -1) {
	  count = 0;
	  curFreq = table[i].freq;
	  words = new string[numDistinct];
	  while(table[i].freq == curFreq) {
	    words[count] = table[i].word;
	    count++;
	    i++;
	  }
	  // LAMBDA!!! woohoo.
	  qsort(words, count, sizeof(string), [](const void *a, const void *b)
	  {
		  string ia = *(string *)(a);
		  string ib = *(string *)(b);
		  return ia.compare(ib);
	  });
	  rankword = to_string((int)rank) + "-" + to_string((int)rank+count-1);
	  if (count == 1) {
		  wrdout << "Words occurring " << curFreq << " times:"
			  << setw((27 - getDigits(curFreq))) << (int)rank;
		  wrdout << fixed << showpoint << setprecision(1);
		  wrdout << setw(13) << rank;
		  csvout << fixed << showpoint << setprecision(1);
		  csvout << setw(7) << rank << ",";
		  csvout << setw(7) << curFreq << ",";
		  csvout << setw(12) << (float)((float)rank*(float)curFreq) << "\n";
	  } else {
		  wrdout << "Words occurring " << curFreq << " times:" 
			  << setw((27 - getDigits(curFreq))) << rankword;
		  wrdout << fixed << showpoint << setprecision(1);
		  wrdout << setw(13) << (float)(((float)(rank + (rank+count-1))) / 2);
		  csvout << setw(7) << (float)(((float)(rank + (rank + count - 1))) / 2) << ",";
		  csvout << setw(7) << curFreq << ",";
		  csvout << setw(12) << (float)((((float)(rank + (rank + count - 1))) / 2)*(float)curFreq) << "\n";
	  }
	  rank = rank + count;
	  for (int j = 0; j < count; j++) {
		  if (j % 5 == 0)
			  wrdout << "\n";
		  wrdout << words[j] << setw(14-words[j].length()) << " ";
	  }
	  wrdout << "\n\n";
	}
	wrdout.close();
	csvout.close();
}

int hashTable::getDigits(int num) {
	int digits = 1, temp = num / 10;
	while (temp != 0) {
		digits++;
		temp = temp / 10;
	}
	return digits;
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
