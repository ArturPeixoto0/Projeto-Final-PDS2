/**
 * @file testBookClub.cpp
 * @brief Testes de unidade da classe BookClub.
 *
 * Organização:
 *  - construtor e estado inicial;
 *  - membros (entrar, sair, clube privado);
 *  - catálogo (adicionar, remover, próxima leitura, votos, avaliações);
 *  - ações de administrador;
 *  - exceções.
 */
#include "doctest.h"
#include "BookClub.hpp"
#include "User.hpp"
#include "Book.hpp"
#include "CheckIn.hpp"
#include <ctime>
#include <stdexcept>
#include <string>

/*
 * Cenário usado na maioria dos testes: um clube sem leitura, com admin e
 * raquel como membros, três livros soltos (fora do catálogo) e artur, que
 * não é membro.
 *
 * Os Users e os Books são declarados ANTES do clube. Como os atributos são
 * destruídos na ordem inversa, o clube é destruído primeiro e ainda encontra
 * os Users vivos para tirá-los do clube.
 */
namespace{
 struct CenarioClube {
    User admin;
    User raquel;
    User artur;
    Book livro1;
    Book livro2;
    Book livro3;
    BookClub clube;

    CenarioClube()
        : admin("Admin", "admin@email.com", "senha123"),
          raquel("Raquel", "raquel@email.com", "senha123"),
          artur("Artur", "artur@email.com", "senha123"),
          livro1("Dom Casmurro", "Machado de Assis", "romance", 300, "01"),
          livro2("A hora da estrela", "Clarice Lispector", "romance", 88, "02"),
          livro3("Grande Sertao: Veredas", "Guimaraes Rosa", "romance", 624, "03"),
          clube(&admin, "Clube do Machado", "classicos") {
        raquel.entrarClube(&clube);
    }
};
}

// =====================================================================
// Construtor e estado inicial
// =====================================================================

TEST_CASE("BookClub: construtor guarda nome e tipo") {
    User admin("Admin", "admin@email.com", "senha123");
    BookClub clube(&admin, "Clube do Machado", "classicos");
    CHECK(clube.getName() == "Clube do Machado");
    CHECK(clube.getType() == "classicos");
}

TEST_CASE("BookClub: clube novo é público e ativo por padrão") {
    User admin("Admin", "admin@email.com", "senha123");
    BookClub clube(&admin, "Clube do Machado", "classicos");
    CHECK_FALSE(clube.isPrivate());
    CHECK(clube.isActive());
}

TEST_CASE("BookClub: construtor pode criar clube privado") {
    User admin("Admin", "admin@email.com", "senha123");
    BookClub clube(&admin, "Clube secreto", "classicos", true);
    CHECK(clube.isPrivate());
}

TEST_CASE("BookClub: fundador vira administrador e membro") {
    User admin("Admin", "admin@email.com", "senha123");
    BookClub clube(&admin, "Clube do Machado", "classicos");
    CHECK(clube.getAdmin() == &admin);
    CHECK(clube.isAdmin(&admin));
    CHECK(clube.isMember(&admin));
    CHECK(clube.getNumberMembers() == 1);
}

TEST_CASE("BookClub: fundador também tem o clube na própria lista") {
    User admin("Admin", "admin@email.com", "senha123");
    BookClub clube(&admin, "Clube do Machado", "classicos");
    CHECK(admin.participaDe(&clube));
    REQUIRE(admin.getCurrentClubs().size() == 1);
    CHECK(admin.getCurrentClubs()[0] == &clube);
}

TEST_CASE("BookClub: clube novo começa sem leitura, catálogo, feed e métricas") {
    User admin("Admin", "admin@email.com", "senha123");
    BookClub clube(&admin, "Clube do Machado", "classicos");
    CHECK_FALSE(clube.isReading());
    CHECK(clube.getCurrentReading() == nullptr);
    CHECK(clube.getCatalog().empty());
    CHECK(clube.getFeed().empty());
    CHECK(clube.getReadPages() == 0);
    CHECK(clube.getReadBooks() == 0);
}

