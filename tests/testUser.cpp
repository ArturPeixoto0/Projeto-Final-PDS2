/**
 * @file testUser.cpp
 * @brief Testes de unidade da classe User (TDD: escritos antes do User.cpp).
 *
 * Convenções:
 *  - Cada TEST_CASE verifica UMA hipótese sobre UM método.
 *  - Os comentários "DECISÃO:" marcam comportamentos que o .hpp ainda não
 *    define. O grupo precisa confirmar (ou mudar o teste) antes de implementar.
 */
#include "doctest.h"
#include "User.hpp"
#include "BookClub.hpp"
#include "Book.hpp"
#include "CheckIn.hpp"
#include "Comment.hpp"
#include <stdexcept>

/*
 * Cenário reutilizado pelos testes de clube/check-in/livro.
 * Um TEST_CASE_FIXTURE cria um objeto NOVO deste struct para cada teste,
 * então um teste nunca depende de outro (o "I" de FIRST: Isolated).
 *
 * - admin: fundador do clube
 * - raquel: usuária testada, já membro do clube
 * - livro: leitura atual do clube (300 páginas)
 *
 * A ordem dos membros importa: o C++ destrói na ordem inversa, então o clube
 * é destruído antes dos usuários e do livro (ele aponta para eles).
 */
struct CenarioClube {
    User admin;
    User raquel;
    Book livro;
    BookClub clube;
 
    CenarioClube()
        : admin("Admin", "admin@email.com", "senha123"),
          raquel("Raquel", "raquel@email.com", "senha123"),
          livro("Dom Casmurro", "Machado de Assis", "romance", 300, "01"),
          clube(&admin, "Clube do Machado", "classicos") {
        clube.adicionarLivro(&livro);
        clube.decidirProximaLeitura(&admin, &livro);  // livro vira a leitura atual
        raquel.entrarClube(&clube);
    }
};

//conta:
TEST_CASE("User: construtor guarda o nome") {
    User u("Raquel", "raquel@email.com", "senha123");
    CHECK(u.getName() == "Raquel");
}
 
TEST_CASE("User: construtor guarda o email") {
    User u("Raquel", "raquel@email.com", "senha123");
    CHECK(u.getEmail() == "raquel@email.com");
}
 
// DECISÃO: US01 exige nome, email e senha. Aqui assumimos que o construtor
// lança std::invalid_argument quando algum deles é vazio.
TEST_CASE("User: construtor com nome vazio lança exceção") {
    CHECK_THROWS_AS(User("", "raquel@email.com", "senha123"), std::invalid_argument);
}
 
TEST_CASE("User: construtor com email vazio lança exceção") {
    CHECK_THROWS_AS(User("Raquel", "", "senha123"), std::invalid_argument);
}
 
TEST_CASE("User: construtor com senha vazia lança exceção") {
    CHECK_THROWS_AS(User("Raquel", "raquel@email.com", ""), std::invalid_argument);
}
 
TEST_CASE("User: setName altera o nome") {
    User u("Raquel", "raquel@email.com", "senha123");
    u.setName("Raquel Oliveira");
    CHECK(u.getName() == "Raquel Oliveira");
}
 
// DECISÃO: setName("") é ignorado (o nome continua o antigo).
TEST_CASE("User: setName com nome vazio não altera o nome") {
    User u("Raquel", "raquel@email.com", "senha123");
    u.setName("");
    CHECK(u.getName() == "Raquel");
}
 
TEST_CASE("User: checkPassword aceita a senha correta") {
    User u("Raquel", "raquel@email.com", "senha123");
    CHECK(u.checkPassword("senha123"));
}
 
TEST_CASE("User: checkPassword recusa senha errada") {
    User u("Raquel", "raquel@email.com", "senha123");
    CHECK_FALSE(u.checkPassword("senha321"));
}
 
TEST_CASE("User: checkPassword recusa senha vazia") {
    User u("Raquel", "raquel@email.com", "senha123");
    CHECK_FALSE(u.checkPassword(""));
}
 
