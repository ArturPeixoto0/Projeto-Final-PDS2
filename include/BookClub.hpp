/**
 * @file BookClub.hpp
 * @brief Declara a classe BookClub.
 */
#ifndef BOOKCLUB_H
#define BOOKCLUB_H
 
#include <ctime>
#include <map>
#include <string>
#include <vector>
 
class User;
class CheckIn;
class Book;
class Comment;
 
/**
 * @brief Representa um clube do livro onde usuários acompanham uma leitura juntos.
 *
 * Armazena os dados do clube (nome, tipo, privacidade, código de convite,
 * administrador, membros, catálogo de livros, feed de check-ins e data de
 * criação), as métricas de leitura (páginas lidas, livros lidos e tempo de
 * leitura) e regula as ações de administrador.
 *
 * O catálogo é único: cada livro é uma CatalogEntry com status (futuro, atual
 * ou passado), avaliações e votos daquele livro neste clube.
 *
 * Posse dos objetos:
 *  - O clube NÃO é dono dos CheckIn do feed: quem cria e apaga os check-ins
 *    é o User autor (ver User::realizarCheckIn e User::apagarCheckIn).
 *    O feed guarda só ponteiros para eles.
 *  - O clube NÃO é dono de User nem de Book, que existem fora dele.
 *
 * O clube não pode ser copiado: uma cópia teria os mesmos membros e o
 * mesmo feed, e os dois clubes ficariam inconsistentes.
 */
class BookClub {
    public:
        /** @brief Situação de um livro no clube. */
        enum class Status { Futuro, Atual, Passado };
 
        /** @brief Item do catálogo: um livro com seu status, notas e votos neste clube. */
        struct CatalogEntry {
            Book* book;                          ///< Livro (não pertence ao clube).
            Status status;                       ///< Futuro, atual ou passado.
            std::map<const User*, int> ratings;  ///< Nota (1 a 5) dada por cada membro.
            std::vector<const User*> votes;      ///< Membros que votaram neste livro como próxima leitura.
        };
 
    private:
        std::string name_;
        std::string type_;
        std::string inviteCode_;
        bool isPrivate_;
        bool active_;
        User* admin_;
        std::vector<User*> members_;
        std::vector<CatalogEntry> catalog_;
        std::vector<CheckIn*> feed_;
        int readPages_;
        int readBooks_;
        std::time_t clubCreation_;
        std::time_t startReading_;
        std::time_t endReading_;
        int id_;                ///< Identificador único do clube (usado nos arquivos de dados).
        static int proximoId_;  ///< Próximo id a ser gerado (um só contador para todos os clubes).
 
    public:
        /**
         * @brief Cria um clube do livro.
         * @param founder Usuário que cria o clube. É adicionado como membro e
         *        como administrador.
         * @param name Nome do clube.
         * @param type Tipo do clube.
         * @param isPrivate Se true, só entra quem tiver o código de convite.
         * @param id Identificador do clube. Se for 0 (padrão), um id novo é
         *        gerado. Se for maior que 0, esse id é usado (ao carregar dos
         *        arquivos) e os próximos ids gerados serão maiores que ele.
         * @throws std::invalid_argument se founder for nulo, se o nome for
         *         vazio ou se id for negativo.
         */
        BookClub(User* founder,
                 const std::string& name,
                 const std::string& type,
                 bool isPrivate = false,
                 int id = 0);

        /**
         * @brief Cria um clube do livro que já começa com uma leitura atual.
         *        Usado por User::criarClube.
         * @param founder Usuário que cria o clube (membro e administrador).
         * @param name Nome do clube.
         * @param firstReading Livro que vira a leitura atual. Se for nullptr,
         *        o clube começa sem leitura.
         * @param type Tipo do clube.
         * @param isPrivate Se true, só entra quem tiver o código de convite.
         * @param id Identificador do clube (0 gera um novo; ver o outro construtor).
         * @throws std::invalid_argument se founder for nulo, se o nome for
         *         vazio ou se id for negativo.
         */
        BookClub(User* founder,
                 const std::string& name,
                 Book* firstReading,
                 const std::string& type = "democratico",
                 bool isPrivate = false,
                 int id = 0);

        BookClub(const BookClub&) = delete;
        BookClub& operator=(const BookClub&) = delete;
 
