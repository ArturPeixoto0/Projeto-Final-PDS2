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
  check in onde foi realizado, texto, data e hora)
 */
class Comment {
    private:
        User* _author;
        BookClub* _club;
        CheckIn* _checkin;
        std::string _text;
        std::time_t _created_at;

    public:
        Comment(std::string text, User* author, CheckIn* checkIn, BookClub* club);

        std::string get_text();
        User* get_author() const;
        CheckIn* get_checkin() const;
        BookClub* get_club() const;
        std::time_t get_createdat() const;

};

#endif