#include "Book.hpp"
#include <string>

Book::Book(std::string title,   
        std::string author,
        std::string genre,
        int pages,
        std::string ISBN) :
            title(title), author(author), genre(genre), pages(pages), ISBN(ISBN) {}

void Book::setTitle(const std::string title){
    this->title = title;
}
std::string Book::getTitle() const {
    return this->title;
}

void Book::setAuthor(const std::string author){
    this->author = author;
}
std::string Book::getAuthor() const{
    return this->author;
}

void Book::setGenre(const std::string genre){
    this->genre = genre;
}
std::string Book::getGenre() const{
    return this->genre;
}

void Book::setPages(int pages){
    this->pages = pages;
}
int Book::getPages() const{
    return this->pages;
}

void Book::setISBN(const std::string ISBN){
    this->ISBN = ISBN;
}
std::string Book::getISBN() const {
    return this->ISBN;
}
        
Book::~Book(){
}