        /**
         * @brief Destrói o clube. Não apaga CheckIn, User nem Book (o clube
         *        não é dono deles), mas tira o clube da lista de cada membro
         *        para que nenhum User fique apontando para um clube destruído.
         */
        ~BookClub();
 
        // ---------- GETTERS ----------
 
        /** @brief Retorna o identificador único do clube. */
        int getId() const;
        /** @brief Retorna o nome do clube. */
        const std::string& getName() const;
        /** @brief Retorna o tipo do clube. */
        const std::string& getType() const;
        /** @brief Retorna true se o clube é privado. */
        bool isPrivate() const;
        /**
         * @brief Retorna true se o clube está ativo (não encerrado).
         *        Como o administrador nunca sai (US28), hoje todo clube
         *        fica ativo; o atributo continua para ser salvo nos arquivos.
         */
        bool isActive() const;
        /** @brief Retorna true se há uma leitura em andamento (livro atual no catálogo). */
        bool isReading() const;
        /** @brief Retorna o administrador do clube. */
        User* getAdmin() const;
        /** @brief Retorna os membros do clube. */
        const std::vector<User*>& getMembers() const;
        /** @brief Retorna a quantidade de membros do clube (incluindo o administrador). */
        int getNumberMembers() const;
        /** @brief Retorna o catálogo completo (futuros, atual e passados). */
        const std::vector<CatalogEntry>& getCatalog() const;
        /** @brief Retorna o livro da leitura atual, ou nullptr se não houver. */
        Book* getCurrentReading() const;
        /** @brief Retorna o feed de check-ins. */
        const std::vector<CheckIn*>& getFeed() const;
        /** @brief Retorna o total de páginas lidas no clube. */
        int getReadPages() const;
        /**
         * @brief Define o total de páginas lidas no clube.
         *        Chamado por User::realizarCheckIn para atualizar a métrica.
         * @param pages Novo total.
         * @throws std::invalid_argument se pages for negativo.
         */
        void setReadPages(int pages);
        /** @brief Retorna o total de livros lidos no clube. */
        int getReadBooks() const;
        /** @brief Retorna a data de criação do clube. */
        std::time_t getClubCreation() const;
        /** @brief Retorna o início da leitura atual (0 se não há leitura). */
        std::time_t getStartReading() const;
        /** @brief Retorna o fim (prazo) da leitura atual (0 se não foi definido). */
        std::time_t getEndReading() const;
 
        /**
         * @brief Retorna o código de convite.
         * @param actor Usuário que pede o código.
         * @return O código se actor é o administrador; string vazia caso contrário.
         */
        std::string getInviteCode(const User* actor) const;
 
        // ---------- MÉTRICAS ----------
 
        /** @brief Média de páginas lidas por membro (0 se não há membros). */
        double MediaPaginasPorUser() const;
        /** @brief Média de páginas lidas por dia na leitura atual (0 se não há leitura). */
        double MediaPaginasPorDia() const;
        /** @brief Média, em porcentagem, do quanto os membros já leram do livro atual. */
        double MediaPorcentagemLeitura() const;
        /** @brief Quantidade de membros que já terminaram o livro atual (0 se não há leitura). */
        int TotalTerminaram() const;
        /** @brief Porcentagem (0 a 100) dos membros que já terminaram o livro atual (0 se não há leitura ou membros). */
        double PorcentagemTerminaram() const;
        /** @brief Membro com mais páginas lidas, ou nullptr se não há membros. */
        User* LiderDeLeitura() const;
        /** @brief Membros ordenados do que mais leu para o que menos leu. */
        std::vector<User*> ranking() const;
        /** @brief Dias restantes para o fim da leitura atual (0 se não há leitura ou já passou). */
        int DiasRestantesLeitura() const;
        /** @brief Média das notas de um livro neste clube (0 se não há notas ou o livro não está no catálogo). */
        double mediaAvaliacoes(const Book* livro) const;
        /** @brief Quantidade de votos de um livro neste clube (0 se o livro não está no catálogo). */
        int totalVotos(const Book* livro) const;
 
        // ---------- MEMBROS ----------
 
        /** @brief Retorna true se user é membro do clube. */
        bool isMember(const User* user) const;
 