TEST_CASE("User: checkPassword diferencia maiúsculas de minúsculas") {
    User u("Raquel", "raquel@email.com", "senha123");
    CHECK_FALSE(u.checkPassword("SENHA123"));
}

//estado inicial: 
TEST_CASE("User: usuário novo começa com paginômetro e livrômetro zerados") {
    User u("Raquel", "raquel@email.com", "senha123");
    CHECK(u.getTotalPages() == 0);
    CHECK(u.getTotalBooks() == 0);
}
 
TEST_CASE("User: usuário novo não participa de nenhum clube") {
    User u("Raquel", "raquel@email.com", "senha123");
    CHECK(u.getCurrentClubs().empty());
}
 
TEST_CASE("User: usuário novo não tem check-ins") {
    User u("Raquel", "raquel@email.com", "senha123");
    CHECK(u.getAllCheckIns().empty());
}

//clubes:
TEST_CASE("User: criarClube retorna o clube criado") {
    User u("Raquel", "raquel@email.com", "senha123");
    BookClub* clube = u.criarClube("Clube da Raquel");
    REQUIRE(clube != nullptr);   // REQUIRE: se falhar, não adianta testar o resto
    CHECK(clube->getName() == "Clube da Raquel");
}
 
TEST_CASE("User: quem cria o clube vira administrador dele") {
    User u("Raquel", "raquel@email.com", "senha123");
    BookClub* clube = u.criarClube("Clube da Raquel");
    REQUIRE(clube != nullptr);
    CHECK(clube->isAdmin(&u));
}
 
TEST_CASE("User: quem cria o clube passa a participar dele") {
    User u("Raquel", "raquel@email.com", "senha123");
    BookClub* clube = u.criarClube("Clube da Raquel");
    REQUIRE(clube != nullptr);
    CHECK(u.participaDe(clube));
    CHECK(u.getCurrentClubs().size() == 1);
}
 
// DECISÃO: nome de clube vazio é inválido (mesma regra do BookClub::renomear).
TEST_CASE("User: criarClube com nome vazio retorna nullptr") {
    User u("Raquel", "raquel@email.com", "senha123");
    CHECK(u.criarClube("") == nullptr);
    CHECK(u.getCurrentClubs().empty());
}
 
TEST_CASE("User: entrarClube adiciona o clube à lista do usuário") {
    User admin("Admin", "admin@email.com", "senha123");
    User u("Raquel", "raquel@email.com", "senha123");
    BookClub clube(&admin, "Clube do Machado", "classicos");
 
    CHECK(u.entrarClube(&clube));
    CHECK(u.participaDe(&clube));
    CHECK(u.getCurrentClubs().size() == 1);
}
 
// Os dois lados precisam concordar: o User sabe que está no clube
// E o clube sabe que o User é membro.
TEST_CASE("User: entrarClube também registra o usuário como membro do clube") {
    User admin("Admin", "admin@email.com", "senha123");
    User u("Raquel", "raquel@email.com", "senha123");
    BookClub clube(&admin, "Clube do Machado", "classicos");
 
    u.entrarClube(&clube);
    CHECK(clube.isMember(&u));
}
 
TEST_CASE("User: entrarClube duas vezes no mesmo clube falha na segunda") {
    User admin("Admin", "admin@email.com", "senha123");
    User u("Raquel", "raquel@email.com", "senha123");
    BookClub clube(&admin, "Clube do Machado", "classicos");
 
    u.entrarClube(&clube);
    CHECK_FALSE(u.entrarClube(&clube));
    CHECK(u.getCurrentClubs().size() == 1);
}
 
TEST_CASE("User: entrarClube com nullptr falha") {
    User u("Raquel", "raquel@email.com", "senha123");
    CHECK_FALSE(u.entrarClube(nullptr));
    CHECK(u.getCurrentClubs().empty());
}
 
