/**
 * @file UserProfile.hpp
 * @brief Declaração da classe UserProfile.
 */
#ifndef USERPROFILE_H
#define USERPROFILE_H

#include <string>
#include <vector>
#include "Profile.hpp"

class User; 
class BookClub;
class CheckIn;

/**
 * @brief Representa o perfil público de um usúario.
 *
 * Exibe as informações do usúario para outros usuários: nome, descrição,
 * foto, leituras atuais, feed de check ins e métricas de leitura.
 */
class UserProfile : public Profile {
private:
    User* user;   // de quem é este perfil

public:
    UserProfile(User* user);

    int getTotalPages()const;
    int getTotalBooks()const;
    
    std::string getName()const override;
    std::vector<CheckIn> getAllCheckIn()const;
    
    std::vector<BookClub*> getClubs();

    void show_name()const override;
    void show_pfp()const override;
    void show_description()const override;
    void show_checkins()const;
    void show_clubs();

};

#endif