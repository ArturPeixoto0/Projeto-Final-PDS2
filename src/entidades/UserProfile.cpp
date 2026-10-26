#include "UserProfile.hpp"
#include "User.hpp"
#include "BookClub.hpp"
#include "CheckIn.hpp"
#include "Book.hpp"
#include <iostream>
#include <string>
#include <vector>

UserProfile::UserProfile(User* user) : user(user) {}

int UserProfile::getTotalPages() const {
    return this->user->getTotalPages();
}

int UserProfile::getTotalBooks() const {
    return this->user->getTotalBooks();
}

std::string UserProfile::getName() const {
    return this->user->getName();
}

std::vector<CheckIn*> UserProfile::getAllCheckIn() const {
    return this->user->getAllCheckIns();
}

std::vector<BookClub*> UserProfile::getClubs() const {
    return this->user->getCurrentClubs();
}

void UserProfile::show_name() const {
    std::cout << this->getName() << std::endl;
}

void UserProfile::show_pfp() const {
    if (this->getPhotoPath().empty()) {
        std::cout << "(sem foto de perfil)" << std::endl;
    } else {
        std::cout << "Foto: " << this->getPhotoPath() << std::endl;
    }
}

void UserProfile::show_description() const {
    if (this->getDescription().empty()) {
        std::cout << "(sem descricao)" << std::endl;
    } else {
        std::cout << this->getDescription() << std::endl;
    }
}

void UserProfile::show_checkins() const {
    std::vector<CheckIn*> checkIns = this->getAllCheckIn();
    if (checkIns.empty()) {
        std::cout << "Nenhum check-in ainda." << std::endl;
        return;
    }

    // O vetor está do mais antigo para o mais novo; andamos de trás para frente.
    for (int i = (int)checkIns.size() - 1; i >= 0; i--) {
        CheckIn* c = checkIns[i];
        std::cout << "[" << c->getDataHora() << "] "
                  << c->getClub()->getName() << " | "
                  << c->getBook()->getTitle() << " | pagina "
                  << c->getPage() << ": " << c->getText() << std::endl;
    }
}

void UserProfile::show_clubs() const {
    std::vector<BookClub*> clubes = this->getClubs();
    if (clubes.empty()) {
        std::cout << "Nao participa de nenhum clube." << std::endl;
        return;
    }

    for (int i = 0; i < (int)clubes.size(); i++) {
        std::cout << "- " << clubes[i]->getName();
        if (clubes[i]->isAdmin(this->user)) {
            std::cout << " (admin)";
        }
        std::cout << std::endl;
    }
}
