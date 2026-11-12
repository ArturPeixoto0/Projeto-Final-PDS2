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
 *
 * Assim como o UserProfile, não copia dados: consulta o BookClub dono dele.
 */
class ClubProfile: public Profile {
private:
    BookClub* club; ///< Clube dono do perfil (o perfil não é dono do clube).

public:
    /**
     * @brief Cria o perfil de um clube.
     * @param club Clube dono do perfil.
     * @throws std::invalid_argument se club for nulo.
     */
    ClubProfile(BookClub* club);

    /** @brief Retorna o clube dono deste perfil. */
    BookClub* getClub()const;

    /** @brief Retorna o nome do clube (sobrescreve Profile::getName). */
    std::string getName()const override;
    /** @brief Check-ins do feed do clube. */
    std::vector<CheckIn*> getAllCheckIn()const;

    /** @brief Retorna os membros do clube. */
    std::vector<User*> getUsers()const;

    /** @brief Mostra o ranking de progresso no livro atual, destacando quem terminou (US20/US29). */
    void showReadingStatus()const;

    /** @brief Mostra o nome do clube no terminal. */
    void show_name()const override;
    /** @brief Mostra o caminho da foto do clube, ou avisa que não tem foto. */
    void show_pfp()const override;
    /** @brief Mostra a descrição do clube, ou avisa que não tem descrição. */
    void show_description()const override;
    /** @brief Mostra o feed do mais novo para o mais antigo (US22). */
    void show_checkins()const;
    /** @brief Mostra a lista de membros, marcando o administrador. */
    void show_users()const;
    /** @brief Mostra o título e o autor da leitura atual, se houver. */
    void show_current_reading()const;
    /**
     * @brief Mostra as métricas do clube: membros, páginas e livros lidos,
     *        média de páginas por membro e, se houver leitura atual, o
     *        progresso médio e quantos membros terminaram.
     */
    void show_metrics()const;

};

#endif
