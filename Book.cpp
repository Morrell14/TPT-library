// Implementation file for the Book class
// Written by: James Qin
// IDE: MS Visual Studio

#include <iostream>
#include <iomanip>
#include <string>
#include "Book.h"

using namespace std;

//**************************************************
// Constructor
//**************************************************
Book::Book()
{
	isbn = -1;
	title = "";
	author = "";
	genre = "";
	quantity = 1;
}

//**************************************************
// Overloaded constructor
//**************************************************
Book::Book(int i, string t, string a, string g, int q)
{
	isbn = i;
	title = t;
	author = a;
	genre = g;
	quantity = q;
}

//**************************************************
// Overloaded stream operator
//**************************************************
ostream& operator << (ostream& os, const Book& b)
{
	os << "ISBN: " << b.getISBN() << endl;
	os << "Title: " << b.getTitle() << endl;
	os << "Author: " << b.getAuthor() << endl;
	os << "Genre: " << b.getGenre() << endl;
	os << "Quantity available: " << b.getQuantity() << endl;
	return os;
}
