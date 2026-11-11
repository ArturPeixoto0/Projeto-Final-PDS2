/**
 * @file testBancoDeDados.cpp
 * @brief Testes de unidade da classe BancoDeDados.
 *
 * Os testes gravam arquivos de verdade na pasta tests/dadosTeste/ (os .json
 * dali estão no .gitignore). O caminho é relativo: os testes precisam rodar
 * a partir da raiz do repositório, como o "make test" já faz.
 *
 * Padrão da maioria dos testes: salvar num banco, carregar num banco NOVO
 * da mesma pasta e comparar os campos. Os ids são comparados entre os
 * objetos (os contadores static continuam crescendo entre os testes, então
 * nunca esperamos números fixos).
 */
#include "doctest.h"
#include "BancoDeDados.hpp"
#include "Book.hpp"
#include "User.hpp"
#include "UserProfile.hpp"
#include <fstream>
#include <stdexcept>
#include <string>

namespace {
const std::string PASTA_TESTE = "tests/dadosTeste/";

// Deixa a pasta de teste "zerada": um banco vazio, ao salvar, reescreve os
// arquivos com listas vazias ([]). Chamado no começo de cada teste, para um
// teste não enxergar o que o anterior salvou.
void limparPastaDeTeste() {
    BancoDeDados vazio(PASTA_TESTE);
    vazio.salvarTudo();
}

// Escreve um texto qualquer num arquivo da pasta de teste (para simular um
// arquivo quebrado).
void escreverArquivo(const std::string& nome, const std::string& conteudo) {
    std::ofstream arquivo(PASTA_TESTE + nome);
    arquivo << conteudo;
}
} // namespace

// ------------------------------ MEMÓRIA ------------------------------

TEST_CASE("BancoDeDados: banco novo começa vazio") {
    BancoDeDados banco(PASTA_TESTE);
    CHECK(banco.getLivros().empty());
    CHECK(banco.getUsuarios().empty());
    CHECK(banco.getClubes().empty());
}

TEST_CASE("BancoDeDados: adicionar e buscar livros e usuários") {
    BancoDeDados banco(PASTA_TESTE);
    Book* livro = new Book("Dom Casmurro", "Machado de Assis", "romance", 300, "01");
    User* raquel = new User("Raquel", "raquel@email.com", "senha123");
    banco.adicionarLivro(livro);     // o banco vira dono
    banco.adicionarUsuario(raquel);  // o banco vira dono

    CHECK(banco.getLivros().size() == 1);
    CHECK(banco.getUsuarios().size() == 1);
    CHECK(banco.buscarLivro(livro->getId()) == livro);
    CHECK(banco.buscarUsuario(raquel->getId()) == raquel);
    CHECK(banco.buscarUsuarioPorEmail("raquel@email.com") == raquel);
}

TEST_CASE("BancoDeDados: busca que não acha devolve nullptr") {
    BancoDeDados banco(PASTA_TESTE);
    CHECK(banco.buscarLivro(123456) == nullptr);
    CHECK(banco.buscarUsuario(123456) == nullptr);
    CHECK(banco.buscarClube(123456) == nullptr);
    CHECK(banco.buscarUsuarioPorEmail("ninguem@email.com") == nullptr);
}

TEST_CASE("BancoDeDados: adicionar nulo lança exceção") {
    BancoDeDados banco(PASTA_TESTE);
    CHECK_THROWS_AS(banco.adicionarLivro(nullptr), std::invalid_argument);
    CHECK_THROWS_AS(banco.adicionarUsuario(nullptr), std::invalid_argument);
}

