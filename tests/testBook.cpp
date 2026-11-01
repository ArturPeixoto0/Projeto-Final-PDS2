#include "doctest.h"
#include "Book.hpp"

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