TEST_CASE("User: usuário pode participar de vários clubes (US07)") {
    User admin("Admin", "admin@email.com", "senha123");
    User u("Raquel", "raquel@email.com", "senha123");
    BookClub clube1(&admin, "Clube do Machado", "classicos");
    BookClub clube2(&admin, "Clube da Clarice", "modernos");
 
    u.entrarClube(&clube1);
    u.entrarClube(&clube2);
    CHECK(u.getCurrentClubs().size() == 2);
    CHECK(u.participaDe(&clube1));
    CHECK(u.participaDe(&clube2));
}
 
TEST_CASE("User: participaDe retorna false para clube em que não entrou") {
    User admin("Admin", "admin@email.com", "senha123");
    User u("Raquel", "raquel@email.com", "senha123");
    BookClub clube(&admin, "Clube do Machado", "classicos");
 
    CHECK_FALSE(u.participaDe(&clube));
}
 
TEST_CASE("User: sairClube remove o clube da lista do usuário") {
    User admin("Admin", "admin@email.com", "senha123");
    User u("Raquel", "raquel@email.com", "senha123");
    BookClub clube(&admin, "Clube do Machado", "classicos");
 
    u.entrarClube(&clube);
    CHECK(u.sairClube(&clube));
    CHECK_FALSE(u.participaDe(&clube));
    CHECK_FALSE(clube.isMember(&u));
}
 
TEST_CASE("User: sairClube de clube em que não participa falha") {
    User admin("Admin", "admin@email.com", "senha123");
    User u("Raquel", "raquel@email.com", "senha123");
    BookClub clube(&admin, "Clube do Machado", "classicos");
 
    CHECK_FALSE(u.sairClube(&clube));
}
 
TEST_CASE("User: administrador não pode sair do próprio clube (US28)") {
    User u("Raquel", "raquel@email.com", "senha123");
    BookClub* clube = u.criarClube("Clube da Raquel");
    REQUIRE(clube != nullptr);
 
    CHECK_FALSE(u.sairClube(clube));
    CHECK(u.participaDe(clube));
}

//check-ins
TEST_CASE_FIXTURE(CenarioClube, "User: realizarCheckIn válido retorna o check-in") {
    CheckIn* c = raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    CHECK(c != nullptr);
}
 
TEST_CASE_FIXTURE(CenarioClube, "User: realizarCheckIn entra no histórico do usuário") {
    raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    CHECK(raquel.getAllCheckIns().size() == 1);
}
 
TEST_CASE_FIXTURE(CenarioClube, "User: realizarCheckIn aparece no feed do clube (US18)") {
    raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    CHECK(clube.getFeed().size() == 1);
}
 
TEST_CASE_FIXTURE(CenarioClube, "User: getCurrentPage retorna a página do último check-in no clube") {
    raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    raquel.realizarCheckIn(&clube, 80, "Bentinho ciumento");
    CHECK(raquel.getCurrentPage(&clube) == 80);
}
 
TEST_CASE_FIXTURE(CenarioClube, "User: getCurrentPage é 0 sem check-ins no clube") {
    CHECK(raquel.getCurrentPage(&clube) == 0);
}
 
TEST_CASE_FIXTURE(CenarioClube, "User: paginômetro soma só as páginas avançadas") {
    raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    raquel.realizarCheckIn(&clube, 80, "Bentinho ciumento");
    CHECK(raquel.getTotalPages() == 80);
}
 
TEST_CASE_FIXTURE(CenarioClube, "User: check-in com finished=true soma 1 no livrômetro") {
    raquel.realizarCheckIn(&clube, 300, "Terminei!", true);
    CHECK(raquel.getTotalBooks() == 1);
}
 
TEST_CASE_FIXTURE(CenarioClube, "User: check-in sem finished não mexe no livrômetro") {
    raquel.realizarCheckIn(&clube, 50, "Capitu é suspeita");
    CHECK(raquel.getTotalBooks() == 0);
}
 
// !! casos negativos (US18: página > 0 e comentário obrigatório) 
 
