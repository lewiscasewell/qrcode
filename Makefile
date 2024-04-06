build:
	@g++ -std=c++11 -o main main.cpp

run:
	@./main

clean:
	@rm -rf main

it: clean build run

dsa:
	@g++ -std=c++11 -o dsa dsa.cpp && ./dsa