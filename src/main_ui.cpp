#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include "Book.hpp"

using namespace ftxui;

int main() {
    Book livro("Dom Casmurro", "Machado de Assis", "Romance", 256, "123");

    Element desenho = vbox({
        text(livro.getTitle()) | bold,
        text(livro.getAuthor()),
    }) | border;

    auto screen = Screen::Create(Dimension::Fit(desenho));
    Render(screen, desenho);
    screen.Print();
    std::cout << std::endl;
}
