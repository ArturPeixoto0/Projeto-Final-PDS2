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
    std::string Description;
    std::string PhotoPath;
    std::string HeaderPath;

public:
    /** @brief Limite de caracteres da descrição/biografia (US27). */
    static const int MAX_DESCRIPTION_LENGTH = 300;

    /**
     * @brief Destrutor virtual: garante que, ao apagar um perfil por um
     *        ponteiro Profile*, o destrutor da classe filha também rode.
     */
    virtual ~Profile();

    virtual std::string getName()const = 0;
    virtual std::string getDescription()const;
    virtual std::string getPhotoPath()const;
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


    virtual void show_name()const = 0;
    virtual void show_pfp()const = 0;
    virtual void show_description()const = 0;


};

#endif
