all:
	# g++ -O3 -std=c++11 -Wall -D DEBUG src/main.cpp src/game.cpp src/board.cpp -D V0 src/negascout.cpp src/node.cpp -o negascout -g
	g++ -O3 -std=c++11 -Wall -D REAL src/main.cpp src/game.cpp src/board.cpp -D V0 src/negascout.cpp src/node.cpp -o negascout -g
	
clean:
	rm -f negascout