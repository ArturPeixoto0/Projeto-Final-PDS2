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
 * @brief Representa o perfil público de um usuário.
 *
 * Exibe as informações do usuário para outros usuários: nome, descrição,
 * foto, leituras atuais, feed de check ins e métricas de leitura.
 *
 * O perfil não copia os dados do User: ele consulta o User dono dele toda
 * vez, então qualquer mudança no User aparece na hora no perfil (US27).
 */
class UserProfile : public Profile {
private:
    User* user; ///< Usuário dono do perfil (o perfil não é dono do User).

public:
    /**
     * @brief Cria o perfil de um usuário.
     * @warning É chamado dentro do construtor do User (profile(this)), quando
     *          o User ainda não terminou de ser construído. Por isso o
     *          construtor do perfil só guarda o ponteiro e não usa o User.
     */
    UserProfile(User* user);

    /** @brief Retorna o paginômetro do usuário (total de páginas lidas). */
    int getTotalPages()const;
    /** @brief Retorna o livrômetro do usuário (total de livros terminados). */
    int getTotalBooks()const;

    /** @brief Retorna o nome do usuário (sobrescreve Profile::getName). */
    std::string getName()const override;
    /** @brief Todos os check-ins do usuário, de todos os clubes (US23). */
    std::vector<CheckIn*> getAllCheckIn()const;

    /** @brief Retorna os clubes de que o usuário participa. */
    std::vector<BookClub*> getClubs()const;

    /** @brief Mostra o nome do usuário no terminal. */
    void show_name()const override;
    /** @brief Mostra o caminho da foto de perfil, ou avisa que não tem foto. */
    void show_pfp()const override;
    /** @brief Mostra a biografia, ou avisa que não tem descrição. */
    void show_description()const override;
    /** @brief Mostra os check-ins do mais novo para o mais antigo (US23). */
    void show_checkins()const;
    /** @brief Mostra a lista de clubes de que o usuário participa. */
    void show_clubs()const;

};

#endif
