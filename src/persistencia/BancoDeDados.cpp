#include "BancoDeDados.hpp"
#include "Book.hpp"
#include "User.hpp"
#include "BookClub.hpp"
#include "UserProfile.hpp"
#include "ReadingMetrics.hpp"
#include "json.hpp"
#include <ctime>
#include <fstream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

// =====================================================================
// Funções auxiliares (só existem dentro deste .cpp, por causa do static)
// =====================================================================

// Lê um arquivo JSON e guarda o conteúdo em "destino".
// Retorna false se o arquivo não existe (ou não abre): quem chamou trata
// isso como lista vazia. Se o arquivo existe mas o JSON está quebrado, o
// parse do nlohmann lança exceção, e ela sobe para quem chamou.
static bool lerJson(const std::string& caminho, nlohmann::json& destino) {
    std::ifstream arquivo(caminho);
    if (!arquivo.is_open()) {
        return false;
    }
    destino = nlohmann::json::parse(arquivo);
    return true;
}

// Igual ao lerJson, mas exige que o arquivo seja uma lista [ ... ].
// Quase todos os arquivos são listas; só clubs/<id>.json guarda um objeto { ... }.
static bool lerLista(const std::string& caminho, nlohmann::json& destino) {
    if (!lerJson(caminho, destino)) {
        return false;
    }
    if (!destino.is_array()) {
        throw std::runtime_error("Arquivo nao contem uma lista JSON: " + caminho);
    }
    return true;
}

// Escreve o JSON no arquivo, apagando o que havia antes.
// dump(4) gera o texto com 4 espaços de recuo, para ficar fácil de ler.
static void escreverJson(const std::string& caminho, const nlohmann::json& conteudo) {
    std::ofstream arquivo(caminho);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir para escrita: " + caminho);
    }
    arquivo << conteudo.dump(4) << std::endl;
}

// =====================================================================
// Construtor e destrutor
// =====================================================================

BancoDeDados::BancoDeDados(const std::string& pasta) : pasta(pasta) {
}

BancoDeDados::~BancoDeDados() {
    // Ordem importa (ver BancoDeDados.hpp): clubes, usuários, livros.
    for (int i = 0; i < (int)this->clubes.size(); i++) {
        delete this->clubes[i];
    }
    for (int i = 0; i < (int)this->usuarios.size(); i++) {
        delete this->usuarios[i];
    }
    for (int i = 0; i < (int)this->livros.size(); i++) {
        delete this->livros[i];
    }
}

std::string BancoDeDados::caminho(const std::string& nomeArquivo) const {
    return this->pasta + nomeArquivo;
}

// =====================================================================
// Busca
// =====================================================================

Book* BancoDeDados::buscarLivro(int id) const {
    for (int i = 0; i < (int)this->livros.size(); i++) {
        if (this->livros[i]->getId() == id) {
            return this->livros[i];
        }
    }
    return nullptr;
}

User* BancoDeDados::buscarUsuario(int id) const {
    for (int i = 0; i < (int)this->usuarios.size(); i++) {
        if (this->usuarios[i]->getId() == id) {
            return this->usuarios[i];
        }
    }
    return nullptr;
}

BookClub* BancoDeDados::buscarClube(int id) const {
    for (int i = 0; i < (int)this->clubes.size(); i++) {
        if (this->clubes[i]->getId() == id) {
            return this->clubes[i];
        }
    }
    return nullptr;
}

User* BancoDeDados::buscarUsuarioPorEmail(const std::string& email) const {
    for (int i = 0; i < (int)this->usuarios.size(); i++) {
        if (this->usuarios[i]->getEmail() == email) {
            return this->usuarios[i];
        }
    }
    return nullptr;
}

// =====================================================================
// Listas
// =====================================================================

const std::vector<Book*>& BancoDeDados::getLivros() const {
    return this->livros;
}

const std::vector<User*>& BancoDeDados::getUsuarios() const {
    return this->usuarios;
}

const std::vector<BookClub*>& BancoDeDados::getClubes() const {
    return this->clubes;
}

void BancoDeDados::adicionarLivro(Book* livro) {
    if (livro == nullptr) {
        throw std::invalid_argument("Ponteiro de livro nulo");
    }
    if (this->buscarLivro(livro->getId()) != nullptr) {
        throw std::invalid_argument("Ja existe um livro com esse id");
    }
    this->livros.push_back(livro);
}

