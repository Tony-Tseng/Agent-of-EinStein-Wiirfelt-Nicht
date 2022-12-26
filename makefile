all:
	g++ -O3 -std=c++11 -Wall -D DEBUG src/main.cpp src/game.cpp src/board.cpp src/negascout.cpp src/table.cpp src/table_entry.cpp -o negascout -g
	# g++ -O3 -std=c++11 -Wall -D REAL src/main.cpp src/game.cpp src/board.cpp src/negascout.cpp src/table.cpp src/table_entry.cpp -o negascout -g

clean:
	rm -f negascout