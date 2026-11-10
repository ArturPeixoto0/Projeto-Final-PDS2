#include "doctest.h"
#include "Comment.hpp"
#include "CheckIn.hpp"
#include "User.hpp"
#include "BookClub.hpp"
#include "Book.hpp"
#include <ctime>
#include <stdexcept>
#include <string>


struct CenarioComentario {
    User admin;
    User raquel;
    User intrusa;
    Book livro;
    BookClub clube;
    CheckIn* checkIn;   // pertence à Raquel (autora), que o apaga no destrutor

    CenarioComentario()
        : admin("Admin", "admin@email.com", "senha123"),
          raquel("Raquel", "raquel@email.com", "senha123"),
          intrusa("Intrusa", "intrusa@email.com", "senha123"),
          livro("Dom Casmurro", "Machado de Assis", "romance", 300, "01"),
          clube(&admin, "Clube do Machado", "classicos"),
          checkIn(nullptr) {
        clube.adicionarLivro(&livro);
        clube.decidirProximaLeitura(&admin, &livro);
        raquel.entrarClube(&clube);
        checkIn = raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    }
};

//construtor:
TEST_CASE_FIXTURE(CenarioComentario, "Comment: construtor guarda o texto") {
    Comment com("Boa leitura!", &raquel, checkIn, &clube);
    CHECK(com.get_text() == "Boa leitura!");
}

TEST_CASE_FIXTURE(CenarioComentario, "Comment: construtor guarda o autor") {
    Comment com("Boa leitura!", &raquel, checkIn, &clube);
    CHECK(com.get_author() == &raquel);
}

TEST_CASE_FIXTURE(CenarioComentario, "Comment: construtor guarda o check-in comentado") {
    Comment com("Boa leitura!", &raquel, checkIn, &clube);
    CHECK(com.get_checkin() == checkIn);
}

TEST_CASE_FIXTURE(CenarioComentario, "Comment: construtor guarda o clube") {
    Comment com("Boa leitura!", &raquel, checkIn, &clube);
    CHECK(com.get_club() == &clube);
}

// Não dá para saber o segundo exato em que o comentário foi criado, mas dá
// para garantir que foi entre "antes" e "depois" da criação (US25: data/hora).
TEST_CASE_FIXTURE(CenarioComentario, "Comment: construtor registra a data e hora de criação") {
    std::time_t antes = std::time(nullptr);
    Comment com("Boa leitura!", &raquel, checkIn, &clube);
    std::time_t depois = std::time(nullptr);

    CHECK(com.get_createdat() >= antes);
    CHECK(com.get_createdat() <= depois);
}


TEST_CASE_FIXTURE(CenarioComentario, "Comment: construtor não se adiciona sozinho ao check-in") {
    Comment com("Boa leitura!", &raquel, checkIn, &clube);
    CHECK(checkIn->getComments().empty());
}

//validação:
// DECISÃO: mesmo padrão do construtor do User: dados obrigatórios inválidos
// fazem o construtor lançar std::invalid_argument.
TEST_CASE_FIXTURE(CenarioComentario, "Comment: construtor com texto vazio lança exceção") {
    CHECK_THROWS_AS(Comment("", &raquel, checkIn, &clube), std::invalid_argument);
}

TEST_CASE_FIXTURE(CenarioComentario, "Comment: construtor com autor nullptr lança exceção") {
    CHECK_THROWS_AS(Comment("Boa leitura!", nullptr, checkIn, &clube), std::invalid_argument);
}

TEST_CASE_FIXTURE(CenarioComentario, "Comment: construtor com check-in nullptr lança exceção") {
    CHECK_THROWS_AS(Comment("Boa leitura!", &raquel, nullptr, &clube), std::invalid_argument);
}

TEST_CASE_FIXTURE(CenarioComentario, "Comment: construtor com clube nullptr lança exceção") {
    CHECK_THROWS_AS(Comment("Boa leitura!", &raquel, checkIn, nullptr), std::invalid_argument);
}


TEST_CASE("Comment: limite de caracteres é menor que um tweet (US25)") {
    int limite = Comment::MAX_LENGTH;
    CHECK(limite > 0);
    CHECK(limite < 280);
}

TEST_CASE_FIXTURE(CenarioComentario, "Comment: texto com exatamente o limite de caracteres funciona") {
    std::string texto(Comment::MAX_LENGTH, 'a');   // string com MAX_LENGTH letras 'a'
    CHECK_NOTHROW(Comment(texto, &raquel, checkIn, &clube));
}

