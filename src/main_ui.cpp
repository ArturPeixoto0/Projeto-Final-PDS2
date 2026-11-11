#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <stdexcept>
#include "Book.hpp"
#include "User.hpp"
#include "BookClub.hpp"
#include "CheckIn.hpp"
#include "ui/Componentes.hpp"
#include "ui/TelaFeed.hpp"
#include "ui/TelaUsuario.hpp"
#include "ui/TelaClube.hpp"
#include "ClubProfile.hpp"
#include "Comment.hpp"

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

    ClubProfile perfilLeitoras(leitoras);

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

    int abaAnterior = 0;
    std::string aviso;
    // o Toggle já reage às setas e muda o abaSelecionada
    Component menu = Toggle(&abas, &abaSelecionada);


    Component app = Renderer(menu, [&] {
        if(abaSelecionada != abaAnterior){
            rolagem = 0.0f;
            aviso = "";
            abaAnterior = abaSelecionada;
        }
        Element conteudo;
        Element rodape;
        if (abaSelecionada == 0) {
            conteudo = telaFeed(julia);
            rodape = rodapeFeed();
        } else if (abaSelecionada == 2) {
            conteudo = telaUsuario(julia->getProfile());
            rodape = rodapeUsuario();
        }else if(abaSelecionada==1){
            conteudo = telaClube(perfilLeitoras);
             rodape = rodapeClube();
        }else {
            conteudo = text("Em construção...") | center;
            rodape = text("←→ abas · q: sair") | color(Color::GrayLight);
        }
        if (!aviso.empty()) {
            rodape = vbox({ text(aviso) | color(Color::Yellow), rodape});
        }
        return vbox({
            menu->Render(),
            separator(),
            conteudo | focusPositionRelative(0.0f, rolagem) | vscroll_indicator | frame | flex,
            separator(),
            rodape,
        }) | border;
    });


    // Caixas de texto de UMA linha (o Enter não pula linha)
    InputOption umaLinha;
    umaLinha.multiline = false;

    // ---------- JANELA DE NOVO CHECK-IN ----------
    
    bool mostrarNovoCheckIn = false;
    std::vector<std::string> nomesClubes;   // preenchido quando a janela abre
    int clubeEscolhido = 0;
    std::string paginaDigitada;
    std::string textoDigitado;
    std::string mensagemErro;


    Component escolhaClube = Radiobox(&nomesClubes, &clubeEscolhido);
    Component campoPagina  = Input(&paginaDigitada, "ex.: 120", umaLinha);
    Component campoTexto   = Input(&textoDigitado, "O que achou?", umaLinha);

    Component botaoPublicar = Button("Publicar", [&] {
        //a página precisa ser um número
        int pagina = 0;
        try {
            pagina = std::stoi(paginaDigitada);
        } catch (...) {
            mensagemErro = "Digite a página como número.";
            return;
        }
        //pede pro User fazer o check-in (as regras ficam na classe)
        BookClub* clube = julia->getCurrentClubs()[clubeEscolhido];
        try {
            if (julia->realizarCheckIn(clube, pagina, textoDigitado) == nullptr) {
                mensagemErro = "Esse clube não tem leitura atual.";
                return;
            }
        } catch (const std::exception& erro) {
            mensagemErro = erro.what();
            return;
        }
        
        paginaDigitada = "";
        textoDigitado = "";
        mensagemErro = "";
        mostrarNovoCheckIn = false;
    });

    Component botaoCancelar = Button("Cancelar", [&] {
        mensagemErro = "";
        mostrarNovoCheckIn = false;
    });

    // ORDEM 
    int campoAtual = 0;   // qual campo está selecionado (0 a 3)
    Component formulario = Container::Vertical({
        escolhaClube,
        campoPagina,
        campoTexto,
        Container::Horizontal({ botaoPublicar, botaoCancelar }),
    }, &campoAtual);

    // O Radiobox "segura" o Tab pra ele; aqui a gente garante que
    // Tab e Shift+Tab sempre pulam de um campo pro outro.
    formulario |= CatchEvent([&](Event evento) {
        if (evento == Event::Tab) {
            campoAtual = (campoAtual + 1) % 4;
            return true;
        }
        if (evento == Event::TabReverse) {
            campoAtual = (campoAtual + 3) % 4;
            return true;
        }
        if (evento == Event::Escape) {   // Esc fecha a janela
            mensagemErro = "";
            mostrarNovoCheckIn = false;
            return true;
        }
        return false;
    });

    // DESENHO da janela
    Component janelaNovoCheckIn = Renderer(formulario, [&] {
        return vbox({
            text("Novo check-in") | bold,
            separator(),
            text("Clube:"),
            escolhaClube->Render(),
            separator(),
            hbox({ text("Parei na página: "), campoPagina->Render() }),
            text("O que achou?"),
            campoTexto->Render() | border,
            hbox({ botaoPublicar->Render(), botaoCancelar->Render() }),
            text(mensagemErro) | color(Color::Red),
        }) | border | size(WIDTH, EQUAL, 50);
    });

    // ---------- JANELA DE NOVO CLUBE ----------
    // ESTADO
    bool mostrarNovoClube = false;
    std::string nomeNovoClube;
    std::string tituloLivro;
    std::string autorLivro;
    std::string generoLivro;
    std::string paginasLivro;
    std::string isbnLivro;
    std::string erroNovoClube;
    std::vector<BookClub*> clubesCriados;   // o main é dono deles: apagar no final!
    std::vector<Book*> livrosCriados;       // idem

    // PEÇAS
    Component campoNomeClube = Input(&nomeNovoClube, "ex.: Leitoras de Sexta", umaLinha);
    Component campoTitulo    = Input(&tituloLivro, "título", umaLinha);
    Component campoAutor     = Input(&autorLivro, "autor", umaLinha);
    Component campoGenero    = Input(&generoLivro, "gênero", umaLinha);
    Component campoPaginas   = Input(&paginasLivro, "ex.: 320", umaLinha);
    Component campoIsbn      = Input(&isbnLivro, "ISBN", umaLinha);

    Component botaoCriar = Button("Criar", [&] {
        // 1. as páginas precisam ser um número
        int paginas = 0;
        try {
            paginas = std::stoi(paginasLivro);
        } catch (...) {
            erroNovoClube = "Digite o número de páginas como número.";
            return;
        }
        // 2. cria o livro (o construtor de Book confere título e páginas)
        Book* livro = nullptr;
        try {
            livro = new Book(tituloLivro, autorLivro, generoLivro, paginas, isbnLivro);
        } catch (const std::exception& erro) {
            erroNovoClube = erro.what();
            return;
        }
        // 3. cria o clube com esse livro (o construtor de BookClub confere o nome)
        try {
            BookClub* novo = julia->criarClube(nomeNovoClube, livro);
            clubesCriados.push_back(novo);
            livrosCriados.push_back(livro);
        } catch (const std::exception& erro) {
            delete livro;   // o clube não foi criado: o livro não serve pra nada
            erroNovoClube = erro.what();
            return;
        }
        // 4. deu certo: limpa e fecha
        nomeNovoClube = tituloLivro = autorLivro = generoLivro = paginasLivro = isbnLivro = "";
        erroNovoClube = "";
        mostrarNovoClube = false;
    });
    Component botaoCancelarClube = Button("Cancelar", [&] {
        erroNovoClube = "";
        mostrarNovoClube = false;
    });

    Component formularioClube = Container::Vertical({
        campoNomeClube,
        campoTitulo,
        campoAutor,
        campoGenero,
        campoPaginas,
        campoIsbn,
        Container::Horizontal({ botaoCriar, botaoCancelarClube }),
    });

    formularioClube |= CatchEvent([&](Event evento) {
        if (evento == Event::Escape) {   // Esc fecha a janela
            erroNovoClube = "";
            mostrarNovoClube = false;
            return true;
        }
        return false;
    });

    // DESENHO
    Component janelaNovoClube = Renderer(formularioClube, [&] {
        return vbox({
            text("Novo clube") | bold,
            separator(),
            hbox({ text("Nome do clube: "), campoNomeClube->Render() }),
            separator(),
            text("Primeira leitura") | bold,
            hbox({ text("Título:  "), campoTitulo->Render() }),
            hbox({ text("Autor:   "), campoAutor->Render() }),
            hbox({ text("Gênero:  "), campoGenero->Render() }),
            hbox({ text("Páginas: "), campoPaginas->Render() }),
            hbox({ text("ISBN:    "), campoIsbn->Render() }),
            separator(),
            hbox({ botaoCriar->Render(), botaoCancelarClube->Render() }),
            text(erroNovoClube) | color(Color::Red),
        }) | border | size(WIDTH, EQUAL, 50);
    });



    // ---------- JANELA DE COMENTÁRIOS ----------
    // ESTADO
    bool mostrarComentarios = false;
    std::vector<CheckIn*> checkinsVisiveis;    // preenchidos quando a janela abre
    std::vector<std::string> nomesCheckIns;
    int checkinEscolhido = 0;
    std::string textoComentario;
    std::string erroComentario;

    // PEÇAS
    // O Menu muda o checkinEscolhido conforme as setas, então os comentários
    // embaixo acompanham o check-in que está marcado.
    Component listaCheckIns   = Menu(&nomesCheckIns, &checkinEscolhido);
    Component campoComentario = Input(&textoComentario, "Escreva um comentário", umaLinha);

    Component botaoComentar = Button("Comentar", [&] {
        CheckIn* escolhido = checkinsVisiveis[checkinEscolhido];
        // as regras (ser membro do clube, tamanho do texto) ficam no User e no Comment
        if (julia->comentar(escolhido, textoComentario) == nullptr) {
            erroComentario = "Comentário vazio ou com mais de "
                           + std::to_string(Comment::MAX_LENGTH) + " caracteres.";
            return;
        }
        textoComentario = "";
        erroComentario = "";
        // a janela continua aberta, pra ver o comentário novo na lista
    });
    Component botaoFecharComentarios = Button("Fechar", [&] {
        erroComentario = "";
        mostrarComentarios = false;
    });

    int campoComentarioAtual = 0;   // qual campo está selecionado (0 a 2)
    Component formularioComentario = Container::Vertical({
        listaCheckIns,
        campoComentario,
        Container::Horizontal({ botaoComentar, botaoFecharComentarios }),
    }, &campoComentarioAtual);

    formularioComentario |= CatchEvent([&](Event evento) {
        if (evento == Event::Tab) {
            campoComentarioAtual = (campoComentarioAtual + 1) % 3;
            return true;
        }
        if (evento == Event::TabReverse) {
            campoComentarioAtual = (campoComentarioAtual + 2) % 3;
            return true;
        }
        if (evento == Event::Escape) {   // Esc fecha a janela
            erroComentario = "";
            mostrarComentarios = false;
            return true;
        }
        return false;
    });

    // DESENHO
    Component janelaComentarios = Renderer(formularioComentario, [&] {
        // os comentários do check-in marcado na lista
        std::vector<Element> comentarios;
        if (!checkinsVisiveis.empty()) {
            for (Comment* com : checkinsVisiveis[checkinEscolhido]->getComments()) {
                comentarios.push_back(hbox({
                    text(com->get_author()->getName() + ": ") | bold,
                    paragraph(com->get_text()),
                }));
            }
        }
        if (comentarios.empty()) {
            comentarios.push_back(text("Nenhum comentário ainda.") | color(Color::GrayLight));
        }

        return vbox({
            text("Comentários") | bold,
            separator(),
            text("Escolha o check-in:"),
            listaCheckIns->Render() | frame | size(HEIGHT, LESS_THAN, 6),
            separator(),
            vbox(comentarios) | frame | size(HEIGHT, LESS_THAN, 8),
            separator(),
            campoComentario->Render() | border,
            hbox({ botaoComentar->Render(), botaoFecharComentarios->Render() }),
            text(erroComentario) | color(Color::Red),
        }) | border | size(WIDTH, EQUAL, 60);
    });

    // AS TECLAS
    app |= CatchEvent([&](Event evento) {
        if (evento == Event::Character('n')) {
            // atualiza a lista de clubes toda vez que a janela abre
            nomesClubes.clear();
            for (BookClub* c : julia->getCurrentClubs()) {
                nomesClubes.push_back(c->getName());
            }
            if (!nomesClubes.empty()) {
                clubeEscolhido = 0;
                campoAtual = 0;
                aviso = "";
                mostrarNovoCheckIn = true;
            } 
            else {
                aviso = "Você não está em nenhum clube. Aperte c para criar um.";
            }
            return true;
        }
        if (evento == Event::Character('c')) {
            mostrarNovoClube = true;
            return true;
        }
        if (evento == Event::Character('m')) {
            // monta a lista com os check-ins dos clubes da usuária
            checkinsVisiveis.clear();
            nomesCheckIns.clear();
            for (BookClub* clube : julia->getCurrentClubs()) {
                for (CheckIn* c : clube->getFeed()) {
                    checkinsVisiveis.push_back(c);
                    nomesCheckIns.push_back(c->getUser()->getName() + ": " + c->getText());
                }
            }
            if (!checkinsVisiveis.empty()) {
                checkinEscolhido = 0;
                campoComentarioAtual = 0;
                aviso = "";
                mostrarComentarios = true;
            } else {
                aviso = "Nenhum check-in para comentar ainda.";
            }
            return true;
        }
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

    // a janela aparece por cima do app quando mostrarNovoCheckIn for true
    app |= Modal(janelaNovoCheckIn, &mostrarNovoCheckIn);
    app |= Modal(janelaNovoClube, &mostrarNovoClube);
    app |= Modal(janelaComentarios, &mostrarComentarios);

    screen.Loop(app);   // só volta daqui quando apertar 'q'

    // ---------- LIMPEZA (nessa ordem!) ----------
    for (BookClub* c : clubesCriados) {
        delete c;
    }
    delete leitoras;
    delete terror;
    delete julia;
    delete raquel;
    delete artur;
    delete Lior;
    delete dracula;
    delete evelyn;
    for (Book* l : livrosCriados) {
        delete l;
    }
    
}
