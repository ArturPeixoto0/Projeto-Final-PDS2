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
 *
 * O perfil não copia os dados do User: ele consulta o User dono dele toda
 * vez, então qualquer mudança no User aparece na hora no perfil (US27).
 */
class UserProfile : public Profile {
private:
    User* user;   // de quem é este perfil

public:
    /**
     * @brief Cria o perfil de um usuário.
     * @warning É chamado dentro do construtor do User (profile(this)), quando
     *          o User ainda não terminou de ser construído. Por isso o
     *          construtor do perfil só guarda o ponteiro e não usa o User.
     */
    UserProfile(User* user);

    int getTotalPages()const;
    int getTotalBooks()const;

    std::string getName()const override;
    /** @brief Todos os check-ins do usuário, de todos os clubes (US23). */
    std::vector<CheckIn*> getAllCheckIn()const;

    std::vector<BookClub*> getClubs()const;

    void show_name()const override;
    void show_pfp()const override;
    void show_description()const override;
    /** @brief Mostra os check-ins do mais novo para o mais antigo (US23). */
    void show_checkins()const;
    void show_clubs()const;

};

#endif
