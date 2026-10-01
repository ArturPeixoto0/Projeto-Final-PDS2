#ifndef BOOK_H
#define BOOK_H

#include <string>

class Book {
    private:
        std::string title;
        std::string author;
        std::string genre;
        int pages;
        std::string ISBN;
        double grade;
        int number_of_readers;
        int number_of_clubs;

    public:
        Book();
        Book(std::string title,
        std::string author,
        std::string genre,
        int pages,
        int ISBN
        ); 

        void setTitle(std::string title);
        std:: string getTitle();

        void setAuthor(std::string author);
        std:: string getAuthor();

        void setGenre(std::string genre);
        std:: string getGenre();

        void setPages(int pages);
        int getPages();

        void setISBN(std::string ISBN);
        std::string getISBN();

        void setGrade(double grade);
        double getGrade();

        void setNumberReaders(int number_of_readers);
        int getNumberReaders();

        void setNumberClubs(int number_of_clubs);
        int getNumberClubs();
        
        ~Book();

};



#endif