        /**
         * @brief Adiciona um usuário ao clube.
         * @param user Usuário que quer entrar.
         * @param codigo Código de convite. Obrigatório em clube privado.
         * @return false se user já for membro, o clube estiver
         *         encerrado ou o código estiver errado em clube privado.
         * @throws std::invalid_argument se user for nulo.
         */
        bool entrarNoClube(User* user, const std::string& codigo = "");
 
        /**
         * @brief Remove um usuário do clube.
         * @param user Usuário que quer sair.
         * @return false se não for membro ou se for o administrador. O
         *         administrador nunca sai (US28): ele transfere a
         *         administração antes ou, se estiver sozinho, apaga o clube.
         * @throws std::invalid_argument se user for nulo.
         */
        bool sairDoClube(User* user);
 
        // ---------- CHECK-INS ----------
 
        /**
         * @brief Adiciona um check-in ao feed.
         *        As páginas lidas são atualizadas pelo User via setReadPages,
         *        então este método não mexe nas métricas (senão contaria duas vezes).
         * @param checkIn Check-in a adicionar (continua pertencendo ao autor).
         * @return false se o autor não for membro, se o check-in
         *         for de outro clube ou se já estiver no feed.
         * @throws std::invalid_argument se checkIn for nulo.
         */
        bool adicionarCheckIn(CheckIn* checkIn);

        /**
         * @brief Tira um check-in do feed, SEM apagá-lo (sem delete).
         *        Chamado por User::apagarCheckIn antes de o User fazer o delete.
         *        Se o check-in não estiver no feed, não faz nada.
         */
        void eraseCheckIn(CheckIn* checkIn);
 
        // ---------- CATÁLOGO ----------
 
        /**
         * @brief Adiciona um livro ao catálogo com status futuro.
         * @return false se o livro já estiver no catálogo.
         * @throws std::invalid_argument se livro for nulo.
         */
        bool adicionarLivro(Book* livro);
 
        /**
         * @brief Remove um livro do catálogo (junto com suas notas e votos).
         * @return false se o livro não estiver no catálogo.
         * @throws std::invalid_argument se livro for nulo.
         */
        bool removerLivro(Book* livro);
 
        /**
         * @brief Registra (ou substitui) a avaliação de um membro para um livro já lido.
         * @param user Membro que avalia.
         * @param livro Livro com status passado.
         * @param nota Nota de 1 a 5.
         * @return false se user não for membro ou o livro não for passado.
         * @throws std::invalid_argument se user ou livro forem nulos, ou se a
         *         nota não estiver entre 1 e 5.
         */
        bool avaliarLivro(User* user, Book* livro, int nota);
 
        /**
         * @brief Registra o voto de um membro para a próxima leitura.
         * @param user Membro que vota.
         * @param livro Livro com status futuro.
         * @return false se user não for membro, o livro não for futuro ou o membro já votou nele.
         * @throws std::invalid_argument se user ou livro forem nulos.
         */
        bool votarProximaLeitura(User* user, Book* livro);
 
        // ---------- ADMINISTRADOR ----------
 
        /** @brief Retorna true se user é o administrador do clube. */
        bool isAdmin(const User* user) const;
 
        /**
         * @brief Gera um novo código de convite.
         * @param actor Deve ser o administrador.
         * @return O novo código, ou string vazia se actor não for admin.
         * @throws std::invalid_argument se actor for nulo.
         */
        std::string criarConvite(User* actor);
 
        /**
         * @brief Remove um membro do clube.
         * @return false se actor não for admin, alvo não for membro ou alvo for o admin.
         * @throws std::invalid_argument se actor ou alvo forem nulos.
         */
        bool removerMembro(User* actor, User* alvo);
 
        /**
         * @brief Define a próxima leitura, passando o livro para status atual.
         *        O livro atual anterior vira passado.
         * @param actor Usuário que está pedindo a mudança (precisa ser o admin).
         * @param livro Livro futuro do catálogo.
         * @return false se actor não for admin ou o livro não for futuro.
         * @throws std::invalid_argument se actor ou livro forem nulos.
         */
        bool decidirProximaLeitura(User* actor, Book* livro);
 
        /**
         * @brief Altera a privacidade.
         * @return false se actor não for admin.
         * @throws std::invalid_argument se actor for nulo.
         */
        bool alterarPrivacidade(User* actor, bool isPrivate);
 
        /**
         * @brief Altera o nome do clube.
         * @return false se actor não for admin.
         * @throws std::invalid_argument se actor for nulo ou o nome for vazio.
         */
        bool renomear(User* actor, const std::string& novoNome);
 
