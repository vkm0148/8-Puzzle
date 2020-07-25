run: 8puzzle.cpp 
	rm -f 8puzzle.o output.txt 8puzzle
	g++ -c 8puzzle.cpp
	g++ 8puzzle.o -o 8puzzle
	./8puzzle < input.txt > output.txt
	cat output.txt