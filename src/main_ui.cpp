#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <iostream>
#include <algorithm>
#include "Book.hpp"
#include "User.hpp"
#include "BookClub.hpp"
#include "CheckIn.hpp"
#include "ui/Componentes.hpp"
#include "ui/TelaFeed.hpp"
#include "ui/TelaUsuario.hpp"

using namespace ftxui;

int main(){
//DADOS PARA TESTE
Book* evelyn = new Book("Os sete maridos de Evelyn Hugo", "Taylor Jenkins Reid",  "Romance", 360, "001");
Book* dracula = new Book("Drácula","Bram Stoker","Terror",360,"002");
    User* julia  = new User("Julia",  "julia@email.com",  "123");
    User* raquel = new User("Raquel", "raquel@email.com", "123");
    User* artur = new User("artur","tu@gmail.com", "123");
    User* Lior = new User("lior","lior@gmail.com","123");


    BookClub* leitoras = julia->criarClube("Leitoras de Quinta", evelyn);
    raquel->entrarClube(leitoras);
    artur->entrarClube(leitoras);

    BookClub* terror = Lior->criarClube("turma assustada",dracula);
    julia->entrarClube(terror);

    julia->realizarCheckIn(leitoras, 120, "Amando esse capitulo!");
    julia->realizarCheckIn(terror, 120, "que capitulo!");
    raquel->realizarCheckIn(leitoras, 45, "Comecei hoje, ja to presa");
    artur->realizarCheckIn(leitoras, 140, "MEU DEUS to chocado");
    Lior->realizarCheckIn(terror, 140, "MEU DEUS");

    // ---------- INTERFACE ----------
    auto screen = ScreenInteractive::Fullscreen();

  
    std::vector<std::string> abas = {"Feed", "Clubes", "Perfil", "Buscar"};
    int abaSelecionada = 0;
    float rolagem = 0.0f;

    // o Toggle já reage às setas e muda o abaSelecionada
    Component menu = Toggle(&abas, &abaSelecionada);


    Component app = Renderer(menu, [&] {
        Element conteudo;
        Element rodape;
        if (abaSelecionada == 0) {
            conteudo = telaFeed(julia);
            rodape = rodapeFeed();
        } else if (abaSelecionada == 2) {
            conteudo = telaUsuario(julia->getProfile());
            rodape = rodapeUsuario();
        } else {
            conteudo = text("Em construção...") | center;
            rodape = text("←→ abas · q: sair") | color(Color::GrayLight);
        }

        return vbox({
            menu->Render(),
            separator(),
            conteudo | focusPositionRelative(0.0f, rolagem) | vscroll_indicator | frame | flex,
            separator(),
            rodape,
        }) | border;
    });

    // AS TECLAS
    app |= CatchEvent([&](Event evento) {
        if (evento == Event::Character('q')) {
            screen.ExitLoopClosure()();
            return true;
        }
        if (evento == Event::ArrowDown) {
        rolagem = std::min(rolagem + 0.1f, 1.0f);
        return true;
        }
        if (evento == Event::ArrowUp) {
            rolagem = std::max(rolagem - 0.1f, 0.0f);
            return true;
        }
        return false;
    });

    screen.Loop(app);   // só volta daqui quando apertar 'q'

    // ---------- LIMPEZA (nessa ordem!) ----------
    delete leitoras;
    delete terror;
    delete julia;
    delete raquel;
    delete artur;
    delete Lior;
    delete dracula;
    delete evelyn;
    
}