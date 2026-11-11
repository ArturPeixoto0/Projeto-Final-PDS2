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
#include "BookClub.hpp"
#include "ReadingMetrics.hpp"
#include "UserProfile.hpp"
#include <ctime>
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

// ------------------------------ CLUBES E MÉTRICAS ------------------------------

namespace {
// Ids e valores do cenário salvo, para os testes conferirem depois da carga.
struct CenarioSalvo {
    int idDom, idHora, idSertao;
    int idJulia, idRaquel, idArtur;
    int idLeitoras, idSecreto;
    std::string codigoSecreto;
    std::time_t criacaoLeitoras, inicioLeitoras, fimLeitoras;
    int paginasLeitoras;
};

/*
 * Monta o cenário pelas ações NORMAIS das entidades e salva:
 *  - "Leitoras de Quinta" (público, admin julia, membros julia, raquel, artur):
 *    dom = Passado (raquel deu nota 4), hora = Atual (prazo de 10 dias),
 *    sertao = Futuro (artur votou). raquel está na página 50; artur terminou
 *    (página 88) e deu nota 4.5 na métrica; julia ainda não fez check-in.
 *  - "Clube Secreto" (privado, criado pela raquel, que passou a administração
 *    para a julia): membros raquel e julia, sem leitura.
 */
CenarioSalvo salvarCenarioDeClubes() {
    limparPastaDeTeste();
    CenarioSalvo salvo;
    BancoDeDados banco(PASTA_TESTE);

    Book* dom = new Book("Dom Casmurro", "Machado de Assis", "romance", 300, "01");
    Book* hora = new Book("A hora da estrela", "Clarice Lispector", "novela", 88, "02");
    Book* sertao = new Book("Grande Sertao: Veredas", "Guimaraes Rosa", "romance", 624, "03");
    banco.adicionarLivro(dom);
    banco.adicionarLivro(hora);
    banco.adicionarLivro(sertao);

    User* julia = new User("Julia", "julia@email.com", "123");
    User* raquel = new User("Raquel", "raquel@email.com", "123");
    User* artur = new User("Artur", "artur@email.com", "123");
    banco.adicionarUsuario(julia);
    banco.adicionarUsuario(raquel);
    banco.adicionarUsuario(artur);

    BookClub* leitoras = julia->criarClube("Leitoras de Quinta", dom);   // dom vira Atual
    banco.adicionarClube(leitoras);
    leitoras->adicionarLivro(hora);
    leitoras->decidirProximaLeitura(julia, hora);   // dom vira Passado, hora vira Atual
    leitoras->adicionarLivro(sertao);               // sertao fica Futuro
    leitoras->definirTempoLeitura(julia, 10);
    raquel->entrarClube(leitoras);
    artur->entrarClube(leitoras);
    leitoras->avaliarLivro(raquel, dom, 4);
    leitoras->votarProximaLeitura(artur, sertao);
    raquel->realizarCheckIn(leitoras, 50, "Comecei hoje");
    artur->realizarCheckIn(leitoras, 88, "Terminei!", true);
    artur->avaliarLeitura(leitoras, 4.5);

    BookClub* secreto = new BookClub(raquel, "Clube Secreto", "classicos", true);
    banco.adicionarClube(secreto);
    julia->entrarClube(secreto, secreto->getInviteCode(raquel));
    secreto->transferirAdmin(raquel, julia);   // o admin deixa de ser o fundador

    salvo.idDom = dom->getId();
    salvo.idHora = hora->getId();
    salvo.idSertao = sertao->getId();
    salvo.idJulia = julia->getId();
    salvo.idRaquel = raquel->getId();
    salvo.idArtur = artur->getId();
    salvo.idLeitoras = leitoras->getId();
    salvo.idSecreto = secreto->getId();
    salvo.codigoSecreto = secreto->getInviteCode(julia);
    salvo.criacaoLeitoras = leitoras->getClubCreation();
    salvo.inicioLeitoras = leitoras->getStartReading();
    salvo.fimLeitoras = leitoras->getEndReading();
    salvo.paginasLeitoras = leitoras->getReadPages();

    banco.salvarTudo();
    return salvo;
}

// Procura o status de um livro no catálogo do clube. Devolve false se o
// livro não estiver no catálogo.
bool statusNoCatalogo(const BookClub* clube, const Book* livro, BookClub::Status& status) {
    const std::vector<BookClub::CatalogEntry>& catalogo = clube->getCatalog();
    for (int i = 0; i < (int)catalogo.size(); i++) {
        if (catalogo[i].book == livro) {
            status = catalogo[i].status;
            return true;
        }
    }
    return false;
}
} // namespace

