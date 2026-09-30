#ifndef USER_H 
#define USER_H

#include <string>
#include <vector>
#include "bookClub.hpp"
#include "CheckIn.hpp"
#include "Coments.hpp"

class User {
    private:
        std::string name;
        std::string email;
        std::string password;
        std::vector<BookClub*> CurrentClubs;
        std::vector<CheckIn*> AllCheckIn;
        int TotalPages;
        int TotalBooks;

        //RESOLVER O PROBLEMA DAS PÁGINAS ATUAIS DO LIVRO PARA CADA USUÁRIO -> CRIAR SUBCLASSE Reading em Book?

    public:
        User();
        User(
        std::string name,
        std::string email,
        std::string password
        );

        std::string getName();
        void setName(std::string Name);
        std::string getEmail();
        std::string getPassword();
        std::vector<BookClub*> getCurrentClubs();

        std::vector<CheckIn*> AllCheckIn();

        int getTotalPages();
        void setTotalPages(int Npages);
        int getTotalBooks();
        void setTotalBooks(int quantity);
        
        BookClub CriarClube();
        std::vector<BookClub*> EntrarClube(BookClub Club);
        std::vector<BookClub*> SairClube(BookClub Club);

        CheckIn RealizarCheckIn(int pages, BookClub& Club);
        Coments Comentar(std:: string text);
        std::vector<Book*> AdicionarLista(std::vector<Book*> Lista, Book AddBook);

        void AvaliarLeitura (Book Book);
};

#endif