#ifndef TELACLUBE_HPP
#define TELACLUBE_HPP

#include <ftxui/dom/elements.hpp>

class ClubProfile;

/// @brief Desenha o perfil de um clube: dados, métricas, clubes, leitura atual e check-ins.
ftxui::Element telaClube(const ClubProfile& perfil);

/// @brief Rodapé com os atalhos da tela do perfil do clube do livro.
ftxui::Element rodapeClube();

#endif