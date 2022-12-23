all:
	g++ -O3 -std=c++11 -Wall src/main.cpp src/game.cpp src/board.cpp -D V0 src/negascout.cpp src/node.cpp -o negascout -g

clean:
	rm -f negascout