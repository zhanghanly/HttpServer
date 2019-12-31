TARGET = HttpServer
SOURCE = ${wildcard *.cpp}
OBJECTS = ${patsubst %.cpp, %.o, ${SOURCE}}


CXX = g++
CFLAGS = -std=c++11 -g
LIB = -lpthread

${TARGET} : ${OBJECTS}
	${CXX} -o ${TARGET} ${CFLAGS} ${OBJECTS} ${LIB}
${OBJECTS} : %.o : %.cpp
	${CXX} -c ${CFLAGS} $< -o $@ 



.PHONY : clean
clean : 
	echo ${TARGET}
	echo ${OBJECTS}
	-rm ${TARGET} *.o
