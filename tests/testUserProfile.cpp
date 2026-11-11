/**
 * @file testUserProfile.cpp
 * @brief Testes de unidade da classe UserProfile (TDD: escritos antes do UserProfile.cpp).
 *
 * O UserProfile não guarda dados próprios: ele "espelha" o User dono dele.
 * Por isso a maioria dos testes segue o mesmo padrão: muda algo no User e
 * confere se o perfil mostra o mesmo valor (US27: "alterações no perfil são
 * refletidas imediatamente").
 *
 * Os métodos show_* imprimem na tela; para testá-los, a saída do cout é
 * captudara com a struct CapturaSaida (mesma ideia do testClubProfile).
 */
#include "doctest.h"
#include "UserProfile.hpp"
#include "User.hpp"
#include "BookClub.hpp"
#include "Book.hpp"
#include "CheckIn.hpp"
#include "ReadingMetrics.hpp"
#include <type_traits>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <string>

TEST_CASE("UserProfile: é um Profile") {
    CHECK(std::is_base_of<Profile, UserProfile>::value);
}

TEST_CASE("UserProfile: Construtor recebendo ponteiro nulo") {
    CHECK_THROWS_AS(UserProfile(nullptr), std::invalid_argument);
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


// IDs do User: ficam aqui porque o testUser.cpp está sendo atualizado por
// outra pessoa do grupo. O contador proximoId é compartilhado por todos os
// testes, então os testes comparam ids entre si em vez de esperar números fixos.

TEST_CASE("User: usuários criados sem id recebem ids diferentes e crescentes") {
    User primeiro("Raquel", "raquel@email.com", "senha123");
    User segundo("Artur", "artur@email.com", "senha123");
    CHECK(primeiro.getId() > 0);
    CHECK(segundo.getId() > primeiro.getId());
}

TEST_CASE("User: construtor respeita o id informado") {
    User u("Raquel", "raquel@email.com", "senha123", 5000);
    CHECK(u.getId() == 5000);
}

TEST_CASE("User: id gerado depois de um id informado é maior que ele") {
    User carregado("Raquel", "raquel@email.com", "senha123", 7000);
    User novo("Artur", "artur@email.com", "senha123");
    CHECK(novo.getId() > carregado.getId());
}

TEST_CASE("User: construtor com id negativo lança exceção") {
    CHECK_THROWS_AS(User("Raquel", "raquel@email.com", "senha123", -1), std::invalid_argument);
}

// Restauração do User (usada pelo BancoDeDados na carga dos arquivos).
// Também fica aqui pelo mesmo motivo dos IDs.

TEST_CASE("User: getPassword devolve a senha da conta") {
    User u("Raquel", "raquel@email.com", "senha123");
    CHECK(u.getPassword() == "senha123");
    CHECK(u.checkPassword(u.getPassword()));
}

TEST_CASE_FIXTURE(CenarioPerfil, "User: getMetrica devolve nullptr antes do primeiro check-in") {
    CHECK(raquel.getMetrica(&clube1) == nullptr);
}

TEST_CASE_FIXTURE(CenarioPerfil, "User: getMetrica devolve a métrica do clube depois do check-in") {
    raquel.realizarCheckIn(&clube1, 50, "Capitu é suspeita");
    ReadingMetrics* m = raquel.getMetrica(&clube1);
    REQUIRE(m != nullptr);
    CHECK(m->getReading() == &livro1);
    CHECK(m->getCurrentPage() == 50);
    CHECK(raquel.getMetrica(&clube2) == nullptr);   // no outro clube ainda não leu
}

TEST_CASE_FIXTURE(CenarioPerfil, "User: getMetrica com clube nulo lança exceção") {
    CHECK_THROWS_AS(raquel.getMetrica(nullptr), std::invalid_argument);
}

TEST_CASE("User: restaurarTotais coloca de volta paginômetro e livrômetro") {
    User u("Raquel", "raquel@email.com", "senha123");
    u.restaurarTotais(1200, 4);
    CHECK(u.getTotalPages() == 1200);
    CHECK(u.getTotalBooks() == 4);
}

TEST_CASE("User: restaurarTotais com valor negativo lança exceção e não muda nada") {
    User u("Raquel", "raquel@email.com", "senha123");
    CHECK_THROWS_AS(u.restaurarTotais(-1, 0), std::invalid_argument);
    CHECK_THROWS_AS(u.restaurarTotais(0, -1), std::invalid_argument);
    CHECK(u.getTotalPages() == 0);
    CHECK(u.getTotalBooks() == 0);
}

TEST_CASE_FIXTURE(CenarioPerfil, "User: restaurarMetrica coloca a métrica sem mexer nos totais") {
    ReadingMetrics* m = new ReadingMetrics(&livro1, 120, -1, false);   // a raquel vira dona
    raquel.restaurarMetrica(&clube1, m);
    CHECK(raquel.getMetrica(&clube1) == m);
    CHECK(raquel.getCurrentPage(&clube1) == 120);
    CHECK(raquel.getTotalPages() == 0);   // os totais vêm do restaurarTotais
}

TEST_CASE_FIXTURE(CenarioPerfil, "User: restaurarMetrica troca a métrica antiga pela nova") {
    raquel.realizarCheckIn(&clube1, 50, "Capitu é suspeita");   // cria a métrica antiga
    ReadingMetrics* nova = new ReadingMetrics(&livro1, 200, -1, false);
    raquel.restaurarMetrica(&clube1, nova);   // a antiga é apagada pela raquel
    CHECK(raquel.getMetrica(&clube1) == nova);
    CHECK(raquel.getCurrentPage(&clube1) == 200);
}

TEST_CASE_FIXTURE(CenarioPerfil, "User: restaurarMetrica com nulo ou fora do clube lança exceção") {
    User fora("Fora", "fora@email.com", "senha123");
    ReadingMetrics* m = new ReadingMetrics(&livro1, 120, -1, false);
    CHECK_THROWS_AS(raquel.restaurarMetrica(nullptr, m), std::invalid_argument);
    CHECK_THROWS_AS(raquel.restaurarMetrica(&clube1, nullptr), std::invalid_argument);
    CHECK_THROWS_AS(fora.restaurarMetrica(&clube1, m), std::invalid_argument);
    // Como todas lançaram, ninguém virou dono de m: o teste apaga.
    delete m;
}

TEST_CASE_FIXTURE(CenarioPerfil, "User: restaurarCheckIn coloca no histórico e no feed sem somar páginas") {
    CheckIn* c = new CheckIn(&raquel, &livro1, &clube1, 120, "Capitu é suspeita");   // a raquel vira dona
    raquel.restaurarCheckIn(c);

    REQUIRE(raquel.getAllCheckIns().size() == 1);
    CHECK(raquel.getAllCheckIns()[0] == c);
    REQUIRE(clube1.getFeed().size() == 1);
    CHECK(clube1.getFeed()[0] == c);
    // Diferente de realizarCheckIn: nada de páginas, totais nem métrica.
    CHECK(raquel.getTotalPages() == 0);
    CHECK(clube1.getReadPages() == 0);
    CHECK(raquel.getMetrica(&clube1) == nullptr);
}

TEST_CASE_FIXTURE(CenarioPerfil, "User: restaurarCheckIn funciona para quem já saiu do clube") {
    raquel.sairClube(&clube1);
    CheckIn* c = new CheckIn(&raquel, &livro1, &clube1, 120, "li antes de sair");
    raquel.restaurarCheckIn(c);
    REQUIRE(clube1.getFeed().size() == 1);
    CHECK(clube1.getFeed()[0] == c);
    CHECK(raquel.getAllCheckIns().size() == 1);
}

TEST_CASE_FIXTURE(CenarioPerfil, "User: restaurarCheckIn recusa nulo, check-in de outro usuário e repetido") {
    CHECK_THROWS_AS(raquel.restaurarCheckIn(nullptr), std::invalid_argument);

    CheckIn* doAdmin = new CheckIn(&admin, &livro1, &clube1, 10, "texto");
    CHECK_THROWS_AS(raquel.restaurarCheckIn(doAdmin), std::invalid_argument);
    delete doAdmin;   // lançou: ninguém virou dono

    CheckIn* c = new CheckIn(&raquel, &livro1, &clube1, 10, "texto");
    raquel.restaurarCheckIn(c);
    CHECK_THROWS_AS(raquel.restaurarCheckIn(c), std::invalid_argument);   // o feed recusa
    CHECK(raquel.getAllCheckIns().size() == 1);   // não entrou duas vezes
}

// ------------------------------ SHOW_* ------------------------------

namespace {
/*
 * Captura tudo o que for impresso no cout enquanto o objeto existir.
 * No construtor, troca o "destino" do cout pela nossa stringstream;
 * no destrutor, devolve o destino antigo (mesmo se o teste falhar).
 */
struct CapturaSaida {
    std::stringstream buffer;
    std::streambuf* antigo;

    CapturaSaida() : antigo(std::cout.rdbuf(buffer.rdbuf())) {}
    ~CapturaSaida() { std::cout.rdbuf(antigo); }

    std::string str() const { return buffer.str(); }
};
} // namespace

TEST_CASE("UserProfile: show_name imprime o nome do usuário") {
    User u("Raquel", "raquel@email.com", "senha123");
    CapturaSaida saida;
    u.getProfile().show_name();
    CHECK(saida.str() == "Raquel\n");
}

TEST_CASE("UserProfile: show_pfp sem foto avisa que não tem foto") {
    User u("Raquel", "raquel@email.com", "senha123");
    CapturaSaida saida;
    u.getProfile().show_pfp();
    CHECK(saida.str() == "(sem foto de perfil)\n");
}

TEST_CASE("UserProfile: show_pfp com foto imprime o caminho") {
    User u("Raquel", "raquel@email.com", "senha123");
    u.getProfile().setPhotoPath("fotos/raquel.png");
    CapturaSaida saida;
    u.getProfile().show_pfp();
    CHECK(saida.str() == "Foto: fotos/raquel.png\n");
}

TEST_CASE("UserProfile: show_description sem descrição avisa que não tem") {
    User u("Raquel", "raquel@email.com", "senha123");
    CapturaSaida saida;
    u.getProfile().show_description();
    CHECK(saida.str() == "(sem descricao)\n");
}

TEST_CASE("UserProfile: show_description com descrição imprime a descrição") {
    User u("Raquel", "raquel@email.com", "senha123");
    u.getProfile().setDescription("Leio um livro por mês");
    CapturaSaida saida;
    u.getProfile().show_description();
    CHECK(saida.str() == "Leio um livro por mês\n");
}

TEST_CASE("UserProfile: show_clubs sem clubes avisa que não participa de nenhum") {
    User u("Raquel", "raquel@email.com", "senha123");
    CapturaSaida saida;
    u.getProfile().show_clubs();
    CHECK(saida.str() == "Nao participa de nenhum clube.\n");
}

TEST_CASE_FIXTURE(CenarioPerfil, "UserProfile: show_clubs lista os clubes do usuário") {
    CapturaSaida saida;
    raquel.getProfile().show_clubs();
    CHECK(saida.str() == "- Clube do Machado\n- Clube da Clarice\n");
}

TEST_CASE_FIXTURE(CenarioPerfil, "UserProfile: show_clubs marca os clubes em que o usuário é admin") {
    CapturaSaida saida;
    admin.getProfile().show_clubs();
    CHECK(saida.str() == "- Clube do Machado (admin)\n- Clube da Clarice (admin)\n");
}

TEST_CASE("UserProfile: show_checkins sem check-ins avisa que não tem nenhum") {
    User u("Raquel", "raquel@email.com", "senha123");
    CapturaSaida saida;
    u.getProfile().show_checkins();
    CHECK(saida.str() == "Nenhum check-in ainda.\n");
}

// A data e a hora mudam a cada execução, então o teste confere só os pedaços
// fixos da linha com find (find devolve std::string::npos se não achar).
TEST_CASE_FIXTURE(CenarioPerfil, "UserProfile: show_checkins mostra clube, livro, página e texto") {
    raquel.realizarCheckIn(&clube1, 50, "Capitu é meio suspeita");
    CapturaSaida saida;
    raquel.getProfile().show_checkins();
    CHECK(saida.str().find("Clube do Machado | Dom Casmurro | pagina 50: Capitu é meio suspeita") != std::string::npos);
}

TEST_CASE_FIXTURE(CenarioPerfil, "UserProfile: show_checkins mostra do mais novo para o mais antigo") {
    raquel.realizarCheckIn(&clube1, 50, "primeiro");
    raquel.realizarCheckIn(&clube2, 20, "segundo");
    CapturaSaida saida;
    raquel.getProfile().show_checkins();
    std::string texto = saida.str();
    REQUIRE(texto.find("segundo") != std::string::npos);
    REQUIRE(texto.find("primeiro") != std::string::npos);
    CHECK(texto.find("segundo") < texto.find("primeiro"));
}