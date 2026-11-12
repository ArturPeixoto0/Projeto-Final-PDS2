/**
 * @file User.hpp
 * @brief Declaração da classe User.
 */
#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <map>
#include "UserProfile.hpp"

class BookClub;
class CheckIn;
class Comment;
class Book;
class ReadingMetrics;

/**
 * @brief Representa um usuário do sistema.
 *
 * Armazena os dados da conta (nome, email e senha), os clubes em que
 * participa, o histórico de check ins e as métricas pessoais de leitura.
 * Oferece as ações que o usuário pode realizar: criar e entrar em clubes,
 * fazer check ins, comentar e avaliar leituras.
 */
class User {
    private:

        std::string name;                                ///< Nome do usuário.
        std::string email;                               ///< Email da conta.
        std::string password;                            ///< Senha da conta.
        std::vector<BookClub*> currentClubs;             ///< Clubes de que o usuário participa.
        std::vector<CheckIn*> allCheckIns;               ///< Check-ins já feitos (o User é dono deles).
        std::map<BookClub*, ReadingMetrics*> allMetrics; ///< Progresso de leitura em cada clube (o User é dono).
        int totalPages = 0;                              ///< Paginômetro: total de páginas lidas.
        int totalBooks = 0;                              ///< Livrômetro: total de livros terminados.
        UserProfile profile;                             ///< Perfil público do usuário.
        int id;                                          ///< Identificador único (usado nos arquivos de dados).
        static int proximoId;                            ///< Próximo id a ser gerado (um contador para todos os Users).

    public:
        /**
         * @brief Cria um usuário.
         * @param name Nome do usuário.
         * @param email Email da conta.
         * @param password Senha da conta.
         * @param id Identificador do usuário. Se for 0 (padrão), um id novo é
         *        gerado. Se for maior que 0, esse id é usado (ao carregar dos
         *        arquivos) e os próximos ids gerados serão maiores que ele.
         * @throws std::invalid_argument se id for negativo.
         */
        User(const std::string& name,
             const std::string& email,
             const std::string& password,
             int id = 0);
             
        /// @brief Destrutor: sai dos clubes e apaga os check-ins e as métricas do usuário.
        ~User();

        User(const User&) = delete;
        User& operator = (const User&) = delete;

        //dados da conta:
        /// @brief Retorna o identificador único do usuário.
        int getId() const;

        /// @brief Retorna o nome do usuário.
        std::string getName() const;

        /**
         * @brief Altera o nome do usuário.
         * @param name Novo nome.
         */
        void setName(const std::string& name);

        /// @brief Retorna o email da conta.
        std::string getEmail() const;

        /**
         * @brief Confere se uma senha está correta (usado no login).
         * @param attempt Senha digitada.
         * @return true se for igual à senha da conta, false caso contrário.
         */
        bool checkPassword(const std::string& attempt) const;

        /**
         * @brief Retorna a senha, para o BancoDeDados salvar nos arquivos.
         * @warning A senha é guardada em texto puro (sem criptografia). É uma
         *          limitação conhecida deste trabalho; não use senhas reais.
         */
        std::string getPassword() const;

        //perfil:
        /// @brief Retorna o perfil público do usuário, permitindo alterá-lo.
        UserProfile& getProfile();

        /// @brief Retorna o perfil público do usuário, só para leitura.
        const UserProfile& getProfile() const;

        //clubes:
        /// @brief Retorna os clubes de que o usuário participa.
        const std::vector<BookClub*>& getCurrentClubs() const;

        /**
         * @brief Verifica se o usuário é membro de um clube.
         * @param club Clube a verificar.
         * @return true se for membro, false caso contrário.
         * @throws std::invalid_argument se club for nulo.
         */
        bool participaDe(const BookClub* club) const;

        /**
         * @brief Cria um clube do livro, com o usuário como membro e administrador.
         * @param clubName Nome do clube.
         * @param reading Primeira leitura do clube.
         * @return O clube criado. Quem chamou fica responsável por apagá-lo
         *         (ou entregá-lo ao BancoDeDados).
         * @throws std::invalid_argument se reading for nulo ou se o nome
         *         for vazio (lançada pelo construtor de BookClub).
         */
        BookClub* criarClube(const std::string& clubName, Book* reading);
        
        /**
         * @brief Entra em um clube, atualizando os dois lados (User e BookClub).
         *        Pode ser chamado pelo User ou pelo próprio clube; nos dois
         *        casos o resultado é o mesmo.
         * @param club Clube em que vai entrar.
         * @param codigo Código de convite (obrigatório em clube privado).
         * @return false se já participa ou se o clube recusar a entrada.
         * @throws std::invalid_argument se club for nulo.
         */
        bool entrarClube(BookClub* club, const std::string& codigo = "");

        /**
         * @brief Sai de um clube, atualizando os dois lados (User e BookClub)
         *        e apagando a métrica de leitura daquele clube.
         * @param club Clube de onde vai sair.
         * @return false se não participa ou se o clube recusar a saída
         *         (o administrador nunca sai, US28).
         * @throws std::invalid_argument se club for nulo.
         */
        bool sairClube(BookClub* club);

