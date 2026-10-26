#include "BookClub.hpp"
#include "User.hpp"
#include "Book.hpp"
#include "CheckIn.hpp"
#include "Comment.hpp"
#include <ctime>
#include <map>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

// =====================================================================
// Funções auxiliares (só existem dentro deste .cpp, por causa do static)
// =====================================================================

// Procura um livro no catálogo. Retorna a posição dele, ou -1 se não achar.
static int indiceNoCatalogo(const std::vector<BookClub::CatalogEntry>& catalogo,
                            const Book* livro) {
    for (int i = 0; i < (int)catalogo.size(); i++) {
        if (catalogo[i].book == livro) {
            return i;
        }
    }
    return -1;
}

// Procura o livro com status Atual. Retorna a posição dele, ou -1 se não houver.
static int indiceDaLeituraAtual(const std::vector<BookClub::CatalogEntry>& catalogo) {
    for (int i = 0; i < (int)catalogo.size(); i++) {
        if (catalogo[i].status == BookClub::Status::Atual) {
            return i;
        }
    }
    return -1;
}

// Página em que o membro está neste clube. O User::getCurrentPage devolve -1
// quando o User não tem o clube na lista dele; nesse caso contamos como 0.
static int paginaDoMembro(const User* membro, const BookClub* clube) {
    int pagina = membro->getCurrentPage(clube);
    if (pagina < 0) {
        return 0;
    }
    return pagina;
}

// Tira um User de uma lista de Users. Retorna true se achou e tirou.
static bool removerDaLista(std::vector<User*>& lista, const User* user) {
    for (int i = 0; i < (int)lista.size(); i++) {
        if (lista[i] == user) {
            lista.erase(lista.begin() + i);
            return true;
        }
    }
    return false;
}

// Gera um código de convite aleatório com 10 letras/números.
// std::random_device pede números aleatórios ao sistema operacional, o que é
// mais difícil de adivinhar do que rand(). Isso importa porque, num clube
// privado, quem descobre o código consegue entrar.
static std::string gerarCodigo() {
    const std::string caracteres =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::random_device gerador;
    std::uniform_int_distribution<int> sorteio(0, (int)caracteres.size() - 1);

    std::string codigo = "";
    for (int i = 0; i < 10; i++) {
        codigo += caracteres[sorteio(gerador)];
    }
    return codigo;
}

static const int SEGUNDOS_POR_DIA = 24 * 60 * 60;

// =====================================================================
// Construtores e destrutor
// =====================================================================

BookClub::BookClub(User* founder,
                   const std::string& name,
                   const std::string& type,
                   bool isPrivate)
    : name_(name), type_(type), inviteCode_(""), isPrivate_(isPrivate),
      active_(true), admin_(founder), readPages_(0), readBooks_(0),
      clubCreation_(std::time(nullptr)), startReading_(0), endReading_(0) {
    if (founder == nullptr) {
        throw std::invalid_argument("Clube precisa de um fundador");
    }
    if (name.empty()) {
        throw std::invalid_argument("Clube precisa de um nome");
    }
    this->members_.push_back(founder);
    this->inviteCode_ = gerarCodigo();
}

// Construtor delegado: primeiro roda o construtor de cima (depois dos ":"),
// depois coloca o primeiro livro como leitura atual.
BookClub::BookClub(User* founder,
                   const std::string& name,
                   Book* firstReading,
                   const std::string& type,
                   bool isPrivate)
    : BookClub(founder, name, type, isPrivate) {
    if (firstReading != nullptr) {
        this->adicionarLivro(firstReading);
        this->decidirProximaLeitura(founder, firstReading);
    }
}

BookClub::~BookClub() {
    // Copiamos a lista porque sairClube pode acabar mexendo em members_
    // enquanto a gente percorre (e apagar de um vetor enquanto anda nele quebra o laço).
    std::vector<User*> copia = this->members_;
    for (int i = 0; i < (int)copia.size(); i++) {
        if (copia[i]->participaDe(this)) {
            copia[i]->sairClube(this);
        }
    }
    // Os CheckIn do feed NÃO são apagados aqui: eles pertencem aos autores.
}

// =====================================================================
// Getters
// =====================================================================

const std::string& BookClub::getName() const {
    return this->name_;
}

const std::string& BookClub::getType() const {
    return this->type_;
}

bool BookClub::isPrivate() const {
    return this->isPrivate_;
}

bool BookClub::isActive() const {
    return this->active_;
}

