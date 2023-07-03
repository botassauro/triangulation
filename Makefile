all: triangulate

triangulate: triangulation.o poligonos.o
	g++ -o triangulate poligonos.o triangulation.o

poligonos.o: poligonos.cpp poligonos.h
	g++ -c poligonos.cpp

triagulation.o: triangulation.cpp
	g++ -c triangulation.cpp
