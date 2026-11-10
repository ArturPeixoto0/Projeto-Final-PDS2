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
    const BookClub* clube = perfil.getClub();
    std::string privacidade = clube->isPrivate()? "privado":"público";
    std::string info = std::to_string(clube->getNumberMembers()) + " membros · " + privacidade + " · admin:" + clube->getAdmin()->getName();

    // leitura do mês
    Element leitura;
    Book* livro = clube->getCurrentReading();
    if(livro == nullptr){
        leitura = text("Nenhuma leitura em andamento.") | color(Color::GrayLight);
    }else{
        float media = clube->MediaPorcentagemLeitura()/ 100.0f;
        std::string prazo = "sem prazo definido";
        if (clube->DiasRestantesLeitura() > 0) {
            prazo = "faltam " + std::to_string(clube->DiasRestantesLeitura()) + " dias";
        }
        leitura = vbox({
            text(livro->getTitle() + " · " + livro->getAuthor()) | bold,
            hbox({
                gauge(media) | flex | color(Color::SpringGreen2),
                text(" " + std::to_string((int)(media * 100)) + "% média do clube"),
            }),
            text(prazo) | color(Color::GrayLight),
        });
    }

 // Ranking
    std::vector<Element> ranking;
    int posicao = 1;
    for (User* u : clube->ranking()) {
        ranking.push_back(text(std::to_string(posicao) + "º " + u->getName()
                               + " · pág. " + std::to_string(u->getCurrentPage(clube))));
        posicao++;
    }

 // check-ins   
    std::vector<Element> checkins;
    std::vector<CheckIn*> todos = perfil.getAllCheckIn();
    for (int i = (int)todos.size() - 1; i >= 0; i--) {
        checkins.push_back(cardCheckIn(todos[i]));
    }
    if (checkins.empty()) {
        checkins.push_back(text("Nenhum check-in ainda.") | color(Color::GrayLight));
    }
    
    return vbox({
        text(perfil.getName()) | bold,
        text(info) | color(Color::GrayLight),
        separator(),
        text("LEITURA DO MÊS") | bold,
        leitura | borderLight,
        text("RANKING") | bold,
        vbox(ranking),
        separator(),
        text("CHECK-INS RECENTES") | bold,
        vbox(checkins),
    });
}

Element rodapeClube() {
    return text("←→ abas · ↑↓ rolar · e: entrar · n: novo check-in · q: sair") | color(Color::GrayLight);
}

