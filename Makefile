INCLUDE = -I./include
LIBS = -L./libs/ 
LINK = -lhtmlcxx -lcurl -lboost_program_options
CXX = g++
FLAGS = -Wall 

all: main  

main: 
	${CXX} ${FLAGS} -o bin/main src/main.cc src/spider.cc src/util.cc ${INCLUDE} ${LIBS} ${LINK} 

spider:
	${CXX} -c src/spider.cc ${INCLUDE} ${LIBS} ${LINK} 

run:
	./bin/main ./config/spider.ini

test:
	${CXX} src/test.cc ${INCLUDE} ${LIBS} ${LINK} -o bin/test

clean: 
	rm -f bin/main 
	rm -f *.o
