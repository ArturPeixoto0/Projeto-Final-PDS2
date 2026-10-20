#include "doctest.h"
#include "ClubProfile.hpp"
#include "bookClub.hpp"
#include "User.hpp"
#include "CheckIn.hpp"
#include "book.hpp"

#include <iostream>
#include <sstream>
#include <string>

TEST_CASE("ClubProfile: getName retorna o nome do clube") {
    BookClub clube("Clube das Leitoras");
    ClubProfile perfil(&clube);
    CHECK(perfil.getName() == "Clube das Leitoras");
}

TEST_CASE("ClubProfile: getName reflete alteração no nome do clube") {
    BookClub clube("Clube das Leitoras");
    ClubProfile perfil(&clube);
    clube.setName("Leitores Noturnos");
    CHECK(perfil.getName() == "Leitores Noturnos");
}

TEST_CASE("ClubProfile: getName funciona via referência à classe base") {
    BookClub clube("Clube das Leitoras");
    ClubProfile perfil(&clube);
    Profile& base = perfil;
    CHECK(base.getName() == "Clube das Leitoras");
}

TEST_CASE("ClubProfile: descrição começa vazia") {
    BookClub clube("Clube das Leitoras");
    ClubProfile perfil(&clube);
    CHECK(perfil.getDescription() == "");
}

TEST_CASE("ClubProfile: caminho da foto começa vazio") {
    BookClub clube("Clube das Leitoras");
    ClubProfile perfil(&clube);
    CHECK(perfil.getPhotoPath() == "");
}

TEST_CASE("ClubProfile: caminho do cabeçalho começa vazio") {
    BookClub clube("Clube das Leitoras");
    ClubProfile perfil(&clube);
    CHECK(perfil.getHeaderPath() == "");
}

TEST_CASE("ClubProfile: clube recém-criado não tem check-ins") {
    BookClub clube("Clube das Leitoras");
    ClubProfile perfil(&clube);
    CHECK(perfil.getAllCheckIn().empty());
}

TEST_CASE("ClubProfile: clube recém-criado não tem usuários") {
    BookClub clube("Clube das Leitoras");
    ClubProfile perfil(&clube);
    CHECK(perfil.getUsers().empty());
}

//------------------------METODOS-----------------------------------------------------------

TEST_CASE("ClubProfile: getUsers retorna os membros do clube") {
    BookClub clube("Clube das Leitoras");
    User ana("ana");
    User bia("bia");
    clube.addMember(&ana);
    clube.addMember(&bia);
    ClubProfile perfil(&clube);
    std::vector<User*> usuarios = perfil.getUsers();
    REQUIRE(usuarios.size() == 2);
    CHECK(usuarios[0] == &ana);
    CHECK(usuarios[1] == &bia);
}

TEST_CASE("ClubProfile: getAllCheckIn retorna os check-ins do clube") {
    BookClub clube("Clube das Leitoras");
    User ana("ana");
    clube.addMember(&ana);
    clube.addCheckIn(CheckIn());
    ClubProfile perfil(&clube);
    CHECK(perfil.getAllCheckIn().size() == 1);
}

TEST_CASE("ClubProfile: show_name exibe o nome do clube") {
    BookClub clube("Clube das Leitoras");
    ClubProfile perfil(&clube);
    CapturaSaida saida;
    perfil.show_name();
    CHECK(saida.str().find("Clube das Leitoras") != std::string::npos);
}

TEST_CASE("ClubProfile: show_name funciona via referência à classe base") {
    BookClub clube("Clube das Leitoras");
    ClubProfile perfil(&clube);
    Profile& base = perfil;
    CapturaSaida saida;
    base.show_name();
    CHECK(saida.str().find("Clube das Leitoras") != std::string::npos);
}

TEST_CASE("ClubProfile: show_description produz saída") {
    BookClub clube("Clube das Leitoras");
    ClubProfile perfil(&clube);
    CapturaSaida saida;
    perfil.show_description();
    CHECK_FALSE(saida.str().empty());
}

TEST_CASE("ClubProfile: show_pfp produz saída") {
    BookClub clube("Clube das Leitoras");
    ClubProfile perfil(&clube);
    CapturaSaida saida;
    perfil.show_pfp();
    CHECK_FALSE(saida.str().empty());
}

TEST_CASE("ClubProfile: show_users lista os nomes dos membros") {
    BookClub clube("Clube das Leitoras");
    User ana("ana");
    clube.addMember(&ana);
    ClubProfile perfil(&clube);
    CapturaSaida saida;
    perfil.show_users();
    CHECK(saida.str().find("ana") != std::string::npos);
}

TEST_CASE("ClubProfile: show_users sem membros não quebra") {
    BookClub clube("Clube das Leitoras");
    ClubProfile perfil(&clube);
    CapturaSaida saida;
    CHECK_NOTHROW(perfil.show_users());
}

TEST_CASE("ClubProfile: show_checkins sem check-ins não quebra") {
    BookClub clube("Clube das Leitoras");
    ClubProfile perfil(&clube);
    CapturaSaida saida;
    CHECK_NOTHROW(perfil.show_checkins());
}

TEST_CASE("ClubProfile: show_current_reading sem leitura atual não quebra") {
    BookClub clube("Clube das Leitoras");
    ClubProfile perfil(&clube);
    CapturaSaida saida;
    CHECK_NOTHROW(perfil.show_current_reading());
}

TEST_CASE("ClubProfile: show_current_reading exibe o título da leitura atual") {
    BookClub clube("Clube das Leitoras");
    Book livro("Os sete maridos de Evelyn Hugo", "Taylor Jenkins Reid", "drama", 360, "01");
    clube.setCurrentBook(&livro);
    ClubProfile perfil(&clube);
    CapturaSaida saida;
    perfil.show_current_reading();
    CHECK(saida.str().find("Os sete maridos de Evelyn Hugo") != std::string::npos);
}

TEST_CASE("ClubProfile: show_metrics produz saída") {
    BookClub clube("Clube das Leitoras");
    ClubProfile perfil(&clube);
    CapturaSaida saida;
    perfil.show_metrics();
    CHECK_FALSE(saida.str().empty());
}

TEST_CASE("ClubProfile: showReadingStatus não quebra com clube vazio") {
    BookClub clube("Clube das Leitoras");
    ClubProfile perfil(&clube);
    CapturaSaida saida;
    CHECK_NOTHROW(perfil.showReadingStatus());
}