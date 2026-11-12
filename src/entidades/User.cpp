#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "User.hpp"
#include "Book.hpp"
#include "BookClub.hpp"
#include "UserProfile.hpp"
#include "CheckIn.hpp"
#include "Comment.hpp"
#include "ReadingMetrics.hpp"

// Definição do contador static declarado no .hpp. Os ids começam em 1,
// porque 0 significa "gere um id novo" no construtor.
int User::proximoId = 1;

User::User(const std::string& name,
             const std::string& email,
             const std::string& password,
             int id) 
    : name(name), email(email), password(password), profile(this), id(0) { // <--- Inicializa aqui!
    
    if (name.empty() || email.empty() || password.empty()) {
        throw std::invalid_argument("Nome, Email ou Senha faltando");
    }
    if (id < 0) {
        throw std::invalid_argument("Id do usuario nao pode ser negativo");
    }

    if (id == 0) {
        this->id = proximoId;
        proximoId++;
    } else {
        this->id = id;
        // Garante que um id gerado depois nunca repita o id informado.
        if (id >= proximoId) {
            proximoId = id + 1;
        }
    }
}
            

             
User::~User() {
    for(int i = 0; i < (int)this->allCheckIns.size(); i++) {
        if (this->allCheckIns[i] != nullptr){
            if (this->allCheckIns[i]->getClub() != nullptr){
                BookClub* club = this->allCheckIns[i]->getClub();
                if(this->participaDe(club)) {
                    club->eraseCheckIn(this->allCheckIns[i]);
                }
            }
        }
    }

    std::vector<BookClub*> copia = this->currentClubs;
    for(int i = 0; i < (int)copia.size(); i++) {
        if (copia[i] != nullptr){
            BookClub* club = copia[i];
            if(club->isAdmin(this)) {
                if(club->getMembers().size() != 0){
                    const std::vector<User*>& membros = club->getMembers();
                    for(int j = 0; j < (int)membros.size(); j++){
                        if(membros[j] != this) {
                            club->transferirAdmin(this, membros[j]);
                            break;
                        }
                    }
                }
            }
            // Se este User for o admin e estiver sozinho, sairClube recusa
            // (US28): o clube continua apontando para ele. Por isso o clube
            // precisa ser apagado ANTES dos seus membros (o BancoDeDados apaga
            // clubes, depois usuários, depois livros).
            this->sairClube(club);
        }
        
    }
    for(auto& metric : allMetrics) {
        delete metric.second;
    }
    for(int i = 0; i < (int)this->allCheckIns.size(); i++) {
        delete this->allCheckIns[i];
    }
}   

        //dados da conta:
int User::getId() const {
    return this->id;
}

std::string User::getName() const {
    return this->name;
} //retorna o nome do usuário

void User::setName(const std::string& name) {
    if (name.empty()){
        throw std::invalid_argument("nome faltando");
    }
    this->name = name;
} //altera o nome do usuário

std::string User::getEmail() const {
    return this->email;
} // retorna email da conta

bool User::checkPassword(const std::string& attempt) const {
    if(this->password == attempt)
        return true;
    else 
        return false;
}

std::string User::getPassword() const {
    return this->password;
}

        //perfil: 

UserProfile& User::getProfile() {
    return this->profile;
}

const UserProfile& User::getProfile() const {
    return this->profile;
}

        //clubes:
const std::vector<BookClub*>& User::getCurrentClubs() const {
    return this->currentClubs;
}
bool User::participaDe(const BookClub* club) const {
    if (club == nullptr){
        throw std::invalid_argument("Ponteiro de clube do livro inválido");
    }
    for (int i=0; i < (int)this->currentClubs.size(); i++) {
        if (this->currentClubs[i] == club){
            return true;
        }
    }
    return false;
} //verifica se participa
    
BookClub* User::criarClube(const std::string& clubName, Book* reading) {
    if (reading == nullptr){
        throw std::invalid_argument("Ponteiro de livro inválido");
    }

    return new BookClub(this, clubName, reading);
}

