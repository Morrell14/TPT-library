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
void hDisplay(Book&);
void vDisplay(Book&);
void iDisplay(Book&, int);
void menu();

int main()
{
	BinarySearchTree bst;
	string choice;
	Stack<Book> undo;
	int deleted = 0;

	cout << "\n~~~~~~~~~~~~~~ Welcome To TPT Library ~~~~~~~~~~~~~~\n";
	cout << "\nPlease provide a file to begin: \n";
	string filename = "";
	int lines = getLines(filename);
	if (lines != -1)
	{
		int hashSize = nextPrime(2 * lines);
		HashTable<Book> hashTable(hashSize);
		readInputFile(hashTable, filename, hashSize, bst);

		menu(); // display the menu
		do
		{
			if (choice == "1") // add data from file
			{
				int lines = getLines(filename);
				if (lines != -1)
				{
					readInputFile(hashTable, filename, hashSize, bst);
				}
			}

			if (choice == "2") // display Books database
			{
				cout << "\n~~~~~~~~~~~~~~~~~~~~~~~ All Books ~~~~~~~~~~~~~~~~~~~~~~~\n";
				bst.inOrder(hDisplay);
			}

			if (choice == "3")
			{
				string isbn;
				string title;
				string author;
				string genre;
				int quantity;
				cout << "\n~~~~~~~~~~~~~~~~~~~~~~~ Add New Book ~~~~~~~~~~~~~~~~~~~~~~~\n";
				if (hashTable.getLoadFactor() >= 75)		// Create a new, bigger hash table if load factor >= 75%
				{
					hashTable = *hashTable.rehash(hashTable.getSize(), nextPrime, key_to_index);
					//cout << "rehash called" << endl;
				}
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
				Book temp;
				if (hashTable.search(temp, item, key_to_index) != -1)
				{
					cout << "Insert failed. Duplicate item!" << endl;
				}
				else
				{
					hashTable.insert(item, key_to_index);
					bst.insert(item);
					cout << "Book " << item.getTitle() << " inserted." << endl;
				}

			}

			if (choice == "4")
			{
				string isbn;
				cout << "\n~~~~~~~~~~~~~~~~~~~~~~~ Search Book By ISBN ~~~~~~~~~~~~~~~~~~~~~~~\n";
				cout << "Enter ISBN number of book to search: ";
				cin >> isbn;
				Book temp(isbn, "", "", "", 0);
				Book result;
				if (hashTable.search(result, temp, key_to_index) != -1)
				{
					cout << "\nBook found!" << endl;
					cout << result << endl;
				}
				else
				{
					cout << "\nNo such book was found." << endl;
				}
			}

			if (choice == "5")
			{
				string isbn;
				cout << "\n~~~~~~~~~~~~~~~~~~~~~~~ Delete ~~~~~~~~~~~~~~~~~~~~~~~\n";
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
				cout << "\n~~~~~~~~~~~~~~~~~~~~~~~ Undo Delete ~~~~~~~~~~~~~~~~~~~~~~~\n";
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
				cout << "\n~~~~~~~~~~~~~~~~~~~~~~~ Statistics ~~~~~~~~~~~~~~~~~~~~~~~\n";
				cout << "Load factor: " << hashTable.getLoadFactor() << endl;
				cout << "Total collisions: " << hashTable.getTotalCollisions() << endl;
				cout << "Longest collision path: " << hashTable.findLargestPath() << endl;
			}

			if (choice == "8")
			{
				cout << "\n~~~~~~~~~~~~~~~~~~~~~~~ Saving The File ~~~~~~~~~~~~~~~~~~~~~~~\n";
				hashTable.saveFile();
				for (int i = 0; i < deleted; i++)
				{
					undo.pop();
				}
				deleted = 0;
				hashTable.setTotalCollisions(0);
				hashTable.setMostCollisions(0);
			}

			if (choice == "9")	// display the menu again
			{
				menu();
			}

			if (choice == "t" || choice == "T") // print indented tree
			{
				cout << "\n~~~~~~~~~~~~~~~~~~~~~~~ Indented Tree ~~~~~~~~~~~~~~~~~~~~~~~\n";
				bst.printTree(iDisplay);
			}

			if (choice == "a" || choice == "A")
			{
				cout << "\n~~~~~~~~~~~~~~~~~~~~~~~ Credits ~~~~~~~~~~~~~~~~~~~~~~~\n";
				cout << "This program was created by: \n";
				cout << "James Qin      - Team Leader, Unit 5: File Input/Output\n";
				cout << "Morrell Nioble - Unit 2: BST Algorithms\n";
				cout << "Henry Vo       - Unit 3: Hash List Algorithms\n";
				cout << "Rimma Esheva   - Unit 4: Screen Output\n";
				cout << "\nThank you for using our program!\n";
			}

			cout << "\nEnter option of choice, 9 to bring up the menu again, or q to quit:" << endl;
			cin >> choice;
		} while (choice != "q" && choice != "Q");

		return 0;
	}
}

/*~*~*~*
  Program Menu.
  Allows user to interact with the program.
  Written by Rimma Esheva
*~**/
// Not a final version
void menu() {
	// Menu options
	cout << "\n~~~~~~~~~~~~~~~~~~~~~~~ Menu ~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << "Please choose one of the options or Q to exit: \n";
	cout << "1 - Add Data From A New File\n"
		<< "2 - Show All Books\n"
		<< "3 - Add New Book To Database\n"
		<< "4 - Search Book By ISBN\n"
		<< "5 - Delete Book From Database\n"
		<< "6 - Undo Delete\n"
		<< "7 - Show Statistics\n"
		<< "8 - Save File\n"
		<< "9 - Print Menu\n";

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
	int preview = 0;

	inFile.open(filename);
	//cout << "hashSize: " << hashSize << endl;
	if (inFile)
	{
		cout << "Preview of books inserted: ";
		while (inFile >> isbn)
		{
			inFile >> title >> author >> genre >> quantity;
			reformatData(title, author, genre);
			Book item(isbn, title, author, genre, quantity);
			Book temp;
			if (hashAry.getLoadFactor() >= 75)		// Create a new, bigger hash table if load factor >= 75%
			{
				hashAry = *hashAry.rehash(hashAry.getSize(), nextPrime, key_to_index);
				//cout << "rehash called" << endl;
			}
			if (hashAry.search(temp, item, key_to_index) == -1)
			{
				hashAry.insert(item, key_to_index);
				hashAry.insert(item, key_to_index);
				bst.insert(item);
				//cout << "inserted: " << item.getTitle() << endl;
				if (preview < 5)	// Shows title of first 5 books
				{
					cout << item.getTitle() << ", ";
					preview++;
				}
			}

		}
		inFile.close();
		cout << "..." << endl;
	}
	else
	{
		cout << "Unable to open file " << filename << endl;
	}

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
	cout << " " << setw(20) << item.getISBN() << " ";
	cout << " " << setw(40) << item.getTitle() << "  ";
	cout << " " << setw(40) << item.getAuthor() << "  ";
	cout << " " << setw(40) << item.getGenre() << "  ";
	cout << right;
	cout << " " << item.getQuantity() << " ";
	cout << left << endl;
}

/*
 Vertical display: one item per line
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
 Indented tree display: one item per line, including the level number
*/
void iDisplay(Book& item, int level)
{
	for (int i = 1; i < level; i++)
		cout << "..";
	cout << level << "). " << item.getTitle() << endl;
}