TEST_CASE_FIXTURE(CenarioClube, "User: realizarCheckIn com página 0 falha") {
    CHECK(raquel.realizarCheckIn(&clube, 0, "texto") == nullptr);
    CHECK(raquel.getAllCheckIns().empty());
}
 
TEST_CASE_FIXTURE(CenarioClube, "User: realizarCheckIn com página negativa falha") {
    CHECK(raquel.realizarCheckIn(&clube, -10, "texto") == nullptr);
    CHECK(raquel.getTotalPages() == 0);
}
 
// não dá para estar numa página maior que o total do livro (300).
TEST_CASE_FIXTURE(CenarioClube, "User: realizarCheckIn com página maior que o livro falha") {
    CHECK(raquel.realizarCheckIn(&clube, 301, "texto") == nullptr);
}
 
// Caso de borda: a última página do livro é válida.
TEST_CASE_FIXTURE(CenarioClube, "User: realizarCheckIn na última página do livro funciona") {
    CHECK(raquel.realizarCheckIn(&clube, 300, "Última página") != nullptr);
}
 
TEST_CASE_FIXTURE(CenarioClube, "User: realizarCheckIn com texto vazio falha") {
    CHECK(raquel.realizarCheckIn(&clube, 50, "") == nullptr);
}
 
TEST_CASE_FIXTURE(CenarioClube, "User: realizarCheckIn com clube nullptr falha") {
    CHECK(raquel.realizarCheckIn(nullptr, 50, "texto") == nullptr);
}
 
TEST_CASE_FIXTURE(CenarioClube, "User: realizarCheckIn em clube do qual não participa falha") {
    User intrusa("Intrusa", "intrusa@email.com", "senha123");
    CHECK(intrusa.realizarCheckIn(&clube, 50, "texto") == nullptr);
    CHECK(clube.getFeed().empty());
}
 
// voltar de página (80 -> 50) é inválido.
TEST_CASE_FIXTURE(CenarioClube, "User: realizarCheckIn com página menor que a anterior falha") {
    raquel.realizarCheckIn(&clube, 80, "texto");
    CHECK(raquel.realizarCheckIn(&clube, 50, "voltei?") == nullptr);
    CHECK(raquel.getCurrentPage(&clube) == 80);
}
 
TEST_CASE_FIXTURE(CenarioClube, "User: realizarCheckIn sem leitura atual no clube falha") {
    BookClub semLeitura(&admin, "Clube sem livro", "variados");
    raquel.entrarClube(&semLeitura);
    CHECK(raquel.realizarCheckIn(&semLeitura, 10, "texto") == nullptr);
}
 
TEST_CASE_FIXTURE(CenarioClube, "User: apagarCheckIn remove o check-in do histórico (US21)") {
    CheckIn* c = raquel.realizarCheckIn(&clube, 50, "postei errado");
    REQUIRE(c != nullptr);
    CHECK(raquel.apagarCheckIn(c));
    CHECK(raquel.getAllCheckIns().empty());
}
 
TEST_CASE_FIXTURE(CenarioClube, "User: apagarCheckIn remove o check-in do feed do clube (US21)") {
    CheckIn* c = raquel.realizarCheckIn(&clube, 50, "postei errado");
    REQUIRE(c != nullptr);
    raquel.apagarCheckIn(c);
    CHECK(clube.getFeed().empty());
}
 
TEST_CASE_FIXTURE(CenarioClube, "User: apagarCheckIn de outro usuário falha (US21)") {
    CheckIn* c = raquel.realizarCheckIn(&clube, 50, "meu check-in");
    REQUIRE(c != nullptr);
    CHECK_FALSE(admin.apagarCheckIn(c));
    CHECK(raquel.getAllCheckIns().size() == 1);
}
 
TEST_CASE("User: apagarCheckIn com nullptr falha") {
    User u("Raquel", "raquel@email.com", "senha123");
    CHECK_FALSE(u.apagarCheckIn(nullptr));
}
//comentarios:
TEST_CASE_FIXTURE(CenarioClube, "User: comentar em check-in retorna o comentário") {
    CheckIn* c = admin.realizarCheckIn(&clube, 30, "Começando!");
    REQUIRE(c != nullptr);
    Comment* com = raquel.comentar(c, "Boa leitura!");
    REQUIRE(com != nullptr);
    CHECK(com->get_text() == "Boa leitura!");
    CHECK(com->get_author() == &raquel);
}
 
