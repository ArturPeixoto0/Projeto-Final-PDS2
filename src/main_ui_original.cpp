#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
using namespace ftxui;

int main() {
  auto doc = text("Olá, Clube do Livro!") | bold | borderDouble | color(Color::RGB(232, 160, 191));
  auto screen = Screen::Create(Dimension::Fit(doc));
  Render(screen, doc);
  screen.Print();
  std::cout << "\n";
}
