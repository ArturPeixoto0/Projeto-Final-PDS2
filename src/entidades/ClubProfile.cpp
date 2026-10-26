#include "ClubProfile.hpp"
#include "BookClub.hpp"
#include "User.hpp"
#include "CheckIn.hpp"
#include "Book.hpp"
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

ClubProfile::ClubProfile(BookClub* club) : club(club) {}

std::string ClubProfile::getName() const {
    return this->club->getName();
}

std::vector<CheckIn*> ClubProfile::getAllCheckIn() const {
    return this->club->getFeed();
}

std::vector<User*> ClubProfile::getUsers() const {
    return this->club->getMembers();
}

void ClubProfile::show_name() const {
    std::cout << this->getName() << std::endl;
}

void ClubProfile::show_pfp() const {
    if (this->getPhotoPath().empty()) {
        std::cout << "(sem foto do clube)" << std::endl;
    } else {
        std::cout << "Foto: " << this->getPhotoPath() << std::endl;
    }
}

void ClubProfile::show_description() const {
    if (this->getDescription().empty()) {
        std::cout << "(sem descricao)" << std::endl;
    } else {
        std::cout << this->getDescription() << std::endl;
    }
}

void ClubProfile::show_checkins() const {
    std::vector<CheckIn*> feed = this->getAllCheckIn();
    if (feed.empty()) {
        std::cout << "Nenhum check-in ainda." << std::endl;
        return;
    }

    // Do mais novo para o mais antigo.
    for (int i = (int)feed.size() - 1; i >= 0; i--) {
        CheckIn* c = feed[i];
        std::cout << "[" << c->getDataHora() << "] "
                  << c->getUser()->getName() << " | pagina "
                  << c->getPage() << ": " << c->getText() << std::endl;
    }
}

void ClubProfile::show_users() const {
    std::vector<User*> membros = this->getUsers();
    if (membros.empty()) {
        std::cout << "Nenhum membro." << std::endl;
        return;
    }

    for (int i = 0; i < (int)membros.size(); i++) {
        std::cout << "- " << membros[i]->getName();
        if (this->club->isAdmin(membros[i])) {
            std::cout << " (admin)";
        }
        std::cout << std::endl;
    }
}

void ClubProfile::show_current_reading() const {
    Book* livro = this->club->getCurrentReading();
    if (livro == nullptr) {
        std::cout << "Nenhuma leitura em andamento." << std::endl;
        return;
    }
    std::cout << "Lendo agora: " << livro->getTitle()
              << " (" << livro->getAuthor() << ")" << std::endl;

    int dias = this->club->DiasRestantesLeitura();
    if (dias > 0) {
        std::cout << "Faltam " << dias << " dia(s) para o fim do prazo." << std::endl;
    }
}

void ClubProfile::show_metrics() const {
    // fixed + setprecision(1): mostra números com uma casa decimal (ex.: 42.5).
    // Essas configurações ficam "grudadas" no cout, então guardamos as antigas
    // para devolver no final e não bagunçar o resto do programa.
    std::ios::fmtflags formatoAntigo = std::cout.flags();
    std::streamsize precisaoAntiga = std::cout.precision();
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Membros: " << this->club->getNumberMembers() << std::endl;
    std::cout << "Paginas lidas no clube: " << this->club->getReadPages() << std::endl;
    std::cout << "Livros lidos no clube: " << this->club->getReadBooks() << std::endl;
    std::cout << "Media de paginas por membro: " << this->club->MediaPaginasPorUser() << std::endl;

    if (this->club->isReading()) {
        std::cout << "Progresso medio no livro atual: "
                  << this->club->MediaPorcentagemLeitura() << "%" << std::endl;
        std::cout << "Membros que terminaram: " << this->club->TotalTerminaram()
                  << " (" << this->club->PorcentagemTerminaram() << "%)" << std::endl;
    }

    std::cout.flags(formatoAntigo);
    std::cout.precision(precisaoAntiga);
}

void ClubProfile::showReadingStatus() const {
    Book* livro = this->club->getCurrentReading();
    if (livro == nullptr) {
        std::cout << "Nenhuma leitura em andamento." << std::endl;
        return;
    }

    std::vector<User*> ranking = this->club->ranking();
    for (int i = 0; i < (int)ranking.size(); i++) {
        int pagina = ranking[i]->getCurrentPage(this->club);
        if (pagina < 0) {
            pagina = 0;
        }
        std::cout << (i + 1) << ". " << ranking[i]->getName()
                  << " - pagina " << pagina << " de " << livro->getPages();
        if (pagina >= livro->getPages()) {
            std::cout << "  *** TERMINOU ***";
        }
        std::cout << std::endl;
    }
}
