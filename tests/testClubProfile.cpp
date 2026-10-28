/**
 * @file testClubProfile.cpp
 * @brief Testes de unidade da classe ClubProfile.
 *
 * Assim como o UserProfile, o ClubProfile não guarda dados próprios: ele
 * "espelha" o BookClub dono dele. Por isso a maioria dos testes muda algo
 * no clube e confere se o perfil mostra o mesmo valor.
 *
 * Os métodos show_* imprimem na tela. Para testá-los, a struct CapturaSaida
 * desvia o std::cout para uma string durante o teste.
 */
#include "doctest.h"
#include "ClubProfile.hpp"
#include "BookClub.hpp"
#include "User.hpp"
#include "CheckIn.hpp"
#include "Book.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

/*
 * Desvia tudo que for escrito no std::cout para uma string.
 *  - No construtor: guarda o "destino" antigo do cout (rdbuf) e troca
 *    pelo da nossa stringstream.
 *  - No destrutor: devolve o destino antigo. Como o destrutor roda sozinho
 *    no fim do TEST_CASE, o cout sempre volta ao normal, mesmo se o teste falhar.
 */
struct CapturaSaida {
    std::stringstream buffer;
    std::streambuf* antigo;

    CapturaSaida() : antigo(std::cout.rdbuf(buffer.rdbuf())) {}
    ~CapturaSaida() { std::cout.rdbuf(antigo); }

    std::string str() const { return buffer.str(); }
};

/*
 * Cenário básico: um clube com admin, sem livro e sem outros membros.
 */
struct CenarioClube {
    User admin;
    BookClub clube;
    ClubProfile perfil;

    CenarioClube()
        : admin("Admin", "admin@email.com", "senha123"),
          clube(&admin, "Clube das Leitoras", "democratico"),
          perfil(&clube) {}
};

/*
 * Cenário com leitura: clube com um livro atual e duas leitoras além do admin.
 */
struct CenarioLeitura {
    User admin;
    User ana;
    User bia;
    Book livro;
    BookClub clube;
    ClubProfile perfil;

    CenarioLeitura()
        : admin("Admin", "admin@email.com", "senha123"),
          ana("Ana", "ana@email.com", "senha123"),
          bia("Bia", "bia@email.com", "senha123"),
          livro("Os sete maridos de Evelyn Hugo", "Taylor Jenkins Reid", "drama", 360, "01"),
          clube(&admin, "Clube das Leitoras", &livro),
          perfil(&clube) {
        ana.entrarClube(&clube);
        bia.entrarClube(&clube);
    }
};

// ------------------------------ HERANÇA ------------------------------

TEST_CASE("ClubProfile: é um Profile") {
    CHECK(std::is_base_of<Profile, ClubProfile>::value);
}

TEST_CASE_FIXTURE(CenarioClube, "ClubProfile: getName funciona via referência à classe base") {
    Profile& base = perfil;
    CHECK(base.getName() == "Clube das Leitoras");
}

// --------------------------- ESTADO INICIAL ---------------------------

TEST_CASE_FIXTURE(CenarioClube, "ClubProfile: getName retorna o nome do clube") {
    CHECK(perfil.getName() == "Clube das Leitoras");
}

TEST_CASE_FIXTURE(CenarioClube, "ClubProfile: getName reflete alteração no nome do clube") {
    clube.renomear(&admin, "Leitores Noturnos");
    CHECK(perfil.getName() == "Leitores Noturnos");
}

TEST_CASE_FIXTURE(CenarioClube, "ClubProfile: descrição, foto e cabeçalho começam vazios") {
    CHECK(perfil.getDescription() == "");
    CHECK(perfil.getPhotoPath() == "");
    CHECK(perfil.getHeaderPath() == "");
}

TEST_CASE_FIXTURE(CenarioClube, "ClubProfile: clube recém-criado não tem check-ins") {
    CHECK(perfil.getAllCheckIn().empty());
}

TEST_CASE_FIXTURE(CenarioClube, "ClubProfile: clube recém-criado tem só o admin como membro") {
    std::vector<User*> usuarios = perfil.getUsers();
    REQUIRE(usuarios.size() == 1);
    CHECK(usuarios[0] == &admin);
}

// ------------------------------ DESCRIÇÃO ------------------------------

TEST_CASE_FIXTURE(CenarioClube, "ClubProfile: setDescription altera a descrição") {
    CHECK(perfil.setDescription("Lemos um livro por mês"));
    CHECK(perfil.getDescription() == "Lemos um livro por mês");
}

TEST_CASE_FIXTURE(CenarioClube, "ClubProfile: descrição acima do limite é recusada") {
    perfil.setDescription("original");
    std::string grande(Profile::MAX_DESCRIPTION_LENGTH + 1, 'a');
    CHECK_FALSE(perfil.setDescription(grande));
    CHECK(perfil.getDescription() == "original");
}

// ------------------------------- MEMBROS -------------------------------

TEST_CASE_FIXTURE(CenarioLeitura, "ClubProfile: getUsers retorna os membros na ordem de entrada") {
    std::vector<User*> usuarios = perfil.getUsers();
    REQUIRE(usuarios.size() == 3);
    CHECK(usuarios[0] == &admin);
    CHECK(usuarios[1] == &ana);
    CHECK(usuarios[2] == &bia);
}

TEST_CASE_FIXTURE(CenarioLeitura, "ClubProfile: getUsers deixa de mostrar quem saiu do clube") {
    ana.sairClube(&clube);
    std::vector<User*> usuarios = perfil.getUsers();
    REQUIRE(usuarios.size() == 2);
    CHECK(usuarios[1] == &bia);
}

// ------------------------------ CHECK-INS ------------------------------

