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

    virtual std::string getName()const = 0;
    virtual std::string getDescription()const;
    virtual std::string getPhotoPath()const;
    virtual std::string getHeaderPath()const;

    
    virtual void show_name()const = 0;
    virtual void show_pfp()const = 0;
    virtual void show_description()const = 0;


};

#endif