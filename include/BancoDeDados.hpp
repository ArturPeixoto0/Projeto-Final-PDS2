/**
 * @file BancoDeDados.hpp
 * @brief Declara a classe BancoDeDados.
 */
#ifndef BANCODEDADOS_H
#define BANCODEDADOS_H

#include <string>
#include <vector>

class Book;
class User;
class BookClub;

/**
 * @brief Guarda todos os objetos do sistema e os salva/carrega em arquivos JSON.
 *
 * É a única classe que lê e escreve arquivos: as entidades (User, Book,
 * BookClub...) não sabem que os arquivos existem. Nos arquivos, um objeto
 * aponta para outro pelo id, porque o endereço de memória muda a cada
 * execução do programa.
 *
 * Arquivos (dentro da pasta passada no construtor):
 *  - books.json: lista de livros;
 *  - users.json: lista de usuários (com perfil, totais e senha).
 *
 * Posse dos objetos:
 *  - O BancoDeDados é DONO dos livros, usuários e clubes e os apaga no
 *    destrutor, nesta ordem: clubes, usuários, livros. Os clubes vão
 *    primeiro porque o destrutor do BookClub tira o clube da lista de cada
 *    membro (precisa dos User vivos), e os livros por último porque clubes,
 *    métricas e check-ins apontam para eles.
 *
 * Por causa dessa posse, o BancoDeDados não pode ser copiado: as duas
 * cópias apagariam os mesmos objetos.
 *
 * Limitações conhecidas:
 *  - a senha é salva em texto puro;
 *  - salvar reescreve o arquivo inteiro; se o programa fechar no meio da
 *    escrita, o arquivo pode ficar cortado;
 *  - os caminhos são relativos à pasta de onde o programa é executado.
 */
class BancoDeDados {
    private:
        std::string pasta;              ///< Pasta dos arquivos, terminando em "/" (ex.: "dados/").
        std::vector<Book*> livros;      ///< Livros (o banco é dono).
        std::vector<User*> usuarios;    ///< Usuários (o banco é dono).
        std::vector<BookClub*> clubes;  ///< Clubes (o banco é dono).

        /** @brief Junta a pasta com o nome do arquivo (ex.: "dados/" + "books.json"). */
        std::string caminho(const std::string& nomeArquivo) const;

        void salvarLivros() const;
        void salvarUsuarios() const;
        void carregarLivros();
        void carregarUsuarios();

    public:
        /**
         * @brief Cria um banco vazio que usa a pasta informada.
         *        Não lê nada ainda: para isso existe carregarTudo.
         * @param pasta Pasta dos arquivos, terminando em "/" (ex.: "dados/").
         *        Usamos "/" mesmo no Windows, que também aceita essa barra.
         */
        BancoDeDados(const std::string& pasta);

        /** @brief Apaga clubes, usuários e livros, nessa ordem (ver a descrição da classe). */
        ~BancoDeDados();

        BancoDeDados(const BancoDeDados&) = delete;
        BancoDeDados& operator=(const BancoDeDados&) = delete;

        // ---------- BUSCA ----------

        /** @brief Procura um livro pelo id. @return O livro, ou nullptr se não achar. */
        Book* buscarLivro(int id) const;
        /** @brief Procura um usuário pelo id. @return O usuário, ou nullptr se não achar. */
        User* buscarUsuario(int id) const;
        /** @brief Procura um clube pelo id. @return O clube, ou nullptr se não achar. */
        BookClub* buscarClube(int id) const;
        /** @brief Procura um usuário pelo email. @return O usuário, ou nullptr se não achar. */
        User* buscarUsuarioPorEmail(const std::string& email) const;

        // ---------- LISTAS ----------

        /** @brief Retorna todos os livros. */
        const std::vector<Book*>& getLivros() const;
        /** @brief Retorna todos os usuários. */
        const std::vector<User*>& getUsuarios() const;
        /** @brief Retorna todos os clubes. */
        const std::vector<BookClub*>& getClubes() const;

        /**
         * @brief Coloca um livro no banco. Se der certo, o banco vira DONO
         *        dele. Se lançar exceção, quem chamou continua dono.
         * @throws std::invalid_argument se livro for nulo ou se já houver um
         *         livro com o mesmo id.
         */
        void adicionarLivro(Book* livro);

        /**
         * @brief Coloca um usuário no banco. Se der certo, o banco vira DONO
         *        dele. Se lançar exceção, quem chamou continua dono.
         * @throws std::invalid_argument se usuario for nulo ou se já houver
         *         um usuário com o mesmo id.
         */
        void adicionarUsuario(User* usuario);

        // ---------- ARQUIVOS ----------

        /**
         * @brief Lê os arquivos da pasta e recria os objetos com os mesmos ids.
         *        Arquivo que não existe conta como lista vazia (é o que
         *        acontece na primeira execução).
         * @throws std::logic_error se o banco já tiver objetos (carregar duas
         *         vezes criaria objetos repetidos).
         * @throws std::runtime_error se um arquivo existir mas não for uma
         *         lista JSON.
         * @throws std::invalid_argument se os dados recriarem um objeto
         *         inválido (ex.: livro sem título).
         * @throws nlohmann::json::exception (derivada de std::exception) se
         *         o arquivo não for um JSON válido ou faltar um campo. Se
         *         lançar no meio, o que já foi carregado fica no banco.
         */
        void carregarTudo();

        /**
         * @brief Reescreve todos os arquivos com o estado atual.
         * @throws std::runtime_error se não conseguir abrir um arquivo para
         *         escrever (ex.: a pasta não existe). A mensagem traz o caminho.
         */
        void salvarTudo() const;
};

#endif
