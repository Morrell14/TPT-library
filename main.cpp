// Main file for the program
// Written by James Qin, Rimma Esheva
// IDE: MS Visual Studio / VS Code

#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include "Book.h"
#include "HashTable.h"
#include "BinarySearchTree.h"

using namespace std;

void readInputFile(HashTable<Book>& hashAry, string filename, int hashSize, BinarySearchTree& bst);
int key_to_index(const Book& key, int size);
int getLines(string& filename);
bool isPrime(int n);
int nextPrime(int n);
void reformatData(string& t, string& a, string& g);
void reformatOutData(string& t, string& a, string& g);
void hDisplay(Book&);
void vDisplay(Book&);
void iDisplay(Book&, int);
void menu();
// string findID(Book&);

int main()
{
	BinarySearchTree bst;
	int choice;
	menu(); 							// display the menu
	cin >> choice;

	if (choice == 1) {					// add data from file
		string filename = "";
		int lines = getLines(filename);
		int hashSize = nextPrime(2 * lines);
		HashTable<Book> hashAry(hashSize);
		readInputFile(hashAry, filename, hashSize, bst);
		hashAry.saveFile();

		menu();							// display the menu again
		cin >> choice;

		if (choice == 2) {					// display Books database
			//cout << "binary search tree:" << endl;
			//bst.inOrder(vDisplay);
			bst.inOrder(hDisplay);
		}
	}

	return 0;
}



/*~*~*~*
  Program Menu.
  Allows user to interact with the program.
  Written by Rimma Esheva
*~**/
// Not a final version
void menu() {
	// Menu options
	cout << "\nPlease choose one of the options or Q to exit: \n";
	cout << "1 - Add Data From File\n"
		<< "2 - Show All Books\n"
		<< "3 - Add New Book To Database\n"
		<< "4 - Search Book By ISBN\n"
		<< "5 - Delete Book From Database\n"
		<< "6 - Undo Delete\n"
		<< "7 - Show Statistics\n"
		<< "8 - Help\n"
		<< "9 -Quit\n";

}



/*~*~*~*
  Reads an input file and inserts the data into the hash table and BST,
  rehashing if necessary. Displays a preview of the first 5 books that were inserted
  so user can verify if the file inserted was the one they want.
  Written by James Qin
*~**/
void readInputFile(HashTable<Book>& hashAry, string filename, int hashSize, BinarySearchTree& bst)
{
	//string filename;
	int isbn;
	string title;
	string author;
	string genre;
	int quantity;
	ifstream inFile;
	int lines = 0;
	int preview = 0;

	inFile.open(filename);
	//cout << "hashSize: " << hashSize << endl;
	cout << "Preview of books inserted: ";
	while (inFile >> title)
	{
		inFile >> isbn >> title >> author >> genre >> quantity;
		reformatData(title, author, genre);
		Book item(isbn, title, author, genre, quantity);
		if (hashAry.getLoadFactor() >= 75)		// Create a new, bigger hash table if load factor >= 75%
		{
			hashAry = *hashAry.rehash(hashAry.getSize(), nextPrime, key_to_index);
			//cout << "rehash called" << endl;
		}
		hashAry.insert(item, key_to_index);
		//item.setISBN(isbn);
		hashAry.insert(item, key_to_index);
		bst.insert(item);
		//cout << "inserted: " << item.getTitle() << endl;
		if (preview < 5)	// Shows title of first 5 books
		{
			cout << item.getTitle() << ", ";
			preview++;
		}
	}
	inFile.close();
	cout << "..." << endl;
	//cout << "all entries inserted" << endl;
}

/*~*~*~*
  Hash function
*~**/
int key_to_index(const Book& key, int size)
{
	string k = key.getTitle();
	int sum = 0;
	for (int i = 0; k[i]; i++)
		sum += k[i];
	return sum % size;
};

