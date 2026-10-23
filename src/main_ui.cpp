#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
using namespace ftxui;

int main() {
  Element desenho = vbox({
    hbox({text("Julia"),filler(), text("23/10 " ), text("13:33") }),
    hbox({text("Leitoras de quinta "), text("*Os sete maridos de evelyn hugo"), }) | color(Color::HotPink2),
    separator(),
    text("Que cap incrivel!"),
    hbox({gauge(0.9)|flex | color(Color::SpringGreen2), text("90% "), text(" pag 30/90")}),
  })|borderLight;

auto screen = Screen::Create(Dimension::Fit(desenho));
    Render(screen, desenho);
    screen.Print();
    std::cout << std::endl;
}