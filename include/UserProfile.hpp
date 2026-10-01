#ifndef USERPROFILE_H
#define USERPROFILE_H


#include <string>
#include <vector>

class User;   // forward declaration
class BookClub;

class UserProfile {
private:
    User* user;   // de quem é este perfil

public:
    UserProfile(User* user);

    std::string getName()const;
    std::string getDescription()const;
    std::string getPhotoPath()const;
    std::string getHeaderPath()const;

    int getTotalPages()const;
    int getTotalBooks()const;
    
    std::vector<BookClub*> getPublicClubs();

    void show_name()const;
    void show_pfp()const;
    void show_description()const;
    void show_checkins()const;

};

#endif