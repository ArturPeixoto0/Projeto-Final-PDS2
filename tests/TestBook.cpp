#include "doctest.h"
#include "book.hpp"

TEST_CASE("Book: contrutor guarda título do livro"){
    Book livro("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    CHECK(livro.getTitle() == "Os sete maridos de Evelyn Hugo");
}

TEST_CASE("Book: setTitle altera título do livro"){
    Book livro("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    livro.setTitle("sete maridos");
    CHECK(livro.getTitle() == "sete maridos");
}

TEST_CASE("Book: contrutor guarda autor do livro"){
    Book livro("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    CHECK(livro.getAuthor() == "Taylor Jenkins Reid");
}

TEST_CASE("Book: setAuthor altera autor do livro"){
    Book livro("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    livro.setTitle("Taylor");
    CHECK(livro.getTitle() == "Taylor");
}

TEST_CASE("Book: contrutor guarda gênero do livro"){
    Book livro("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    CHECK(livro.getGenre() == "drama");
}

TEST_CASE("Book: setGenre altera gênero do livro"){
    Book livro("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    livro.setGenre("terror");
    CHECK(livro.getGenre() == "terror");
}

TEST_CASE("Book: contrutor guarda número de páginas do livro"){
    Book livro("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    CHECK(livro.getPages() == 360);
}

TEST_CASE("Book: setPages altera número de páginas do livro"){
    Book livro("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    livro.setPages(100);
    CHECK(livro.getPages() == 100);
}

TEST_CASE("Book: contrutor guarda ISBN do livro"){
    Book livro("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    CHECK(livro.getISBN() == "01");
}

TEST_CASE("Book: setISBN altera ISBN do livro"){
    Book livro("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    livro.setISBN("02");
    CHECK(livro.getISBN() == "02");
}

//------------------------METODOS-----------------------------------------------------------

TEST_CASE("Book: livro vazio começa com nota e contadores zerados"){
    Book livro;
    CHECK(livro.getGrade() == 0);
    CHECK(livro.getNumberReaders() == 0);
    CHECK(livro.getNumberClubs() == 0);
}
 
TEST_CASE("Book: livro sem avaliações tem nota 0") {
    Book livro("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    CHECK(livro.getGrade() == 0);
}
 
TEST_CASE("Book: nota geral é a média das avaliações") {
    Book livro("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    livro.addGrade(4.0);
    livro.addGrade(2.0);
    CHECK(livro.getGrade() == doctest::Approx(3.0));
}
 
TEST_CASE("Book: contador de leitores sobe e desce") {
    Book livro("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    livro.incrementReaders();
    livro.incrementReaders();
    livro.decrementReaders();
    CHECK(livro.getNumberReaders() == 1);
}
 
TEST_CASE("Book: contador de leitores não fica negativo") {
    Book livro("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    livro.decrementReaders();
    CHECK(livro.getNumberReaders() == 0);
}
 
TEST_CASE("Book: contador de clubes sobe e desce") {
    Book livro("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    livro.incrementClubs();
    livro.incrementClubs();
    livro.decrementClubs();
    CHECK(livro.getNumberClubs() == 1);
}
 
TEST_CASE("Book: contador de clubes não fica negativo") {
    Book livro("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    livro.decrementClubs();
    CHECK(livro.getNumberClubs() == 0);
}