/**
 * @file testUserProfile.cpp
 * @brief Testes de unidade da classe UserProfile (TDD: escritos antes do UserProfile.cpp).
 *
 * O UserProfile não guarda dados próprios: ele "espelha" o User dono dele.
 * Por isso a maioria dos testes segue o mesmo padrão: muda algo no User e
 * confere se o perfil mostra o mesmo valor (US27: "alterações no perfil são
 * refletidas imediatamente").
 *
 * Os métodos show_* não são testados aqui porque só imprimem na tela.
 */
#include "doctest.h"
#include "UserProfile.hpp"
#include "User.hpp"
#include "bookClub.hpp"
#include "book.hpp"
#include "CheckIn.hpp"
#include <type_traits>

TEST_CASE("UserProfile: é um Profile") {
    CHECK(std::is_base_of<Profile, UserProfile::value);
}

TEST_CASE("UserProfile: getName pelo ponteiro de Profile chama a versão do UserProfile") {
    User u("Raquel", "raquel@email.com", "senha123");
    Profile* p = &u.getProfile();   
    CHECK(p->getName() == "Raquel");
}

TEST_CASE("UserProfile: getName retorna o nome do usuário dono") {
    User u("Raquel", "raquel@email.com", "senha123");
    CHECK(u.getProfile().getName() == "Raquel");
}
 
TEST_CASE("UserProfile: getName acompanha a mudança de nome do usuário") {
    User u("Raquel", "raquel@email.com", "senha123");
    u.setName("Raquel Oliveira");
    CHECK(u.getProfile().getName() == "Raquel Oliveira");
}
 
TEST_CASE("UserProfile: perfis de usuários diferentes mostram nomes diferentes") {
    User a("Raquel", "raquel@email.com", "senha123");
    User b("Artur", "artur@email.com", "senha123");
    CHECK(a.getProfile().getName() == "Raquel");
    CHECK(b.getProfile().getName() == "Artur");
}
//garantir que getProfile não devolve uma cópia nova a cada chamada
TEST_CASE("UserProfile: getProfile sempre devolve o mesmo perfil") {
    User u("Raquel", "raquel@email.com", "senha123");
    CHECK(&u.getProfile() == &u.getProfile());
}
 
TEST_CASE("UserProfile: getProfile funciona em usuário const") {
    const User u("Raquel", "raquel@email.com", "senha123");
    const UserProfile& p = u.getProfile();
    CHECK(p.getName() == "Raquel");
}

//estado inicial:
TEST_CASE("UserProfile: perfil novo começa com descrição, foto e header vazios") {
    User u("Raquel", "raquel@email.com", "senha123");
    CHECK(u.getProfile().getDescription() == "");
    CHECK(u.getProfile().getPhotoPath() == "");
    CHECK(u.getProfile().getHeaderPath() == "");
}
 
TEST_CASE("UserProfile: perfil novo mostra métricas zeradas") {
    User u("Raquel", "raquel@email.com", "senha123");
    CHECK(u.getProfile().getTotalPages() == 0);
    CHECK(u.getProfile().getTotalBooks() == 0);
}
 
TEST_CASE("UserProfile: perfil novo não mostra clubes nem check-ins") {
    User u("Raquel", "raquel@email.com", "senha123");
    CHECK(u.getProfile().getClubs().empty());
    CHECK(u.getProfile().getAllCheckIn().empty());
}

//clubes:
TEST_CASE("UserProfile: getClubs mostra os clubes do usuário") {
    User admin("Admin", "admin@email.com", "senha123");
    User u("Raquel", "raquel@email.com", "senha123");
    BookClub clube1(&admin, "Clube do Machado", "classicos");
    BookClub clube2(&admin, "Clube da Clarice", "modernos");
 
    u.entrarClube(&clube1);
    u.entrarClube(&clube2);
 
    std::vector<BookClub*> clubes = u.getProfile().getClubs();
    REQUIRE(clubes.size() == 2);
    CHECK(clubes == u.getCurrentClubs());
}
 
TEST_CASE("UserProfile: getClubs deixa de mostrar clube do qual o usuário saiu") {
    User admin("Admin", "admin@email.com", "senha123");
    User u("Raquel", "raquel@email.com", "senha123");
    BookClub clube(&admin, "Clube do Machado", "classicos");
 
    u.entrarClube(&clube);
    u.sairClube(&clube);
    CHECK(u.getProfile().getClubs().empty());
}

/*
 * Cenário com DOIS clubes, cada um com uma leitura atual, para testar que o
 * perfil junta check-ins de todos os clubes (US07 e US23).
 */

struct CenarioPerfil {
    User admin;
    User raquel;
    Book livro1;
    Book livro2;
    BookClub clube1;
    BookClub clube2;
 
    CenarioPerfil()
        : admin("Admin", "admin@email.com", "senha123"),
          raquel("Raquel", "raquel@email.com", "senha123"),
          livro1("Dom Casmurro", "Machado de Assis", "romance", 300, "01"),
          livro2("A hora da estrela", "Clarice Lispector", "romance", 88, "02"),
          clube1(&admin, "Clube do Machado", "classicos"),
          clube2(&admin, "Clube da Clarice", "modernos") {
        clube1.adicionarLivro(&livro1);
        clube1.decidirProximaLeitura(&admin, &livro1);
        clube2.adicionarLivro(&livro2);
        clube2.decidirProximaLeitura(&admin, &livro2);
        raquel.entrarClube(&clube1);
        raquel.entrarClube(&clube2);
    }
};
 
TEST_CASE_FIXTURE(CenarioPerfil, "UserProfile: getAllCheckIn junta check-ins de todos os clubes") {
    raquel.realizarCheckIn(&clube1, 50, "Capitu é meio suspeita");
    raquel.realizarCheckIn(&clube2, 20, "Macabéa :(, tadinha");
    CHECK(raquel.getProfile().getAllCheckIn().size() == 2);
}
 
TEST_CASE_FIXTURE(CenarioPerfil, "UserProfile: check-in apagado some do perfil (US21/US23)") {
    CheckIn* c = raquel.realizarCheckIn(&clube1, 50, "postei errado");
    REQUIRE(c != nullptr);
    raquel.apagarCheckIn(c);
    CHECK(raquel.getProfile().getAllCheckIn().empty());
}
 
TEST_CASE_FIXTURE(CenarioPerfil, "UserProfile: paginômetro do perfil é igual ao do usuário") {
    raquel.realizarCheckIn(&clube1, 50, "Capitu é meio suspeita");
    raquel.realizarCheckIn(&clube2, 20, "Macabéa :(, tadinha");
    CHECK(raquel.getProfile().getTotalPages() == raquel.getTotalPages());
    CHECK(raquel.getProfile().getTotalPages() == 70);
}
 
TEST_CASE_FIXTURE(CenarioPerfil, "UserProfile: livrômetro do perfil é igual ao do usuário") {
    raquel.realizarCheckIn(&clube2, 88, "Terminei!", true);
    CHECK(raquel.getProfile().getTotalBooks() == raquel.getTotalBooks());
    CHECK(raquel.getProfile().getTotalBooks() == 1);
}

