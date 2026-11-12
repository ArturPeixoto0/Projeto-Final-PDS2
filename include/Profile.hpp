/**
 * @file Profile.hpp
 * @brief Declaração da classe abstrata Profile.
 */

#ifndef PROFILE_H
#define PROFILE_H

#include <string>

/**
* @brief Representa a classe base abstrata para os perfis do sistema.
*
* Armazena informações comuns aos diferentes tipos de perfil, como
* descrição, foto e imagem de cabeçalho, além de definir comportamentos
* que devem ser implementados pelas classes derivadas (ClubProfile e UserProfile).
  */


class Profile {

private:
    std::string Description; ///< Descrição/biografia (até MAX_DESCRIPTION_LENGTH caracteres).
    std::string PhotoPath;   ///< Caminho da foto de perfil (vazio = sem foto).
    std::string HeaderPath;  ///< Caminho da imagem de cabeçalho (vazio = sem header).

public:
    /** @brief Limite de caracteres da descrição/biografia (US27). */
    static const int MAX_DESCRIPTION_LENGTH = 300;

    /**
     * @brief Destrutor virtual: garante que, ao apagar um perfil por um
     *        ponteiro Profile*, o destrutor da classe filha também rode.
     */
    virtual ~Profile();

    /**
     * @brief Retorna o nome exibido no perfil.
     *
     * Método virtual puro: cada filha decide de onde vem o nome
     * (UserProfile pega do User, ClubProfile pega do BookClub).
     */
    virtual std::string getName()const = 0;
    /** @brief Retorna a descrição (biografia) do perfil. Pode ser vazia. */
    virtual std::string getDescription()const;
    /** @brief Retorna o caminho da foto de perfil. Vazio = sem foto. */
    virtual std::string getPhotoPath()const;
    /** @brief Retorna o caminho da imagem de cabeçalho. Vazio = sem header. */
    virtual std::string getHeaderPath()const;

    /**
     * @brief Altera a descrição (biografia). Pode ser vazia (US27: campos opcionais).
     * @return false se passar de MAX_DESCRIPTION_LENGTH (a descrição não muda).
     */
    bool setDescription(const std::string& description);
    /** @brief Altera o caminho da foto de perfil. Vazio = sem foto. */
    void setPhotoPath(const std::string& path);
    /** @brief Altera o caminho da imagem de cabeçalho. Vazio = sem header. */
    void setHeaderPath(const std::string& path);


    /** @brief Mostra o nome do perfil no terminal (implementado por cada filha). */
    virtual void show_name()const = 0;
    /** @brief Mostra a foto de perfil no terminal (implementado por cada filha). */
    virtual void show_pfp()const = 0;
    /** @brief Mostra a descrição do perfil no terminal (implementado por cada filha). */
    virtual void show_description()const = 0;


};

#endif
