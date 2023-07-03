#ifndef POLIGONO_HPP
#define POLIGONO_HPP
#include <algorithm>
#include <vector>

class Ponto{
    public:
    Ponto(int x, int y, bool convexo, int index);
    int x;
    int y;
    bool convexo;
    int index;

    bool operator< (const Ponto &right) const {
        return y < right.y;
    }
};

class Aresta {
    public:
    Aresta(Ponto ini, Ponto fim);
    Aresta();
    Ponto ini; Ponto fim;
    int tri1;
    int tri2;

    bool operator< (const Aresta &right) const {
        return ini.y < right.ini.y;
    }
};

class Triangulo {
    public:
    Triangulo(std::vector<Ponto> vertices, std::vector<Aresta> arestas, std::vector<Triangulo> vizinhos, int tri_index);

    std::vector<Ponto> vertices;
    std::vector<Aresta> arestas;
    std::vector<Triangulo> vizinhos;
    int tri_index;
};

int binarySearch(std::vector<int> v, int n, int target);

int produtoVetorialPonto(Ponto p1, Ponto p2, Ponto p3);

bool comparaPontos(Ponto p1, Ponto p2);

bool estaDentroTriangulo(std::vector<Ponto> vertices, Ponto p1, Ponto p2, Ponto p3);

Aresta* earClipping(Ponto first, Ponto mid, Ponto last, Aresta* edge);

void imprimeArestas(std::vector<Aresta> tri_arestas);

void calcularVizinhos(std::vector<Aresta>* all_edges);

void trocaConvexidade(std::vector<Ponto> *polygon, int size);

void imprimeSaida(int vertices_num, int num_of_triangles, std::vector<Ponto> polygon_copy, std::vector<Triangulo> triangulation);

bool compare(Aresta a1, Aresta a2);

bool compare3(Aresta a1, Aresta a2);

bool compare4(Ponto v1, Ponto v2);

bool compare5(Aresta a1, Aresta a2);

#endif