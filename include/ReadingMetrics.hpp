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