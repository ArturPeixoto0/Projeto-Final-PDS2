/**
 * @file Book.hpp
 * @brief Declaração da classe Book.
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
        std::string title;  ///< Título do livro (nunca vazio).
        std::string author; ///< Autor do livro.
        std::string genre;  ///< Gênero literário.
        int pages;          ///< Número total de páginas (sempre maior que zero).
        std::string ISBN;   ///< Código ISBN do livro.
        int id; ///< Identificador único do livro (usado nos arquivos de dados).

        /// Próximo id a ser gerado. É static: um só contador para todos os livros.
        static int proximoId;
    
    public: 

        /**
         * @brief Cria um livro.
         * @param title Título do livro.
         * @param author Autor do livro.
         * @param genre Gênero literário.
         * @param pages Número total de páginas.
         * @param ISBN Código ISBN do livro.
         * @param id Identificador do livro. Se for 0 (padrão), um id novo é
         *        gerado. Se for maior que 0, esse id é usado (ao carregar dos
         *        arquivos) e os próximos ids gerados serão maiores que ele.
         * @throws std::invalid_argument se o título for vazio, se pages
         *         for menor ou igual a zero ou se id for negativo.
         */
        Book(std::string title,
        std::string author,
        std::string genre,
        int pages,
        std::string ISBN,
        int id = 0
        );

        /** @brief Retorna o identificador único do livro. */
        int getId() const;

        /** @throws std::invalid_argument se o título for vazio. */
        /**
         * @brief Altera o título do livro.
         * @param title Novo título.
         * @throws std::invalid_argument se o título for vazio.
         */
        void setTitle(const std::string title);
        /** @brief Retorna o título do livro. */
        std::string getTitle() const;

        /**
         * @brief Altera o autor do livro.
         * @param author Novo autor.
         */
        void setAuthor(const std::string author);
        /** @brief Retorna o autor do livro. */
        std::string getAuthor() const;

        /**
         * @brief Altera o gênero literário do livro.
         * @param genre Novo gênero.
         */
        void setGenre(const std::string genre);
        /** @brief Retorna o gênero literário do livro. */
        std::string getGenre() const;

        /**
         * @brief Altera o número total de páginas do livro.
         * @param pages Novo número de páginas.
         * @throws std::invalid_argument se pages for menor ou igual a zero.
         */
        void setPages(int pages);
        /** @brief Retorna o número total de páginas do livro. */
        int getPages() const;

        /**
         * @brief Altera o código ISBN do livro.
         * @param ISBN Novo código ISBN.
         */
        void setISBN(const std::string ISBN);
        /** @brief Retorna o código ISBN do livro. */
        std::string getISBN() const;

        /** @brief Destrutor. O livro não é dono de nenhum outro objeto. */
        ~Book();

};


#endif