TEST_CASE("BookClub: construtor registra a data de criação") {
    User admin("Admin", "admin@email.com", "senha123");
    std::time_t antes = std::time(nullptr);
    BookClub clube(&admin, "Clube do Machado", "classicos");
    std::time_t depois = std::time(nullptr);
    CHECK(clube.getClubCreation() >= antes);
    CHECK(clube.getClubCreation() <= depois);
}

TEST_CASE("BookClub: construtor com primeiro livro já começa a leitura") {
    User admin("Admin", "admin@email.com", "senha123");
    Book livro("Dom Casmurro", "Machado de Assis", "romance", 300, "01");
    BookClub clube(&admin, "Clube do Machado", &livro);

    CHECK(clube.isReading());
    CHECK(clube.getCurrentReading() == &livro);
    REQUIRE(clube.getCatalog().size() == 1);
    CHECK(clube.getCatalog()[0].status == BookClub::Status::Atual);
    CHECK(clube.getType() == "democratico");   // valor padrão
}

TEST_CASE("BookClub: construtor com primeiro livro nullptr começa sem leitura") {
    User admin("Admin", "admin@email.com", "senha123");
    Book* nenhum = nullptr;
    BookClub clube(&admin, "Clube do Machado", nenhum);
    CHECK_FALSE(clube.isReading());
    CHECK(clube.getCatalog().empty());
}

TEST_CASE("BookClub: User::criarClube cria o clube com o usuário como admin") {
    User admin("Admin", "admin@email.com", "senha123");
    Book livro("Dom Casmurro", "Machado de Assis", "romance", 300, "01");
    BookClub* clube = admin.criarClube("Clube do Machado", &livro);

    REQUIRE(clube != nullptr);
    CHECK(clube->isAdmin(&admin));
    CHECK(admin.participaDe(clube));
    CHECK(clube->getCurrentReading() == &livro);
    delete clube;
    CHECK(admin.getCurrentClubs().empty());   // o destrutor tira o clube da lista
}

// =====================================================================
// Membros
// =====================================================================

TEST_CASE_FIXTURE(CenarioClube, "BookClub: User::entrarClube atualiza os dois lados") {
    CHECK(artur.entrarClube(&clube));
    CHECK(clube.isMember(&artur));
    CHECK(artur.participaDe(&clube));
    CHECK(clube.getNumberMembers() == 3);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: entrarNoClube pelo clube também atualiza o User") {
    CHECK(clube.entrarNoClube(&artur));
    CHECK(clube.isMember(&artur));
    CHECK(artur.participaDe(&clube));
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: entrar duas vezes falha na segunda") {
    CHECK_FALSE(raquel.entrarClube(&clube));
    CHECK_FALSE(clube.entrarNoClube(&raquel));
    CHECK(clube.getNumberMembers() == 2);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: membros ficam na ordem em que entraram") {
    artur.entrarClube(&clube);
    REQUIRE(clube.getMembers().size() == 3);
    CHECK(clube.getMembers()[0] == &admin);
    CHECK(clube.getMembers()[1] == &raquel);
    CHECK(clube.getMembers()[2] == &artur);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: User::sairClube atualiza os dois lados") {
    CHECK(raquel.sairClube(&clube));
    CHECK_FALSE(clube.isMember(&raquel));
    CHECK_FALSE(raquel.participaDe(&clube));
    CHECK(clube.getNumberMembers() == 1);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: sairDoClube pelo clube também atualiza o User") {
    CHECK(clube.sairDoClube(&raquel));
    CHECK_FALSE(raquel.participaDe(&clube));
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: quem não é membro não consegue sair") {
    CHECK_FALSE(clube.sairDoClube(&artur));
    CHECK_FALSE(artur.sairClube(&clube));
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: admin não sai enquanto houver outros membros") {
    CHECK_FALSE(admin.sairClube(&clube));
    CHECK(clube.isMember(&admin));
    CHECK(admin.participaDe(&clube));
    CHECK(clube.isAdmin(&admin));
}

