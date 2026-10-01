#ifndef COMENTS_H
#define COMENTS_H

#include <ctime>
#include <string>

class User;
class BookClub;
class CheckIn;
class Comment {
    private:
        User* _author;
        BookClub* _club;
        CheckIn* _checkin;
        std::string _text;
        std::time_t _created_at;

    public:
        Comment(std::string text, User* author, CheckIn* checkIn, BookClub* club);

        std::string get_text();
        User* get_author() const;
        CheckIn* get_checkin() const;
        BookClub* get_club() const;
        std::time_t get_createdat() const;

};

#endif