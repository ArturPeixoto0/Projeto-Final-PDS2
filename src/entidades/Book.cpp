#include "Book.hpp"
#include "BookClub.hpp"
#include <string>
#include <algorithm>


Book::Book() {
}

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

void Book::addGrade(double grade){

} //PENSAR COMO FAZER

double Book::getGrade() const {
    return this->grade;
}


void Book::incrementReaders(BookClub* club){
    this->currentReaders + club->getNumberMembers();
}
void Book::decrementReaders(BookClub* club){
    this->currentReaders - club->getNumberMembers();
}
int Book::getNumberReaders() const {
    return this->currentReaders;
}

void Book::addClub(BookClub* club){
    this->currentClubs.push_back(club);
    this->incrementReaders(club);
}
void Book::removeClub(BookClub* club){
    auto it = std::find(this->currentClubs.begin(), currentClubs.end(), club);
    this->decrementReaders(club);
    this->currentClubs.erase(it);
}
int Book::getNumberClubs() const {
    return this->currentClubs.size();
}
        
Book::~Book(){
}
