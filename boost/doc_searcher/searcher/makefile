FLAG=-std=c++11 -I ../../install/cppjieba-master/include -lboost_filesystem -lboost_system -ljsoncpp

.PHONY:all
all:testindex testsearcher libsearcher.so

testsearcher:testsearcher.cpp searcher.cpp
	g++ $^ -o $@ $(FLAG)

testindex:testindex.cpp searcher.cpp
	g++ $^ -o $@ $(FLAG)

libsearcher.so:searcher.cpp searcher.hpp
	g++ searcher.cpp -o $@ -shared -fPIC $(FLAG)

.PHONY:clean
clean:
	rm -rf testsearcher testindex libsearcher.so
