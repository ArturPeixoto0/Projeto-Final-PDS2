#include "Comment.hpp"
#include <stdexcept>
#include <string>
#include <ctime>

// Definição da constante declarada no .hpp. Sem esta linha, alguns usos de
// MAX_LENGTH (por exemplo, passá-lo por referência) dão erro de link.
const int Comment::MAX_LENGTH;

// Definição do contador static declarado no .hpp. Os ids começam em 1,
// porque 0 significa "gere um id novo" no construtor.
int Comment::_proximo_id = 1;

Comment::Comment(std::string text, User* author, CheckIn* checkIn, BookClub* club,
                 int id)
    : _author(author), _club(club), _checkin(checkIn), _text(text),
      _created_at(std::time(nullptr)), _id(0) {
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
    if (id < 0) {
        throw std::invalid_argument("Id do comentario nao pode ser negativo");
    }

    // O id só é definido depois das validações: se o construtor lançar
    // exceção, nenhum id é gasto.
    if (id == 0) {
        this->_id = _proximo_id;
        _proximo_id++;
    } else {
        this->_id = id;
        // Garante que um id gerado depois nunca repita o id informado.
        if (id >= _proximo_id) {
            _proximo_id = id + 1;
        }
    }
}

int Comment::get_id() const {
    return this->_id;
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

void Comment::restaurarData(std::time_t data) {
    this->_created_at = data;
}
