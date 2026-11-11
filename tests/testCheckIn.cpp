#include "doctest.h"
#include "CheckIn.hpp"
#include "Comment.hpp"
#include "User.hpp"
#include "BookClub.hpp"
#include "Book.hpp"
#include <stdexcept>
#include <ctime>
#include <string>


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


TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: guarda o texto escrito pelo usuário") {
    CheckIn* c = raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    REQUIRE(c != nullptr);   // REQUIRE: se falhar, não adianta testar o resto
    CHECK(c->getText() == "Capitu é suspeita");
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: guarda a página informada") {
    CheckIn* c = raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    REQUIRE(c != nullptr);
    CHECK(c->getPage() == 50);
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
    CHECK(c->getComments().empty());
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: comentário publicado aparece no check-in (US25)") {
    CheckIn* c = raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    REQUIRE(c != nullptr);
    Comment* com = admin.comentar(c, "Também acho!");
    REQUIRE(com != nullptr);

    REQUIRE(c->getComments().size() == 1);
    CHECK(c->getComments()[0] == com);
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: aceita vários comentários de membros diferentes, na ordem (US25)") {
    CheckIn* c = raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    REQUIRE(c != nullptr);
    Comment* primeiro = admin.comentar(c, "Também acho!");
    Comment* segundo = raquel.comentar(c, "Né? Olha os olhos de ressaca");

    REQUIRE(c->getComments().size() == 2);
    CHECK(c->getComments()[0] == primeiro);
    CHECK(c->getComments()[1] == segundo);
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: comentário em um check-in não aparece em outro") {
    CheckIn* c1 = raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    CheckIn* c2 = raquel.realizarCheckIn(&clube, 80, "Bentinho ciumento");
    REQUIRE(c1 != nullptr);
    REQUIRE(c2 != nullptr);

    admin.comentar(c1, "Também acho!");
    CHECK(c1->getComments().size() == 1);
    CHECK(c2->getComments().empty());
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: comentário recusado não entra na lista") {
    CheckIn* c = raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    REQUIRE(c != nullptr);
    admin.comentar(c, "");   // texto vazio é recusado (ver testUser.cpp)
    CHECK(c->getComments().empty());
}
//exceções:
// O construtor é chamado direto (sem realizarCheckIn) para testar só a
// validação do CheckIn.
TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: construtor com usuário nullptr lança exceção") {
    CHECK_THROWS_AS(CheckIn(nullptr, &livro, &clube, 10), std::invalid_argument);
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: construtor com livro nullptr lança exceção") {
    CHECK_THROWS_AS(CheckIn(&raquel, nullptr, &clube, 10), std::invalid_argument);
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: construtor com clube nullptr lança exceção") {
    CHECK_THROWS_AS(CheckIn(&raquel, &livro, nullptr, 10), std::invalid_argument);
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: construtor com página 0 lança exceção") {
    CHECK_THROWS_AS(CheckIn(&raquel, &livro, &clube, 0), std::invalid_argument);
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: construtor com página negativa lança exceção") {
    CHECK_THROWS_AS(CheckIn(&raquel, &livro, &clube, -5), std::invalid_argument);
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: construtor com página maior que o livro lança exceção") {
    // livro tem 300 páginas
    CHECK_THROWS_AS(CheckIn(&raquel, &livro, &clube, 301), std::invalid_argument);
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: primeira e última página do livro são aceitas") {
    CHECK_NOTHROW(CheckIn(&raquel, &livro, &clube, 1));
    CHECK_NOTHROW(CheckIn(&raquel, &livro, &clube, 300));
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: realizarCheckIn com página inválida lança exceção e não muda nada") {
    CHECK_THROWS_AS(raquel.realizarCheckIn(&clube, 301, "passei do fim"), std::invalid_argument);
    CHECK(raquel.getAllCheckIns().empty());
    CHECK(clube.getFeed().empty());
    CHECK(raquel.getTotalPages() == 0);
    CHECK(clube.getReadPages() == 0);
}

// IDs: o contador proximoId é compartilhado por todos os testes, então os
// testes comparam ids entre si em vez de esperar números fixos (como 1, 2...).

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: check-ins criados sem id recebem ids diferentes e crescentes") {
    CheckIn* primeiro = raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    CheckIn* segundo = raquel.realizarCheckIn(&clube, 80, "Bentinho ciumento");
    REQUIRE(primeiro != nullptr);
    REQUIRE(segundo != nullptr);
    CHECK(primeiro->getId() > 0);
    CHECK(segundo->getId() > primeiro->getId());
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: construtor respeita o id informado") {
    CheckIn c(&raquel, &livro, &clube, 50, "Capitu é suspeita", 5000);
    CHECK(c.getId() == 5000);
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: id gerado depois de um id informado é maior que ele") {
    CheckIn carregado(&raquel, &livro, &clube, 50, "Capitu é suspeita", 7000);
    CheckIn novo(&raquel, &livro, &clube, 80, "Bentinho ciumento");
    CHECK(novo.getId() > carregado.getId());
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: construtor com id negativo lança exceção") {
    CHECK_THROWS_AS(CheckIn(&raquel, &livro, &clube, 50, "texto", -1), std::invalid_argument);
}

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: construtor que lança exceção não gasta id") {
    CheckIn antes(&raquel, &livro, &clube, 50, "texto");
    CHECK_THROWS_AS(CheckIn(&raquel, &livro, &clube, 0, "texto"), std::invalid_argument);
    CheckIn depois(&raquel, &livro, &clube, 60, "texto");
    CHECK(depois.getId() == antes.getId() + 1);
}

// ------------------------- DATA E HORA -------------------------

TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: getDataHoraNumero devolve a hora da criação") {
    std::time_t antes = std::time(nullptr);
    CheckIn c(&raquel, &livro, &clube, 50, "Capitu é suspeita");
    std::time_t depois = std::time(nullptr);
    CHECK(c.getDataHoraNumero() >= antes);
    CHECK(c.getDataHoraNumero() <= depois);
}

// 1592222400 = 15/06/2020 12:00 no horário de Greenwich (UTC). Em qualquer
// fuso do Brasil ainda é 15/06/2020, mas para não depender do fuso o teste
// confere só "/06/2020" com find (npos = não achou).
TEST_CASE_FIXTURE(CenarioCheckIn, "CheckIn: restaurarDataHora coloca de volta a data salva") {
    CheckIn c(&raquel, &livro, &clube, 50, "Capitu é suspeita");
    c.restaurarDataHora(1592222400);
    CHECK(c.getDataHoraNumero() == 1592222400);
    CHECK(c.getDataHora().find("/06/2020") != std::string::npos);
}