TEST_CASE("User: comentar em check-in nullptr falha") {
    User u("Raquel", "raquel@email.com", "senha123");
    CHECK(u.comentar(nullptr, "texto") == nullptr);
}
 
TEST_CASE_FIXTURE(CenarioClube, "User: comentar com texto vazio falha") {
    CheckIn* c = admin.realizarCheckIn(&clube, 30, "Começando!");
    REQUIRE(c != nullptr);
    CHECK(raquel.comentar(c, "") == nullptr);
}

TEST_CASE_FIXTURE(CenarioClube, "User: sugerirLivro adiciona o livro ao catálogo") {
    Book novo("A hora da estrela", "Clarice Lispector", "romance", 88, "02");
    size_t antes = clube.getCatalog().size();
    CHECK(raquel.sugerirLivro(&clube, &novo));
    CHECK(clube.getCatalog().size() == antes + 1);
}
 
TEST_CASE_FIXTURE(CenarioClube, "User: sugerirLivro de livro que já está no catálogo falha") {
    CHECK_FALSE(raquel.sugerirLivro(&clube, &livro));
}
 
TEST_CASE_FIXTURE(CenarioClube, "User: sugerirLivro com livro nullptr falha") {
    CHECK_FALSE(raquel.sugerirLivro(&clube, nullptr));
}
 
TEST_CASE_FIXTURE(CenarioClube, "User: sugerirLivro em clube do qual não participa falha") {
    User intrusa("Intrusa", "intrusa@email.com", "senha123");
    Book novo("A hora da estrela", "Clarice Lispector", "romance", 88, "02");
    CHECK_FALSE(intrusa.sugerirLivro(&clube, &novo));
}
 
// Para avaliar, o livro precisa já ter sido lido (status Passado ou lido).
// Trocar a leitura atual faz o "livro" do cenário virar Passado.
TEST_CASE_FIXTURE(CenarioClube, "User: avaliarLeitura de livro já lido funciona") {
    Book proximo("A hora da estrela", "Clarice Lispector", "romance", 88, "02");
    clube.adicionarLivro(&proximo);
    clube.decidirProximaLeitura(&admin, &proximo);
 
    CHECK(raquel.avaliarLeitura(&clube, &livro, 5));
}
 
// Casos de borda da nota (o BookClub aceita de 1 a 5).
TEST_CASE_FIXTURE(CenarioClube, "User: avaliarLeitura aceita os limites 1 e 5") {
    Book proximo("A hora da estrela", "Clarice Lispector", "romance", 88, "02");
    clube.adicionarLivro(&proximo);
    clube.decidirProximaLeitura(&admin, &proximo);
 
    CHECK(raquel.avaliarLeitura(&clube, &livro, 1));
    CHECK(admin.avaliarLeitura(&clube, &livro, 5));
}
 
TEST_CASE_FIXTURE(CenarioClube, "User: avaliarLeitura com nota fora de 1 a 5 falha") {
    Book proximo("A hora da estrela", "Clarice Lispector", "romance", 88, "02");
    clube.adicionarLivro(&proximo);
    clube.decidirProximaLeitura(&admin, &proximo);
 
    CHECK_FALSE(raquel.avaliarLeitura(&clube, &livro, 0));
    CHECK_FALSE(raquel.avaliarLeitura(&clube, &livro, 6));
    CHECK_FALSE(raquel.avaliarLeitura(&clube, &livro, -1));
}
 
TEST_CASE_FIXTURE(CenarioClube, "User: avaliarLeitura de livro ainda não terminado falha") {
    CHECK_FALSE(raquel.avaliarLeitura(&clube, &livro, 4));  // livro ainda é o atual
}
