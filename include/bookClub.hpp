#ifndef BOOKCLUB_H
#define BOOKCLUB_H

#include <string>
#include <vector>
#include <chrono>
#include "book.hpp"
#include "user.hpp"
#include "checkIn.hpp"

class BookClub {
    private:
        std:: string name;
        Book CurrentReading;
        std:: vector<User*> CurrentUsers;
        std:: vector<Book*> FutureReading;
        std:: vector<Book*> PastReadings;
        std:: vector<CheckIn*> Feed;
        int ReadPages;
        int ReadBooks;
        bool Reading;
        time_t ClubCreation;

        time_t StartReading;
        time_t EndReading;



    public: 
        BookClub();
        BookClub(
        std:: string name,
        Book CurrentReading
        );

        std:: string getName();
        Book getCurrentReading();
        std:: vector<Book*> getFutureReading();
        std:: vector<Book*> getPastReadings();
        std:: vector<CheckIn*> getFeed();
        int getReadPages();
        int getReadBooks();
        bool getReading();

        double MediaPaginasPorUser();
        double MediaPaginasPorDia();
        double MediaPorcentagemLeitura(double MediaPaginasPorUser, int pages);
        User LiderDeLeitura(std:: vector<User*> CurrentUsers);

        time_t DiasRestantesLeitura();


        ~BookClub();
};


#endif