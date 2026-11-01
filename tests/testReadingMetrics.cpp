#include "doctest.h"
#include "ReadingMetrics.hpp"
#include "Book.hpp"

TEST_CASE("ReadingMetrics: Construtor correto"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro);
    CHECK(metrica.getReading() == livro);
    CHECK(metrica.getCurrentPage() == 0);
    CHECK(metrica.getGrade() == -1);
    CHECK(metrica.isFinished() == false);
    delete livro;
}

TEST_CASE("ReadingMetrics: addPages dentro do limite aceito"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro);
    metrica.addPages(100);
    CHECK(metrica.getCurrentPage() == 100);
    CHECK(metrica.isFinished() == false);
    delete livro;
}

TEST_CASE("ReadingMetrics: addPages completando o livro"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro);
    metrica.addPages(360);
    CHECK(metrica.getCurrentPage() == 360);
    CHECK(metrica.isFinished() == true);
    delete livro;
}

TEST_CASE("ReadingMetrics: addPages com limite superior violado"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro);
    metrica.addPages(400);
    CHECK(metrica.getCurrentPage() == 0);
    CHECK(metrica.isFinished() == false);
    delete livro;
}

TEST_CASE("ReadingMetrics: addPages com limite inferior violado"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro);
    metrica.addPages(-100);
    CHECK(metrica.getCurrentPage() == 0);
    CHECK(metrica.isFinished() == false);
    delete livro;
}

TEST_CASE("ReadingMetrics: addPages recebendo um char"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro);
    metrica.addPages('a');
    CHECK(metrica.getCurrentPage() == 0);
    CHECK(metrica.isFinished() == false);
    delete livro;
}

TEST_CASE("ReadingMetrics: addPages recebendo um double"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro);
    metrica.addPages(100.5);
    CHECK(metrica.getCurrentPage() == 100);
    CHECK(metrica.isFinished() == false);
    delete livro;
}

TEST_CASE("ReadingMetrics: finish sem ter terminado o livro"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro);
    metrica.finish();
    CHECK(metrica.isFinished() == false);
    delete livro;
}

TEST_CASE("ReadingMetrics: finish já tendo terminado o livro"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro);
    metrica.addPages(360);
    metrica.finish();
    CHECK(metrica.isFinished() == true);
    delete livro;
}

TEST_CASE("ReadingMetrics: reset sem ter começado o livro"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro);
    metrica.reset();
    CHECK(metrica.isFinished() == false);
    CHECK(metrica.getCurrentPage() == 0);
    CHECK(metrica.getGrade() == -1);
    delete livro;
}

TEST_CASE("ReadingMetrics: reset já tendo começado o livro"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro);
    metrica.addPages(150);
    metrica.reset();
    CHECK(metrica.isFinished() == false);
    CHECK(metrica.getCurrentPage() == 0);
    CHECK(metrica.getGrade() == -1);
    delete livro;
}

TEST_CASE("ReadingMetrics: reset já tendo terminado o livro"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro);
    metrica.addPages(360);
    metrica.reset();
    CHECK(metrica.isFinished() == false);
    CHECK(metrica.getCurrentPage() == 0);
    CHECK(metrica.getGrade() == -1);
    delete livro;
}

TEST_CASE("ReadingMetrics: setGrade sem ter terminado o livro"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro);
    metrica.setGrade(4.3);
    CHECK(metrica.getGrade() == -1);
    delete livro;
}

TEST_CASE("ReadingMetrics: setGrade com limite superior violado"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro);
    metrica.addPages(360);
    metrica.setGrade(5.1);
    CHECK(metrica.getGrade() == -1);
    delete livro;
}

TEST_CASE("ReadingMetrics: setGrade com o livro terminado e valor correto de nota"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro);
    metrica.addPages(360);
    metrica.setGrade(4.3);
    CHECK(metrica.getGrade() == 4.3);
    delete livro;
}

TEST_CASE("ReadingMetrics: setGrade com limite inferior violado"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro);
    metrica.addPages(360);
    metrica.setGrade(-1.2);
    CHECK(metrica.getGrade() == -1);
    delete livro;
}

TEST_CASE("ReadingMetrics: setGrade recebendo um char"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro);
    metrica.addPages(360);
    metrica.setGrade('a');
    CHECK(metrica.getGrade() == -1);
    delete livro;
}