        //check-ins e comentarios:
        /// @brief Retorna todos os check-ins do usuário, do mais antigo para o mais novo.
        const std::vector<CheckIn*>& getAllCheckIns() const;

        /**
         * @brief Página em que o usuário está na leitura do clube.
         * @return -1 se não participa do clube; 0 se participa mas ainda não
         *         fez check-in; senão, a página do último check-in.
         * @throws std::invalid_argument se club for nulo.
         */
        int getCurrentPage(const BookClub* club) const;

        /**
         * @brief Registra um check-in no livro atual do clube e atualiza o
         *        paginômetro e o livrômetro. A métrica de leitura do clube é
         *        criada no primeiro check-in e recomeça quando o livro muda.
         * @param club Clube do check-in.
         * @param page Página em que o usuário está.
         * @param text Texto do check-in.
         * @param finished Se true, a página vira a última do livro.
         * @return O check-in criado, ou nullptr se o usuário não participa do
         *         clube ou o clube não tem leitura atual.
         * @throws std::invalid_argument se club for nulo ou a página for inválida.
         */
        CheckIn* realizarCheckIn(BookClub* club, int page, const std::string& text,
                                bool finished = false);
    
        /**
         * @brief Apaga um check-in do próprio usuário: tira do feed do clube
         *        e libera a memória.
         * @param checkIn Check-in a apagar.
         * @return true se apagou; false se checkIn for nulo ou não for
         *         deste usuário (cada um só apaga os seus).
         */
        bool apagarCheckIn(CheckIn* checkIn);

        /**
         * @brief Comenta em um check-in. O comentário entra na lista do
         *        check-in, que passa a ser dono dele.
         * @param checkIn Check-in comentado.
         * @param text Texto do comentário (1 a Comment::MAX_LENGTH caracteres).
         * @return O comentário criado, ou nullptr se checkIn for nulo, se o
         *         usuário não participar do clube do check-in ou se o texto
         *         for inválido.
         */
        Comment* comentar(CheckIn* checkIn, const std::string& text);
    
        //livros:
        /**
         * @brief Sugere um livro para o catálogo de futuras leituras do clube.
         * @param club Clube que recebe a sugestão.
         * @param book Livro sugerido.
         * @return true se o livro entrou no catálogo; false se o usuário não
         *         for membro ou se o clube recusar (por exemplo, livro repetido).
         * @throws std::invalid_argument se club ou book forem nulos.
         */
        bool sugerirLivro(BookClub* club, Book* book);

        /**
         * @brief Dá uma nota ao livro que o usuário leu no clube.
         * @param club Clube da leitura.
         * @param nota Nota de 0 a 5.
         * @return true se a nota foi registrada; false se o usuário não tiver
         *         leitura nesse clube ou se a nota estiver fora de 0 a 5.
         * @throws std::invalid_argument se club for nulo.
         * @throws std::logic_error se a leitura ainda não terminou.
         */
        bool avaliarLeitura (BookClub* club, double nota);

        //métricas:
        /// @brief Paginômetro: total de páginas já lidas, somando todos os clubes.
        int getTotalPages() const;

        /// @brief Livrômetro: total de livros que o usuário já terminou.
        int getTotalBooks() const;

        /**
         * @brief Métrica de leitura do usuário em um clube (para salvar).
         * @return A métrica, ou nullptr se ainda não há (nenhum check-in no
         *         clube). A métrica continua pertencendo ao User.
         * @throws std::invalid_argument se club for nulo.
         */
        ReadingMetrics* getMetrica(const BookClub* club) const;

        //restauração (usada só pelo BancoDeDados):
        //na carga, as ações normais não servem (realizarCheckIn somaria as
        //páginas de novo, por exemplo). Estes métodos só colocam de volta o
        //estado salvo, sem efeito colateral.

        /**
         * @brief Coloca de volta o paginômetro e o livrômetro salvos.
         * @throws std::invalid_argument se algum for negativo.
         */
        void restaurarTotais(int totalPages, int totalBooks);

        /**
         * @brief Coloca de volta a métrica de leitura de um clube.
         *        Se der certo, o User vira DONO da métrica (e a apaga no
         *        destrutor). Se já havia uma métrica nesse clube, a antiga é
         *        apagada. Se lançar exceção, o User NÃO vira dono: quem chamou
         *        continua responsável pelo delete.
         * @throws std::invalid_argument se club ou metrica forem nulos ou se
         *         o usuário não participar do clube.
         */
        void restaurarMetrica(BookClub* club, ReadingMetrics* metrica);

        /**
         * @brief Coloca de volta um check-in carregado dos arquivos: entra em
         *        allCheckIns e no feed do clube (BookClub::restaurarCheckIn).
         *        NÃO mexe em páginas, totais nem métricas, que já foram
         *        restaurados à parte. Funciona mesmo se o usuário já saiu do
         *        clube. Se der certo, o User vira DONO do check-in; se lançar
         *        exceção, quem chamou continua responsável pelo delete.
         * @throws std::invalid_argument se checkIn for nulo, se for de outro
         *         usuário, de outro clube ou se já estiver no feed.
         */
        void restaurarCheckIn(CheckIn* checkIn);
    
};

#endif