bool BookClub::isReading() const {
    return indiceDaLeituraAtual(this->catalog_) != -1;
}

User* BookClub::getAdmin() const {
    return this->admin_;
}

const std::vector<User*>& BookClub::getMembers() const {
    return this->members_;
}

int BookClub::getNumberMembers() const {
    return (int)this->members_.size();
}

const std::vector<BookClub::CatalogEntry>& BookClub::getCatalog() const {
    return this->catalog_;
}

Book* BookClub::getCurrentReading() const {
    int atual = indiceDaLeituraAtual(this->catalog_);
    if (atual == -1) {
        return nullptr;
    }
    return this->catalog_[atual].book;
}

const std::vector<CheckIn*>& BookClub::getFeed() const {
    return this->feed_;
}

int BookClub::getReadPages() const {
    return this->readPages_;
}

void BookClub::setReadPages(int pages) {
    if (pages >= 0) {
        this->readPages_ = pages;
    }
}

int BookClub::getReadBooks() const {
    return this->readBooks_;
}

std::time_t BookClub::getClubCreation() const {
    return this->clubCreation_;
}

std::string BookClub::getInviteCode(const User* actor) const {
    if (this->isAdmin(actor)) {
        return this->inviteCode_;
    }
    return "";
}

// =====================================================================
// Métricas
// =====================================================================

double BookClub::MediaPaginasPorUser() const {
    if (this->members_.empty()) {
        return 0;
    }
    return (double)this->readPages_ / this->members_.size();
}

double BookClub::MediaPaginasPorDia() const {
    if (!this->isReading()) {
        return 0;
    }

    int soma = 0;
    for (int i = 0; i < (int)this->members_.size(); i++) {
        soma += paginaDoMembro(this->members_[i], this);
    }

    // Dias desde o início da leitura atual. Contamos no mínimo 1 dia
    // para não dividir por zero no primeiro dia.
    double segundos = std::difftime(std::time(nullptr), this->startReading_);
    int dias = (int)(segundos / SEGUNDOS_POR_DIA);
    if (dias < 1) {
        dias = 1;
    }
    return (double)soma / dias;
}

double BookClub::MediaPorcentagemLeitura() const {
    Book* livro = this->getCurrentReading();
    if (livro == nullptr || this->members_.empty() || livro->getPages() <= 0) {
        return 0;
    }

    double soma = 0;
    for (int i = 0; i < (int)this->members_.size(); i++) {
        int pagina = paginaDoMembro(this->members_[i], this);
        if (pagina > livro->getPages()) {
            pagina = livro->getPages();
        }
        soma += pagina * 100.0 / livro->getPages();
    }
    return soma / this->members_.size();
}

int BookClub::TotalTerminaram() const {
    Book* livro = this->getCurrentReading();
    if (livro == nullptr) {
        return 0;
    }

    int total = 0;
    for (int i = 0; i < (int)this->members_.size(); i++) {
        if (paginaDoMembro(this->members_[i], this) >= livro->getPages()) {
            total++;
        }
    }
    return total;
}

double BookClub::PorcentagemTerminaram() const {
    if (!this->isReading() || this->members_.empty()) {
        return 0;
    }
    return this->TotalTerminaram() * 100.0 / this->members_.size();
}

User* BookClub::LiderDeLeitura() const {
    if (this->members_.empty()) {
        return nullptr;
    }
    return this->ranking()[0];
}

std::vector<User*> BookClub::ranking() const {
    std::vector<User*> ordem = this->members_;

    // Insertion sort: cada membro "anda para a esquerda" enquanto o da
    // esquerda tiver lido MENOS que ele. Em caso de empate ninguém passa
    // ninguém, então quem entrou antes no clube fica na frente.
    for (int i = 1; i < (int)ordem.size(); i++) {
        User* atual = ordem[i];
        int paginaAtual = paginaDoMembro(atual, this);
        int j = i - 1;
        while (j >= 0 && paginaDoMembro(ordem[j], this) < paginaAtual) {
            ordem[j + 1] = ordem[j];
            j--;
        }
        ordem[j + 1] = atual;
    }
    return ordem;
}

int BookClub::DiasRestantesLeitura() const {
    if (!this->isReading() || this->endReading_ == 0) {
        return 0;
    }
    double segundos = std::difftime(this->endReading_, std::time(nullptr));
    if (segundos <= 0) {
        return 0;
    }
    // Arredonda para cima: faltando 1 hora, ainda resta "1 dia".
    long seg = (long)segundos;
    return (int)((seg + SEGUNDOS_POR_DIA - 1) / SEGUNDOS_POR_DIA);
}