TEST_CASE_FIXTURE(CenarioLeitura, "ClubProfile: getAllCheckIn mostra os check-ins dos membros") {
    ana.realizarCheckIn(&clube, 50, "Que começo!");
    bia.realizarCheckIn(&clube, 30, "Amando");
    CHECK(perfil.getAllCheckIn().size() == 2);
}

TEST_CASE_FIXTURE(CenarioLeitura, "ClubProfile: check-in apagado some do feed (US21/US22)") {
    CheckIn* c = ana.realizarCheckIn(&clube, 50, "postei errado");
    REQUIRE(c != nullptr);
    ana.apagarCheckIn(c);
    CHECK(perfil.getAllCheckIn().empty());
}

// ------------------------------ SHOW_* ------------------------------

TEST_CASE_FIXTURE(CenarioClube, "ClubProfile: show_name exibe o nome do clube") {
    CapturaSaida saida;
    perfil.show_name();
    CHECK(saida.str().find("Clube das Leitoras") != std::string::npos);
}

TEST_CASE_FIXTURE(CenarioClube, "ClubProfile: show_name funciona via referência à classe base") {
    Profile& base = perfil;
    CapturaSaida saida;
    base.show_name();
    CHECK(saida.str().find("Clube das Leitoras") != std::string::npos);
}

TEST_CASE_FIXTURE(CenarioClube, "ClubProfile: show_description sem descrição avisa que está vazia") {
    CapturaSaida saida;
    perfil.show_description();
    CHECK(saida.str().find("sem descricao") != std::string::npos);
}

TEST_CASE_FIXTURE(CenarioClube, "ClubProfile: show_description mostra a descrição") {
    perfil.setDescription("Lemos um livro por mês");
    CapturaSaida saida;
    perfil.show_description();
    CHECK(saida.str().find("Lemos um livro por mês") != std::string::npos);
}

TEST_CASE_FIXTURE(CenarioClube, "ClubProfile: show_pfp sem foto avisa que não tem foto") {
    CapturaSaida saida;
    perfil.show_pfp();
    CHECK(saida.str().find("sem foto") != std::string::npos);
}

TEST_CASE_FIXTURE(CenarioClube, "ClubProfile: show_pfp mostra o caminho da foto") {
    perfil.setPhotoPath("fotos/clube.png");
    CapturaSaida saida;
    perfil.show_pfp();
    CHECK(saida.str().find("fotos/clube.png") != std::string::npos);
}

TEST_CASE_FIXTURE(CenarioLeitura, "ClubProfile: show_users lista os membros e marca o admin") {
    CapturaSaida saida;
    perfil.show_users();
    std::string texto = saida.str();
    CHECK(texto.find("Admin (admin)") != std::string::npos);
    CHECK(texto.find("Ana") != std::string::npos);
    CHECK(texto.find("Bia") != std::string::npos);
}

TEST_CASE_FIXTURE(CenarioClube, "ClubProfile: show_checkins sem check-ins avisa que está vazio") {
    CapturaSaida saida;
    perfil.show_checkins();
    CHECK(saida.str().find("Nenhum check-in") != std::string::npos);
}

TEST_CASE_FIXTURE(CenarioLeitura, "ClubProfile: show_checkins mostra do mais novo para o mais antigo (US22)") {
    ana.realizarCheckIn(&clube, 50, "primeiro");
    bia.realizarCheckIn(&clube, 30, "segundo");
    CapturaSaida saida;
    perfil.show_checkins();
    std::string texto = saida.str();
    REQUIRE(texto.find("primeiro") != std::string::npos);
    REQUIRE(texto.find("segundo") != std::string::npos);
    CHECK(texto.find("segundo") < texto.find("primeiro"));
}

TEST_CASE_FIXTURE(CenarioClube, "ClubProfile: show_current_reading sem leitura avisa") {
    CapturaSaida saida;
    perfil.show_current_reading();
    CHECK(saida.str().find("Nenhuma leitura") != std::string::npos);
}

TEST_CASE_FIXTURE(CenarioLeitura, "ClubProfile: show_current_reading exibe título e autor") {
    CapturaSaida saida;
    perfil.show_current_reading();
    std::string texto = saida.str();
    CHECK(texto.find("Os sete maridos de Evelyn Hugo") != std::string::npos);
    CHECK(texto.find("Taylor Jenkins Reid") != std::string::npos);
}

TEST_CASE_FIXTURE(CenarioLeitura, "ClubProfile: show_metrics mostra membros e páginas lidas") {
    ana.realizarCheckIn(&clube, 50, "a");
    bia.realizarCheckIn(&clube, 30, "b");
    CapturaSaida saida;
    perfil.show_metrics();
    std::string texto = saida.str();
    CHECK(texto.find("Membros: 3") != std::string::npos);
    CHECK(texto.find("Paginas lidas no clube: 80") != std::string::npos);
}

TEST_CASE_FIXTURE(CenarioClube, "ClubProfile: showReadingStatus sem leitura avisa") {
    CapturaSaida saida;
    perfil.showReadingStatus();
    CHECK(saida.str().find("Nenhuma leitura") != std::string::npos);
}

TEST_CASE_FIXTURE(CenarioLeitura, "ClubProfile: showReadingStatus ordena o ranking e marca quem terminou (US20/US29)") {
    bia.realizarCheckIn(&clube, 100, "indo");
    ana.realizarCheckIn(&clube, 0, "Terminei!", true);
    CapturaSaida saida;
    perfil.showReadingStatus();
    std::string texto = saida.str();
    CHECK(texto.find("1. Ana") != std::string::npos);
    CHECK(texto.find("2. Bia") != std::string::npos);
    CHECK(texto.find("TERMINOU") != std::string::npos);
}