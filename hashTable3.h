/* hashTable.h */

#ifndef _HashTable_H_
#define _HashTable_H_

#include <string>
using namespace std;

class hashTable
{
    public:
        // constructor
        hashTable( int tableSize = 0, int tableFilled = 0 );
        
        // copy constructor
        hashTable( const hashTable& );

        // destructor
        ~hashTable();
    
        // insert
        bool insertItem( const string& );
        
        // delete
        bool deleteItem( const string& );

        // find
        int findItem( const string& );


        friend int stringToInt( const string& );

        struct hashItem
        {
            string word;
            int freq;       
        };

        hashItem* table;

    private:
        int tableSize;
        int tableFilled;
//        hashItem* table;
};


#endif
