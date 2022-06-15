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
	ID = "";
	title = "";
	author = "";
	genre = "";
	quantity = 1;
}

//**************************************************
// Overloaded constructor
//**************************************************
Book::Book(string id, string t, string a, string g, int q)
{
	ID = id;
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
	os << "ID: " << b.getID() << endl;
	os << "Title: " << b.getTitle() << endl;
	os << "Author: " << b.getAuthor() << endl;
	os << "Genre: " << b.getGenre() << endl;
	os << "Quantity available: " << b.getQuantity() << endl;
	return os;
}

//**************************************************
// checkOut function - subtracts a specified quantity
// from the given book
//**************************************************
void Book::checkOut(Book b, int q)
{
	if (q > b.getQuantity())
	{
		cout << "Error: Specified quantity is too high. Maximum is " << b.getQuantity() << endl;
		return;
	}

	b.setQuantity(b.getQuantity() - q);
}

//**************************************************
// returnBook function - adds a specified quantity
// to the given book
//**************************************************
void Book::returnBook(Book b, int q)
{
	if (q <= 0)
	{
		cout << "Error: Invalid quantity. Must be greater than 0." << endl;
		return;
	}
	b.setQuantity(b.getQuantity() + q);
}