void BancoDeDados::adicionarUsuario(User* usuario) {
    if (usuario == nullptr) {
        throw std::invalid_argument("Ponteiro de usuario nulo");
    }
    if (this->buscarUsuario(usuario->getId()) != nullptr) {
        throw std::invalid_argument("Ja existe um usuario com esse id");
    }
    this->usuarios.push_back(usuario);
}

void BancoDeDados::adicionarClube(BookClub* clube) {
    if (clube == nullptr) {
        throw std::invalid_argument("Ponteiro de clube nulo");
    }
    if (this->buscarClube(clube->getId()) != nullptr) {
        throw std::invalid_argument("Ja existe um clube com esse id");
    }
    this->clubes.push_back(clube);
}

// =====================================================================
// Arquivos: carregar e salvar tudo
// =====================================================================

void BancoDeDados::carregarTudo() {
    if (!this->livros.empty() || !this->usuarios.empty() || !this->clubes.empty()) {
        throw std::logic_error("O banco ja tem dados; carregue so num banco vazio");
    }
    // Ordem de carga: cada tipo precisa dos anteriores (ex.: um clube
    // precisa dos usuários e dos livros já carregados para achar pelo id).
    this->carregarLivros();
    this->carregarUsuarios();
    this->carregarClubes();
    this->carregarMetricas();
}

void BancoDeDados::salvarTudo() const {
    this->salvarLivros();
    this->salvarUsuarios();
    this->salvarClubes();
    this->salvarMetricas();
}

// =====================================================================
// Livros: books.json
// [{"id": 1, "titulo": "...", "autor": "...", "genero": "...",
//   "paginas": 300, "isbn": "..."}, ...]
// =====================================================================

void BancoDeDados::salvarLivros() const {
    nlohmann::json lista = nlohmann::json::array();
    for (int i = 0; i < (int)this->livros.size(); i++) {
        Book* livro = this->livros[i];
        nlohmann::json item;
        item["id"] = livro->getId();
        item["titulo"] = livro->getTitle();
        item["autor"] = livro->getAuthor();
        item["genero"] = livro->getGenre();
        item["paginas"] = livro->getPages();
        item["isbn"] = livro->getISBN();
        lista.push_back(item);
    }
    escreverJson(this->caminho("books.json"), lista);
}

void BancoDeDados::carregarLivros() {
    nlohmann::json lista;
    if (!lerLista(this->caminho("books.json"), lista)) {
        return;   // arquivo não existe: nenhum livro
    }
    for (int i = 0; i < (int)lista.size(); i++) {
        nlohmann::json item = lista[i];
        // Cada valor é lido declarando e iniciando na mesma linha
        // (int id = item["id"];). Assim o nlohmann converte sozinho para o
        // tipo da variável.
        int id = item["id"];
        std::string titulo = item["titulo"];
        std::string autor = item["autor"];
        std::string genero = item["genero"];
        int paginas = item["paginas"];
        std::string isbn = item["isbn"];

        // O id repetido é conferido ANTES do new: se adicionarLivro lançasse
        // depois do new, o livro ficaria sem dono e nunca seria apagado.
        if (this->buscarLivro(id) != nullptr) {
            throw std::invalid_argument("Id de livro repetido em books.json");
        }
        // O id salvo é passado ao construtor: assim os ids continuam os
        // mesmos, e os próximos livros criados recebem ids maiores.
        this->adicionarLivro(new Book(titulo, autor, genero, paginas, isbn, id));
    }
}

// =====================================================================
// Usuários: users.json
// [{"id": 1, "nome": "...", "email": "...", "senha": "...",
//   "descricao": "...", "foto": "...", "header": "...",
//   "totalPaginas": 0, "totalLivros": 0}, ...]
// =====================================================================

void BancoDeDados::salvarUsuarios() const {
    nlohmann::json lista = nlohmann::json::array();
    for (int i = 0; i < (int)this->usuarios.size(); i++) {
        User* usuario = this->usuarios[i];
        nlohmann::json item;
        item["id"] = usuario->getId();
        item["nome"] = usuario->getName();
        item["email"] = usuario->getEmail();
        item["senha"] = usuario->getPassword();   // texto puro: limitação conhecida
        item["descricao"] = usuario->getProfile().getDescription();
        item["foto"] = usuario->getProfile().getPhotoPath();
        item["header"] = usuario->getProfile().getHeaderPath();
        item["totalPaginas"] = usuario->getTotalPages();
        item["totalLivros"] = usuario->getTotalBooks();
        lista.push_back(item);
    }
    escreverJson(this->caminho("users.json"), lista);
}

