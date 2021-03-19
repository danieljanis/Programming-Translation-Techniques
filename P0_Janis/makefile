P0: main.o tree.o traversals.o
		g++ main.o tree.o traversals.o -o P0

main.o: main.cpp
		g++ -c main.cpp

tree.o: tree.cpp
		g++ -c tree.cpp

traversals.o: traversals.cpp
		g++ -c traversals.cpp

clean:
		rm -rf *.o P0
