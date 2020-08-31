FLAG=-std=c++11

demo:demo.cpp
	g++ $^ -o $@ $(FLAG) -I ../cppjieba-master/include -lboost_filesystem -lboost_system

.PHONY:clean
clean:
	rm demo