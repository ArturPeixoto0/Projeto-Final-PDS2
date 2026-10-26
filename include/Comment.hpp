/**
 * @file Comment.hpp
 * @brief Declaração da classe Comment.
 */
#ifndef COMMENT_H
#define COMMENT_H

#include <ctime>
#include <string>

class User;
class BookClub;
class CheckIn;
/**
 * @brief Representa o comentário feito em check in.
 *
 * Armazena os dados do comentário (autor do comentário, clube do livro,
 * check in onde foi realizado, texto, data e hora)
 *
 * O comentário pertence ao CheckIn, que o apaga no destrutor
 * (ver CheckIn::eraseAllComments).
 */
class Comment {
    private:
        User* _author;
        BookClub* _club;
        CheckIn* _checkin;
        std::string _text;
        std::time_t _created_at;

    public:
        /** @brief Limite de caracteres do comentário (US25: menor que um tweet). */
        static const int MAX_LENGTH = 200;

        /**
         * @brief Cria um comentário.
         * @throws std::invalid_argument se o texto for vazio ou maior que
         *         MAX_LENGTH, ou se author, checkIn ou club forem nullptr.
         */
        Comment(std::string text, User* author, CheckIn* checkIn, BookClub* club);

        std::string get_text() const;
        User* get_author() const;
        CheckIn* get_checkin() const;
        BookClub* get_club() const;
        std::time_t get_createdat() const;

};

#endif
