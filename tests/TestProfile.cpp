#include"doctest.h"
#include"Profile.hpp"
#include <type_traits>
 
/* Por ser uma classe abstrata, foi necessário criar um perfil teste para realizar o TDD
*/
class PerfilDeTeste : public Profile {
public:
    std::string getName() const override { return "Perfil de Teste"; }
    void show_name() const override {}
    void show_pfp() const override {}
    void show_description() const override {}
};

TEST_CASE("Profile: é uma classe abstrata") {
    CHECK(std::is_abstract<Profile>::value);
}
 
TEST_CASE("Profile: getName chama a versão da subclasse") {
    PerfilDeTeste perfil;
    Profile* p = &perfil;
    CHECK(p->getName() == "Perfil de Teste");
}
 
TEST_CASE("Profile: perfil novo começa com descrição vazia") {
    PerfilDeTeste perfil;
    CHECK(perfil.getDescription() == "");
}
 
TEST_CASE("Profile: perfil novo começa sem foto") {
    PerfilDeTeste perfil;
    CHECK(perfil.getPhotoPath() == "");
}
 
TEST_CASE("Profile: perfil novo começa sem imagem de cabeçalho") {
    PerfilDeTeste perfil;
    CHECK(perfil.getHeaderPath() == "");
}