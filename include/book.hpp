/**
 * @file book.hpp
 * @brief declara a classe book
 */
#ifndef BOOK_H
#define BOOK_H

#include <string>

/**
 * @brief Representa um livro que pode ser lido pelos clubes do livro.
 *
 * Armazena os dados do livro (título, autor, gênero, páginas e ISBN)
 * e acompanha a nota geral e quantos usuários e clubes estão
 * realizando a leitura.
 */
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
        /// @brief Cria um livro vazio, com valores padrões.
        Book();

        /**
         * @brief Cria um livro.
         * @param title Título do livro.
         * @param author Autor do livro.
         * @param genre Gênero literário.
         * @param pages Número total de páginas.
         * @param ISBN Código ISBN do livro.
         */
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