double BookClub::mediaAvaliacoes(const Book* livro) const {
    int indice = indiceNoCatalogo(this->catalog_, livro);
    if (indice == -1) {
        return 0;
    }

    const std::map<const User*, int>& notas = this->catalog_[indice].ratings;
    if (notas.empty()) {
        return 0;
    }

    int soma = 0;
    std::map<const User*, int>::const_iterator it;
    for (it = notas.begin(); it != notas.end(); it++) {
        soma += it->second;   // it->first é o User, it->second é a nota
    }
    return (double)soma / notas.size();
}

int BookClub::totalVotos(const Book* livro) const {
    int indice = indiceNoCatalogo(this->catalog_, livro);
    if (indice == -1) {
        return 0;
    }
    return (int)this->catalog_[indice].votes.size();
}

// =====================================================================
// Membros
// =====================================================================

bool BookClub::isMember(const User* user) const {
    for (int i = 0; i < (int)this->members_.size(); i++) {
        if (this->members_[i] == user) {
            return true;
        }
    }
    return false;
}

bool BookClub::entrarNoClube(User* user, const std::string& codigo) {
    if (user == nullptr || !this->active_ || this->isMember(user)) {
        return false;
    }
    if (this->isPrivate_ && codigo != this->inviteCode_) {
        return false;
    }

    this->members_.push_back(user);

    // Mantém os dois lados de acordo: o User também precisa saber que entrou.
    if (!user->participaDe(this)) {
        user->entrarClube(this);
    }
    return true;
}

bool BookClub::sairDoClube(User* user) {
    if (!this->isMember(user) || user == this->admin_) {
        return false;
    }

    removerDaLista(this->members_, user);

    if (user->participaDe(this)) {
        user->sairClube(this);
    }
    return true;
}

// =====================================================================
// Check-ins
// =====================================================================

bool BookClub::adicionarCheckIn(CheckIn* checkIn) {
    if (checkIn == nullptr) {
        return false;
    }
    if (checkIn->getClub() != this) {
        return false;
    }
    if (!this->isMember(checkIn->getUser())) {
        return false;
    }
    for (int i = 0; i < (int)this->feed_.size(); i++) {
        if (this->feed_[i] == checkIn) {
            return false;   // já está no feed
        }
    }

    this->feed_.push_back(checkIn);
    return true;
}

void BookClub::eraseCheckIn(CheckIn* checkIn) {
    for (int i = 0; i < (int)this->feed_.size(); i++) {
        if (this->feed_[i] == checkIn) {
            this->feed_.erase(this->feed_.begin() + i);
            return;
        }
    }
}

// =====================================================================
// Catálogo
// =====================================================================

bool BookClub::adicionarLivro(Book* livro) {
    if (livro == nullptr) {
        return false;
    }
    if (indiceNoCatalogo(this->catalog_, livro) != -1) {
        return false;
    }

    CatalogEntry novo;
    novo.book = livro;
    novo.status = Status::Futuro;
    this->catalog_.push_back(novo);
    return true;
}

bool BookClub::removerLivro(Book* livro) {
    int indice = indiceNoCatalogo(this->catalog_, livro);
    if (indice == -1) {
        return false;
    }

    // Se era a leitura atual, o clube fica sem leitura (e sem prazo).
    if (this->catalog_[indice].status == Status::Atual) {
        this->startReading_ = 0;
        this->endReading_ = 0;
    }
    this->catalog_.erase(this->catalog_.begin() + indice);
    return true;
}

bool BookClub::avaliarLivro(User* user, Book* livro, int nota) {
    if (!this->isMember(user)) {
        return false;
    }
    int indice = indiceNoCatalogo(this->catalog_, livro);
    if (indice == -1 || this->catalog_[indice].status != Status::Passado) {
        return false;
    }
    if (nota < 1 || nota > 5) {
        return false;
    }

    // Se o membro já tinha avaliado, a nota nova substitui a antiga.
    this->catalog_[indice].ratings[user] = nota;
    return true;
}

