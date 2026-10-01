#ifndef USER_H 
#define USER_H

#include <string>
#include <vector>
#include "UserProfile.hpp"

class BookClub;
class CheckIn;
class Coments;
class Book;

class User {
    private:
        std::string name;
        std::string email;
        std::string password;
        std::vector<BookClub*> CurrentPublicClubs;
        std::vector<BookClub*> CurrentPrivateClubs;
        std::vector<CheckIn*> AllCheckIn;
        int TotalPages;
        int TotalBooks;

        userProfile profile; 
        //RESOLVER O PROBLEMA DAS PÁGINAS ATUAIS DO LIVRO PARA CADA USUÁRIO -> CRIAR SUBCLASSE Reading em Book?

    public:
        User();
        User(const std::string& name,
            const std::string& email,
            const std::string& password);
        ~User();

        std::string getName() const;
        void setName(const std::string& name);
        std::string getEmail() const;
        bool checkPassword(const std::string& attempt) const;


        userProfile& getprofile();

        std::vector<BookClub*> getCurrentClubs() const;
        bool participaDe(const BookClub* club) const;

        std::vector<CheckIn*> getAllCheckIns() const;
        int getCurrentPage(const BookClub* club) const;

        int getTotalPages() const;
        void setTotalPages(int Npages); //paginômetro total do usuário?
        int getTotalBooks() const;
        void setTotalBooks(int quantity); 
        
        BookClub* CriarClube(const std::string& clubName);

        bool EntrarClube(BookClub* Club);
        bool SairClube(BookClub* Club);

        CheckIn* RealizarCheckIn(BookClub* club, int page, const std::string& text, 
                                bool finished = false);

        bool ApagarCheckIn(CheckIn* checkIn);

        Coments* Comentar(CheckIn* chekIn, const std:: string& text);
    

        bool SugerirLivro(BookClub* club, Book* book); //depednde do clube
        void AvaliarLeitura (BookClub* club, Book* book, double nota);
};

#endif