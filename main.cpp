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
#include "StackADT.h"

using namespace std;

void readInputFile(HashTable<Book>& hashAry, string filename, int hashSize, BinarySearchTree& bst);
int key_to_index(const Book& key, int size);
int getLines(string& filename);
bool isPrime(int n);
int nextPrime(int n);
void reformatData(string& t, string& a, string& g);
//void reformatOutData(string& t, string& a, string& g);
void hDisplay(Book&);
void vDisplay(Book&);
void iDisplay(Book&, int);
void menu();
// string findID(Book&);

int main()
{
	BinarySearchTree bst;
	string choice;
	HashTable<Book> hashTable;
	Stack<Book> undo;
	int deleted = 0;

	menu(); 							// display the menu

	do
	{
		//cin >> choice;
		if (choice == "1") // add data from file
		{					
			string filename = "";
			int lines = getLines(filename);
			if (lines != -1)
			{
				int hashSize = nextPrime(2 * lines);
				HashTable<Book> hashAry(hashSize);
				readInputFile(hashAry, filename, hashSize, bst);
				hashTable = hashAry;
			}
			
			//hashAry.saveFile();
		}

		if (choice == "2") // display Books database
		{					
			//cout << "binary search tree:" << endl;
			//bst.inOrder(vDisplay);
			bst.inOrder(hDisplay);
		}

		if (choice == "3")
		{
			string isbn;
			string title;
			string author;
			string genre;
			int quantity;
			cout << "Enter ISBN: ";
			cin >> isbn;
			cin.ignore();
			cout << "Enter title: ";
			getline(cin, title);
			//cin.ignore();
			cout << "Enter author: ";
			getline(cin, author);
			//cin.ignore();
			cout << "Enter genre: ";
			getline(cin, genre);
			//cin.ignore();
			cout << "Enter quantity: ";
			cin >> quantity;
			//cin.ignore();
			Book item(isbn, title, author, genre, quantity);
			hashTable.insert(item, key_to_index);
			bst.insert(item);
		}

		if (choice == "4")
		{
			string isbn;
			cout << "Enter ISBN number of book to search: ";
			cin >> isbn;
			Book temp(isbn, "", "", "", 0);
			Book result;
			if (hashTable.search(result, temp, key_to_index))
			{
				cout << "Book found!" << endl;
				cout << result << endl;
			}
			else
			{
				cout << "No such book was found." << endl;
			}
		}

		if (choice == "5")
		{
			string isbn;
			cout << "Enter ISBN number of book to delete: ";
			cin >> isbn;
			Book temp(isbn, "", "", "", 0);
			Book output;
			if (hashTable.remove(output, temp, key_to_index))
			{
				bst.remove(output);
				undo.push(output);
				deleted++;
				cout << "Book " << output.getTitle() << " successfully removed." << endl;
			}
			else
				cout << "No such book to delete!" << endl;
			
		}

		if (choice == "6")
		{
			if (deleted == 0)
			{
				cout << "No entries to restore!" << endl;
			}
			else
			{
				Book restored = undo.pop();
				hashTable.insert(restored, key_to_index);
				bst.insert(restored);
				cout << "Book " << restored.getTitle() << " restored." << endl;
				deleted--;
			}
			
		}

		if (choice == "7")
		{
			cout << "Load factor: " << hashTable.getLoadFactor() << endl;
			cout << "Total collisions: " << hashTable.getTotalCollisions() << endl;
			cout << "Longest collision path: " << hashTable.findLargestPath() << endl;
		}

		if (choice == "8")
		{
			hashTable.saveFile();
			for (int i = 0; i < deleted; i++)
			{
				undo.pop();
			}
			deleted = 0;
		}

		if (choice == "9")	// display the menu again
		{
			menu();
		}

		if (choice == "t")
		{
			bst.printTree(iDisplay);
		}

		if (choice == "a")
		{
			cout << "Team 10: Tagbaley Tanguy Paul Morrell Nioble, Henry Vo, Rimma Esheva, and James Qin." << endl;
		}

		cout << "Enter option of choice, 9 to bring up the menu again, or q to quit." << endl;
		cin >> choice;
	} while (choice != "q" && choice != "Q");

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
		<< "8 - Save to file\n"
		<< "9 - Help\n"
		<< "q - Quit\n";

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
	string isbn;
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
	while (inFile >> isbn)
	{
		inFile >> title >> author >> genre >> quantity;
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
	string k = key.getISBN();
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
	string i;
	int count = 0;
	string input;
	//inFile.open(filename);
	bool fileOpened = false;
	do
	{
		cout << "What is the file name? (q to return)" << endl;
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
		{
			if (input != "q")
				cout << "Error: File could not be opened. Please try again." << endl;
		}
			
	} while (!fileOpened && input != "q");
	if (input == "q")
	{
		return -1;
	}
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
*~*
void reformatOutData(string& t, string& a, string& g)
{
	replace(t.begin(), t.end(), ' ', '_');
	replace(a.begin(), a.end(), ' ', '_');
	replace(g.begin(), g.end(), ' ', '_');
}
*/

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