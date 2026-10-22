#include "ReadingMetrics.hpp"
#include "Book.hpp"

ReadingMetrics::ReadingMetrics(Book* reading): reading(reading), currentPage(0), grade(-1), finished(false) {}

Book* ReadingMetrics::getReading(){
    return reading;
}

int ReadingMetrics::getCurrentPage(){
    return currentPage;
}

void ReadingMetrics::addPages(int pages){
    if (currentPage+pages < reading->getPages() && pages >= 0)
        currentPage += pages;

}

double ReadingMetrics::getGrade(){
    return grade;
}

void ReadingMetrics::setGrade(double grade){
    if (grade > 5 || grade < 0 || finished == false)
        return;
    this->grade = grade;
}

bool ReadingMetrics::isFinished(){
    return finished;
}

void ReadingMetrics::finish(){
    finished = true;
}

void ReadingMetrics::start(){
    finished = false;
    grade = 0;
    currentPage = 0;

}

ReadingMetrics::~ReadingMetrics(){}