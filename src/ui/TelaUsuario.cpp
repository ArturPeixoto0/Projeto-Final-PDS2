#include "ui/TelaUsuario.hpp"
#include "ui/Componentes.hpp"
#include "CheckIn.hpp"
#include "UserProfile.hpp"
#include "User.hpp"
#include "Book.hpp"
#include "BookClub.hpp"
#include <string>

using namespace ftxui;

Element telaUsuario(const UserProfile& usuario){
        std::vector<Element> clubes;
    for (BookClub* c : usuario.getClubs()) {
        clubes.push_back(text(" " + c->getName() + " ") | borderLight);
    }
    
           std::vector<Element> checkins;
    std::vector<CheckIn*> todos = usuario.getAllCheckIn();
    for (int i = (int)todos.size() - 1; i >= 0; i--) {
        checkins.push_back(cardCheckIn(todos[i]));
    }
    if (checkins.empty()) {
        checkins.push_back(text("Nenhum check-in ainda.") | color(Color::GrayLight));
    }

    return vbox({
        text(usuario.getName()),
        text(usuario.getDescription()),
        hbox({
            vbox({
                text(std::to_string(usuario.getTotalPages())),
                text("páginas lidas"),
            })|borderLight | flex,
             vbox({
                text(std::to_string(usuario.getTotalBooks())),
                text("livros lidos"),
            })|borderLight | flex,
             vbox({
                text(std::to_string(usuario.getAllCheckIn().size())),
                text("check-ins"),
            })|borderLight | flex,
            }),

        hbox({
            text("Membro de:"),
            hbox(clubes) 
            }),

        vbox({
            text("CHECK-INS RECENTES") | bold,
            vbox(checkins),
            }),
    });
}

Element rodapeUsuario() {
    return text("←→ abas · ↑↓ navegar · e: editar perfil · q: sair") | color(Color::GrayLight);
}