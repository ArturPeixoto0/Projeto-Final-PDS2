#include "doctest.h"
#include "ReadingMetrics.hpp"
#include "Book.hpp"
#include <stdexcept>

TEST_CASE("ReadingMetrics: Construtor correto"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro);
    CHECK(metrica.getReading() == livro);
    CHECK(metrica.getCurrentPage() == 0);
    CHECK(metrica.getGrade() == -1);
    CHECK(metrica.isFinished() == false);
    delete livro;
}

TEST_CASE("ReadingMetrics: Construtor recebendo ponteiro nulo"){
    CHECK_THROWS_AS(ReadingMetrics(nullptr), std::invalid_argument);
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
    CHECK_THROWS_AS(metrica.addPages(400), std::out_of_range);
    delete livro;
}

TEST_CASE("ReadingMetrics: addPages com limite inferior violado"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro);
    CHECK_THROWS_AS(metrica.addPages(-100), std::invalid_argument);
    delete livro;
}

TEST_CASE("ReadingMetrics: addPages com número de páginas igual a zero"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro);
    CHECK_THROWS_AS(metrica.addPages(0), std::invalid_argument);

    delete livro;
}

TEST_CASE("ReadingMetrics: finish sem ter terminado o livro"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro);
    CHECK_THROWS_AS(metrica.finish(), std::logic_error);
    
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
    CHECK_THROWS_AS(metrica.setGrade(4.3), std::logic_error);

    delete livro;
}

TEST_CASE("ReadingMetrics: setGrade com limite superior violado"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro);
    metrica.addPages(360);
    CHECK_THROWS_AS(metrica.setGrade(5.1), std::invalid_argument);
    
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
    CHECK_THROWS_AS(metrica.setGrade(-1.2), std::invalid_argument);
    
    delete livro;
}

// ---------------------- CONSTRUTOR DE CARGA ----------------------
// Usado pelo BancoDeDados para recriar uma métrica salva nos arquivos.

TEST_CASE("ReadingMetrics: construtor de carga guarda os valores salvos"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro, 120, -1, false);
    CHECK(metrica.getReading() == livro);
    CHECK(metrica.getCurrentPage() == 120);
    CHECK(metrica.getGrade() == -1);
    CHECK(metrica.isFinished() == false);
    delete livro;
}

TEST_CASE("ReadingMetrics: construtor de carga com leitura terminada e nota"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    ReadingMetrics metrica(livro, 360, 4.5, true);
    CHECK(metrica.getCurrentPage() == 360);
    CHECK(metrica.getGrade() == 4.5);
    CHECK(metrica.isFinished() == true);
    delete livro;
}

TEST_CASE("ReadingMetrics: construtor de carga aceita os limites (página 0, notas 0 e 5)"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    CHECK_NOTHROW(ReadingMetrics(livro, 0, -1, false));
    CHECK_NOTHROW(ReadingMetrics(livro, 360, 0, true));
    CHECK_NOTHROW(ReadingMetrics(livro, 360, 5, true));
    delete livro;
}

TEST_CASE("ReadingMetrics: construtor de carga com ponteiro nulo"){
    CHECK_THROWS_AS(ReadingMetrics(nullptr, 0, -1, false), std::invalid_argument);
}

TEST_CASE("ReadingMetrics: construtor de carga com página fora do livro"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    CHECK_THROWS_AS(ReadingMetrics(livro, -1, -1, false), std::invalid_argument);
    CHECK_THROWS_AS(ReadingMetrics(livro, 361, -1, false), std::invalid_argument);
    delete livro;
}

TEST_CASE("ReadingMetrics: construtor de carga com nota inválida"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    CHECK_THROWS_AS(ReadingMetrics(livro, 360, 5.5, true), std::invalid_argument);
    CHECK_THROWS_AS(ReadingMetrics(livro, 360, -0.5, true), std::invalid_argument);
    delete livro;
}

TEST_CASE("ReadingMetrics: construtor de carga terminado fora da última página"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    CHECK_THROWS_AS(ReadingMetrics(livro, 200, -1, true), std::invalid_argument);
    delete livro;
}

TEST_CASE("ReadingMetrics: construtor de carga com nota sem ter terminado"){
    Book* livro = new Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    CHECK_THROWS_AS(ReadingMetrics(livro, 200, 3, false), std::invalid_argument);
    delete livro;
}