void BancoDeDados::carregarUsuarios() {
    nlohmann::json lista;
    if (!lerLista(this->caminho("users.json"), lista)) {
        return;   // arquivo não existe: nenhum usuário
    }
    for (int i = 0; i < (int)lista.size(); i++) {
        nlohmann::json item = lista[i];
        int id = item["id"];
        std::string nome = item["nome"];
        std::string email = item["email"];
        std::string senha = item["senha"];
        std::string descricao = item["descricao"];
        std::string foto = item["foto"];
        std::string header = item["header"];
        int totalPaginas = item["totalPaginas"];
        int totalLivros = item["totalLivros"];

        // Mesmo cuidado dos livros: conferir o id repetido antes do new.
        if (this->buscarUsuario(id) != nullptr) {
            throw std::invalid_argument("Id de usuario repetido em users.json");
        }
        // O usuário entra no banco logo depois do new: se algo abaixo lançar
        // exceção, o banco já é dono e o apaga no destrutor (sem vazar memória).
        User* usuario = new User(nome, email, senha, id);
        this->adicionarUsuario(usuario);

        // setDescription devolve false se a descrição passar do limite; como
        // ela foi salva pelo próprio programa, isso não deve acontecer.
        usuario->getProfile().setDescription(descricao);
        usuario->getProfile().setPhotoPath(foto);
        usuario->getProfile().setHeaderPath(header);
        usuario->restaurarTotais(totalPaginas, totalLivros);
    }
}

// =====================================================================
// Clubes: clubs.json (índice) e clubs/<id>.json
//
// clubs.json: [1, 4, 7]
// clubs/<id>.json:
// {"id": 1, "nome": "...", "tipo": "...", "privado": false, "ativo": true,
//  "codigoConvite": "...", "admin": 3, "membros": [3, 5],
//  "catalogo": [{"livro": 2, "status": "Atual",
//                "notas": [{"user": 5, "nota": 4}], "votos": [3]}],
//  "paginasLidas": 0, "livrosLidos": 0,
//  "criacao": 1700000000, "inicioLeitura": 0, "fimLeitura": 0}
// As datas (time_t) são salvas como número de segundos desde 1970.
// =====================================================================

// O enum Status vira texto no arquivo, para dar para ler o JSON.
static std::string statusParaTexto(BookClub::Status status) {
    if (status == BookClub::Status::Futuro) {
        return "Futuro";
    }
    if (status == BookClub::Status::Atual) {
        return "Atual";
    }
    return "Passado";
}

static BookClub::Status textoParaStatus(const std::string& texto) {
    if (texto == "Futuro") {
        return BookClub::Status::Futuro;
    }
    if (texto == "Atual") {
        return BookClub::Status::Atual;
    }
    if (texto == "Passado") {
        return BookClub::Status::Passado;
    }
    throw std::invalid_argument("Status de livro desconhecido: " + texto);
}