TEST_CASE("BookClub: admin sozinho pode sair, e o clube é encerrado") {
    User admin("Admin", "admin@email.com", "senha123");
    User raquel("Raquel", "raquel@email.com", "senha123");
    BookClub clube(&admin, "Clube do Machado", "classicos");

    CHECK(admin.sairClube(&clube));
    CHECK_FALSE(clube.isActive());
    CHECK(clube.getAdmin() == nullptr);
    CHECK(clube.getNumberMembers() == 0);
    CHECK_FALSE(raquel.entrarClube(&clube));   // clube encerrado não aceita ninguém
}

TEST_CASE("BookClub: destrutor do clube tira o clube da lista dos membros") {
    User admin("Admin", "admin@email.com", "senha123");
    User raquel("Raquel", "raquel@email.com", "senha123");
    BookClub* clube = new BookClub(&admin, "Clube do Machado", "classicos");
    raquel.entrarClube(clube);

    delete clube;
    CHECK(admin.getCurrentClubs().empty());
    CHECK(raquel.getCurrentClubs().empty());
}

// Entrar em clube sem leitura atual não pode lançar exceção: a métrica de
// leitura só é criada no primeiro check-in, quando o clube já tem um livro.
TEST_CASE_FIXTURE(CenarioClube, "BookClub: entrar em clube sem leitura atual funciona") {
    REQUIRE(clube.getCurrentReading() == nullptr);
    CHECK_NOTHROW(artur.entrarClube(&clube));
    CHECK(artur.participaDe(&clube));
    CHECK(artur.getCurrentPage(&clube) == 0);   // participa, mas ainda sem check-in
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: quem entrou sem leitura faz check-in depois que o clube escolhe o livro") {
    artur.entrarClube(&clube);
    clube.adicionarLivro(&livro1);
    clube.decidirProximaLeitura(&admin, &livro1);

    CHECK(artur.realizarCheckIn(&clube, 50, "Começando") != nullptr);
    CHECK(artur.getCurrentPage(&clube) == 50);
    CHECK(artur.getTotalPages() == 50);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: quando o clube troca de livro, a página do membro recomeça") {
    clube.adicionarLivro(&livro1);
    clube.adicionarLivro(&livro2);
    clube.decidirProximaLeitura(&admin, &livro1);
    raquel.realizarCheckIn(&clube, 50, "Começando");

    clube.decidirProximaLeitura(&admin, &livro2);
    raquel.realizarCheckIn(&clube, 10, "Livro novo");

    CHECK(raquel.getCurrentPage(&clube) == 10);   // conta do zero no livro novo
    CHECK(raquel.getTotalPages() == 60);          // o paginômetro não zera
}

TEST_CASE("BookClub: clube privado aceita quem tem o código certo") {
    User admin("Admin", "admin@email.com", "senha123");
    User raquel("Raquel", "raquel@email.com", "senha123");
    BookClub clube(&admin, "Clube secreto", "classicos", true);

    std::string codigo = clube.getInviteCode(&admin);
    CHECK(raquel.entrarClube(&clube, codigo));
    CHECK(clube.isMember(&raquel));
}

TEST_CASE("BookClub: clube privado recusa código errado ou ausente") {
    User admin("Admin", "admin@email.com", "senha123");
    User raquel("Raquel", "raquel@email.com", "senha123");
    BookClub clube(&admin, "Clube secreto", "classicos", true);

    CHECK_FALSE(raquel.entrarClube(&clube, "codigo-errado"));
    CHECK_FALSE(raquel.entrarClube(&clube));
    CHECK_FALSE(clube.isMember(&raquel));
    CHECK_FALSE(raquel.participaDe(&clube));
}

TEST_CASE("BookClub: clube público ignora o código") {
    User admin("Admin", "admin@email.com", "senha123");
    User raquel("Raquel", "raquel@email.com", "senha123");
    BookClub clube(&admin, "Clube do Machado", "classicos");
    CHECK(raquel.entrarClube(&clube, "qualquer coisa"));
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: só o admin vê o código de convite") {
    CHECK_FALSE(clube.getInviteCode(&admin).empty());
    CHECK(clube.getInviteCode(&raquel) == "");
    CHECK(clube.getInviteCode(&artur) == "");
}