TEST_CASE_FIXTURE(CenarioComentario, "Comment: texto acima do limite de caracteres lança exceção") {
    std::string texto(Comment::MAX_LENGTH + 1, 'a');
    CHECK_THROWS_AS(Comment(texto, &raquel, checkIn, &clube), std::invalid_argument);
}

TEST_CASE_FIXTURE(CenarioComentario, "Comment: comentário criado pelo User fica no clube do check-in") {
    REQUIRE(checkIn != nullptr);
    Comment* com = admin.comentar(checkIn, "Também acho!");
    REQUIRE(com != nullptr);
    CHECK(com->get_club() == &clube);
}

TEST_CASE_FIXTURE(CenarioComentario, "Comment: comentário criado pelo User aponta para o check-in") {
    REQUIRE(checkIn != nullptr);
    Comment* com = admin.comentar(checkIn, "Também acho!");
    REQUIRE(com != nullptr);
    CHECK(com->get_checkin() == checkIn);
}

TEST_CASE_FIXTURE(CenarioComentario, "Comment: quem não é membro do clube não pode comentar (US25)") {
    REQUIRE(checkIn != nullptr);
    CHECK(intrusa.comentar(checkIn, "Oi, posso entrar?") == nullptr);
    CHECK(checkIn->getComments().empty());
}

TEST_CASE_FIXTURE(CenarioComentario, "Comment: User::comentar com texto acima do limite falha") {
    REQUIRE(checkIn != nullptr);
    std::string texto(Comment::MAX_LENGTH + 1, 'a');
    CHECK(admin.comentar(checkIn, texto) == nullptr);
    CHECK(checkIn->getComments().empty());
}

TEST_CASE_FIXTURE(CenarioComentario, "Comment: autor do check-in pode comentar no próprio check-in") {
    REQUIRE(checkIn != nullptr);
    Comment* com = raquel.comentar(checkIn, "Obrigada!");
    REQUIRE(com != nullptr);
    CHECK(com->get_author() == &raquel);
}
TEST_CASE_FIXTURE(CenarioComentario, "Comment: User::comentar com exatamente o limite de caracteres funciona") {
    REQUIRE(checkIn != nullptr);
    std::string texto(Comment::MAX_LENGTH, 'a');
    Comment* com = admin.comentar(checkIn, texto);
    REQUIRE(com != nullptr);
    CHECK(com->get_text() == texto);
    CHECK(checkIn->getComments().size() == 1);
}

// IDs: o contador _proximo_id é compartilhado por todos os testes, então os
// testes comparam ids entre si em vez de esperar números fixos (como 1, 2...).

TEST_CASE_FIXTURE(CenarioComentario, "Comment: comentários criados sem id recebem ids diferentes e crescentes") {
    REQUIRE(checkIn != nullptr);
    Comment* primeiro = admin.comentar(checkIn, "Também acho!");
    Comment* segundo = raquel.comentar(checkIn, "Olha os olhos de ressaca");
    REQUIRE(primeiro != nullptr);
    REQUIRE(segundo != nullptr);
    CHECK(primeiro->get_id() > 0);
    CHECK(segundo->get_id() > primeiro->get_id());
}

TEST_CASE_FIXTURE(CenarioComentario, "Comment: construtor respeita o id informado") {
    REQUIRE(checkIn != nullptr);
    Comment c("Também acho!", &admin, checkIn, &clube, 5000);
    CHECK(c.get_id() == 5000);
}

TEST_CASE_FIXTURE(CenarioComentario, "Comment: id gerado depois de um id informado é maior que ele") {
    REQUIRE(checkIn != nullptr);
    Comment carregado("Também acho!", &admin, checkIn, &clube, 7000);
    Comment novo("Eu também", &raquel, checkIn, &clube);
    CHECK(novo.get_id() > carregado.get_id());
}

TEST_CASE_FIXTURE(CenarioComentario, "Comment: construtor com id negativo lança exceção") {
    REQUIRE(checkIn != nullptr);
    CHECK_THROWS_AS(Comment("Também acho!", &admin, checkIn, &clube, -1), std::invalid_argument);
}

TEST_CASE_FIXTURE(CenarioComentario, "Comment: construtor que lança exceção não gasta id") {
    REQUIRE(checkIn != nullptr);
    Comment antes("Também acho!", &admin, checkIn, &clube);
    CHECK_THROWS_AS(Comment("", &admin, checkIn, &clube), std::invalid_argument);
    Comment depois("Eu também", &raquel, checkIn, &clube);
    CHECK(depois.get_id() == antes.get_id() + 1);
}
