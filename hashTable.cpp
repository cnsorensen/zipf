/* hashTable.cpp */
#include "hashTable.h"

int stringToInt ( const string & );

/**
 * hashTable() Constructor, initializes the list and generates a hash table
 * of size 26*(26*2)
**/
hashTable :: hashTable()
{
    cout << "Generating a hash table of size " << fullSize << ".\n";
    // Initialize hashtable to size tableSize and check for proper allocation
    table = new tableItem[fullSize];

    if ( table == nullptr )
    {
        cout << "Unable to allocate space for hash table.\n";
        exit ( 1 ); // exit with an error
    }
}

/**
 * ~hashTable() Destructor, de-allocates memory used for the hash table.
**/
hashTable :: ~hashTable()
{
    delete[] table;
}

/**
 * resize(int), this function is called when the hash table reaches over 75%
 * capacity, it will allocate new space for the hash table and copy over the data.
 * It then deletes the original table and returns the new one.
**/
hashTable::tableItem* hashTable :: resize ( int newSize )
{
    cout << "Rehashing to size " << newSize << " ... ";

    // create a new table with the new size given
    tableItem* newTable = new tableItem[newSize];

    // if it failed to create a new table, output error and exit
    if ( newTable == nullptr )
    {
        cout << "Unable to resize hashtable to size " << newSize << ". Exiting program.\n";
        exit ( 1 );
    }

    // rehash items from the original hash table
    for ( int i = 0; i < fullSize; i++ )
    {
        // if it's not an empty element
        if ( table[i].freq != -1 )
        {
            int currKey = hashFunc ( table[i].word );

            while ( newTable[currKey].word != "" )
            {
                if ( currKey == tableSize )
                    currKey = -1;

                currKey++;
            }

            newTable[currKey].word = table[i].word;
            newTable[currKey].freq = table[i].freq;
        }
    }

    fullSize = newSize;
    delete[] table;

    cout << "finished!\n";

    return newTable;
}

