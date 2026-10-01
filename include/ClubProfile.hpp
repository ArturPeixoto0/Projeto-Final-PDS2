#ifndef CLUBPROFILE_H
#define CLUBPROFILE_H

#include <string>
#include <vector>

class BookClub;

class ClubProfile {
private:
    BookClub* club;   // club dono do perfil

public:
    ClubProfile(BookClub* club);

    std::string getName()const;
    std::string getDescription()const;
    std::string getPhotoPath()const;
    std::string getHeaderPath()const;

    void showReadingStatus();
    
    void show_name()const;
    void show_pfp()const;
    void show_description()const;
    void show_checkins()const;
    void show_members()const;
    void show_current_reading()const;
    void show_metrics()const;

};

#endif