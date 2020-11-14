main_busqueda: src/main_busqueda.o src/table.o
	g++ src/main_busqueda.o src/table.o -o bin/main_busqueda
main_busqueda.o: src/main_busqueda.cpp
	g++ -c src/main_busqueda.cpp
table.o: src/table.cpp
	g++ -c src/table.cpp
clean:
	rm -rf src/*.o