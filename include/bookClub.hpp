#ifndef BOOKCLUB_H
#define BOOKCLUB_H

#include <string>
#include <vector>
#include <ctime>

class User;
class CheckIn;
class Book;
class Coments;

class BookClub {
    private:
        std:: string name;
        Book* CurrentReading;
        std:: vector<User*> Admins;
        std:: vector<User*> CurrentUsers;
        std:: vector<Book*> FutureReading;
        std:: vector<Book*> PastReadings;
        std:: vector<CheckIn*> Feed;
        int ReadPages;
        int ReadBooks;
        bool Reading;
        bool isPrivate;
        std::time_t ClubCreation;

        std::time_t StartReading;
        std::time_t EndReading;



    public: 
        BookClub(
        User* founder, //Ele vai estar na lista de admins desde o início
        std:: string name,
        Book* CurrentReading
        );

        std:: string getName();
        Book* getCurrentReading();
        std:: vector<Book*> getFutureReading();
        std:: vector<Book*> getPastReadings();
        std:: vector<CheckIn*> getFeed();
        int getReadPages();
        int getReadBooks();
        bool getReading();

        double MediaPaginasPorUser();
        double MediaPaginasPorDia();
        double MediaPorcentagemLeitura(double MediaPaginasPorUser, int pages);
        User* LiderDeLeitura();

        int DiasRestantesLeitura();

        //MÉTODOS DO ADMINISTRADOR

        bool isAdmin(User* user);

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