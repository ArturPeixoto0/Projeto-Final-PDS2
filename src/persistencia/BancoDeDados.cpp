#include "BancoDeDados.hpp"
#include "Book.hpp"
#include "User.hpp"
#include "BookClub.hpp"
#include "UserProfile.hpp"
#include "json.hpp"
#include <fstream>
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
}

void BancoDeDados::salvarTudo() const {
    this->salvarLivros();
    this->salvarUsuarios();
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
    if (!lerJson(this->caminho("books.json"), lista)) {
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
    if (!lerJson(this->caminho("users.json"), lista)) {
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
