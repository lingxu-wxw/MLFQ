CXX=g++
CXXFLAGS=-g -D_GLIBCXX_ASSERTIONS -Wall
TARGET=scheduler
OBJS=main.o mlfq.o

${TARGET}: ${OBJS}
	${CXX} ${CXXFLAGS} ${OBJS} -o ${TARGET}

main.o: main.cpp
	${CXX} ${CXXFLAGS} -c main.cpp

mlfq.o: mlfq.cpp
	${CXX} ${CXXFLAGS} -c mlfq.cpp

.PHONY : clean
clean:
	rm -f ${OBJS} ${TARGET}

.PHONY : run
run:
	./${TARGET}