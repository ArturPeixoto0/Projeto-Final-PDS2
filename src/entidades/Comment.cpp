#include "Comment.hpp"
#include <stdexcept>
#include <string>
#include <ctime>

// Definição da constante declarada no .hpp. Sem esta linha, alguns usos de
// MAX_LENGTH (por exemplo, passá-lo por referência) dão erro de link.
const int Comment::MAX_LENGTH;

Comment::Comment(std::string text, User* author, CheckIn* checkIn, BookClub* club)
    : _author(author), _club(club), _checkin(checkIn), _text(text),
      _created_at(std::time(nullptr)) {
    if (text.empty()) {
        throw std::invalid_argument("Comentario nao pode ser vazio");
    }
    if ((int)text.size() > MAX_LENGTH) {
        throw std::invalid_argument("Comentario passou do limite de caracteres");
    }
    if (author == nullptr) {
        throw std::invalid_argument("Comentario precisa de um autor");
    }
    if (checkIn == nullptr) {
        throw std::invalid_argument("Comentario precisa de um check-in");
    }
    if (club == nullptr) {
        throw std::invalid_argument("Comentario precisa de um clube");
    }
}

std::string Comment::get_text() const {
    return this->_text;
}

User* Comment::get_author() const {
    return this->_author;
}

CheckIn* Comment::get_checkin() const {
    return this->_checkin;
}

BookClub* Comment::get_club() const {
    return this->_club;
}

std::time_t Comment::get_createdat() const {
    return this->_created_at;
}