// =====================================================================
// Catálogo
// =====================================================================

TEST_CASE_FIXTURE(CenarioClube, "BookClub: adicionarLivro coloca o livro como futuro") {
    CHECK(clube.adicionarLivro(&livro1));
    REQUIRE(clube.getCatalog().size() == 1);
    CHECK(clube.getCatalog()[0].book == &livro1);
    CHECK(clube.getCatalog()[0].status == BookClub::Status::Futuro);
    CHECK_FALSE(clube.isReading());   // adicionar não começa a leitura
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: adicionar o mesmo livro duas vezes falha") {
    clube.adicionarLivro(&livro1);
    CHECK_FALSE(clube.adicionarLivro(&livro1));
    CHECK(clube.getCatalog().size() == 1);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: removerLivro tira o livro do catálogo") {
    clube.adicionarLivro(&livro1);
    clube.adicionarLivro(&livro2);
    CHECK(clube.removerLivro(&livro1));
    REQUIRE(clube.getCatalog().size() == 1);
    CHECK(clube.getCatalog()[0].book == &livro2);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: remover livro que não está no catálogo falha") {
    CHECK_FALSE(clube.removerLivro(&livro1));
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: remover a leitura atual deixa o clube sem leitura") {
    clube.adicionarLivro(&livro1);
    clube.decidirProximaLeitura(&admin, &livro1);
    CHECK(clube.removerLivro(&livro1));
    CHECK_FALSE(clube.isReading());
    CHECK(clube.getCurrentReading() == nullptr);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: User::sugerirLivro adiciona ao catálogo só se for membro") {
    CHECK(raquel.sugerirLivro(&clube, &livro1));
    CHECK_FALSE(artur.sugerirLivro(&clube, &livro2));
    CHECK(clube.getCatalog().size() == 1);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: decidirProximaLeitura transforma o futuro em atual") {
    clube.adicionarLivro(&livro1);
    CHECK(clube.decidirProximaLeitura(&admin, &livro1));
    CHECK(clube.isReading());
    CHECK(clube.getCurrentReading() == &livro1);
    CHECK(clube.getReadBooks() == 0);   // nenhum livro terminado ainda
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: decidirProximaLeitura passa o atual para passado") {
    clube.adicionarLivro(&livro1);
    clube.adicionarLivro(&livro2);
    clube.decidirProximaLeitura(&admin, &livro1);
    CHECK(clube.decidirProximaLeitura(&admin, &livro2));

    CHECK(clube.getCurrentReading() == &livro2);
    REQUIRE(clube.getCatalog().size() == 2);
    CHECK(clube.getCatalog()[0].status == BookClub::Status::Passado);
    CHECK(clube.getCatalog()[1].status == BookClub::Status::Atual);
    CHECK(clube.getReadBooks() == 1);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: decidirProximaLeitura só aceita livro futuro do catálogo") {
    clube.adicionarLivro(&livro1);
    clube.adicionarLivro(&livro2);
    clube.decidirProximaLeitura(&admin, &livro1);
    clube.decidirProximaLeitura(&admin, &livro2);   // livro1 vira passado

    CHECK_FALSE(clube.decidirProximaLeitura(&admin, &livro1));   // passado
    CHECK_FALSE(clube.decidirProximaLeitura(&admin, &livro2));   // já é o atual
    CHECK_FALSE(clube.decidirProximaLeitura(&admin, &livro3));   // fora do catálogo
    CHECK(clube.getCurrentReading() == &livro2);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: membro vota em livro futuro") {
    clube.adicionarLivro(&livro1);
    CHECK(clube.votarProximaLeitura(&raquel, &livro1));
    CHECK(clube.votarProximaLeitura(&admin, &livro1));
    CHECK(clube.totalVotos(&livro1) == 2);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: votar duas vezes no mesmo livro falha") {
    clube.adicionarLivro(&livro1);
    clube.votarProximaLeitura(&raquel, &livro1);
    CHECK_FALSE(clube.votarProximaLeitura(&raquel, &livro1));
    CHECK(clube.totalVotos(&livro1) == 1);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: votar falha para não membro, livro atual ou fora do catálogo") {
    clube.adicionarLivro(&livro1);
    clube.adicionarLivro(&livro2);
    clube.decidirProximaLeitura(&admin, &livro2);

    CHECK_FALSE(clube.votarProximaLeitura(&artur, &livro1));    // não é membro
    CHECK_FALSE(clube.votarProximaLeitura(&raquel, &livro2));   // livro atual
    CHECK_FALSE(clube.votarProximaLeitura(&raquel, &livro3));   // fora do catálogo
    CHECK(clube.totalVotos(&livro3) == 0);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: decidirProximaLeitura zera os votos de todos os livros") {
    clube.adicionarLivro(&livro1);
    clube.adicionarLivro(&livro2);
    clube.votarProximaLeitura(&raquel, &livro1);
    clube.votarProximaLeitura(&raquel, &livro2);

    clube.decidirProximaLeitura(&admin, &livro1);
    CHECK(clube.totalVotos(&livro1) == 0);
    CHECK(clube.totalVotos(&livro2) == 0);
}

