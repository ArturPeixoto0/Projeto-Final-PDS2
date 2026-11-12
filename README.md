# Projeto-Final-PDS2

## Integrantes:
| Matrícula |  Nome | 
|-- | -- |
| 2026046560 | Artur Peixoto Gomes da Costa | 
| 2026046934 | Julia de Oliveira Delfino |
| 2026046705 | Lior Kestenberg Boukai Leitão | 
| 2026046551 | Raquel Leite de Oliveira | 

## Descrição Inicial do Problema:
  Usualmente, Clubes do Livro são organizados por grupos em redes sociais diversas, como WhatsApp, Telegram, Instagram. Contudo, as discussões podem acabar se perdendo no meio de outros assuntos e conversas, dificultando o acompanhamento do progresso da leitura geral e de cada membro, das leituras antigas e futuras e das avaliações de cada livro. 

## Objetivos:
- Facilitar a organização de Clubes do Livro e melhorar a experiência de quem já está em um
- Permitir o acompanhamento do progresso de leitura dos demais membros
- Fomentar uma competição saudável entre os membros
- Estimular uma constância de leitura

## Motivação:
  Desde o começo o grupo já desejava realizar um trabalho voltado a livros, inicialmente seria feito uma plataforma similar ao Kindle - "Netflix de Livros", na qual seria possível realizar a leitura de obras de domínio público, depois, pensamos em elaborar uma ferramenta voltada a aulas e professores de literatura, onde os docentes poderiam acompanhar as leituras que passaram para seus alunos, assim, percebemos que o nosso foco deveria ser a experiência de ler em conjunto, fazendo com que pensemos em um sistema para Clubes do Livro.
  Com isso, decidimos criar um sistema que funciona como um "GymRats" para clubes do livro, pois, de forma similar a aplicativos de academia, em que cada integrante registra seus treinos para incentivar os demais, os leitores poderão registrar o progresso de quantas páginas leram e avaliar a leitura ao finalizá-la, centralizando os aspectos essenciais de um clube do livro em um único local.

## Visão geral da solução
O sistema foi feito em C++ com orientação a objetos. As principais funcionalidades são:
- Usuários: cadastro com nome, email e senha, e perfil com descrição.
- Clubes do livro: criação de clubes, entrada e saída de membros e um administrador por clube.
- Check-ins: o usuário registra a página em que parou com um comentário sobre a leitura; os outros membros podem comentar nos check-ins.
- Métricas e ranking: paginômetro e livrômetro de cada usuário, ranking de leitura do clube, média de páginas e média das avaliações.
- Persistência: os dados são salvos e carregados em arquivos JSON (pasta dados/) pela classe BancoDeDados.
- Interface: interface no terminal feita com a biblioteca FTXUI, com abas de Feed, Clubes e Perfil e janelas para novo check-in, novo clube e comentários.

## Classes

| Classe | Responsabilidade |
|---|---|
| `User` | Dados da conta e ações do usuário (criar/entrar em clube, check-in, comentar, avaliar) |
| `BookClub` | Membros, administrador, leitura atual, feed |
| `Book` | Dados de um livro (título, autor, gênero, páginas, ISBN) |
| `CheckIn` | Registro de progresso de leitura (página, texto, data) e seus comentários |
| `Comment` | Comentário feito em um check-in |
| `ReadingMetrics` | Progresso de um usuário na leitura atual de um clube |
| `Profile` | Classe abstrata com o que todo perfil tem (nome, descrição) |
| `UserProfile` | Perfil do usuário (métricas pessoais, clubes, check-ins) |
| `ClubProfile` | Perfil do clube (membros, feed, leitura atual, ranking, métricas) |
| `BancoDeDados` | Salva e carrega todos os objetos em arquivos JSON |

A modelagem completa está nos cartões CRC (design/CRC Maker.pdf) e as histórias de usuário estão em design/User Stories - trabalho PDS2.md.

## Estrutura do repositório

```
include/           headers das classes (.hpp)
include/ui/        headers das telas da interface
src/entidades/     implementação das classes (.cpp)
src/persistencia/  BancoDeDados (salvar/carregar em JSON)
src/ui/            telas da interface (FTXUI)
src/main_ui.cpp    programa principal (interface)
tests/             testes de unidade (doctest)
dados/             arquivos JSON salvos pelo programa
design/            cartões CRC e histórias de usuário
```

## Requisitos
- Compilador C++ com suporte a C++11 (testes) e C++17 (interface): g++ ou clang++
- make
- cmake 3.14 ou mais novo (só para a interface)
- Internet na primeira compilação da interface (o CMake baixa o FTXUI automaticamente)

## Como rodar os testes
Sempre a partir da pasta raiz do projeto:

```bash
make test                # roda todos os testes
make testUser test       # roda só os testes de uma classe (testBook, testBookClub, testCheckIn, ...)
```

### Cobertura dos testes
A cobertura é medida com o [gcovr](https://gcovr.com) (instalação: `pip install gcovr`):

```bash
make coverage            # recompila com --coverage, roda os testes e mostra a cobertura de cada arquivo
```

## Como rodar a interface
```bash
cmake -S . -B build-ui           # só na primeira vez
cmake --build build-ui
./build-ui/clube_ui
```

Teclas: ← → trocam de aba, ↑ ↓ rolam a tela, n abre um novo check-in, c cria um clube, m abre os comentários, Tab passa de um campo para outro, Esc fecha a janela e q sai.
Documentação

## A documentação das classes é gerada com o Doxygen:

```bash
doxygen Doxyfile

