#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include "Book.hpp"
#include "User.hpp"
#include "BookClub.hpp"
#include "CheckIn.hpp"
#include "ui/Componentes.hpp"

using namespace ftxui;

int main(){
//DADOS PARA TESTE
Book* evelyn = new Book("Os sete maridos de Evelyn Hugo", "Taylor Jenkins Reid",  "Romance", 360, "001");
    User* julia  = new User("Julia",  "julia@email.com",  "123");
    User* raquel = new User("Raquel", "raquel@email.com", "123");
    User* artur = new User("artur","tu@gmail.com", "123");


    BookClub* leitoras = julia->criarClube("Leitoras de Quinta", evelyn);
    raquel->entrarClube(leitoras);
    artur->entrarClube(leitoras);

    julia->realizarCheckIn(leitoras, 120, "Amando esse capitulo!");
    raquel->realizarCheckIn(leitoras, 45, "Comecei hoje, ja to presa");
    artur->realizarCheckIn(leitoras, 140, "MEU DEUS to chocado");

    // ---------- DESENHO ----------
    std::vector<Element> linhas;
    for (CheckIn* c : leitoras->getFeed()) {
        linhas.push_back(cardCheckIn(c));
    }
    Element desenho = vbox(linhas) | border;

    auto screen = Screen::Create(Dimension::Fit(desenho));
    Render(screen, desenho);
    screen.Print();
    std::cout << std::endl;

    // ---------- LIMPEZA (nessa ordem!) ----------
    delete leitoras;
    delete julia;
    delete raquel;
    delete artur;
    delete evelyn;
}