TEST_CASE("BancoDeDados: clubes voltam com nome, tipo, privacidade, admin e membros") {
    CenarioSalvo salvo = salvarCenarioDeClubes();
    BancoDeDados banco(PASTA_TESTE);
    banco.carregarTudo();
    REQUIRE(banco.getClubes().size() == 2);

    User* julia = banco.buscarUsuario(salvo.idJulia);
    User* raquel = banco.buscarUsuario(salvo.idRaquel);
    User* artur = banco.buscarUsuario(salvo.idArtur);
    REQUIRE(julia != nullptr);
    REQUIRE(raquel != nullptr);
    REQUIRE(artur != nullptr);

    BookClub* leitoras = banco.buscarClube(salvo.idLeitoras);
    REQUIRE(leitoras != nullptr);
    CHECK(leitoras->getName() == "Leitoras de Quinta");
    CHECK(leitoras->getType() == "democratico");
    CHECK_FALSE(leitoras->isPrivate());
    CHECK(leitoras->isActive());
    CHECK(leitoras->isAdmin(julia));
    CHECK(leitoras->getNumberMembers() == 3);
    CHECK(leitoras->isMember(julia));
    CHECK(leitoras->isMember(raquel));
    CHECK(leitoras->isMember(artur));

    BookClub* secreto = banco.buscarClube(salvo.idSecreto);
    REQUIRE(secreto != nullptr);
    CHECK(secreto->isPrivate());
    CHECK(secreto->isAdmin(julia));   // a administração transferida continua
    CHECK(secreto->getNumberMembers() == 2);
    CHECK(secreto->isMember(raquel));
    CHECK_FALSE(secreto->isMember(artur));
    CHECK(secreto->getInviteCode(julia) == salvo.codigoSecreto);
    CHECK_FALSE(secreto->isReading());

    // O lado do User também foi atualizado (entrarClube sincroniza os dois).
    CHECK(raquel->participaDe(leitoras));
    CHECK(raquel->participaDe(secreto));
    CHECK_FALSE(artur->participaDe(secreto));
}

TEST_CASE("BancoDeDados: catálogo volta com os três status, notas e votos") {
    CenarioSalvo salvo = salvarCenarioDeClubes();
    BancoDeDados banco(PASTA_TESTE);
    banco.carregarTudo();

    BookClub* leitoras = banco.buscarClube(salvo.idLeitoras);
    Book* dom = banco.buscarLivro(salvo.idDom);
    Book* hora = banco.buscarLivro(salvo.idHora);
    Book* sertao = banco.buscarLivro(salvo.idSertao);
    REQUIRE(leitoras != nullptr);
    REQUIRE(leitoras->getCatalog().size() == 3);

    BookClub::Status status = BookClub::Status::Futuro;
    REQUIRE(statusNoCatalogo(leitoras, dom, status));
    CHECK(status == BookClub::Status::Passado);
    REQUIRE(statusNoCatalogo(leitoras, hora, status));
    CHECK(status == BookClub::Status::Atual);
    REQUIRE(statusNoCatalogo(leitoras, sertao, status));
    CHECK(status == BookClub::Status::Futuro);

    CHECK(leitoras->getCurrentReading() == hora);
    CHECK(leitoras->mediaAvaliacoes(dom) == 4);
    CHECK(leitoras->totalVotos(sertao) == 1);
}

TEST_CASE("BancoDeDados: datas e totais do clube voltam iguais") {
    CenarioSalvo salvo = salvarCenarioDeClubes();
    BancoDeDados banco(PASTA_TESTE);
    banco.carregarTudo();

    BookClub* leitoras = banco.buscarClube(salvo.idLeitoras);
    REQUIRE(leitoras != nullptr);
    CHECK(leitoras->getClubCreation() == salvo.criacaoLeitoras);
    CHECK(leitoras->getStartReading() == salvo.inicioLeitoras);
    CHECK(leitoras->getEndReading() == salvo.fimLeitoras);
    CHECK(leitoras->getReadPages() == salvo.paginasLeitoras);
    CHECK(leitoras->getReadPages() == 50 + 88);
    CHECK(leitoras->getReadBooks() == 1);   // dom passou para Passado
}

