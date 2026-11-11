#ifndef READINGMETRICS_H
#define READINGMETRICS_H


class Book;

class ReadingMetrics {
    private:
    Book* reading;
    int currentPage;
    double grade;
    bool finished;

    public:
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

    Book* getReading();
    int getCurrentPage();
    void addPages(int pages);
    double getGrade();
    void setGrade(double grade);
    bool isFinished();
    void finish();
    void reset();

    ~ReadingMetrics();
};


#endif