#include "ui/TelaFeed.hpp"
#include "ui/Componentes.hpp"
#include "CheckIn.hpp"
#include "User.hpp"
#include "Book.hpp"
#include "BookClub.hpp"
#include <string>
#include <vector>

using namespace ftxui;

Element telaFeed(const User* usuario){
    std::vector<Element> cards;
    for (BookClub* clube : usuario->getCurrentClubs()) {
        for (CheckIn* c : clube->getFeed()) {
        cards.push_back(cardCheckIn(c));
    }
}
    if(cards.empty()){
        cards.push_back(text("Nenhum check-in ainda. Entre num clube!")|color(Color::GrayLight));
    }

    return vbox({
        vbox(cards),
    });
}

Element rodapeFeed() {
    return text("←→ abas · ↑↓ navegar · n: novo check-in · c: comentar · q: sair") | color(Color::GrayLight);
}