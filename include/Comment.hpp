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
        int _id; ///< Identificador único do comentário (usado nos arquivos de dados).

        static int _proximo_id; ///< Próximo id a ser gerado (um só contador para todos os comentários).

    public:
        /** @brief Limite de caracteres do comentário (US25: menor que um tweet). */
        static const int MAX_LENGTH = 200;

        /**
         * @brief Cria um comentário.
         * @param text Texto do comentário (1 a MAX_LENGTH caracteres).
         * @param author Autor do comentário.
         * @param checkIn Check-in comentado.
         * @param club Clube do check-in.
         * @param id Identificador do comentário. Se for 0 (padrão), um id novo
         *        é gerado. Se for maior que 0, esse id é usado (ao carregar dos
         *        arquivos) e os próximos ids gerados serão maiores que ele.
         * @throws std::invalid_argument se o texto for vazio ou maior que
         *         MAX_LENGTH, se author, checkIn ou club forem nullptr, ou se
         *         id for negativo.
         */
        Comment(std::string text, User* author, CheckIn* checkIn, BookClub* club,
                int id = 0);

        /** @brief Retorna o identificador único do comentário. */
        int get_id() const;

        std::string get_text() const;
        User* get_author() const;
        CheckIn* get_checkin() const;
        BookClub* get_club() const;
        std::time_t get_createdat() const;

        /**
         * @brief Coloca de volta a data salva nos arquivos. Usado só pelo
         *        BancoDeDados: o construtor usa a hora atual, e na carga
         *        precisamos da data original.
         */
        void restaurarData(std::time_t data);

};

#endif