        /**
         * @brief Apaga o check-in de outro membro (moderação do administrador).
         *        Como o dono do check-in é o autor, o clube pede ao autor que o
         *        apague (User::apagarCheckIn), que tira do feed e faz o delete.
         * @return false se actor não for admin ou o check-in não estiver no feed.
         * @throws std::invalid_argument se actor ou checkIn forem nulos.
         */
        bool apagarCheckIn(User* actor, CheckIn* checkIn);
 
        /**
         * @brief Define a duração da leitura atual.
         * @param actor Usuário que está pedindo a mudança (precisa ser o admin).
         * @param dias Quantidade de dias (maior que 0).
         * @return false se actor não for admin ou não houver leitura atual.
         * @throws std::invalid_argument se actor for nulo ou dias for menor ou igual a zero.
         */
        bool definirTempoLeitura(User* actor, int dias);
 
        /**
         * @brief Apaga um comentário (tira do check-in e faz o delete).
         * @return false se actor não for admin, se o comentário for de
         *         outro clube ou não estiver na lista do seu check-in.
         * @throws std::invalid_argument se actor ou comentario forem nulos.
         */
        bool apagarComentario(User* actor, Comment* comentario);
 
        /**
         * @brief Transfere a administração para outro membro.
         * @return false se actor não for admin ou alvo não for membro.
         * @throws std::invalid_argument se actor ou alvo forem nulos.
         */
        bool transferirAdmin(User* actor, User* alvo);

        // ---------- RESTAURAÇÃO (usada só pelo BancoDeDados) ----------
        //
        // Na carga dos arquivos, as ações normais não servem: elas conferem
        // regras de "ação nova" (ser membro, ser admin, status do livro) e
        // mudam datas e contadores. Estes métodos só colocam de volta o
        // estado salvo, sem efeito colateral.

        /**
         * @brief Coloca de volta os dados salvos que não vêm pelo construtor.
         * @param inviteCode Código de convite salvo.
         * @param active Se o clube está ativo.
         * @param readPages Total de páginas lidas no clube.
         * @param readBooks Total de livros lidos no clube.
         * @param clubCreation Data de criação.
         * @param startReading Início da leitura atual (0 se não há).
         * @param endReading Prazo da leitura atual (0 se não há).
         * @throws std::invalid_argument se o código for vazio (num clube
         *         privado, o código vazio deixaria qualquer um entrar) ou se
         *         readPages ou readBooks forem negativos.
         */
        void restaurarEstado(const std::string& inviteCode, bool active,
                             int readPages, int readBooks,
                             std::time_t clubCreation,
                             std::time_t startReading,
                             std::time_t endReading);

        /**
         * @brief Coloca um livro no catálogo já com o status salvo.
         *        Não mexe em datas nem em livros lidos (isso vem do restaurarEstado).
         * @throws std::invalid_argument se livro for nulo, se já estiver no
         *         catálogo ou se for um segundo livro com status Atual.
         */
        void restaurarLivroNoCatalogo(Book* livro, Status status);

        /**
         * @brief Coloca de volta a nota de um usuário para um livro do catálogo.
         *        Não exige que o usuário ainda seja membro (quem saiu do clube
         *        continua com a nota).
         * @throws std::invalid_argument se livro ou user forem nulos, se o
         *         livro não estiver no catálogo ou se a nota não for de 1 a 5.
         */
        void restaurarNota(Book* livro, const User* user, int nota);

        /**
         * @brief Coloca de volta o voto de um usuário num livro do catálogo.
         *        Não exige que o usuário ainda seja membro.
         * @throws std::invalid_argument se livro ou user forem nulos, se o
         *         livro não estiver no catálogo ou se o voto for repetido.
         */
        void restaurarVoto(Book* livro, const User* user);

        /**
         * @brief Coloca no feed um check-in carregado dos arquivos.
         *        Diferente de adicionarCheckIn, NÃO exige que o autor ainda
         *        seja membro: quem saiu do clube continua com os check-ins no
         *        feed (US08). Chamado por User::restaurarCheckIn.
         * @throws std::invalid_argument se checkIn for nulo, se for de outro
         *         clube ou se já estiver no feed.
         */
        void restaurarCheckIn(CheckIn* checkIn);
};
 
#endif
