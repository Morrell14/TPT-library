// By Henry Vo


#ifndef HASHTABLE_H_
#define HASHTABLE_H_
#include <fstream>
#include <iostream>
#include "HashNode.h"

template<class ItemType>
class HashTable
{
private:
    HashNode<ItemType>* hashAry;
    int hashSize;
    int count;
    int totalCollisions;
    int longestCollision;

public:
    HashTable() { count = 0; hashSize = 53; hashAry = new HashNode<ItemType>[hashSize]; totalCollisions = 0; longestCollision = 0; }
    HashTable(int n) { count = 0; hashSize = n;	hashAry = new HashNode<ItemType>[hashSize]; totalCollisions = 0; longestCollision = 0; }
    ~HashTable() { delete[] hashAry; }

    int getCount() const { return count; }
    int getSize() const { return hashSize; }
    double getLoadFactor() const { return 100.0 * count / hashSize; }
    bool isEmpty() const { return count == 0; }
    bool isFull()  const { return count == hashSize; }
    int getTotalCollisions() const { return totalCollisions; }
    void setTotalCollisions(int c) { totalCollisions = c; }

    bool insert(const ItemType& itemIn, int h(const ItemType& key, int size));
    bool remove(ItemType& itemOut, const ItemType& key, int h(const ItemType& key, int size));
    int search(ItemType& itemOut, const ItemType& key, int h(const ItemType& key, int size));
    int findLargestPath();
    int getMostCollisions() { longestCollision = findLargestPath(); return longestCollision; }
    void setMostCollisions(int p) { longestCollision = p; }
    HashTable<ItemType>* rehash(int size, int nextPrime(int n), int h(const ItemType& key, int size));
    //void saveFile(void reformatOutData(string& t, string& a, string& g));     // Book data type only
    void saveFile();    // Generic data type version

};

/*~*~*~*
   Insert an item into the hash table
   It does not reject duplicates
*~**/
template<class ItemType>
bool HashTable<ItemType>::insert(const ItemType& itemIn, int h(const ItemType& key, int size))
{
    if (count == hashSize)
        return false;

    int dex = h(itemIn, hashSize);
    int col = 0;

    while (hashAry[dex].getOccupied() == 1) {
        dex = (dex + 1) % hashSize;
        col++;
        totalCollisions++;
    }

    hashAry[dex] = HashNode<ItemType>(itemIn, 1, col);
    ++count;

    return true;
}

/*~*~*~*
   Removes the item with the matching key from the hash table
     - copies data in the hash node to itemOut
     - replaces data in the hash node with an empty record
*~**/
template<class ItemType>
bool HashTable<ItemType>::remove(ItemType& itemOut, const ItemType& key, int h(const ItemType& key, int size))
{
    int dex = h(key, hashSize);
    int start = dex;

    do
    {
        if (hashAry[dex].getItem() == key)
        {
            itemOut = hashAry[dex].getItem();
            hashAry[dex] = HashNode<ItemType>();
            count--;
            return true;
        }
        dex = (dex + 1) % hashSize;
    } while (dex != start);

    return false;
}

/*~*~*~*
   hash search - linear probe
   if found:
      - copy data to itemOut
      - returns the number of collisions for this key
   if not found, returns -1
*~**/
template<class ItemType>
int HashTable<ItemType>::search(ItemType& itemOut, const ItemType& key, int h(const ItemType& key, int size))
{

    int dex = h(key, hashSize);
    int start = dex;

    do
    {
        if (hashAry[dex].getItem() == key)
        {
            itemOut = hashAry[dex].getItem();
            return hashAry[dex].getNoCollisions();;
        }
        dex = (dex + 1) % hashSize;
    } while (dex != start);

    return -1;
}

/*~*~*~*
  Goes through the array to find the node
  with the largest number of collisions
  returns the largest number of collisons
*~**/
template<class ItemType>
int HashTable<ItemType> :: findLargestPath()
{
    int largest = 0;
    int dex = 0;

    do
    {
        if (hashAry[dex].getNoCollisions() > largest)
        {
            largest = hashAry[dex].getNoCollisions();
        }
        dex++;
    } while (dex != hashSize);
    return largest; 
}

/*~*~*~*
  Allocates a new hash table, removing all items from the
  old hash table and rehashes them into the new hash table,
  deleting the old hash table afterwards.
  Written by James Qin
*~**/
template<class ItemType>
HashTable<ItemType>* HashTable<ItemType>::rehash(int size, int nextPrime(int n), int h(const ItemType& key, int size))
{
    int newSize = nextPrime(2 * size);
    //std::cout << "newSize: " << newSize << std::endl;
    HashTable<ItemType>* newHash = new HashTable<ItemType>(newSize);
    for (int i = 0; i < size; i++)
    {
        if (hashAry[i].getOccupied() == 1)
        {
            ItemType item = hashAry[i].getItem();
            newHash->insert(item, h);
            //std::cout << "rehashed " << item.getTitle() << std::endl;
        }
    }
    delete [] hashAry;
    return newHash;
}

/*~*~*~*
  Saves all stored items in an output file specified by the user.
  Has two different versions and will ask to overwrite if the 
  specified file name already exists.
  Written by James Qin
*~**/
template<class ItemType>
//void HashTable<ItemType>::saveFile(void reformatOutData(string& t, string& a, string& g))
void HashTable<ItemType>::saveFile()
{
    std::ofstream outFile;
    std::ifstream check;		// To check if file already exists
    string overwrite = "n";
    string input;

    bool cont = false;
    do
    {
        std::cout << "Please enter a name for the save file (including file extension):" << std::endl;
        std::cin >> input;
        std::cin.ignore();
        check.open(input);
        if (check)      // If file exists, ask to overwrite
        {
            check.close();
            std::cout << "A file already exists with the name " << input << "." << std::endl;
            std::cout << "Do you wish to overwrite the file? (y for yes)" << std::endl;
            std::cin >> overwrite;
        }

        if (!check || overwrite == "y")
            cont = true;

    } while (!cont);

    outFile.open(input);
    for (int i = 0; i < getSize(); i++)
    {
        if (hashAry[i].getOccupied() == 1)
        {
            outFile << hashAry[i].getItem() << std::endl;

            // Code below formats the save file in the same exact format
            // as input files, but since it requires calling member
            // functions of Book, it would make this function
            // not template-friendly.

            //string title, author, genre;
            //int quantity;
            //isbn = hashAry[i].getItem.getISBN();
            //title = hashAry[i].getItem().getTitle();
            //author = hashAry[i].getItem().getAuthor();
            //genre = hashAry[i].getItem().getGenre();
            //quantity = hashAry[i].getItem().getQuantity();
            //reformatOutData(title, author, genre);
            //outFile << isbn << " " << title << " " << author << " " << genre << " " << quantity << std::endl;
             

            //std::cout << "wrote " << hashAry[i].getItem().getTitle() << " to file" << std::endl;
        }

    }
    outFile.close();
    std::cout << "File saved as " << input << std::endl;
}
#endif // HASHTABLE_H_
