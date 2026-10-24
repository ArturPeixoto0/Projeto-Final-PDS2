#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
using namespace ftxui;

int main() {
  Element cardCheckIn(std::string nome, std::string data, std::string hora, std::string texto, float progresso){
    return vbox({
    hbox({text(nome),filler(), text(data ), text(hora) }),
    hbox({text("Leitoras de quinta "), text("*Os sete maridos de evelyn hugo"), }) | color(Color::HotPink2),
    separator(),
    text(texto),
    hbox({gauge(progresso)|flex | color(Color::SpringGreen2), text("90% "), text(" pag 30/90")}),
  })|borderLight
  }

auto screen = Screen::Create(Dimension::Fit(desenho));
    Render(screen, );
    screen.Print();
    std::cout << std::endl;
}