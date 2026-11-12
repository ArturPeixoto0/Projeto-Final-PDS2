/**
 * @file ReadingMetrics.hpp
 * @brief Declaração da classe ReadingMetrics.
 */
#ifndef READINGMETRICS_H
#define READINGMETRICS_H


class Book;

/**
 * @brief Progresso de um usuário na leitura atual de um clube.
 *
 * Guarda em que página o usuário está, se terminou o livro e a nota que
 * deu a ele. Cada User tem uma métrica por clube (é dono dela). A nota só
 * pode ser dada depois que a leitura termina.
 */
class ReadingMetrics {
    private:
    Book* reading;   ///< Livro que está sendo lido (a métrica não é dona dele).
    int currentPage; ///< Página atual, de 0 até o total do livro.
    double grade;    ///< Nota de 0 a 5, ou -1 se ainda não tem nota.
    bool finished;   ///< true quando a leitura chegou à última página.

    public:
    /**
     * @brief Cria uma métrica nova: página 0, sem nota e não terminada.
     * @param reading Livro que vai ser lido.
     * @throws std::invalid_argument se reading for nulo.
     */
    ReadingMetrics(Book* reading);

    /**
     * @brief Construtor de carga: recria uma métrica salva nos arquivos.
     *        Usado só pelo BancoDeDados. Não passa por addPages, então não
     *        exige que o progresso tenha sido feito aos poucos.
     * @param reading Livro da métrica.
     * @param currentPage Página salva (de 0 até o total do livro).
     * @param grade Nota salva: -1 (sem nota) ou de 0 a 5.
     * @param finished Se a leitura já tinha terminado.
     * @throws std::invalid_argument se reading for nulo, se a página estiver
     *         fora de 0 até o total do livro, se a nota não for -1 nem de 0
     *         a 5, se finished for true sem a página ser a última ou se
     *         houver nota sem a leitura ter terminado (as mesmas regras de
     *         finish e setGrade).
     */
    ReadingMetrics(Book* reading, int currentPage, double grade, bool finished);

    /** @brief Retorna o livro desta métrica. */
    Book* getReading();
    /** @brief Retorna a página em que o usuário está (0 se ainda não começou). */
    int getCurrentPage();

    /**
     * @brief Avança a leitura. Se chegar na última página, a leitura
     *        termina sozinha (chama finish()).
     * @param pages Quantas páginas foram lidas desde a última vez.
     * @throws std::invalid_argument se pages for zero ou negativo.
     * @throws std::out_of_range se passar do total de páginas do livro.
     */
    void addPages(int pages);

    /** @brief Retorna a nota dada ao livro, ou -1 se ainda não tem nota. */
    double getGrade();

    /**
     * @brief Dá uma nota ao livro.
     * @param grade Nota de 0 a 5.
     * @throws std::logic_error se a leitura ainda não terminou.
     * @throws std::invalid_argument se a nota estiver fora de 0 a 5.
     */
    void setGrade(double grade);

    /** @brief Retorna true se a leitura já terminou. */
    bool isFinished();

    /**
     * @brief Marca a leitura como terminada.
     * @throws std::logic_error se o usuário não estiver na última página.
     */
    void finish();

    /**
     * @brief Recomeça a métrica: página 0, sem nota e não terminada.
     *        Usado quando o clube troca de livro.
     */
    void reset();

    /** @brief Destrutor. A métrica não é dona do livro. */
    ~ReadingMetrics();
};


#endif
