// Specification file for the Book class
// Written by: James Qin
// IDE: MS Visual Studio

#ifndef BOOK_H
#define BOOK_H

#include <string>

using std::string;

class Book
{
private:

    string isbn;
    string title;
    string author;
    string genre;
    int quantity;

public:
    // constructors
    Book();
    Book(int, string, string, string, int);

    // setters
    void setISBN(string i) { isbn = i; }
    void setTitle(string t) { title = t; }
    void setAuthor(string a) { author = a; }
    void setGenre(string g) { genre = g; }
    void setQuantity(int q) { quantity = q; }

    // getters
    string getISBN() const { return isbn; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getGenre() const { return genre; }
    int getQuantity() const { return quantity; }

    // overloaded operators
    bool operator > (const Book& b) { return title > b.getTitle(); };
    bool operator < (const Book& b) { return title < b.getTitle(); };
    bool operator == (const Book& b) { return title == b.getTitle(); };
    friend std::ostream& operator << (std::ostream& os, const Book& b);

    // other functions (may or may not be used)

};

#endif