bool User::entrarClube(BookClub* club, const std::string& codigo) {
    if (club == nullptr){
        throw std::invalid_argument("Ponteiro nulo");
    }
    
    if (this->participaDe(club)) {
        return false;
    }

    // O clube já tem este User como membro: a chamada veio do próprio clube
    // (BookClub::entrarNoClube ou o construtor, no caso do fundador).
    // Falta só o lado do User. A métrica de leitura não é criada aqui, porque
    // o clube pode ainda não ter leitura atual; ela é criada no primeiro
    // check-in (ver realizarCheckIn).
    if (club->isMember(this)) {
        this->currentClubs.push_back(club);
        return true;
    }

    // Chamada veio de fora: pede ao clube para entrar. O clube confere o
    // código, se adiciona e chama de volta este método (o caso de cima).
    return club->entrarNoClube(this, codigo);
}

bool User::sairClube(BookClub* club) {
    if (club == nullptr){
        throw std::invalid_argument("Ponteiro de clube do livro inválido");
    }

    if (!this->participaDe(club)) {
        return false;
    }

    // Chamada veio de fora: pede ao clube para sair. O clube pode recusar
    // (admin com outros membros). Se aceitar, ele chama de volta este método,
    // que cai no caso de baixo.
    if (club->isMember(this)) {
        return club->sairDoClube(this);
    }

    // O clube já tirou este User (sairDoClube, removerMembro ou o destrutor
    // do clube): falta só o lado do User.
    auto it = std::find(this->currentClubs.begin(), this->currentClubs.end(), club);
    if (it != this->currentClubs.end()) {
        this->currentClubs.erase(it);
    }

    auto metrica = allMetrics.find(club);
    if (metrica != allMetrics.end()) {
        delete metrica->second;
        allMetrics.erase(metrica);
    }
    return true;
}

        //check-ins e comentarios:
const std::vector<CheckIn*>& User::getAllCheckIns() const {
    return this->allCheckIns;
}
int User::getCurrentPage(const BookClub* club) const { 
    if (club == nullptr){
        throw std::invalid_argument("Ponteiro de clube do livro inválido");
    }

    if (!this->participaDe(club)) {
        return -1; //user n ta no club
    }

    auto it = allMetrics.find(const_cast<BookClub*>(club));
    if (it == allMetrics.end()) {
        return 0; //participa, mas ainda não fez check-in
    }
    return it->second->getCurrentPage();
}

CheckIn* User::realizarCheckIn(BookClub* club, int page, const std::string& text, bool finished){
    if (club == nullptr){
        throw std::invalid_argument("Ponteiro de clube do livro inválido");
    }
    if (text.empty()){
        return nullptr;
    }


    if (!this->participaDe(club)) {
        return nullptr; //usuário nao esta no clube
    }
    Book* livro = club->getCurrentReading();

    if(livro == nullptr){
        return nullptr; //clube sem leitura atual
    }

    // Primeiro check-in neste clube: cria a métrica agora que existe livro.
    auto it = allMetrics.find(club);
    if (it == allMetrics.end()) {
        allMetrics.insert(std::pair<BookClub*, ReadingMetrics*>(club, new ReadingMetrics(livro)));
        it = allMetrics.find(club);
    }

    if (page < getCurrentPage(club)){
        return nullptr;
    }

    // O clube trocou de livro desde o último check-in: começa do zero.
    if(it->second->getReading() != livro) {
        delete it->second;
        it->second = new ReadingMetrics(livro);
    }

    if(finished == true){
        page = livro->getPages();
    }

    CheckIn* novo = new CheckIn(this, livro, club, page, text);
    this->allCheckIns.push_back(novo);
    club->adicionarCheckIn(novo);

    bool jaTinhaTerminado = it->second->isFinished();



    // page é ONDE a pessoa está, não QUANTO ela leu.
    // O quanto ela leu é a diferença para o último check-in.
    int avanco = page - it->second->getCurrentPage();
    if (avanco > 0) {
        it->second->addPages(avanco);
        this->totalPages += avanco;
        club->setReadPages(club->getReadPages() + avanco);
        }

    if(jaTinhaTerminado == false && it->second->isFinished() == true){
        this->totalBooks++;
    }

    return novo;
} //registra check-in e atualiza paginometro/livrometro
    