void BancoDeDados::salvarClubes() const {
    nlohmann::json indice = nlohmann::json::array();
    for (int i = 0; i < (int)this->clubes.size(); i++) {
        BookClub* clube = this->clubes[i];
        // Todo clube tem admin (US28: o admin nunca sai). O if é só uma
        // proteção: sem admin não daria para recriar o clube na carga.
        if (clube->getAdmin() == nullptr) {
            continue;
        }

        nlohmann::json item;
        item["id"] = clube->getId();
        item["nome"] = clube->getName();
        item["tipo"] = clube->getType();
        item["privado"] = clube->isPrivate();
        item["ativo"] = clube->isActive();
        // getInviteCode só devolve o código para o admin, então pedimos
        // "em nome" dele.
        item["codigoConvite"] = clube->getInviteCode(clube->getAdmin());
        item["admin"] = clube->getAdmin()->getId();

        nlohmann::json membros = nlohmann::json::array();
        const std::vector<User*>& listaMembros = clube->getMembers();
        for (int j = 0; j < (int)listaMembros.size(); j++) {
            membros.push_back(listaMembros[j]->getId());
        }
        item["membros"] = membros;

        nlohmann::json catalogo = nlohmann::json::array();
        const std::vector<BookClub::CatalogEntry>& entradas = clube->getCatalog();
        for (int j = 0; j < (int)entradas.size(); j++) {
            nlohmann::json entrada;
            entrada["livro"] = entradas[j].book->getId();
            entrada["status"] = statusParaTexto(entradas[j].status);

            // ratings é um map: it->first é o User, it->second é a nota.
            nlohmann::json notas = nlohmann::json::array();
            std::map<const User*, int>::const_iterator it;
            for (it = entradas[j].ratings.begin(); it != entradas[j].ratings.end(); it++) {
                nlohmann::json nota;
                nota["user"] = it->first->getId();
                nota["nota"] = it->second;
                notas.push_back(nota);
            }
            entrada["notas"] = notas;

            nlohmann::json votos = nlohmann::json::array();
            for (int k = 0; k < (int)entradas[j].votes.size(); k++) {
                votos.push_back(entradas[j].votes[k]->getId());
            }
            entrada["votos"] = votos;

            catalogo.push_back(entrada);
        }
        item["catalogo"] = catalogo;

        item["paginasLidas"] = clube->getReadPages();
        item["livrosLidos"] = clube->getReadBooks();
        item["criacao"] = clube->getClubCreation();
        item["inicioLeitura"] = clube->getStartReading();
        item["fimLeitura"] = clube->getEndReading();

        escreverJson(this->caminho("clubs/" + std::to_string(clube->getId()) + ".json"), item);
        indice.push_back(clube->getId());
    }
    escreverJson(this->caminho("clubs.json"), indice);
}

void BancoDeDados::carregarClubes() {
    nlohmann::json indice;
    if (!lerLista(this->caminho("clubs.json"), indice)) {
        return;   // índice não existe: nenhum clube
    }
    for (int i = 0; i < (int)indice.size(); i++) {
        int idDoIndice = indice[i];
        std::string arquivo = this->caminho("clubs/" + std::to_string(idDoIndice) + ".json");
        nlohmann::json item;
        // Referência quebrada: o índice cita um arquivo que sumiu. Pula o clube.
        if (!lerJson(arquivo, item)) {
            continue;
        }
        if (!item.is_object()) {
            throw std::runtime_error("Arquivo de clube nao contem um objeto JSON: " + arquivo);
        }

        int id = item["id"];
        std::string nome = item["nome"];
        std::string tipo = item["tipo"];
        bool privado = item["privado"];
        bool ativo = item["ativo"];
        std::string codigo = item["codigoConvite"];
        int idAdmin = item["admin"];
        int paginasLidas = item["paginasLidas"];
        int livrosLidos = item["livrosLidos"];
        std::time_t criacao = item["criacao"];
        std::time_t inicioLeitura = item["inicioLeitura"];
        std::time_t fimLeitura = item["fimLeitura"];

        // Referência quebrada: o admin não existe mais. Sem ele não dá para
        // criar o clube (o construtor exige um fundador), então pula.
        User* admin = this->buscarUsuario(idAdmin);
        if (admin == nullptr) {
            continue;
        }
        if (this->buscarClube(id) != nullptr) {
            throw std::invalid_argument("Id de clube repetido em clubs.json");
        }

        // Construtor SEM leitura: o catálogo vem do arquivo logo abaixo.
        // O admin entra como fundador (primeiro membro e administrador).
        // O clube entra no banco logo depois do new: se algo abaixo lançar
        // exceção, o banco já é dono e o apaga no destrutor.
        BookClub* clube = new BookClub(admin, nome, tipo, privado, id);
        this->clubes.push_back(clube);

        // Antes dos membros: o código salvo é o que deixa eles entrarem
        // num clube privado.
        clube->restaurarEstado(codigo, ativo, paginasLidas, livrosLidos,
                               criacao, inicioLeitura, fimLeitura);

        nlohmann::json catalogo = item["catalogo"];
        for (int j = 0; j < (int)catalogo.size(); j++) {
            nlohmann::json entrada = catalogo[j];
            int idLivro = entrada["livro"];
            std::string status = entrada["status"];
            Book* livro = this->buscarLivro(idLivro);
            if (livro == nullptr) {
                continue;   // referência quebrada: livro que não existe mais
            }
            clube->restaurarLivroNoCatalogo(livro, textoParaStatus(status));

            nlohmann::json notas = entrada["notas"];
            for (int k = 0; k < (int)notas.size(); k++) {
                nlohmann::json nota = notas[k];
                int idUser = nota["user"];
                int valor = nota["nota"];
                User* user = this->buscarUsuario(idUser);
                if (user != nullptr) {   // se não existe, ignora a nota
                    clube->restaurarNota(livro, user, valor);
                }
            }

            nlohmann::json votos = entrada["votos"];
            for (int k = 0; k < (int)votos.size(); k++) {
                int idUser = votos[k];
                User* user = this->buscarUsuario(idUser);
                if (user != nullptr) {   // se não existe, ignora o voto
                    clube->restaurarVoto(livro, user);
                }
            }
        }

        // Os outros membros entram pelo caminho normal, com o código salvo.
        // entrarClube atualiza os dois lados (User e BookClub).
        nlohmann::json membros = item["membros"];
        for (int j = 0; j < (int)membros.size(); j++) {
            int idMembro = membros[j];
            if (idMembro == idAdmin) {
                continue;   // o admin já entrou como fundador
            }
            User* membro = this->buscarUsuario(idMembro);
            if (membro != nullptr) {   // se não existe, ignora o membro
                membro->entrarClube(clube, codigo);
            }
        }
    }
}

