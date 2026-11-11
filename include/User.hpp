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

        std::string name; //nome do User
        std::string email; //email da conta do User
        std::string password; //senha da conta do User
        std::vector<BookClub*> currentClubs; //vetor com todos os clubes atuais do User
        std::vector<CheckIn*> allCheckIns; //vetor com todos os check-ins já feitos pelo User
        std::map<BookClub*, ReadingMetrics*> allMetrics;
        int totalPages = 0; //paginômetro total do usuário
        int totalBooks = 0; //livrômetro total do usuário
        UserProfile profile; //perfil do User (inicializar com profile(this) no .cpp)
        int id; //identificador único do User (usado nos arquivos de dados)
        static int proximoId; //próximo id a ser gerado (um só contador para todos os Users)

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
             
        ~User();

        User(const User&) = delete;
        User& operator = (const User&) = delete;

        //dados da conta:
        int getId() const; //retorna o identificador único do usuário

        std::string getName() const; //retorna o nome do usuário

        void setName(const std::string& name); //altera o nome do usuário

        std::string getEmail() const; // retorna email da conta

        bool checkPassword(const std::string& attempt) const;

        //perfil: 
        UserProfile& getProfile();
        const UserProfile& getProfile() const;

        //clubes:
        const std::vector<BookClub*>& getCurrentClubs() const;
        bool participaDe(const BookClub* club) const; //verifica se participa
    
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
         *         (administrador com outros membros).
         * @throws std::invalid_argument se club for nulo.
         */
        bool sairClube(BookClub* club);

        //check-ins e comentarios:
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
        bool sugerirLivro(BookClub* club, Book* book); 
        bool avaliarLeitura (BookClub* club, double nota);

        //métricas:
        int getTotalPages() const; //paginometro
        int getTotalBooks() const; //livrometro
    
};

#endif