// Cenário auxiliar das avaliações: livro1 já foi lido (passado) e livro2 é o atual.
static void prepararLivroPassado(CenarioClube& c) {
    c.clube.adicionarLivro(&c.livro1);
    c.clube.adicionarLivro(&c.livro2);
    c.clube.decidirProximaLeitura(&c.admin, &c.livro1);
    c.clube.decidirProximaLeitura(&c.admin, &c.livro2);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: membro avalia livro já lido") {
    prepararLivroPassado(*this);
    CHECK(clube.avaliarLivro(&raquel, &livro1, 4));
    CHECK(clube.mediaAvaliacoes(&livro1) == 4);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: média das avaliações considera todos os membros") {
    prepararLivroPassado(*this);
    clube.avaliarLivro(&raquel, &livro1, 4);
    clube.avaliarLivro(&admin, &livro1, 5);
    CHECK(clube.mediaAvaliacoes(&livro1) == doctest::Approx(4.5));
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: avaliar de novo substitui a nota antiga") {
    prepararLivroPassado(*this);
    clube.avaliarLivro(&raquel, &livro1, 2);
    CHECK(clube.avaliarLivro(&raquel, &livro1, 5));
    CHECK(clube.mediaAvaliacoes(&livro1) == 5);
    CHECK(clube.getCatalog()[0].ratings.size() == 1);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: avaliar falha para não membro ou livro que não é passado") {
    prepararLivroPassado(*this);
    clube.adicionarLivro(&livro3);
    CHECK_FALSE(clube.avaliarLivro(&artur, &livro1, 4));    // não é membro
    CHECK_FALSE(clube.avaliarLivro(&raquel, &livro2, 4));   // livro atual
    CHECK_FALSE(clube.avaliarLivro(&raquel, &livro3, 4));   // livro futuro
    CHECK(clube.mediaAvaliacoes(&livro1) == 0);             // ninguém avaliou
}

// =====================================================================
// Administrador
// =====================================================================

