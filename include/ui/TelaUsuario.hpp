#ifndef TELAPERFIL_HPP
#define TELAPERFIL_HPP

#include <ftxui/dom/elements.hpp>

class UserProfile;

/// @brief Desenha o perfil de um usuário: dados, métricas, clubes, leitura atual e check-ins.
ftxui::Element telaUsuario(const UserProfile& usuario);

/// @brief Rodapé com os atalhos da tela do perfil do usuário.
ftxui::Element rodapeUsuario();

#endif