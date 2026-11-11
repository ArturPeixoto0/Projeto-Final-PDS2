#include "Book.hpp"
#include <string>
#include <stdexcept>

// Um atributo static é declarado no .hpp, mas precisa ser definido (e
// inicializado) uma única vez num .cpp. Os ids começam em 1, porque 0
// significa "gere um id novo" no construtor.
int Book::proximoId = 1;

Book::Book(std::string title,
        std::string author,
        std::string genre,
        int pages,
        std::string ISBN,
        int id) :
            title(title), author(author), genre(genre), pages(pages), ISBN(ISBN), id(0) {
    if (title.empty()) {
        throw std::invalid_argument("Livro precisa de um titulo");
    }
    if (pages <= 0) {
        throw std::invalid_argument("Numero de paginas do livro deve ser maior que zero");
    }
    if (id < 0) {
        throw std::invalid_argument("Id do livro nao pode ser negativo");
    }

    // O id só é definido depois das validações: se o construtor lançar
    // exceção, nenhum id é gasto.
    if (id == 0) {
        this->id = proximoId;
        proximoId++;
    } else {
        this->id = id;
        // Garante que um id gerado depois nunca repita o id informado.
        if (id >= proximoId) {
            proximoId = id + 1;
        }
    }
}

int Book::getId() const {
    return this->id;
}

void Book::setTitle(const std::string title){
    if (title.empty()) {
        throw std::invalid_argument("Livro precisa de um titulo");
    }
    this->title = title;
}
std::string Book::getTitle() const {
    return this->title;
}

void Book::setAuthor(const std::string author){
    this->author = author;
}
std::string Book::getAuthor() const{
    return this->author;
}

void Book::setGenre(const std::string genre){
    this->genre = genre;
}
std::string Book::getGenre() const{
    return this->genre;
}

void Book::setPages(int pages){
    if (pages <= 0) {
        throw std::invalid_argument("Numero de paginas do livro deve ser maior que zero");
    }
    this->pages = pages;
}
int Book::getPages() const{
    return this->pages;
}

void Book::setISBN(const std::string ISBN){
    this->ISBN = ISBN;
}
std::string Book::getISBN() const {
    return this->ISBN;
}
        
Book::~Book(){
}