TEST_CASE_FIXTURE(CenarioClube, "BookClub: só o admin decide a próxima leitura") {
    clube.adicionarLivro(&livro1);
    CHECK_FALSE(clube.decidirProximaLeitura(&raquel, &livro1));
    CHECK_FALSE(clube.isReading());
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: removerMembro tira o membro dos dois lados") {
    CHECK(clube.removerMembro(&admin, &raquel));
    CHECK_FALSE(clube.isMember(&raquel));
    CHECK_FALSE(raquel.participaDe(&clube));
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: removerMembro falha se quem chama não é admin") {
    artur.entrarClube(&clube);
    CHECK_FALSE(clube.removerMembro(&raquel, &artur));
    CHECK(clube.isMember(&artur));
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: removerMembro falha para não membro ou para o próprio admin") {
    CHECK_FALSE(clube.removerMembro(&admin, &artur));
    CHECK_FALSE(clube.removerMembro(&admin, &admin));
    CHECK(clube.isMember(&admin));
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: check-ins de membro removido continuam no feed (US08)") {
    clube.adicionarLivro(&livro1);
    clube.decidirProximaLeitura(&admin, &livro1);
    CheckIn* c = raquel.realizarCheckIn(&clube, 10, "Comecei");
    REQUIRE(c != nullptr);

    clube.removerMembro(&admin, &raquel);
    REQUIRE(clube.getFeed().size() == 1);
    CHECK(clube.getFeed()[0] == c);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: transferirAdmin passa a administração para outro membro") {
    CHECK(clube.transferirAdmin(&admin, &raquel));
    CHECK(clube.isAdmin(&raquel));
    CHECK_FALSE(clube.isAdmin(&admin));
    CHECK(clube.isMember(&admin));   // o antigo admin continua membro
    CHECK(admin.sairClube(&clube));  // e agora pode sair
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: transferirAdmin falha se quem chama não é admin") {
    CHECK_FALSE(clube.transferirAdmin(&raquel, &raquel));
    CHECK(clube.isAdmin(&admin));
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: transferirAdmin falha para não membro") {
    CHECK_FALSE(clube.transferirAdmin(&admin, &artur));
    CHECK(clube.isAdmin(&admin));
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: admin renomeia o clube") {
    CHECK(clube.renomear(&admin, "Clube da Clarice"));
    CHECK(clube.getName() == "Clube da Clarice");
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: renomear falha se quem chama não é admin") {
    CHECK_FALSE(clube.renomear(&raquel, "Clube da Raquel"));
    CHECK(clube.getName() == "Clube do Machado");
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: admin altera a privacidade") {
    CHECK(clube.alterarPrivacidade(&admin, true));
    CHECK(clube.isPrivate());
    CHECK(clube.alterarPrivacidade(&admin, false));
    CHECK_FALSE(clube.isPrivate());
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: alterarPrivacidade falha se quem chama não é admin") {
    CHECK_FALSE(clube.alterarPrivacidade(&raquel, true));
    CHECK_FALSE(clube.isPrivate());
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: criarConvite gera um código novo e o antigo deixa de valer") {
    clube.alterarPrivacidade(&admin, true);
    std::string antigo = clube.getInviteCode(&admin);
    std::string novo = clube.criarConvite(&admin);

    CHECK_FALSE(novo.empty());
    CHECK(novo != antigo);
    CHECK(clube.getInviteCode(&admin) == novo);
    CHECK_FALSE(artur.entrarClube(&clube, antigo));
    CHECK(artur.entrarClube(&clube, novo));
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: criarConvite falha se quem chama não é admin") {
    std::string antes = clube.getInviteCode(&admin);
    CHECK(clube.criarConvite(&raquel) == "");
    CHECK(clube.getInviteCode(&admin) == antes);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: definirTempoLeitura define os dias restantes") {
    clube.adicionarLivro(&livro1);
    clube.decidirProximaLeitura(&admin, &livro1);
    CHECK(clube.definirTempoLeitura(&admin, 7));
    CHECK(clube.DiasRestantesLeitura() == 7);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: definirTempoLeitura falha sem leitura ou se não é admin") {
    CHECK_FALSE(clube.definirTempoLeitura(&admin, 7));   // sem leitura atual
    clube.adicionarLivro(&livro1);
    clube.decidirProximaLeitura(&admin, &livro1);
    CHECK_FALSE(clube.definirTempoLeitura(&raquel, 7));  // não é admin
    CHECK(clube.DiasRestantesLeitura() == 0);
}

// =====================================================================
// Exceções
// =====================================================================

TEST_CASE("BookClub: construtor com fundador nullptr lança exceção") {
    Book livro("Dom Casmurro", "Machado de Assis", "romance", 300, "01");
    CHECK_THROWS_AS(BookClub(nullptr, "Clube do Machado", "classicos"), std::invalid_argument);
    CHECK_THROWS_AS(BookClub(nullptr, "Clube do Machado", &livro), std::invalid_argument);
}

