CC=g++
CFLAGS=-std=c++11 -Wall

BUILD_DIR=./build
INCLUDE_DIR=./include
TESTS_DIR=./tests
BUILD_TESTS_DIR=./tests/buildTests
SRC_DIR=./src
ENTIDADES_DIR=./src/entidades
    

CLASS_OBJS = ${BUILD_DIR}/Book.o \
${BUILD_DIR}/ReadingMetrics.o \
${BUILD_DIR}/BookClub.o \
${BUILD_DIR}/CheckIn.o \
${BUILD_DIR}/Comment.o \
${BUILD_DIR}/User.o \
${BUILD_DIR}/Profile.o \
${BUILD_DIR}/ClubProfile.o \
${BUILD_DIR}/UserProfile.o 

TESTS_OBJS = ${BUILD_TESTS_DIR}/testBook.o \
${BUILD_TESTS_DIR}/testBookClub.o \
${BUILD_TESTS_DIR}/testCheckIn.o \
${BUILD_TESTS_DIR}/testComment.o \
${BUILD_TESTS_DIR}/testUser.o \
${BUILD_TESTS_DIR}/testProfile.o \
${BUILD_TESTS_DIR}/testClubProfile.o \
${BUILD_TESTS_DIR}/testUserProfile.o \
${BUILD_TESTS_DIR}/testReadingMetrics.o

OBJS = ${CLASS_OBJS} \
${BUILD_DIR}/main.o
    
all: ${BUILD_DIR} ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${BUILD_DIR}/programa

${BUILD_DIR}/Book.o: ${INCLUDE_DIR}/Book.hpp ${ENTIDADES_DIR}/Book.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${ENTIDADES_DIR}/Book.cpp -o ${BUILD_DIR}/Book.o 
    
${BUILD_DIR}/ReadingMetrics.o: ${INCLUDE_DIR}/ReadingMetrics.hpp ${ENTIDADES_DIR}/ReadingMetrics.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${ENTIDADES_DIR}/ReadingMetrics.cpp -o ${BUILD_DIR}/ReadingMetrics.o

${BUILD_DIR}/BookClub.o: ${INCLUDE_DIR}/BookClub.hpp ${INCLUDE_DIR}/ClubProfile.hpp ${ENTIDADES_DIR}/BookClub.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${ENTIDADES_DIR}/BookClub.cpp -o ${BUILD_DIR}/BookClub.o 
    
${BUILD_DIR}/CheckIn.o: ${INCLUDE_DIR}/CheckIn.hpp ${ENTIDADES_DIR}/CheckIn.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${ENTIDADES_DIR}/CheckIn.cpp -o ${BUILD_DIR}/CheckIn.o 
    
${BUILD_DIR}/Comment.o: ${INCLUDE_DIR}/Comment.hpp ${ENTIDADES_DIR}/Comment.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${ENTIDADES_DIR}/Comment.cpp -o ${BUILD_DIR}/Comment.o 

${BUILD_DIR}/User.o: ${INCLUDE_DIR}/User.hpp ${INCLUDE_DIR}/UserProfile.hpp ${INCLUDE_DIR}/ReadingMetrics.hpp ${ENTIDADES_DIR}/User.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${ENTIDADES_DIR}/User.cpp -o ${BUILD_DIR}/User.o

${BUILD_DIR}/Profile.o: ${INCLUDE_DIR}/Profile.hpp ${ENTIDADES_DIR}/Profile.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${ENTIDADES_DIR}/Profile.cpp -o ${BUILD_DIR}/Profile.o 

${BUILD_DIR}/ClubProfile.o: ${INCLUDE_DIR}/ClubProfile.hpp ${INCLUDE_DIR}/Profile.hpp ${ENTIDADES_DIR}/ClubProfile.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${ENTIDADES_DIR}/ClubProfile.cpp -o ${BUILD_DIR}/ClubProfile.o 

${BUILD_DIR}/UserProfile.o: ${INCLUDE_DIR}/UserProfile.hpp ${INCLUDE_DIR}/Profile.hpp ${ENTIDADES_DIR}/UserProfile.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${ENTIDADES_DIR}/UserProfile.cpp -o ${BUILD_DIR}/UserProfile.o 

${BUILD_DIR}/main.o: ${INCLUDE_DIR}/Book.hpp ${INCLUDE_DIR}/BookClub.hpp ${INCLUDE_DIR}/CheckIn.hpp ${INCLUDE_DIR}/Comment.hpp ${INCLUDE_DIR}/User.hpp ${INCLUDE_DIR}/Profile.hpp ${INCLUDE_DIR}/ClubProfile.hpp ${SRC_DIR}/main.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${SRC_DIR}/main.cpp -o ${BUILD_DIR}/main.o
    
#Compila os tests

${BUILD_TESTS_DIR}/testBook.o: ${INCLUDE_DIR}/doctest.h ${TESTS_DIR}/testBook.cpp ${INCLUDE_DIR}/Book.hpp 
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${TESTS_DIR}/testBook.cpp -o ${BUILD_TESTS_DIR}/testBook.o 

${BUILD_TESTS_DIR}/testBookClub.o: ${INCLUDE_DIR}/doctest.h ${TESTS_DIR}/testBookClub.cpp ${INCLUDE_DIR}/BookClub.hpp 
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${TESTS_DIR}/testBookClub.cpp -o ${BUILD_TESTS_DIR}/testBookClub.o 

