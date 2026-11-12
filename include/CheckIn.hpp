/**
 * @file CheckIn.hpp
 * @brief Declaração da classe CheckIn.
 */
#ifndef CHECKIN_H
#define CHECKIN_H

#include <string>
#include <vector>
#include <ctime>

class User;
class BookClub;
class Comment;
class Book;

/**
 * @brief Representa um check-in em um clube do livro
 *
 * Armazena os dados do check-in (texto, quem realizou, livro comentado,
 * clube do livro relacionado, página em que o usuário está e data e hora
 * da postagem) e recebe comentários.
 *
 * Posse dos objetos:
 *  - O CheckIn é dono dos seus Comment e os apaga no destrutor.
 *  - O CheckIn NÃO é dono de User, Book nem BookClub.
 *
 * Por causa dessa posse, o CheckIn não pode ser copiado.
 */
class CheckIn {
private:
    std::string text; ///< Comentário do check-in (pode ser vazio).

    User* user;     ///< Autor do check-in.
    Book* book;     ///< Livro lido.
    BookClub* club; ///< Clube em que o check-in foi feito.

    int page; ///< Página em que o usuário estava ao fazer o check-in.

    std::vector<Comment*> comments; ///< Comentários recebidos (o CheckIn é dono deles).

    std::time_t dataHora; ///< Data e hora da postagem.

    int id; ///< Identificador único do check-in (usado nos arquivos de dados).

    static int proximoId; ///< Próximo id a ser gerado (um só contador para todos os check-ins).

    /**
     * @brief Define a página do check-in.
     * @throws std::invalid_argument se a página for menor que 1 ou maior
     *         que o total de páginas do livro.
     */
    void setPage(int page);

public:
    /**
     * @brief Cria um check-in.
     * @param user Autor do check-in.
     * @param book Livro lido (normalmente a leitura atual do clube).
     * @param club Clube em que o check-in foi feito.
     * @param page Página em que o usuário está (de 1 até o total do livro).
     * @param text Comentário do check-in (opcional).
     * @param id Identificador do check-in. Se for 0 (padrão), um id novo é
     *        gerado. Se for maior que 0, esse id é usado (ao carregar dos
     *        arquivos) e os próximos ids gerados serão maiores que ele.
     * @throws std::invalid_argument se user, book ou club forem nullptr,
     *         se a página for inválida ou se id for negativo.
     */
    CheckIn(User* user, Book* book, BookClub* club, int page,
            const std::string& text = "", int id = 0);

    /** @brief Retorna o identificador único do check-in. */
    int getId() const;

    /** @brief Destrói o check-in, apagando todos os seus comentários. */
    ~CheckIn();

    CheckIn(const CheckIn&) = delete;
    CheckIn& operator=(const CheckIn&) = delete;

    /** @brief Retorna o texto do check-in. */
    std::string getText() const;
    /** @brief Retorna a página em que o usuário estava ao fazer o check-in. */
    int getPage() const;
    /** @brief Retorna o autor do check-in. */
    User* getUser() const;
    /** @brief Retorna o livro do check-in. */
    Book* getBook() const;
    /** @brief Retorna o clube em que o check-in foi feito. */
    BookClub* getClub() const;

    /** @brief Retorna os comentários do check-in (pode ser alterado). */
    std::vector<Comment*>& getComments();
    /** @brief Retorna os comentários do check-in (só leitura). */
    const std::vector<Comment*>& getComments() const;

    /** @brief Retorna a data e hora da postagem no formato "dd/mm/aaaa hh:mm". */
    std::string getDataHora() const;

    /**
     * @brief Retorna a data e hora da postagem como número (segundos desde
     *        1970), o formato usado para salvar nos arquivos.
     */
    std::time_t getDataHoraNumero() const;

    /**
     * @brief Coloca de volta a data e hora salvas nos arquivos. Usado só pelo
     *        BancoDeDados: o construtor usa a hora atual, e na carga
     *        precisamos da hora original.
     */
    void restaurarDataHora(std::time_t dataHora);

    /** @brief Apaga (com delete) todos os comentários e esvazia a lista. */
    void eraseAllComments();
};

#endif
