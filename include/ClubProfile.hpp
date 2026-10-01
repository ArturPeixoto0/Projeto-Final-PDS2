/**
 * @file ClubProfile.hpp
 * @brief Declaração da classe ClubProfile.
 */
#ifndef CLUBPROFILE_H
#define CLUBPROFILE_H

#include <string>
#include <vector>

class BookClub;

/**
 * @brief Representa o perfil público de um clube do livro.
 *
 * Exibe as informações do clube para outros usuários: nome, descrição,
 * foto, membros, leitura atual, feed de check ins e métricas de leitura.
 */
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