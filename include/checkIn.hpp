
#ifndef CHECKIN_H
#define CHECKIN_H

#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <set>

using namespace std;

class User;
//class AdminClub;
class BookClub;
class Coments;
class Book;

class CheckIn {
private:
    string text;

    User* user;
    Book* book;
    BookClub* club;

    vector<Coments*> coments;

    void setPages (int pages); 

    chrono::system_clock::time_point dataHora;

public:

    CheckIn();
    ~CheckIn();

    string getText();
    string getComents();
    string getPages();
    string getDataHora();

};

#endif