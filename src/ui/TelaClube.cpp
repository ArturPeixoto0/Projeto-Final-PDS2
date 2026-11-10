#include "ui/TelaUsuario.hpp"
#include "ui/Componentes.hpp"
#include "CheckIn.hpp"
#include "ClubProfile.hpp"
#include "User.hpp"
#include "Book.hpp"
#include "BookClub.hpp"
#include <string>

using namespace ftxui;

Element telaClube(const ClubProfile& perfil){
    const BookClub* clube = perfil.club;
    std::string info = std::to_string(perfil.club->())
}