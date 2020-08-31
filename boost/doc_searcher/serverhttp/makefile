FLAG=-std=c++11 -I ../../install/cppjieba-master/include -lpthread -L ../searcher -lsearcher


main:main.cpp 
	g++ $^ -o $@ $(FLAG)

.PHONY:clean
clean:
	rm -rf main