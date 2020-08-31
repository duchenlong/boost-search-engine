FLAG=-std=c++11 -I ../../install/cppjieba-master/include -lboost_filesystem -lboost_system

parser_test:parser_test.cpp parser.hpp parser.cpp
	g++ $^ -o $@ $(FLAG)

.PHONY:clean
clean:
	rm -rf parser_test