bool User::apagarCheckIn(CheckIn* checkIn){
    if (checkIn != nullptr){
        auto it = std::find(this->allCheckIns.begin(), this->allCheckIns.end(), checkIn); //pode apagar APENAS os seus CheckIns
        if(it != this->allCheckIns.end()){
            checkIn->getClub()->eraseCheckIn(checkIn); //o eraseCheckIn do BookClub deve ser um void e APENAS remover a referência ao check-in do clube
            delete *it;
            this->allCheckIns.erase(it);
            return true;
        }
        return false;
    }
    return false;
}

Comment* User::comentar(CheckIn* checkIn, const std::string& text){
    if (checkIn == nullptr){
        return nullptr;
    }

    // Só membros do clube podem comentar nos check-ins dele (US25).
    if (!this->participaDe(checkIn->getClub())) {
        return nullptr;
    }

    // O construtor do Comment lança invalid_argument se o texto for vazio
    // ou passar do limite. Aqui isso vira "comentário recusado" (nullptr).
    try {
        Comment* novo = new Comment(text, this, checkIn, checkIn->getClub());
        checkIn->getComments().push_back(novo);
        return novo;
    } catch (std::invalid_argument& e) {
        return nullptr;
    }
}
    
        //livros: 
bool User::sugerirLivro(BookClub* club, Book* book){
    if (club == nullptr){
        throw std::invalid_argument("Ponteiro de clube do livro inválido");
    }
    if (book == nullptr){
        throw std::invalid_argument("Ponteiro de livro inválido");
    }

    if (this->participaDe(club) && club->adicionarLivro(book)){
        return true;
    }
    return false;
}

bool User::avaliarLeitura (BookClub* club, double grade){
    if (club == nullptr){
        throw std::invalid_argument("Ponteiro de clube do livro inválido");
    }
    try{
        auto it = allMetrics.find(const_cast<BookClub*>(club));

        if (it != allMetrics.end()){
            it->second->setGrade(grade);
            return true;
        }
    } catch (std::invalid_argument& e){
        return false;
    }
    return false;
}

        //métricas:
int User::getTotalPages() const {
    return this->totalPages;
}
int User::getTotalBooks() const {
    return this->totalBooks;
}

ReadingMetrics* User::getMetrica(const BookClub* club) const {
    if (club == nullptr){
        throw std::invalid_argument("Ponteiro de clube do livro inválido");
    }
    // A chave do map é BookClub* (sem const), por isso o const_cast,
    // igual ao getCurrentPage. Ele só serve para procurar; nada é alterado.
    auto it = allMetrics.find(const_cast<BookClub*>(club));
    if (it == allMetrics.end()) {
        return nullptr; //ainda não fez check-in neste clube
    }
    return it->second;
}

        //restauração:
void User::restaurarTotais(int totalPages, int totalBooks){
    if (totalPages < 0 || totalBooks < 0){
        throw std::invalid_argument("Totais do usuário não podem ser negativos");
    }
    this->totalPages = totalPages;
    this->totalBooks = totalBooks;
}

void User::restaurarMetrica(BookClub* club, ReadingMetrics* metrica){
    if (club == nullptr){
        throw std::invalid_argument("Ponteiro de clube do livro inválido");
    }
    if (metrica == nullptr){
        throw std::invalid_argument("Ponteiro de métrica inválido");
    }
    if (!this->participaDe(club)){
        throw std::invalid_argument("Usuário não participa do clube");
    }

    auto it = allMetrics.find(club);
    if (it == allMetrics.end()) {
        allMetrics.insert(std::pair<BookClub*, ReadingMetrics*>(club, metrica));
    } else {
        // Já havia uma métrica: apaga a antiga (o User era dono dela).
        // O if evita apagar a própria métrica nova, se for a mesma.
        if (it->second != metrica) {
            delete it->second;
        }
        it->second = metrica;
    }
}

void User::restaurarCheckIn(CheckIn* checkIn){
    if (checkIn == nullptr){
        throw std::invalid_argument("Ponteiro de check-in inválido");
    }
    if (checkIn->getUser() != this){
        throw std::invalid_argument("Check-in de outro usuário");
    }
    // Primeiro o clube: se ele recusar (lança exceção), o check-in não entra
    // em allCheckIns, e os dois lados continuam de acordo.
    checkIn->getClub()->restaurarCheckIn(checkIn);
    this->allCheckIns.push_back(checkIn);
}
