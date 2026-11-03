#include "ReadingMetrics.hpp"
#include "Book.hpp"
#include <stdexcept>

ReadingMetrics::ReadingMetrics(Book* reading){
if (reading == nullptr){
    throw std::invalid_argument("Ponteiro nulo");
}
this->reading = reading;
currentPage = 0;
grade = -1; 
finished = false;
}

Book* ReadingMetrics::getReading(){
    return reading;
}

int ReadingMetrics::getCurrentPage(){
    return currentPage;
}

void ReadingMetrics::addPages(int pages){
    if (pages < 0){
        throw std::invalid_argument("Número de páginas negativo");
    }
    else if (pages = 0){
        throw std::invalid_argument("Número de páginas igual a zero");
    }
    else if (currentPage+pages > reading->getPages()){
        throw std::out_of_range("Número de páginas além do total");
    }
    else {
        currentPage += pages;
        if (currentPage == reading->getPages()){
            this->finish();
        }
    }
    
}

double ReadingMetrics::getGrade(){
    return grade;
}

void ReadingMetrics::setGrade(double grade){
    if (finished == false) {
        throw std::logic_error("Leitura incompleta");
    }
    else if (grade > 5) {
        throw std::invalid_argument("Nota maior que 5");
    }
    else if (grade < 0) {
        throw std::invalid_argument("Nota menor que 0");
    }
    else {
        this->grade = grade;
    }
}

bool ReadingMetrics::isFinished(){
    return finished;
}

void ReadingMetrics::finish(){
    if (currentPage != reading->getPages()){
        throw std::logic_error("Leitura incompleta");
    }
    else {
        finished = true;
    }
}

void ReadingMetrics::reset(){
    finished = false;
    grade = -1;
    currentPage = 0;
}

ReadingMetrics::~ReadingMetrics(){}