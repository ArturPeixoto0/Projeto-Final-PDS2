#include "doctest.h"
#include "Book.hpp"
#include <stdexcept>

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
    livro.setAuthor("Taylor");
    CHECK(livro.getAuthor() == "Taylor");
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

//exceções:
TEST_CASE("Book: construtor com título vazio lança exceção"){
    CHECK_THROWS_AS(Book("","Taylor Jenkins Reid","drama",360,"01"), std::invalid_argument);
}

TEST_CASE("Book: construtor com zero páginas lança exceção"){
    CHECK_THROWS_AS(Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",0,"01"), std::invalid_argument);
}

TEST_CASE("Book: construtor com páginas negativas lança exceção"){
    CHECK_THROWS_AS(Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",-5,"01"), std::invalid_argument);
}

TEST_CASE("Book: construtor com uma página funciona"){
    CHECK_NOTHROW(Book("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",1,"01"));
}

TEST_CASE("Book: setTitle com título vazio lança exceção e não muda o título"){
    Book livro("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    CHECK_THROWS_AS(livro.setTitle(""), std::invalid_argument);
    CHECK(livro.getTitle() == "Os sete maridos de Evelyn Hugo");
}

TEST_CASE("Book: setPages com zero páginas lança exceção e não muda as páginas"){
    Book livro("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    CHECK_THROWS_AS(livro.setPages(0), std::invalid_argument);
    CHECK(livro.getPages() == 360);
}

TEST_CASE("Book: setPages com páginas negativas lança exceção e não muda as páginas"){
    Book livro("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    CHECK_THROWS_AS(livro.setPages(-10), std::invalid_argument);
    CHECK(livro.getPages() == 360);
}

// IDs: o contador proximoId é compartilhado por todos os testes, então os
// testes comparam ids entre si em vez de esperar números fixos (como 1, 2...).

TEST_CASE("Book: livros criados sem id recebem ids diferentes e crescentes"){
    Book primeiro("Os sete maridos de Evelyn Hugo","Taylor Jenkins Reid","drama",360,"01");
    Book segundo("Dom Casmurro","Machado de Assis","romance",300,"02");
    CHECK(primeiro.getId() > 0);
    CHECK(segundo.getId() > primeiro.getId());
}

TEST_CASE("Book: construtor respeita o id informado"){
    Book livro("Dom Casmurro","Machado de Assis","romance",300,"02",5000);
    CHECK(livro.getId() == 5000);
}

TEST_CASE("Book: id gerado depois de um id informado é maior que ele"){
    Book carregado("Dom Casmurro","Machado de Assis","romance",300,"02",7000);
    Book novo("A hora da estrela","Clarice Lispector","romance",88,"03");
    CHECK(novo.getId() > carregado.getId());
}

TEST_CASE("Book: construtor com id negativo lança exceção"){
    CHECK_THROWS_AS(Book("Dom Casmurro","Machado de Assis","romance",300,"02",-1), std::invalid_argument);
}

TEST_CASE("Book: construtor que lança exceção não gasta id"){
    Book antes("Dom Casmurro","Machado de Assis","romance",300,"02");
    CHECK_THROWS_AS(Book("","Machado de Assis","romance",300,"02"), std::invalid_argument);
    Book depois("A hora da estrela","Clarice Lispector","romance",88,"03");
    CHECK(depois.getId() == antes.getId() + 1);
}
