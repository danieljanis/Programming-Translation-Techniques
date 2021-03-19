scanner: testScanner.o driver.o scanner.o table.o
		g++ testScanner.o driver.o scanner.o table.o -o scanner

testScanner.o: testScanner.cpp
		g++ -c testScanner.cpp

driver.o: driver.cpp
		g++ -c driver.cpp

scanner.o: scanner.cpp
		g++ -c scanner.cpp

table.o: table.cpp
		g++ -c table.cpp

clean:
		rm -rf *.o scanner