TEST_CASE("BancoDeDados: métricas de leitura voltam para cada membro") {
    CenarioSalvo salvo = salvarCenarioDeClubes();
    BancoDeDados banco(PASTA_TESTE);
    banco.carregarTudo();

    BookClub* leitoras = banco.buscarClube(salvo.idLeitoras);
    User* julia = banco.buscarUsuario(salvo.idJulia);
    User* raquel = banco.buscarUsuario(salvo.idRaquel);
    User* artur = banco.buscarUsuario(salvo.idArtur);
    REQUIRE(leitoras != nullptr);

    CHECK(julia->getCurrentPage(leitoras) == 0);   // sem check-in: sem métrica
    CHECK(julia->getMetrica(leitoras) == nullptr);
    CHECK(raquel->getCurrentPage(leitoras) == 50);
    CHECK(artur->getCurrentPage(leitoras) == 88);

    ReadingMetrics* metricaArtur = artur->getMetrica(leitoras);
    REQUIRE(metricaArtur != nullptr);
    CHECK(metricaArtur->getReading() == banco.buscarLivro(salvo.idHora));
    CHECK(metricaArtur->isFinished());
    CHECK(metricaArtur->getGrade() == 4.5);

    // As métricas do clube usam as páginas dos membros: ranking e quem terminou.
    CHECK(leitoras->LiderDeLeitura() == artur);
    CHECK(leitoras->TotalTerminaram() == 1);

    // Os totais do usuário vêm do users.json.
    CHECK(raquel->getTotalPages() == 50);
    CHECK(artur->getTotalPages() == 88);
    CHECK(artur->getTotalBooks() == 1);
}

TEST_CASE("BancoDeDados: o código salvo continua abrindo o clube privado") {
    CenarioSalvo salvo = salvarCenarioDeClubes();
    BancoDeDados banco(PASTA_TESTE);
    banco.carregarTudo();

    BookClub* secreto = banco.buscarClube(salvo.idSecreto);
    User* artur = banco.buscarUsuario(salvo.idArtur);
    REQUIRE(secreto != nullptr);
    CHECK_FALSE(artur->entrarClube(secreto, "errado"));
    CHECK(artur->entrarClube(secreto, salvo.codigoSecreto));
}

TEST_CASE("BancoDeDados: salvar de novo depois da carga dá o mesmo resultado") {
    CenarioSalvo salvo = salvarCenarioDeClubes();
    {
        BancoDeDados primeiro(PASTA_TESTE);
        primeiro.carregarTudo();
        primeiro.salvarTudo();   // reescreve tudo a partir do que carregou
    }
    BancoDeDados segundo(PASTA_TESTE);
    segundo.carregarTudo();
    BookClub* leitoras = segundo.buscarClube(salvo.idLeitoras);
    REQUIRE(leitoras != nullptr);
    CHECK(leitoras->getNumberMembers() == 3);
    CHECK(leitoras->getCatalog().size() == 3);
    CHECK(segundo.buscarUsuario(salvo.idRaquel)->getCurrentPage(leitoras) == 50);
}

TEST_CASE("BancoDeDados: índice citando um clube cujo arquivo sumiu é ignorado") {
    limparPastaDeTeste();
    escreverArquivo("clubs.json", "[987654]");   // não existe clubs/987654.json
    BancoDeDados banco(PASTA_TESTE);
    CHECK_NOTHROW(banco.carregarTudo());
    CHECK(banco.getClubes().empty());
    limparPastaDeTeste();
}

