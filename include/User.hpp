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
        std::string name; //nome do User
        std::string email; //email da conta do User
        std::string password; //senha da conta do User
        std::vector<BookClub*> CurrentClubs; //vetor com todos os clubes atuais do User
        std::vector<CheckIn*> AllCheckIn; //vetor qcom todos os check-ins já feitos pelo User
        int TotalPages; //paginômetro total do usuário
        int TotalBooks; //livrômetro total do usuário
        UserProfile profile; //perfil do User

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
        void setTotalPages(int Npages); 
        int getTotalBooks() const;
        void setTotalBooks(int quantity); 
        
        BookClub* CriarClube(const std::string& clubName);

        bool EntrarClube(BookClub* Club);
        bool SairClube(BookClub* Club);

        CheckIn* RealizarCheckIn(BookClub* club, int page, const std::string& text, 
                                bool finished = false);

        bool ApagarSelfCheckIn(CheckIn* checkIn);

        Coments* Comentar(CheckIn* chekIn, const std:: string& text);
    

        bool SugerirLivro(BookClub* club, Book* book); 
        void AvaliarLeitura (BookClub* club, Book* book, double nota);
};

#endif