bool BookClub::votarProximaLeitura(User* user, Book* livro) {
    if (!this->isMember(user)) {
        return false;
    }
    int indice = indiceNoCatalogo(this->catalog_, livro);
    if (indice == -1 || this->catalog_[indice].status != Status::Futuro) {
        return false;
    }

    std::vector<const User*>& votos = this->catalog_[indice].votes;
    for (int i = 0; i < (int)votos.size(); i++) {
        if (votos[i] == user) {
            return false;   // já votou neste livro
        }
    }
    votos.push_back(user);
    return true;
}

// =====================================================================
// Administrador
// =====================================================================

bool BookClub::isAdmin(const User* user) const {
    return user != nullptr && user == this->admin_;
}

std::string BookClub::criarConvite(User* actor) {
    if (!this->isAdmin(actor)) {
        return "";
    }
    // O código novo substitui o antigo, que deixa de funcionar (US05: revogar).
    this->inviteCode_ = gerarCodigo();
    return this->inviteCode_;
}

bool BookClub::removerMembro(User* actor, User* alvo) {
    if (!this->isAdmin(actor)) {
        return false;
    }
    if (alvo == nullptr || !this->isMember(alvo) || alvo == this->admin_) {
        return false;
    }

    removerDaLista(this->members_, alvo);
    if (alvo->participaDe(this)) {
        alvo->sairClube(this);
    }
    // Os check-ins do expulso continuam no feed (US08).
    return true;
}

bool BookClub::decidirProximaLeitura(User* actor, Book* livro) {
    if (!this->isAdmin(actor)) {
        return false;
    }
    int indice = indiceNoCatalogo(this->catalog_, livro);
    if (indice == -1 || this->catalog_[indice].status != Status::Futuro) {
        return false;
    }

    // O livro atual (se houver) vira passado e conta como livro lido.
    int atual = indiceDaLeituraAtual(this->catalog_);
    if (atual != -1) {
        this->catalog_[atual].status = Status::Passado;
        this->readBooks_++;
    }

    this->catalog_[indice].status = Status::Atual;
    this->startReading_ = std::time(nullptr);
    this->endReading_ = 0;   // prazo novo precisa ser definido de novo

    // A rodada de votação acabou: zera os votos para a próxima.
    for (int i = 0; i < (int)this->catalog_.size(); i++) {
        this->catalog_[i].votes.clear();
    }
    return true;
}

bool BookClub::alterarPrivacidade(User* actor, bool isPrivate) {
    if (!this->isAdmin(actor)) {
        return false;
    }
    this->isPrivate_ = isPrivate;
    return true;
}

bool BookClub::renomear(User* actor, const std::string& novoNome) {
    if (!this->isAdmin(actor) || novoNome.empty()) {
        return false;
    }
    this->name_ = novoNome;
    return true;
}

bool BookClub::apagarCheckIn(User* actor, CheckIn* checkIn) {
    if (!this->isAdmin(actor) || checkIn == nullptr) {
        return false;
    }

    bool estaNoFeed = false;
    for (int i = 0; i < (int)this->feed_.size(); i++) {
        if (this->feed_[i] == checkIn) {
            estaNoFeed = true;
        }
    }
    if (!estaNoFeed) {
        return false;
    }

    // Quem é dono do check-in é o autor. Ele tira do feed (eraseCheckIn)
    // e faz o delete. Depois desta linha, checkIn não pode mais ser usado.
    User* autor = checkIn->getUser();
    return autor->apagarCheckIn(checkIn);
}

bool BookClub::definirTempoLeitura(User* actor, int dias) {
    if (!this->isAdmin(actor) || !this->isReading() || dias <= 0) {
        return false;
    }
    this->endReading_ = this->startReading_ + (std::time_t)dias * SEGUNDOS_POR_DIA;
    return true;
}

bool BookClub::apagarComentario(User* actor, Comment* comentario) {
    if (!this->isAdmin(actor) || comentario == nullptr) {
        return false;
    }
    if (comentario->get_club() != this) {
        return false;
    }
    CheckIn* checkIn = comentario->get_checkin();
    if (checkIn == nullptr) {
        return false;
    }

    std::vector<Comment*>& lista = checkIn->getComments();
    for (int i = 0; i < (int)lista.size(); i++) {
        if (lista[i] == comentario) {
            lista.erase(lista.begin() + i);
            delete comentario;
            return true;
        }
    }
    return false;
}

bool BookClub::transferirAdmin(User* actor, User* alvo) {
    if (!this->isAdmin(actor)) {
        return false;
    }
    if (alvo == nullptr || !this->isMember(alvo) || alvo == this->admin_) {
        return false;
    }
    this->admin_ = alvo;
    return true;
}