/**
 * insert(string), this function inserts a string into the hashtable. The key used
 * takes the first letter of the string (a being 0 and z being 25) and multiplies
 * it by the tablesize/26 to ensure the keys are placed evenly throughout the table.
**/
bool hashTable :: insert ( string s )
{
    // If the table is 75% full, resize into a new table and rehash the items
    if ( tableSize >= ( fullSize * 0.75 ) )
    {
        cout << "WARNING: Hash table (size " << fullSize << ") is 75% full!\n";
        table = resize ( fullSize * 2 );
    }

    int startKey = hashFunc ( s );
    int currKey = startKey;

    // iterate from the starting point finding an empty spot or the given
    // word's existing spot
    while ( table[currKey].word != "" && table[currKey].word != s )
    {
        currKey = ( currKey + 1 ) % fullSize;
    }

    // new word, fill it into the empty item
    if ( table[currKey].word != s )
    {
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
 * deleteWord( string ), finds the word in the hash table and removes it by
 * replacing the word and frequency in that item with default. False is
 * returned if the word was not found in the table.
 **/
bool hashTable :: deleteWord ( string s )
{
    int i = findWord ( s );

    if ( i )
    {
        table[i].freq = -1;
        table[i].word = "";
        return true;
    }

    return false;
}

/**
 * findWord( string ), finds a word in the hash table and returns the index
 * to it. If the word is not found, then -1 is returned.
 **/
int hashTable :: findWord ( string s )
{
    int startKey = hashFunc ( s );
    int currKey = startKey;

    while ( table[currKey].word != s )
    {
        if ( currKey == tableSize )
            currKey = -1;

        currKey++;

        if ( currKey == startKey )
            return -1;
    }

    return currKey;
}

/**
 * findLength( int, const hashTable* ), recursively finds the number of words
 * with the same frequency in the hash table
 **/
int findLength ( int index, const hashTable* h )
{
    int length = 0;

    // Base case: hit the end of the line of similar frequencies
    if ( h -> table[index].freq != h -> table[index + 1].freq )
    {
        return 0;
    }

    // Add one more to the number of words with the same frequency
    length += findLength ( index + 1, h ) + 1;
    return length;
}

/**
 * sort(), this function runs qsort on the hash Table sorting the items
 * by frequency.
**/
void hashTable :: sort()
{
    // Sort by the frequency values
    qsort ( table, fullSize, sizeof ( tableItem ), [] ( const void* a, const void* b )
    {
        hashTable::tableItem* ia = ( hashTable::tableItem* ) a;
        hashTable::tableItem* ib = ( hashTable::tableItem* ) b;
        return ( int ) ( ib->freq - ia->freq );
    } );

    // Sort alphabetically within the frequency values
    for ( int i = 0; i < tableSize; i++ )
    {
        // Compare with the following word
        if ( table[i].freq == table[i + 1].freq )
        {
            // If they have the same frequency, find the number of words with
            // that frequency following it. Sort only those in the table
            int length = findLength ( i, this );
            qsort ( table + i, length + 1, sizeof ( tableItem ), [] ( const void* a, const void* b )
            {
                hashTable::tableItem* ia = ( hashTable::tableItem* ) a;
                hashTable::tableItem* ib = ( hashTable::tableItem* ) b;

                return ia -> word.compare ( ib -> word );
            } );

            i += length;
        }
    }
}

/**
 * printStats(string), this function creates two output files (.wrd and .csv) and
 * prints to them a formatted version of the hashTable that orders the words by freq
 * and gives each freq a rank.
**/
void hashTable :: printStats ( string file )
{
    //Declare ofstreams for file output
    ofstream wrdout;
    ofstream csvout;

    //Find the file extension and give a new extension
    int ext = file.find_last_of ( '.' );

    if ( ext == -1 )
        ext = file.length();

    string wrdFile = file.substr ( 0, ext ) + ".wrd";
    string csvFile = file.substr ( 0, ext ) + ".csv";

    //Open and begin writing to the files..
    wrdout.open ( wrdFile, ios::out | ios::trunc );
    csvout.open ( csvFile, ios::out | ios::trunc );

    //First we write to the wrd file, then the csv file
    wrdout << "\nZipf's Law\n----------\nFile: " << file;
    wrdout << "\nTotal number of words = " << numWords;
    wrdout << "\nNumber of distinct words = " << numDistinct << "\n\n";
    wrdout << "Word Frequencies" << setw ( 47 ) << "Ranks     Avg Rank";
    wrdout << "\n";
    wrdout << "----------------" << setw ( 47 ) << "-----     --------";
    wrdout << "\n\n";

    csvout << "\n    Zipf's Law\n    ----------\n    File: " << file;
    csvout << "\n    Total number of words = " << numWords;
    csvout << "\n    Number of distinct words = " << numDistinct;
    csvout << "\n\n    rank    freq    rank*freq\n    ----    ----    ---------\n";

    // Here we handle the ranks and frequencies of our hash table
    int i = 0, count, curFreq;
    float rank = 1.0;
    string rankword;
    string* words;

    while ( table[i].freq != -1 )
    {
        count = 0;
        curFreq = table[i].freq;
        words = new string[numDistinct];

        while ( table[i].freq == curFreq )
        {
            words[count] = table[i].word;
            count++;
            i++;
        }

        // Here we print out the word frequency, rank, avg rank, and the array of word
        if ( count == 1 )
        {
            wrdout << "Words occurring " << curFreq << " times:"
                   << setw ( ( 27 - getDigits ( curFreq ) ) ) << ( int ) rank;
            wrdout << fixed << showpoint << setprecision ( 1 );
            wrdout << setw ( 13 ) << rank;
            csvout << fixed << showpoint << setprecision ( 1 );
            csvout << setw ( 7 ) << rank << ",";
            csvout << setw ( 7 ) << curFreq << ",";
            csvout << setw ( 12 ) << ( float ) ( ( float ) rank * ( float ) curFreq ) << "\n";
        }

        else
        {
            rankword = to_string ( ( int ) rank ) + "-" + to_string ( ( int ) rank + count - 1 );
            wrdout << "Words occurring " << curFreq << " times:"
                   << setw ( ( 27 - getDigits ( curFreq ) ) ) << rankword;
            wrdout << fixed << showpoint << setprecision ( 1 );
            wrdout << setw ( 13 ) << ( float ) ( ( ( float ) ( rank + ( rank + count - 1 ) ) ) / 2 );
            csvout << setw ( 7 ) << ( float ) ( ( ( float ) ( rank + ( rank + count - 1 ) ) ) / 2 ) << ",";
            csvout << setw ( 7 ) << curFreq << ",";
            csvout << setw ( 12 ) << ( float ) ( ( ( ( float ) ( rank + ( rank + count - 1 ) ) ) / 2 ) * ( float ) curFreq ) << "\n";
        }

        rank = rank + count;

        for ( int j = 0; j < count; j++ )
        {
            if ( j % 5 == 0 )
                wrdout << "\n";

            wrdout << words[j] << setw ( 14 - words[j].length() ) << " ";
        }

        wrdout << "\n\n";
    }

    wrdout.close();
    csvout.close();
}

/**
 * getDigits(int), this function returns the number of digits in (int).
**/
int hashTable :: getDigits ( int num )
{
    int digits = 1, temp = num / 10;

    while ( temp != 0 )
    {
        digits++;
        temp = temp / 10;
    }

    return digits;
}

/**
 * getNumWords(), returns the number of words in file
**/
int hashTable :: getNumWords()
{
    return numWords;
}

/**
 * getNumDistinct(), returns the number of words in the hash table
**/
int hashTable :: getNumDistinct()
{
    return numDistinct;
}

/**
 * getSize(), returns the size of the hash table
**/
int hashTable :: getSize()
{
    return fullSize;
}

/**
 * printHashTable(), prints table to console
 **/
void hashTable :: printHashTable()
{
    for ( int i = 0; i < fullSize; i++ )
        if ( table[i].freq != -1 )
            cout << table[i].word << " " << table[i].freq << "\n";
}

/**
 * hashFunc( string ), the function for the hash table that calculates where the
 *  word should be inserted
 **/
inline int hashTable :: hashFunc ( string s )
{
    return ( ( stringToInt ( s ) * s.length() ) % fullSize );
}

/**
 *  stringToInt( const string& )
 **/
int stringToInt ( const string &s )
{
    int val = 0;

    for ( unsigned int i = 0; i < s.length(); i++ )
        val += s[i];

    return val;
}
