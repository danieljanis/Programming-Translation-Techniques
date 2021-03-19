frontEnd: testScanner.o driver.o parser.o scanner.o tree.o table.o
		g++ testScanner.o driver.o parser.o scanner.o tree.o table.o -o frontEnd

testScanner.o: testScanner.cpp
		g++ -c testScanner.cpp

driver.o: driver.cpp
		g++ -c driver.cpp

parser.o: parser.cpp
		g++ -c parser.cpp

scanner.o: scanner.cpp
		g++ -c scanner.cpp

tree.o: tree.cpp
		g++ -c tree.cpp

table.o: table.cpp
		g++ -c table.cpp

clean:
		rm -rf *.o frontEnd
