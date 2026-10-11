CC=g++
CFLAGS=-std=c++11 -Wall

BUILD_DIR=./build
INCLUDE_DIR=./include
SRC_DIR=./src
ENTIDADES_DIR=./src/entidades
    
OBJS = ${BUILD_DIR}/book.o \
${BUILD_DIR}/bookClub.o \
${BUILD_DIR}/CheckIn.o \
${BUILD_DIR}/Comment.o \
${BUILD_DIR}/User.o \
${BUILD_DIR}/Profile.o \
${BUILD_DIR}/ClubProfile.o \
${BUILD_DIR}/UserProfile.o \
${BUILD_DIR}/main.o
    
all: ${BUILD_DIR} ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${BUILD_DIR}/programa

${BUILD_DIR}/book.o: ${INCLUDE_DIR}/book.hpp ${ENTIDADES_DIR}/book.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${ENTIDADES_DIR}/book.cpp -o ${BUILD_DIR}/book.o 
    
${BUILD_DIR}/bookClub.o: ${INCLUDE_DIR}/bookClub.hpp ${INCLUDE_DIR}/ClubProfile.hpp ${ENTIDADES_DIR}/bookClub.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${ENTIDADES_DIR}/bookClub.cpp -o ${BUILD_DIR}/bookClub.o 
    
${BUILD_DIR}/CheckIn.o: ${INCLUDE_DIR}/CheckIn.hpp ${ENTIDADES_DIR}/CheckIn.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${ENTIDADES_DIR}/CheckIn.cpp -o ${BUILD_DIR}/CheckIn.o 
    
${BUILD_DIR}/Comment.o: ${INCLUDE_DIR}/Comment.hpp ${ENTIDADES_DIR}/Comment.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${ENTIDADES_DIR}/Comment.cpp -o ${BUILD_DIR}/Comment.o 

${BUILD_DIR}/User.o: ${INCLUDE_DIR}/User.hpp ${INCLUDE_DIR}/UserProfile.hpp ${ENTIDADES_DIR}/User.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${ENTIDADES_DIR}/User.cpp -o ${BUILD_DIR}/User.o 

${BUILD_DIR}/Profile.o: ${INCLUDE_DIR}/Profile.hpp ${ENTIDADES_DIR}/Profile.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${ENTIDADES_DIR}/Profile.cpp -o ${BUILD_DIR}/Profile.o 

${BUILD_DIR}/ClubProfile.o: ${INCLUDE_DIR}/ClubProfile.hpp ${INCLUDE_DIR}/Profile.hpp ${ENTIDADES_DIR}/ClubProfile.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${ENTIDADES_DIR}/ClubProfile.cpp -o ${BUILD_DIR}/ClubProfile.o 

${BUILD_DIR}/UserProfile.o: ${INCLUDE_DIR}/UserProfile.hpp ${INCLUDE_DIR}/Profile.hpp ${ENTIDADES_DIR}/UserProfile.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${ENTIDADES_DIR}/UserProfile.cpp -o ${BUILD_DIR}/UserProfile.o 

${BUILD_DIR}/main.o: ${INCLUDE_DIR}/book.hpp ${INCLUDE_DIR}/bookClub.hpp ${INCLUDE_DIR}/CheckIn.hpp ${INCLUDE_DIR}/Comment.hpp ${INCLUDE_DIR}/User.hpp ${INCLUDE_DIR}/Profile.hpp ${INCLUDE_DIR}/ClubProfile.hpp ${INCLUDE_DIR}/ClubProfile.hpp ${SRC_DIR}/main.cpp
	${CC} ${CFLAGS} -I ${INCLUDE_DIR}/ -c ${SRC_DIR}/main.cpp -o ${BUILD_DIR}/main.o
    
clean:
	rm -f ${BUILD_DIR}/*.o ${BUILD_DIR}/programa