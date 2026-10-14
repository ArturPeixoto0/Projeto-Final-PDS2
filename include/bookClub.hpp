/**
 * @file bookClub.hpp
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
 *  - O clube é dono dos CheckIn do feed e os apaga no destrutor.
 *  - O clube NÃO é dono de User nem de Book, que existem fora dele.
 *
 * Por causa dessa posse, o clube não pode ser copiado.
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
 
    public:
        /**
         * @brief Cria um clube do livro.
         * @param founder Usuário que cria o clube. É adicionado como membro e
         *        como administrador.
         * @param name Nome do clube.
         * @param type Tipo do clube.
         * @param isPrivate Se true, só entra quem tiver o código de convite.
         */
        BookClub(User* founder,
                 const std::string& name,
                 const std::string& type,
                 bool isPrivate = false);
 
        BookClub(const BookClub&) = delete;
        BookClub& operator=(const BookClub&) = delete;
 
        /**
         * @brief Destrói o clube, apagando os CheckIn do feed.
         *        Usuários e livros não são apagados.
         */
        ~BookClub();
 
        // ---------- GETTERS ----------
 
        /** @brief Retorna o nome do clube. */
        const std::string& getName() const;
        /** @brief Retorna o tipo do clube. */
        const std::string& getType() const;
        /** @brief Retorna true se o clube é privado. */
        bool isPrivate() const;
        /** @brief Retorna true se o clube está ativo (não encerrado). */
        bool isActive() const;
        /** @brief Retorna true se há uma leitura em andamento (livro atual no catálogo). */
        bool isReading() const;
        /** @brief Retorna o administrador do clube. */
        User* getAdmin() const;
        /** @brief Retorna os membros do clube. */
        const std::vector<User*>& getMembers() const;
        /** @brief Retorna o catálogo completo (futuros, atual e passados). */
        const std::vector<CatalogEntry>& getCatalog() const;
        /** @brief Retorna o livro da leitura atual, ou nullptr se não houver. */
        Book* getCurrentReading() const;
        /** @brief Retorna o feed de check-ins. */
        const std::vector<CheckIn*>& getFeed() const;
        /** @brief Retorna o total de páginas lidas no clube. */
        int getReadPages() const;
        /** @brief Retorna o total de livros lidos no clube. */
        int getReadBooks() const;
        /** @brief Retorna a data de criação do clube. */
        std::time_t getClubCreation() const;
 
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
         * @return false se user for nulo, já for membro, o clube estiver
         *         encerrado ou o código estiver errado em clube privado.
         */
        bool entrarNoClube(User* user, const std::string& codigo = "");
 
        /**
         * @brief Remove um usuário do clube.
         * @param user Usuário que quer sair.
         * @return false se não for membro ou se for o administrador (que
         *         deve antes transferir a administração).
         */
        bool sairDoClube(User* user);
 
        // ---------- CHECK-INS ----------
 
        /**
         * @brief Adiciona um check-in ao feed e atualiza as métricas.
         * @param checkIn Check-in a adicionar; passa a pertencer ao clube.
         * @return false se for nulo ou se o autor não for membro.
         */
        bool adicionarCheckIn(CheckIn* checkIn);
 
        // ---------- CATÁLOGO ----------
 
        /**
         * @brief Adiciona um livro ao catálogo com status futuro.
         * @return false se livro for nulo ou já estiver no catálogo.
         */
        bool adicionarLivro(Book* livro);
 
        /**
         * @brief Remove um livro do catálogo (junto com suas notas e votos).
         * @return false se o livro não estiver no catálogo.
         */
        bool removerLivro(Book* livro);
 
        /**
         * @brief Registra (ou substitui) a avaliação de um membro para um livro já lido.
         * @param user Membro que avalia.
         * @param livro Livro com status passado.
         * @param nota Nota de 1 a 5.
         * @return false se user não for membro, o livro não for passado ou a nota for inválida.
         */
        bool avaliarLivro(User* user, Book* livro, int nota);
 
        /**
         * @brief Registra o voto de um membro para a próxima leitura.
         * @param user Membro que vota.
         * @param livro Livro com status futuro.
         * @return false se user não for membro, o livro não for futuro ou o membro já votou nele.
         */
        bool votarProximaLeitura(User* user, Book* livro);
 
        // ---------- ADMINISTRADOR ----------
 
        /** @brief Retorna true se user é o administrador do clube. */
        bool isAdmin(const User* user) const;
 
        /**
         * @brief Gera um novo código de convite.
         * @param actor Deve ser o administrador.
         * @return O novo código, ou string vazia se actor não for admin.
         */
        std::string criarConvite(User* actor);
 
        /**
         * @brief Remove um membro do clube.
         * @return false se actor não for admin, alvo não for membro ou alvo for o admin.
         */
        bool removerMembro(User* actor, User* alvo);
 
        /**
         * @brief Define a próxima leitura, passando o livro para status atual.
         *        O livro atual anterior vira passado.
         * @param livro Livro futuro do catálogo.
         * @return false se actor não for admin ou o livro não for futuro.
         */
        bool decidirProximaLeitura(User* actor, Book* livro);
 
        /** @brief Altera a privacidade. @return false se actor não for admin. */
        bool alterarPrivacidade(User* actor, bool isPrivate);
 
        /** @brief Altera o nome do clube. @return false se actor não for admin ou o nome for vazio. */
        bool renomear(User* actor, const std::string& novoNome);
 
        /**
         * @brief Apaga um check-in do feed e atualiza as métricas.
         * @return false se actor não for admin ou o check-in não estiver no feed.
         */
        bool apagarCheckIn(User* actor, CheckIn* checkIn);
 
        /**
         * @brief Define a duração da leitura atual.
         * @param dias Quantidade de dias (maior que 0).
         * @return false se actor não for admin, não houver leitura atual ou dias for inválido.
         */
        bool definirTempoLeitura(User* actor, int dias);
 
        /**
         * @brief Apaga um comentário.
         * @return false se actor não for admin.
         */
        bool apagarComentario(User* actor, Comment* comentario);
 
        /**
         * @brief Transfere a administração para outro membro.
         * @return false se actor não for admin ou alvo não for membro.
         */
        bool transferirAdmin(User* actor, User* alvo);
};
 
#endif