TEST_CASE("BookClub: construtor com nome vazio lança exceção") {
    User admin("Admin", "admin@email.com", "senha123");
    Book livro("Dom Casmurro", "Machado de Assis", "romance", 300, "01");
    CHECK_THROWS_AS(BookClub(&admin, "", "classicos"), std::invalid_argument);
    CHECK_THROWS_AS(BookClub(&admin, "", &livro), std::invalid_argument);
    CHECK(admin.getCurrentClubs().empty());   // nenhum clube pela metade ficou na lista
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: entrar e sair com usuário nullptr lança exceção") {
    CHECK_THROWS_AS(clube.entrarNoClube(nullptr), std::invalid_argument);
    CHECK_THROWS_AS(clube.sairDoClube(nullptr), std::invalid_argument);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: adicionarCheckIn com nullptr lança exceção") {
    CHECK_THROWS_AS(clube.adicionarCheckIn(nullptr), std::invalid_argument);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: adicionar e remover livro nullptr lança exceção") {
    CHECK_THROWS_AS(clube.adicionarLivro(nullptr), std::invalid_argument);
    CHECK_THROWS_AS(clube.removerLivro(nullptr), std::invalid_argument);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: votar com usuário ou livro nullptr lança exceção") {
    clube.adicionarLivro(&livro1);
    CHECK_THROWS_AS(clube.votarProximaLeitura(nullptr, &livro1), std::invalid_argument);
    CHECK_THROWS_AS(clube.votarProximaLeitura(&raquel, nullptr), std::invalid_argument);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: avaliar com usuário ou livro nullptr lança exceção") {
    prepararLivroPassado(*this);
    CHECK_THROWS_AS(clube.avaliarLivro(nullptr, &livro1, 4), std::invalid_argument);
    CHECK_THROWS_AS(clube.avaliarLivro(&raquel, nullptr, 4), std::invalid_argument);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: nota fora de 1 a 5 lança exceção") {
    prepararLivroPassado(*this);
    CHECK_THROWS_AS(clube.avaliarLivro(&raquel, &livro1, 0), std::invalid_argument);
    CHECK_THROWS_AS(clube.avaliarLivro(&raquel, &livro1, 6), std::invalid_argument);
    CHECK_THROWS_AS(clube.avaliarLivro(&raquel, &livro1, -3), std::invalid_argument);
    CHECK(clube.mediaAvaliacoes(&livro1) == 0);   // nenhuma nota inválida entrou
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: notas 1 e 5 (limites) são aceitas") {
    prepararLivroPassado(*this);
    CHECK(clube.avaliarLivro(&raquel, &livro1, 1));
    CHECK(clube.avaliarLivro(&admin, &livro1, 5));
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: ações de admin com actor nullptr lançam exceção") {
    clube.adicionarLivro(&livro1);
    CHECK_THROWS_AS(clube.criarConvite(nullptr), std::invalid_argument);
    CHECK_THROWS_AS(clube.removerMembro(nullptr, &raquel), std::invalid_argument);
    CHECK_THROWS_AS(clube.decidirProximaLeitura(nullptr, &livro1), std::invalid_argument);
    CHECK_THROWS_AS(clube.alterarPrivacidade(nullptr, true), std::invalid_argument);
    CHECK_THROWS_AS(clube.renomear(nullptr, "Novo nome"), std::invalid_argument);
    CHECK_THROWS_AS(clube.apagarCheckIn(nullptr, nullptr), std::invalid_argument);
    CHECK_THROWS_AS(clube.definirTempoLeitura(nullptr, 7), std::invalid_argument);
    CHECK_THROWS_AS(clube.apagarComentario(nullptr, nullptr), std::invalid_argument);
    CHECK_THROWS_AS(clube.transferirAdmin(nullptr, &raquel), std::invalid_argument);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: ações de admin com alvo nullptr lançam exceção") {
    CHECK_THROWS_AS(clube.removerMembro(&admin, nullptr), std::invalid_argument);
    CHECK_THROWS_AS(clube.transferirAdmin(&admin, nullptr), std::invalid_argument);
    CHECK_THROWS_AS(clube.decidirProximaLeitura(&admin, nullptr), std::invalid_argument);
    CHECK_THROWS_AS(clube.apagarCheckIn(&admin, nullptr), std::invalid_argument);
    CHECK_THROWS_AS(clube.apagarComentario(&admin, nullptr), std::invalid_argument);
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: renomear com nome vazio lança exceção e mantém o nome") {
    CHECK_THROWS_AS(clube.renomear(&admin, ""), std::invalid_argument);
    CHECK(clube.getName() == "Clube do Machado");
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: definirTempoLeitura com dias <= 0 lança exceção") {
    clube.adicionarLivro(&livro1);
    clube.decidirProximaLeitura(&admin, &livro1);
    CHECK_THROWS_AS(clube.definirTempoLeitura(&admin, 0), std::invalid_argument);
    CHECK_THROWS_AS(clube.definirTempoLeitura(&admin, -1), std::invalid_argument);
    CHECK(clube.definirTempoLeitura(&admin, 1));   // 1 dia é o mínimo aceito
}

