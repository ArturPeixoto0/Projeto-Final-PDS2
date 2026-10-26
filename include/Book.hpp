/**
 * @file Book.hpp
 * @brief declara a classe book
 */
#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <vector>

class BookClub;
class User;

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
    
    public: 

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
        std::string ISBN
        ); 

        void setTitle(const std::string title);
        std::string getTitle() const;

        void setAuthor(const std::string author);
        std::string getAuthor() const;

        void setGenre(const std::string genre);
        std::string getGenre() const;

        void setPages(int pages);
        int getPages() const;

        void setISBN(const std::string ISBN);
        std::string getISBN() const;
        
        ~Book();

};


#endif