// =====================================================================
// Métricas de leitura: readingmetrics/<id do usuário>.json
// [{"clube": 1, "livro": 2, "paginaAtual": 120, "nota": -1,
//   "terminou": false}, ...]
// O arquivo é escrito para TODO usuário, mesmo sem métricas ([]): assim um
// arquivo velho de outra execução nunca é lido por engano.
// =====================================================================

void BancoDeDados::salvarMetricas() const {
    for (int i = 0; i < (int)this->usuarios.size(); i++) {
        User* usuario = this->usuarios[i];
        nlohmann::json lista = nlohmann::json::array();

        const std::vector<BookClub*>& clubesDoUsuario = usuario->getCurrentClubs();
        for (int j = 0; j < (int)clubesDoUsuario.size(); j++) {
            BookClub* clube = clubesDoUsuario[j];
            ReadingMetrics* metrica = usuario->getMetrica(clube);
            if (metrica == nullptr) {
                continue;   // ainda não fez check-in neste clube
            }
            nlohmann::json item;
            item["clube"] = clube->getId();
            item["livro"] = metrica->getReading()->getId();
            item["paginaAtual"] = metrica->getCurrentPage();
            item["nota"] = metrica->getGrade();
            item["terminou"] = metrica->isFinished();
            lista.push_back(item);
        }

        escreverJson(this->caminho("readingmetrics/" + std::to_string(usuario->getId()) + ".json"), lista);
    }
}

void BancoDeDados::carregarMetricas() {
    for (int i = 0; i < (int)this->usuarios.size(); i++) {
        User* usuario = this->usuarios[i];
        nlohmann::json lista;
        if (!lerLista(this->caminho("readingmetrics/" + std::to_string(usuario->getId()) + ".json"), lista)) {
            continue;   // arquivo não existe: usuário sem métricas
        }
        for (int j = 0; j < (int)lista.size(); j++) {
            nlohmann::json item = lista[j];
            int idClube = item["clube"];
            int idLivro = item["livro"];
            int paginaAtual = item["paginaAtual"];
            double nota = item["nota"];
            bool terminou = item["terminou"];

            // Referência quebrada (clube ou livro que não existem, ou clube
            // do qual o usuário não participa mais): ignora a métrica.
            // Conferir ANTES do new evita criar uma métrica sem dono.
            BookClub* clube = this->buscarClube(idClube);
            Book* livro = this->buscarLivro(idLivro);
            if (clube == nullptr || livro == nullptr || !usuario->participaDe(clube)) {
                continue;
            }
            // Construtor de carga: valida página, nota e terminou. O User
            // vira dono da métrica no restaurarMetrica.
            usuario->restaurarMetrica(clube, new ReadingMetrics(livro, paginaAtual, nota, terminou));
        }
    }
}