TEST_CASE("BancoDeDados: adicionar id repetido lança exceção e não muda nada") {
    BancoDeDados banco(PASTA_TESTE);
    Book* livro = new Book("Dom Casmurro", "Machado de Assis", "romance", 300, "01");
    User* raquel = new User("Raquel", "raquel@email.com", "senha123");
    banco.adicionarLivro(livro);
    banco.adicionarUsuario(raquel);

    Book* mesmoIdLivro = new Book("Outro", "Autor", "genero", 10, "02", livro->getId());
    User* mesmoIdUser = new User("Outra", "outra@email.com", "senha", raquel->getId());
    CHECK_THROWS_AS(banco.adicionarLivro(mesmoIdLivro), std::invalid_argument);
    CHECK_THROWS_AS(banco.adicionarUsuario(mesmoIdUser), std::invalid_argument);
    CHECK(banco.getLivros().size() == 1);
    CHECK(banco.getUsuarios().size() == 1);

    // Lançaram exceção: o banco não virou dono, então o teste apaga.
    delete mesmoIdLivro;
    delete mesmoIdUser;
}

// ------------------------------ ARQUIVOS ------------------------------

TEST_CASE("BancoDeDados: livros salvos são carregados iguais, com os mesmos ids") {
    limparPastaDeTeste();
    int idDom = 0;
    int idHora = 0;
    {
        // Bloco só para o primeiro banco ser destruído antes de carregar.
        BancoDeDados banco(PASTA_TESTE);
        Book* dom = new Book("Dom Casmurro", "Machado de Assis", "romance", 300, "978-01");
        Book* hora = new Book("A hora da estrela", "Clarice Lispector", "novela", 88, "978-02");
        banco.adicionarLivro(dom);
        banco.adicionarLivro(hora);
        idDom = dom->getId();
        idHora = hora->getId();
        banco.salvarTudo();
    }

    BancoDeDados carregado(PASTA_TESTE);
    carregado.carregarTudo();
    REQUIRE(carregado.getLivros().size() == 2);

    Book* dom = carregado.buscarLivro(idDom);
    REQUIRE(dom != nullptr);
    CHECK(dom->getTitle() == "Dom Casmurro");
    CHECK(dom->getAuthor() == "Machado de Assis");
    CHECK(dom->getGenre() == "romance");
    CHECK(dom->getPages() == 300);
    CHECK(dom->getISBN() == "978-01");

    Book* hora = carregado.buscarLivro(idHora);
    REQUIRE(hora != nullptr);
    CHECK(hora->getTitle() == "A hora da estrela");
    CHECK(hora->getPages() == 88);
    // A ordem da lista também se mantém.
    CHECK(carregado.getLivros()[0] == dom);
    CHECK(carregado.getLivros()[1] == hora);
}

TEST_CASE("BancoDeDados: usuários salvos são carregados com perfil, totais e senha") {
    limparPastaDeTeste();
    int idRaquel = 0;
    {
        BancoDeDados banco(PASTA_TESTE);
        User* raquel = new User("Raquel", "raquel@email.com", "senha123");
        raquel->getProfile().setDescription("Leio de tudo um pouco");
        raquel->getProfile().setPhotoPath("fotos/raquel.png");
        raquel->getProfile().setHeaderPath("fotos/capa.png");
        raquel->restaurarTotais(1500, 5);
        banco.adicionarUsuario(raquel);
        idRaquel = raquel->getId();
        banco.salvarTudo();
    }

    BancoDeDados carregado(PASTA_TESTE);
    carregado.carregarTudo();
    REQUIRE(carregado.getUsuarios().size() == 1);

    User* raquel = carregado.buscarUsuario(idRaquel);
    REQUIRE(raquel != nullptr);
    CHECK(raquel->getName() == "Raquel");
    CHECK(raquel->getEmail() == "raquel@email.com");
    CHECK(raquel->getProfile().getDescription() == "Leio de tudo um pouco");
    CHECK(raquel->getProfile().getPhotoPath() == "fotos/raquel.png");
    CHECK(raquel->getProfile().getHeaderPath() == "fotos/capa.png");
    CHECK(raquel->getTotalPages() == 1500);
    CHECK(raquel->getTotalBooks() == 5);
    CHECK(carregado.buscarUsuarioPorEmail("raquel@email.com") == raquel);
}

