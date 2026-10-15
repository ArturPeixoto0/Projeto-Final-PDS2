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
        double grade_sum;      
        int grade_count;      
    
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

         /**
         * @brief Registra uma nova avaliação do livro.
         * @param grade Nota dada por um usuário, de 0 a 5.
         */
        void addGrade(double grade);
           /**
         * @brief Retorna a nota geral do livro.
         * @return Média das avaliações recebidas, ou 0 se não houver nenhuma.
         */
        double getGrade() const;

        /// @brief Soma um usuário à contagem de leitores do livro.
        void incrementReaders();
        /// @brief Subtrai um usuário à contagem de leitores do livro.
        void decrementReaders();
        int getNumberReaders() const;

        /// @brief Soma um clube à contagem de clubes lendo o livro.
        void incrementClubs();
        /// @brief Subtrai um clube à contagem de clubes lendo o livro.
        void decrementClubs();
        int getNumberClubs() const;
        
        ~Book();

};



#endif