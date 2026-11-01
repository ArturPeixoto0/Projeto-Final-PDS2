#include <string>
#include <vector>
#include <algorithm>
#include "User.hpp"
#include "Book.hpp"
#include "BookClub.hpp"
#include "UserProfile.hpp"
#include "CheckIn.hpp"
#include "Comment.hpp"
#include "ReadingMetrics.hpp"

User::User(const std::string& name,
             const std::string& email,
             const std::string& password) :
            name(name), email(email), password(password), profile(this) {}

             
User::~User() {

    for(int i = 0; i < (int)this->allCheckIns.size(); i++) {
        BookClub* club = this->allCheckIns[i]->getClub();
        if(this->participaDe(club)) {
            club->eraseCheckIn(this->allCheckIns[i]);
        }
    }

    std::vector<BookClub*> copia = this->currentClubs;
    for(int i = 0; i < (int)copia.size(); i++) {
        BookClub* club = copia[i];
            if(club->isAdmin(this)) {
                const std::vector<User*>& membros = club->getMembers();
                for(int j = 0; j < (int)membros.size(); j++){
                    if(membros[j] != this) {
                        club->transferirAdmin(this, membros[j]);
                        break;
                    }
                }
            }
            this->sairClube(club);
    }
    for(auto& metric : allMetrics) {
        delete metric.second;
    }
    for(int i = 0; i < (int)this->allCheckIns.size(); i++) {
        delete this->allCheckIns[i];
    }
}   

/*
User::User(const User&) = delete;
User::User& operator = (const User&) = delete;
*/

        //dados da conta:
std::string User::getName() const {
    return this->name;
} //retorna o nome do usuário

void User::setName(const std::string& name) {
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
    for (int i=0; i < (int)this->currentClubs.size(); i++) {
        if (this->currentClubs[i] == club){
            return true;
        }
    }
    return false;
} //verifica se participa
    
BookClub* User::criarClube(const std::string& clubName, Book* reading) {

    return new BookClub(this, clubName, reading);
}

bool User::entrarClube(BookClub* club, const std::string& codigo) {
    if (club == nullptr || this->participaDe(club) == true) {
        return false;
    }

    if (club->isMember(this) == false) {
        return club->entrarNoClube(this, codigo);
    }

    this->currentClubs.push_back(club);
    allMetrics.insert(std::pair<BookClub*, ReadingMetrics*>(club, new ReadingMetrics(club->getCurrentReading())));
    return true;
}

bool User::sairClube(BookClub* club) {
    if (this->participaDe(club) == false) {
        return false;
    }

    if (club->isMember(this) == true) {
        return club->sairDoClube(this);
    }

    auto it = std::find(this->currentClubs.begin(), this->currentClubs.end(), club);
    this->currentClubs.erase(it);
    delete allMetrics[club];
    allMetrics.erase(club);
    return true;
}

        //check-ins e comentarios:
const std::vector<CheckIn*>& User::getAllCheckIns() const {
    return this->allCheckIns;
}
int User::getCurrentPage(const BookClub* club) const { 
    auto it = allMetrics.find(const_cast<BookClub*>(club));

    if (it != allMetrics.end()){
        return it->second->getCurrentPage();
    }

    return -1; //user n ta no club
}

CheckIn* User::realizarCheckIn(BookClub* club, int page, const std::string& text, bool finished){
    auto it = allMetrics.find(const_cast<BookClub*>(club));

    if(it == allMetrics.end()){
        return nullptr; //usuário nao esta no clube
    }
    Book* livro = club->getCurrentReading();

    if(livro == nullptr){
        return nullptr; //clube sem leitura atual
    }

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
    if (checkIn != nullptr){
        Comment* novo = new Comment(text, this, checkIn, checkIn->getClub());
        checkIn->getComments().push_back(novo);
        return novo;
    }
    return nullptr;   
}
    
        //livros: 
bool User::sugerirLivro(BookClub* club, Book* book){
    if (this->participaDe(club) == true){
        if (club->adicionarLivro(book) == true){
            return true;
        }
        return false;
    }
    return false;
}

bool User::avaliarLeitura (BookClub* club, double grade){
    if (grade > 5 || grade < 0)
        return false;
    auto it = allMetrics.find(const_cast<BookClub*>(club));

    if (it != allMetrics.end()){
        it->second->setGrade(grade);
        return true;
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