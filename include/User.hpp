/**
 * @file User.hpp
 * @brief Declaração da classe User.
 */
#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <map>
#include "UserProfile.hpp"

class BookClub;
class CheckIn;
class Comment;
class Book;
class ReadingMetrics;

/**
 * @brief Representa um usuário do sistema.
 *
 * Armazena os dados da conta (nome, email e senha), os clubes em que
 * participa, o histórico de check ins e as métricas pessoais de leitura.
 * Oferece as ações que o usuário pode realizar: criar e entrar em clubes,
 * fazer check ins, comentar e avaliar leituras.
 */

class User {
    private:

        std::string name; //nome do User
        std::string email; //email da conta do User
        std::string password; //senha da conta do User
        std::vector<BookClub*> currentClubs; //vetor com todos os clubes atuais do User
        std::vector<CheckIn*> allCheckIns; //vetor com todos os check-ins já feitos pelo User
        std::map<BookClub*, ReadingMetrics*> allMetrics;
        int totalPages = 0; //paginômetro total do usuário
        int totalBooks = 0; //livrômetro total do usuário
        UserProfile profile; //perfil do User (inicializar com profile(this) no .cpp)

    public:
        User(const std::string& name,
             const std::string& email,
             const std::string& password);
             
        ~User();

        User(const User&) = delete;
        User& operator = (const User&) = delete;

        //dados da conta:
        std::string getName() const; //retorna o nome do usuário

        void setName(const std::string& name); //altera o nome do usuário

        std::string getEmail() const; // retorna email da conta

        bool checkPassword(const std::string& attempt) const;

        //perfil: 
        UserProfile& getProfile();
        const UserProfile& getProfile() const;

        //clubes:
        const std::vector<BookClub*>& getCurrentClubs() const;
        bool participaDe(const BookClub* club) const; //verifica se participa
    
        BookClub* criarClube(const std::string& clubName, Book* reading);
        
        bool entrarClube(BookClub* club);
        bool sairClube(BookClub* club);

        //check-ins e comentarios:
        const std::vector<CheckIn*>& getAllCheckIns() const;
        int getCurrentPage(const BookClub* club) const; //pagina do ultimo checkin do clube

        CheckIn* realizarCheckIn(BookClub* club, int page, const std::string& text, 
                                bool finished = false); //registra check-in e atualiza paginometro/livrometro
    
        bool apagarCheckIn(CheckIn* checkIn);

        Comment* comentar(CheckIn* checkIn, const std::string& text);
    
        //livros: 
        bool sugerirLivro(BookClub* club, Book* book); 
        bool avaliarLeitura (BookClub* club, double nota);

        //métricas:
        int getTotalPages() const; //paginometro
        int getTotalBooks() const; //livrometro
    
};

#endif