TEST_CASE("BancoDeDados: checkPassword funciona depois da carga") {
    limparPastaDeTeste();
    {
        BancoDeDados banco(PASTA_TESTE);
        banco.adicionarUsuario(new User("Raquel", "raquel@email.com", "senha123"));
        banco.salvarTudo();
    }

    BancoDeDados carregado(PASTA_TESTE);
    carregado.carregarTudo();
    User* raquel = carregado.buscarUsuarioPorEmail("raquel@email.com");
    REQUIRE(raquel != nullptr);
    CHECK(raquel->checkPassword("senha123"));
    CHECK_FALSE(raquel->checkPassword("errada"));
}

TEST_CASE("BancoDeDados: livro criado depois da carga recebe id maior que os carregados") {
    limparPastaDeTeste();
    {
        BancoDeDados banco(PASTA_TESTE);
        // Um id bem alto, como se viesse de uma execução anterior longa.
        banco.adicionarLivro(new Book("Dom Casmurro", "Machado de Assis", "romance", 300, "01", 900000));
        banco.salvarTudo();
    }

    BancoDeDados carregado(PASTA_TESTE);
    carregado.carregarTudo();
    REQUIRE(carregado.buscarLivro(900000) != nullptr);
    Book* novo = new Book("A hora da estrela", "Clarice Lispector", "novela", 88, "02");
    carregado.adicionarLivro(novo);
    CHECK(novo->getId() > 900000);
}

TEST_CASE("BancoDeDados: salvar um banco vazio grava listas vazias") {
    limparPastaDeTeste();
    BancoDeDados carregado(PASTA_TESTE);
    carregado.carregarTudo();
    CHECK(carregado.getLivros().empty());
    CHECK(carregado.getUsuarios().empty());
}

// ------------------------------ ERROS ------------------------------

TEST_CASE("BancoDeDados: pasta sem arquivos começa vazia, sem erro") {
    BancoDeDados banco("tests/pastaQueNaoExiste/");
    CHECK_NOTHROW(banco.carregarTudo());
    CHECK(banco.getLivros().empty());
    CHECK(banco.getUsuarios().empty());
}

TEST_CASE("BancoDeDados: JSON quebrado lança exceção") {
    limparPastaDeTeste();
    escreverArquivo("books.json", "{quebrado");
    BancoDeDados banco(PASTA_TESTE);
    // A exceção vem do nlohmann; todas as dele derivam de std::exception.
    CHECK_THROWS_AS(banco.carregarTudo(), std::exception);
    limparPastaDeTeste();
}

TEST_CASE("BancoDeDados: arquivo que não é lista lança runtime_error") {
    limparPastaDeTeste();
    escreverArquivo("books.json", "{\"titulo\": \"Dom Casmurro\"}");
    BancoDeDados banco(PASTA_TESTE);
    CHECK_THROWS_AS(banco.carregarTudo(), std::runtime_error);
    limparPastaDeTeste();
}

TEST_CASE("BancoDeDados: id repetido no arquivo lança exceção") {
    limparPastaDeTeste();
    escreverArquivo("books.json",
        "[{\"id\": 5, \"titulo\": \"A\", \"autor\": \"\", \"genero\": \"\", \"paginas\": 10, \"isbn\": \"\"},"
        " {\"id\": 5, \"titulo\": \"B\", \"autor\": \"\", \"genero\": \"\", \"paginas\": 10, \"isbn\": \"\"}]");
    BancoDeDados banco(PASTA_TESTE);
    CHECK_THROWS_AS(banco.carregarTudo(), std::invalid_argument);
    CHECK(banco.getLivros().size() == 1);   // o primeiro entrou antes do erro
    limparPastaDeTeste();
}

TEST_CASE("BancoDeDados: salvar em pasta inexistente lança runtime_error") {
    BancoDeDados banco("tests/pastaQueNaoExiste/");
    CHECK_THROWS_AS(banco.salvarTudo(), std::runtime_error);
}

TEST_CASE("BancoDeDados: carregarTudo com o banco já preenchido lança logic_error") {
    limparPastaDeTeste();
    BancoDeDados banco(PASTA_TESTE);
    banco.adicionarLivro(new Book("Dom Casmurro", "Machado de Assis", "romance", 300, "01"));
    CHECK_THROWS_AS(banco.carregarTudo(), std::logic_error);
    CHECK(banco.getLivros().size() == 1);
}
