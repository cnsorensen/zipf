/* hashTable.cpp */

#include <string>
#include "hashTable.h"

using namespace std;

int stringToInt( const string& s )
{
    int val = 0;

    for( int i = 0; i < s.length(); i++ )
        val += s[i];

    return val;
}

// constructor
hashTable :: hashTable( int tableSize, int tableFilled ) : tableSize( tableSize ), tableFilled( tableFilled )
{
    // create new table of given size
    table = new hashItem[ tableSize ];

    // create a null item that will indicate and empty item
    hashItem nullItem;
    nullItem.freq = -1;

    // fill table with empty items
    for( int i = 0; i < tableSize; i++ )
        table[i] = nullItem;   
}

// copy constructor
hashTable :: hashTable( const hashTable& h ) : tableSize( h.tableSize )
{}

// destructor
hashTable :: ~hashTable()
{
//    delete table;
}

// insert
bool hashTable :: insertItem( const string& newWord )
{
    int index;
    index = findItem( newWord );

    // word is already in table
    if( index )
    {
        // increment the freq val
        table[index].freq++;
    }

    else
    {
        index = stringToInt( newWord ) % tableSize;

        // move along table until an empty spot is reached                
        while( table[index].freq != -1 )
            index++;
        
        // insert item
        table[index].word = newWord;
        table[index].freq = 1;  
        tableFilled++;

        //check to see if table is filled past 75%
    }

    return true;
}

// delete
bool hashTable :: deleteItem( const string& newWord )
{
    int index;
    index = findItem( newWord );
    
    if( index )
    {
        table[index].freq = -1;
        table[index].word = "";
        return true;
    }

    return true;
}

int hashTable :: findItem( const string& findWord )
{
    int index;
    index = stringToInt( findWord ) % tableSize;
    
    if( table[index].word == findWord )
        return index;

    ///need to find some way to loop through table
    else
    {
        while( table[index].word != findWord )
            index++;
        return index;
    }

    // unsuccessful at finding word
    return -1;
}