// Arquivos escritos à mão com ids que não existem: 999999 (usuário),
// 999998 (livro) e 999997 (clube). A carga ignora cada um desses itens.
TEST_CASE("BancoDeDados: referências quebradas são ignoradas, sem crash") {
    limparPastaDeTeste();
    escreverArquivo("users.json",
        "[{\"id\": 987001, \"nome\": \"Julia\", \"email\": \"julia@email.com\", \"senha\": \"123\","
        " \"descricao\": \"\", \"foto\": \"\", \"header\": \"\", \"totalPaginas\": 0, \"totalLivros\": 0}]");
    escreverArquivo("books.json",
        "[{\"id\": 987002, \"titulo\": \"Dom Casmurro\", \"autor\": \"\", \"genero\": \"\","
        " \"paginas\": 300, \"isbn\": \"\"}]");
    escreverArquivo("clubs.json", "[987003, 987004]");
    // Clube bom, mas com membro, nota, voto e livro que não existem.
    escreverArquivo("clubs/987003.json",
        "{\"id\": 987003, \"nome\": \"Clube\", \"tipo\": \"t\", \"privado\": false, \"ativo\": true,"
        " \"codigoConvite\": \"ABC1234567\", \"admin\": 987001, \"membros\": [987001, 999999],"
        " \"catalogo\": [{\"livro\": 987002, \"status\": \"Atual\","
        "                 \"notas\": [{\"user\": 999999, \"nota\": 4}], \"votos\": [999999]},"
        "                {\"livro\": 999998, \"status\": \"Futuro\", \"notas\": [], \"votos\": []}],"
        " \"paginasLidas\": 0, \"livrosLidos\": 0, \"criacao\": 1592222400,"
        " \"inicioLeitura\": 0, \"fimLeitura\": 0}");
    // Clube cujo admin não existe: o clube inteiro é ignorado.
    escreverArquivo("clubs/987004.json",
        "{\"id\": 987004, \"nome\": \"Sem admin\", \"tipo\": \"t\", \"privado\": false, \"ativo\": true,"
        " \"codigoConvite\": \"ABC1234567\", \"admin\": 999999, \"membros\": [999999],"
        " \"catalogo\": [], \"paginasLidas\": 0, \"livrosLidos\": 0, \"criacao\": 1592222400,"
        " \"inicioLeitura\": 0, \"fimLeitura\": 0}");
    // Métrica de um clube que não existe.
    escreverArquivo("readingmetrics/987001.json",
        "[{\"clube\": 999997, \"livro\": 987002, \"paginaAtual\": 10, \"nota\": -1, \"terminou\": false}]");

    BancoDeDados banco(PASTA_TESTE);
    CHECK_NOTHROW(banco.carregarTudo());
    REQUIRE(banco.getClubes().size() == 1);
    BookClub* clube = banco.buscarClube(987003);
    REQUIRE(clube != nullptr);
    CHECK(clube->getNumberMembers() == 1);       // o membro 999999 foi ignorado
    CHECK(clube->getCatalog().size() == 1);      // o livro 999998 foi ignorado
    CHECK(clube->mediaAvaliacoes(banco.buscarLivro(987002)) == 0);   // nota ignorada
    CHECK(clube->totalVotos(banco.buscarLivro(987002)) == 0);        // voto ignorado
    CHECK(banco.buscarClube(987004) == nullptr);
    CHECK(banco.buscarUsuario(987001)->getMetrica(clube) == nullptr);   // métrica ignorada
    limparPastaDeTeste();
}

TEST_CASE("BancoDeDados: status desconhecido no catálogo lança exceção") {
    limparPastaDeTeste();
    escreverArquivo("users.json",
        "[{\"id\": 987101, \"nome\": \"Julia\", \"email\": \"julia@email.com\", \"senha\": \"123\","
        " \"descricao\": \"\", \"foto\": \"\", \"header\": \"\", \"totalPaginas\": 0, \"totalLivros\": 0}]");
    escreverArquivo("books.json",
        "[{\"id\": 987102, \"titulo\": \"Dom Casmurro\", \"autor\": \"\", \"genero\": \"\","
        " \"paginas\": 300, \"isbn\": \"\"}]");
    escreverArquivo("clubs.json", "[987103]");
    escreverArquivo("clubs/987103.json",
        "{\"id\": 987103, \"nome\": \"Clube\", \"tipo\": \"t\", \"privado\": false, \"ativo\": true,"
        " \"codigoConvite\": \"ABC1234567\", \"admin\": 987101, \"membros\": [987101],"
        " \"catalogo\": [{\"livro\": 987102, \"status\": \"Lendo\", \"notas\": [], \"votos\": []}],"
        " \"paginasLidas\": 0, \"livrosLidos\": 0, \"criacao\": 1592222400,"
        " \"inicioLeitura\": 0, \"fimLeitura\": 0}");
    BancoDeDados banco(PASTA_TESTE);
    CHECK_THROWS_AS(banco.carregarTudo(), std::invalid_argument);
    limparPastaDeTeste();
}

TEST_CASE("BancoDeDados: adicionarClube recusa nulo e id repetido") {
    BancoDeDados banco(PASTA_TESTE);
    User* julia = new User("Julia", "julia@email.com", "123");
    banco.adicionarUsuario(julia);
    BookClub* clube = new BookClub(julia, "Clube", "t");
    banco.adicionarClube(clube);
    CHECK(banco.buscarClube(clube->getId()) == clube);

    BookClub* mesmoId = new BookClub(julia, "Outro", "t", false, clube->getId());
    CHECK_THROWS_AS(banco.adicionarClube(nullptr), std::invalid_argument);
    CHECK_THROWS_AS(banco.adicionarClube(mesmoId), std::invalid_argument);
    CHECK(banco.getClubes().size() == 1);
    delete mesmoId;   // lançou: o banco não virou dono
}
