all:
	g++ -std=c++11 -Wall -D REAL src/main.cpp src/game.cpp src/board.cpp src/negascout.cpp src/table.cpp src/table_entry.cpp src/strategy.cpp src/time_controler.cpp -o negascout -g	

debug:
	g++ -O3 -std=c++11 -Wall -D DEBUG src/main.cpp src/game.cpp src/board.cpp src/negascout.cpp src/table.cpp src/table_entry.cpp src/strategy.cpp src/time_controler.cpp -o negascout -g

clean:
	rm -f negascout