/*~*~*~*
  Determines how many lines are in a user specified
  input file if successfully opened, prompting another
  file name if opening failed.
  Written by James Qin
*~**/
int getLines(string& filename)
{
	ifstream inFile;
	string s;
	int i;
	int count = 0;
	string input;
	//inFile.open(filename);
	bool fileOpened = false;
	do
	{
		cout << "What is the file name? " << endl;
		cin >> input;
		inFile.open(input);
		if (inFile)
		{
			//cout << "file opened" << endl;
			while (inFile >> i >> s >> s >> s >> i)
				count++;
			//cout << "lines read: " << lines << endl;
			fileOpened = true;
			filename = input;
			inFile.close();
		}
		else
			cout << "Error: File could not be opened. Please try again." << endl;
	} while (!fileOpened);
	//cout << "lines read: " << count << endl;
	cout << "File " << input << " successfully opened!" << endl;
	return count;
}

/*~*~*~*
  Determines if a number is prime. Used as part of nextPrime().
  Written by James Qin
*~**/
bool isPrime(int n)
{
	// Corner cases
	if (n <= 1)  return false;
	if (n <= 3)  return true;

	// This is checked so that we can skip 
	// middle five numbers in below loop
	if (n % 2 == 0 || n % 3 == 0)
		return false;

	for (int i = 5; i * i <= n; i = i + 6)
		if (n % i == 0 || n % (i + 2) == 0)
			return false;

	return true;
}

/*~*~*~*
  Determines the next prime number starting from an input value.
  Used to find the size for the hash table.
  Written by James Qin
*~**/
int nextPrime(int n)
{
	// Base case
	if (n <= 1)
		return 2;

	int prime = n;
	bool found = false;

	// Loop continuously until isPrime returns
	// true for a number greater than n
	while (!found)
	{
		prime++;
		if (isPrime(prime))
			found = true;
	}

	return prime;
}

/*~*~*~*
  Removes underscores and replaces them with spaces.
  Written by James Qin
*~**/
void reformatData(string& t, string& a, string& g)
{
	replace(t.begin(), t.end(), '_', ' ');
	replace(a.begin(), a.end(), '_', ' ');
	replace(g.begin(), g.end(), '_', ' ');
}


/*~*~*~*
  Removes spaces and replaces them with underscores in the format
  of input files.
  Written by James Qin
*~**/
void reformatOutData(string& t, string& a, string& g)
{
	replace(t.begin(), t.end(), ' ', '_');
	replace(a.begin(), a.end(), ' ', '_');
	replace(g.begin(), g.end(), ' ', '_');
}

void hDisplay(Book& item)
{
	cout << left;
	cout << " " << setw(40) << item.getISBN() << " ";
	cout << " " << setw(10) << item.getTitle() << "  ";
	cout << " " << setw(35) << item.getAuthor() << "  ";
	cout << " " << setw(40) << item.getGenre() << "  ";
	cout << right;
	cout << " " << setw(10) << item.getQuantity() << " ";
	cout << left << endl;
}

/*
 vertical display: one item per line
*/
void vDisplay(Book& item)
{
	cout << "ISBN: " << item.getISBN() << endl;
	cout << "Title: " << item.getTitle() << endl;
	cout << "Author: " << item.getAuthor() << endl;
	cout << "Genre: " << item.getGenre() << endl;
	cout << "Quantity: " << item.getQuantity() << endl;
}

/*
 indented tree display: one item per line, including the level number
*/
void iDisplay(Book& item, int level)
{
	for (int i = 1; i < level; i++)
		cout << "..";
	cout << level << "). " << item.getTitle() << endl;
}


/*~*~*~*
  Finds the Id of a given book.
  Written by James Qin
*~**
string findID(Book& b)
{
	string title = b.getTitle();
	string author = b.getAuthor();
	string genre = b.getGenre();
	char tFront = title.front();
	char tBack = title.back();
	char a = author.front();
	char g = genre.front();
	string key = "";
	key.push_back(tFront);
	key.push_back(tBack);
	key.push_back(a);
	key.push_back(g);
	// cout << "key: " << key << endl;
	return key;
} */