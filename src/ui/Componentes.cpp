#include "ui/Componentes.hpp"
#include "CheckIn.hpp"
#include "User.hpp"
#include "Book.hpp"
#include "BookClub.hpp"
#include <string>

using namespace ftxui;

Element cardCheckIn(const CheckIn* c){
    int pagina = c->getPage();
    int totalPaginas = c->getBook()->getPages();

    float progresso = (float)pagina / totalPaginas;   // de 0.0 a 1.0, pro gauge
    int porcentagem = (int)(progresso * 100);          // de 0 a 100, pro texto

    return vbox({
        hbox({ text(c->getUser()->getName()) | bold, filler(), text(c->getDataHora()) }),
        hbox({ text(c->getClub()->getName()), text(" · "), text(c->getBook()->getTitle()) })
            | color(Color::HotPink2),
        separator(),
        text(c->getText()),
        hbox({
            gauge(progresso) | flex | color(Color::SpringGreen2),
            text(" " + std::to_string(porcentagem) + "%"),
            text("  pág: " + std::to_string(pagina) + "/" + std::to_string(totalPaginas)),
        }),
        text("💬 "+ std::to_string(c->getComments().size())) | color(Color::GrayLight),
    }) | borderLight;
}
