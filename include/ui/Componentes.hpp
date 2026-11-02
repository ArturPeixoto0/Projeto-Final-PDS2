#ifndef COMPONENTES_HPP
#define COMPONENTES_HPP
#include <ftxui/dom/elements.hpp>

class CheckIn;

/// @brief Desenha o card de um check-in (usado no feed, no clube e no perfil).
ftxui::Element cardCheckIn(const CheckIn* c);

#endif