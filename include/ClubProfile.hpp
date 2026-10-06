/**
 * @file ClubProfile.hpp
 * @brief Declaração da classe ClubProfile.
 */
#ifndef CLUBPROFILE_H
#define CLUBPROFILE_H

#include <string>
#include <vector>
#include "Profile.hpp"

class User;
class BookClub;
class CheckIn;

/**
 * @brief Representa o perfil público de um clube do livro.
 *
 * Exibe as informações do clube para outros usuários: nome, descrição,
 * foto, membros, leitura atual, feed de check ins e métricas de leitura.
 */
class ClubProfile: public Profile {
private:
    BookClub* club;   // club dono do perfil

public:
    ClubProfile(BookClub* club);

    std::string getName()const override;
    std::vector<CheckIn> getAllCheckIn()const;

    std::vector<User*> getUsers();

    void showReadingStatus();
    
    void show_name()const override;
    void show_pfp()const override;
    void show_description()const override;
    void show_checkins()const;
    void show_users()const;
    void show_current_reading()const;
    void show_metrics()const;

};

#endif