/**
 * @file CheckIn.hpp
 * @brief declara a classe CheckIn
 */
#ifndef CHECKIN_H
#define CHECKIN_H

#include <string>
#include <vector>
#include <ctime>

class User;
class BookClub;
class Comment;
class Book;

/**
 * @brief Representa um check-in em um clube do livro
 *
 * Armazena os dados do check-in (texto, quem realizou, livro comentado,
 * clube do livro relacionado, página em que o usuário está e data e hora
 * da postagem) e recebe comentários.
 *
 * Posse dos objetos:
 *  - O CheckIn é dono dos seus Comment e os apaga no destrutor.
 *  - O CheckIn NÃO é dono de User, Book nem BookClub.
 *
 * Por causa dessa posse, o CheckIn não pode ser copiado.
 */
class CheckIn {
private:
    std::string text;

    User* user;
    Book* book;
    BookClub* club;

    int page; ///< Página em que o usuário estava ao fazer o check-in.

    std::vector<Comment*> comments;

    std::time_t dataHora;

    /**
     * @brief Define a página do check-in.
     * @throws std::invalid_argument se a página for menor que 1 ou maior
     *         que o total de páginas do livro.
     */
    void setPage(int page);

public:
    /**
     * @brief Cria um check-in.
     * @param user Autor do check-in.
     * @param book Livro lido (normalmente a leitura atual do clube).
     * @param club Clube em que o check-in foi feito.
     * @param page Página em que o usuário está (de 1 até o total do livro).
     * @param text Comentário do check-in (opcional).
     * @throws std::invalid_argument se user, book ou club forem nullptr,
     *         ou se a página for inválida.
     */
    CheckIn(User* user, Book* book, BookClub* club, int page,
            const std::string& text = "");

    /** @brief Destrói o check-in, apagando todos os seus comentários. */
    ~CheckIn();

    CheckIn(const CheckIn&) = delete;
    CheckIn& operator=(const CheckIn&) = delete;

    std::string getText() const;
    int getPage() const;
    User* getUser() const;
    Book* getBook() const;
    BookClub* getClub() const;

    /** @brief Retorna os comentários do check-in (pode ser alterado). */
    std::vector<Comment*>& getComments();
    /** @brief Retorna os comentários do check-in (só leitura). */
    const std::vector<Comment*>& getComments() const;

    /** @brief Retorna a data e hora da postagem no formato "dd/mm/aaaa hh:mm". */
    std::string getDataHora() const;

    /** @brief Apaga (com delete) todos os comentários e esvazia a lista. */
    void eraseAllComments();
};

#endif
