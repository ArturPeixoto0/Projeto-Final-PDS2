/**
 * @file Checkin.hpp
 * @brief declara a classe Checkin
 */
#ifndef CHECKIN_H
#define CHECKIN_H

#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <set>

class User;
//class AdminClub;
class BookClub;
class Coments;
class Book;

/**
 * @brief Representa um check-in em um clube do livro
 *
 * Armazena os dados do check-in (texto, quem realizou, livro comentado,
 * clube do livro relacionado e data e hora da postagem)
 * recebe comentários
 * e atualiaza a porcetagem lida (atualizando o número de paginas).
 */
class CheckIn {
private:
    std::string text;

    User* user;
    Book* book;
    BookClub* club;

    std::vector<Coments*> coments;

    void setPages (int pages); 

    chrono::system_clock::time_point dataHora;

public:

    CheckIn();
    ~CheckIn();

    std::string getText();
    std::string getComents();
    int getPages();
    std::string getDataHora();

};

#endif