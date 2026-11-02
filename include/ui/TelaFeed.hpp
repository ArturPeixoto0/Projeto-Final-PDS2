#ifndef TELAFEED_HPP
#define TELAFEED_HPP

#include <ftxui/dom/elements.hpp>

class User;

/// @brief Desenha o feed do usuário: os check-ins de todos os clubes dele.
ftxui::Element telaFeed(const User* usuario);

#endif