TEST_CASE_FIXTURE(CenarioClube, "BookClub: setReadPages com valor negativo lança exceção") {
    clube.setReadPages(10);
    CHECK_THROWS_AS(clube.setReadPages(-1), std::invalid_argument);
    CHECK(clube.getReadPages() == 10);
    CHECK_NOTHROW(clube.setReadPages(0));   // zero é permitido
    CHECK(clube.getReadPages() == 0);
}

// =====================================================================
// IDs
// =====================================================================
// O contador proximoId_ é compartilhado por todos os testes, então os testes
// comparam ids entre si em vez de esperar números fixos (como 1, 2...).

TEST_CASE("BookClub: clubes criados sem id recebem ids diferentes e crescentes") {
    User admin("Admin", "admin@email.com", "senha123");
    BookClub primeiro(&admin, "Clube 1", "classicos");
    BookClub segundo(&admin, "Clube 2", "classicos");
    CHECK(primeiro.getId() > 0);
    CHECK(segundo.getId() > primeiro.getId());
}

TEST_CASE("BookClub: os dois construtores respeitam o id informado") {
    User admin("Admin", "admin@email.com", "senha123");
    Book livro("Dom Casmurro", "Machado de Assis", "romance", 300, "01");
    BookClub semLivro(&admin, "Clube 1", "classicos", false, 5000);
    BookClub comLivro(&admin, "Clube 2", &livro, "democratico", false, 5001);
    CHECK(semLivro.getId() == 5000);
    CHECK(comLivro.getId() == 5001);
}

TEST_CASE("BookClub: id gerado depois de um id informado é maior que ele") {
    User admin("Admin", "admin@email.com", "senha123");
    BookClub carregado(&admin, "Clube 1", "classicos", false, 7000);
    BookClub novo(&admin, "Clube 2", "classicos");
    CHECK(novo.getId() > carregado.getId());
}

TEST_CASE("BookClub: construtor com id negativo lança exceção") {
    User admin("Admin", "admin@email.com", "senha123");
    Book livro("Dom Casmurro", "Machado de Assis", "romance", 300, "01");
    CHECK_THROWS_AS(BookClub(&admin, "Clube", "classicos", false, -1), std::invalid_argument);
    CHECK_THROWS_AS(BookClub(&admin, "Clube", &livro, "democratico", false, -1), std::invalid_argument);
    CHECK(admin.getCurrentClubs().empty());   // o clube que falhou não fica na lista
}

TEST_CASE("BookClub: construtor que lança exceção não gasta id") {
    User admin("Admin", "admin@email.com", "senha123");
    BookClub antes(&admin, "Clube 1", "classicos");
    CHECK_THROWS_AS(BookClub(&admin, "", "classicos"), std::invalid_argument);
    BookClub depois(&admin, "Clube 2", "classicos");
    CHECK(depois.getId() == antes.getId() + 1);
}
