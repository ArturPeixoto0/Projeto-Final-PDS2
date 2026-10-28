#include "doctest.h"
#include "CheckIn.hpp"
#include "Comment.hpp"
#include "User.hpp"
#include "BookClub.hpp"
#include "Book.hpp"


struct CenarioCheckIn {
    User admin;
    User raquel;
    Book livro;
    BookClub clube;

    CenarioCheckIn()
        : admin("Admin", "admin@email.com", "senha123"),
          raquel("Raquel", "raquel@email.com", "senha123"),
          livro("Dom Casmurro", "Machado de Assis", "romance", 300, "01"),
          clube(&admin, "Clube do Machado", "classicos") {
        clube.adicionarLivro(&livro);
        clube.decidirProximaLeitura(&admin, &livro);  // livro vira a leitura atual
        raquel.entrarClube(&clube);
    }
};


TEST_CASE("CheckIn: construtor padrão começa com texto vazio") {
    CheckIn c;
    CHECK(c.getText() == "");
}

TEST_CASE("CheckIn: construtor padrão começa na página 0") {
    CheckIn c;
    CHECK(c.getPages() == 0);
}

TEST_CASE("CheckIn: construtor padrão começa sem comentários") {
    CheckIn c;
    CHECK(c.getComents().empty());
}

TEST_CASE("CheckIn: construtor padrão não aponta para autor, livro nem clube") {
    CheckIn c;
    CHECK(c.getUser() == nullptr);
    CHECK(c.getBook() == nullptr);
    CHECK(c.getClub() == nullptr);
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: guarda o texto escrito pelo usuário") {
    CheckIn* c = raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    REQUIRE(c != nullptr);   // REQUIRE: se falhar, não adianta testar o resto
    CHECK(c->getText() == "Capitu é suspeita");
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: guarda a página informada") {
    CheckIn* c = raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    REQUIRE(c != nullptr);
    CHECK(c->getPages() == 50);
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: guarda o autor (US18)") {
    CheckIn* c = raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    REQUIRE(c != nullptr);
    CHECK(c->getUser() == &raquel);
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: guarda o clube em que foi feito (US23)") {
    CheckIn* c = raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    REQUIRE(c != nullptr);
    CHECK(c->getClub() == &clube);
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: é associado automaticamente ao livro atual do clube (US18)") {
    CheckIn* c = raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    REQUIRE(c != nullptr);
    CHECK(c->getBook() == &livro);
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: continua no livro antigo depois que o clube troca de leitura") {
    CheckIn* c = raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    REQUIRE(c != nullptr);

    Book proximo("A hora da estrela", "Clarice Lispector", "romance", 88, "02");
    clube.adicionarLivro(&proximo);
    clube.decidirProximaLeitura(&admin, &proximo);

    CHECK(c->getBook() == &livro);
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: check-ins de usuários diferentes guardam autores diferentes") {
    CheckIn* daRaquel = raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    CheckIn* doAdmin = admin.realizarCheckIn(&clube, 30, "Começando!");
    REQUIRE(daRaquel != nullptr);
    REQUIRE(doAdmin != nullptr);
    CHECK(daRaquel->getUser() == &raquel);
    CHECK(doAdmin->getUser() == &admin);
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: registra data e hora da postagem") {
    CheckIn* c = raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    REQUIRE(c != nullptr);
    CHECK_FALSE(c->getDataHora().empty());
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: check-in novo não tem comentários") {
    CheckIn* c = raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    REQUIRE(c != nullptr);
    CHECK(c->getComents().empty());
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: comentário publicado aparece no check-in (US25)") {
    CheckIn* c = raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    REQUIRE(c != nullptr);
    Comment* com = admin.comentar(c, "Também acho!");
    REQUIRE(com != nullptr);

    REQUIRE(c->getComents().size() == 1);
    CHECK(c->getComents()[0] == com);
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: aceita vários comentários de membros diferentes, na ordem (US25)") {
    CheckIn* c = raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    REQUIRE(c != nullptr);
    Comment* primeiro = admin.comentar(c, "Também acho!");
    Comment* segundo = raquel.comentar(c, "Né? Olha os olhos de ressaca");

    REQUIRE(c->getComents().size() == 2);
    CHECK(c->getComents()[0] == primeiro);
    CHECK(c->getComents()[1] == segundo);
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: comentário em um check-in não aparece em outro") {
    CheckIn* c1 = raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    CheckIn* c2 = raquel.realizarCheckIn(&clube, 80, "Bentinho ciumento");
    REQUIRE(c1 != nullptr);
    REQUIRE(c2 != nullptr);

    admin.comentar(c1, "Também acho!");
    CHECK(c1->getComents().size() == 1);
    CHECK(c2->getComents().empty());
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: comentário recusado não entra na lista") {
    CheckIn* c = raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    REQUIRE(c != nullptr);
    admin.comentar(c, "");   // texto vazio é recusado (ver testUser.cpp)
    CHECK(c->getComents().empty());
}