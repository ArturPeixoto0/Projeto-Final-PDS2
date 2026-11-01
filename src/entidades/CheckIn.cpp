#include "CheckIn.hpp"
#include "Comment.hpp"
#include "Book.hpp"
#include <stdexcept>
#include <string>
#include <vector>
#include <ctime>

CheckIn::CheckIn(User* user, Book* book, BookClub* club, int page,
                 const std::string& text)
    : text(text), user(user), book(book), club(club), page(0),
      dataHora(std::time(nullptr)) {
    // Um check-in não faz sentido sem autor, livro e clube.
    if (user == nullptr) {
        throw std::invalid_argument("CheckIn precisa de um usuario");
    }
    if (book == nullptr) {
        throw std::invalid_argument("CheckIn precisa de um livro");
    }
    if (club == nullptr) {
        throw std::invalid_argument("CheckIn precisa de um clube");
    }
    // setPage só pode ser chamado depois de conferir o livro,
    // porque ele usa book->getPages().
    setPage(page);
}

CheckIn::~CheckIn() {
    eraseAllComments();
}

void CheckIn::setPage(int page) {
    if (page < 1 || page > this->book->getPages()) {
        throw std::invalid_argument("Pagina do check-in invalida");
    }
    this->page = page;
}

std::string CheckIn::getText() const {
    return this->text;
}

int CheckIn::getPage() const {
    return this->page;
}

User* CheckIn::getUser() const {
    return this->user;
}

Book* CheckIn::getBook() const {
    return this->book;
}

BookClub* CheckIn::getClub() const {
    return this->club;
}

std::vector<Comment*>& CheckIn::getComments() {
    return this->comments;
}

const std::vector<Comment*>& CheckIn::getComments() const {
    return this->comments;
}

std::string CheckIn::getDataHora() const {
    char buffer[20];
    std::tm* local = std::localtime(&this->dataHora);
    std::strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M", local);
    return std::string(buffer);
}

void CheckIn::eraseAllComments() {
    for (int i = 0; i < (int)this->comments.size(); i++) {
        delete this->comments[i];
    }
    this->comments.clear();
}
