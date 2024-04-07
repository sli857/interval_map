all: clean a
a: interval_map.cpp
	g++ -g -std=c++17 interval_map.cpp -o a -Wall
clean:
	rm -f a