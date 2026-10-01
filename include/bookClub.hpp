/**
 * @file bookClub.hpp
 * @brief declara a classe bookClub
 */
#ifndef BOOKCLUB_H
#define BOOKCLUB_H

#include <string>
#include <vector>
#include <ctime>

class User;
class CheckIn;
class Book;
class Comment;

/**
 * @brief Representa um clube do livro onde usúarios acompanham uma leitura juntos
 *
 * Armazena os dados do clube (nome, administradores, membros,
 * leituras passadas e futuras, feed de check ins e data de criação),
 * as métricas de leitura (páginas lidas, livros lidos e tempo de leitura),
 * indica se o clube é privado ou público
 * e regula as ações de administrador do clube.
 */
class BookClub {
    private:
        std::string name;
        Book* CurrentReading;
        std::vector<User*> Admins;
        std::vector<User*> CurrentUsers;
        std::vector<Book*> FutureReading;
        std::vector<Book*> PastReadings;
        std::vector<CheckIn*> Feed;
        int ReadPages;
        int ReadBooks;
        bool Reading;
        bool isPrivate;
        std::time_t ClubCreation;

        std::time_t StartReading;
        std::time_t EndReading;



    public: 
        /**
         * @brief Cria umm clube do livro.
         * @param fouder Usuário cria o clube e é adicionado automaticamente
           como administrador.
         * @param name Nome do clube.
         * @param CurrentReading Livro a ser lido. Inicialmente pode ser null caso
           o ponteiro ainda não esteja definido.
         */
        BookClub(
        User* founder, //Ele vai estar na lista de admins desde o início
        std::string name,
        Book* CurrentReading
        );

        std::string getName() const;
        Book* getCurrentReading()  const;
        std::vector<Book*> getFutureReading() const;
        std::vector<Book*> getPastReadings() const;
        std::vector<CheckIn*> getFeed() const;
        int getReadPages() const;
        int getReadBooks() const;
        bool getReading() const;

        double MediaPaginasPorUser();
        double MediaPaginasPorDia();
        double MediaPorcentagemLeitura(double MediaPaginasPorUser, int pages);
        User* LiderDeLeitura();

        int DiasRestantesLeitura();

        //MÉTODOS DO ADMINISTRADOR

        bool isAdmin(User* user) const;

        bool criarConvite(User* actor, User* convidado);
        bool removerMembro(User* actor, User* alvo);
        bool decidirProximaLeitura(User* actor, Book* livro);
        bool alterarPrivacidade(User* actor, bool isPrivate);
        bool apagarCheckIn(User* actor, CheckIn* checkIn);
        bool definirTempoLeitura(User* actor, int dias);
        bool apagarComentario(User* actor, Comment* comentario);
        bool promoverAdmin(User* actor, User* alvo);


        ~BookClub();
};


#endif