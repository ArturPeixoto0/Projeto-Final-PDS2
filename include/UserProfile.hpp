#ifndef PROFILE_H
#define PROFILE_H


#include <string>
#include <vector>
#include "bookClub.hpp"

class User;   // forward declaration

class Profile {
private:
    User* user;   // de quem é este perfil

public:
    Profile(User* user);

    std::string getName();
    std::string getDescription();
    std::string getPhotoPath();
    std::string getHeaderPath();

    int getTotalPages();
    int getTotalBooks();
    
    vector<BookClub*> getPublicClubs();

    void show_name();
    void show_pfp();
    void show_description();
    void show_checkins();

};