${BUILD_TESTS_DIR}/testCheckIn.o: ${INCLUDE_DIR}/doctest.h ${TESTS_DIR}/testCheckIn.cpp ${INCLUDE_DIR}/CheckIn.hpp 
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${TESTS_DIR}/testCheckIn.cpp -o ${BUILD_TESTS_DIR}/testCheckIn.o 

${BUILD_TESTS_DIR}/testComment.o: ${INCLUDE_DIR}/doctest.h ${TESTS_DIR}/testComment.cpp ${INCLUDE_DIR}/Comment.hpp 
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${TESTS_DIR}/testComment.cpp -o ${BUILD_TESTS_DIR}/testComment.o 

${BUILD_TESTS_DIR}/testUser.o: ${INCLUDE_DIR}/doctest.h ${TESTS_DIR}/testUser.cpp ${INCLUDE_DIR}/User.hpp 
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${TESTS_DIR}/testUser.cpp -o ${BUILD_TESTS_DIR}/testUser.o 

${BUILD_TESTS_DIR}/testProfile.o: ${INCLUDE_DIR}/doctest.h ${TESTS_DIR}/testProfile.cpp ${INCLUDE_DIR}/Profile.hpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${TESTS_DIR}/testProfile.cpp -o ${BUILD_TESTS_DIR}/testProfile.o 

${BUILD_TESTS_DIR}/testClubProfile.o: ${INCLUDE_DIR}/doctest.h ${TESTS_DIR}/testClubProfile.cpp ${INCLUDE_DIR}/ClubProfile.hpp ${INCLUDE_DIR}/Profile.hpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${TESTS_DIR}/testClubProfile.cpp -o ${BUILD_TESTS_DIR}/testClubProfile.o 

${BUILD_TESTS_DIR}/testUserProfile.o: ${INCLUDE_DIR}/doctest.h ${TESTS_DIR}/testUserProfile.cpp ${INCLUDE_DIR}/UserProfile.hpp ${INCLUDE_DIR}/Profile.hpp 
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${TESTS_DIR}/testUserProfile.cpp -o ${BUILD_TESTS_DIR}/testUserProfile.o 

${BUILD_TESTS_DIR}/testMain.o: ${INCLUDE_DIR}/doctest.h ${TESTS_DIR}/testMain.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${TESTS_DIR}/testMain.cpp -o ${BUILD_TESTS_DIR}/testMain.o

${BUILD_TESTS_DIR}/testReadingMetrics.o: ${INCLUDE_DIR}/doctest.h ${TESTS_DIR}/testReadingMetrics.cpp ${INCLUDE_DIR}/ReadingMetrics.hpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${TESTS_DIR}/testReadingMetrics.cpp -o ${BUILD_TESTS_DIR}/testReadingMetrics.o

ifneq ($(filter testBook,$(MAKECMDGOALS)),)
SELECTED_TEST = ${BUILD_TESTS_DIR}/testBook.o
NAME_EXECUTABLE = testBook
else ifneq ($(filter testBookClub,$(MAKECMDGOALS)),)
SELECTED_TEST = ${BUILD_TESTS_DIR}/testBookClub.o
NAME_EXECUTABLE = testBookClub
else ifneq ($(filter testCheckIn,$(MAKECMDGOALS)),)
SELECTED_TEST = ${BUILD_TESTS_DIR}/testCheckIn.o
NAME_EXECUTABLE = testCheckIn
else ifneq ($(filter testComment,$(MAKECMDGOALS)),)
SELECTED_TEST = ${BUILD_TESTS_DIR}/testComment.o
NAME_EXECUTABLE = testComment
else ifneq ($(filter testUser,$(MAKECMDGOALS)),)
SELECTED_TEST = ${BUILD_TESTS_DIR}/testUser.o
NAME_EXECUTABLE = testUser
else ifneq ($(filter testProfile,$(MAKECMDGOALS)),)
SELECTED_TEST = ${BUILD_TESTS_DIR}/testProfile.o
NAME_EXECUTABLE = testProfile
else ifneq ($(filter testClubProfile,$(MAKECMDGOALS)),)
SELECTED_TEST = ${BUILD_TESTS_DIR}/testClubProfile.o
NAME_EXECUTABLE = testClubProfile
else ifneq ($(filter testUserProfile,$(MAKECMDGOALS)),)
SELECTED_TEST = ${BUILD_TESTS_DIR}/testUserProfile.o
NAME_EXECUTABLE = testUserProfile
else ifneq ($(filter testReadingMetrics,$(MAKECMDGOALS)),)
SELECTED_TEST = ${BUILD_TESTS_DIR}/testReadingMetrics.o
NAME_EXECUTABLE = testReadingMetrics
else 
SELECTED_TEST = ${TESTS_OBJS}
NAME_EXECUTABLE = allTests
endif


.PHONY: test testBook testBookClub testCheckIn testComment testUser testProfile testClubProfile testUserProfile testReadingMetrics

test: ${SELECTED_TEST} ${CLASS_OBJS} ${BUILD_TESTS_DIR}/testMain.o
	${CC} $^ -o ${BUILD_TESTS_DIR}/${NAME_EXECUTABLE}
	./${BUILD_TESTS_DIR}/${NAME_EXECUTABLE}

testBook testBookClub testCheckIn testComment testUser testProfile testClubProfile testUserProfile testReadingMetrics:



clean:
	rm -f ${BUILD_DIR}/*.o ${BUILD_DIR}/programa ${BUILD_TESTS_DIR}/*.o