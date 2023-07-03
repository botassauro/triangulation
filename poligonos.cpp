#include <iostream>
#include <iterator>
#include <vector>
#include "poligonos.h"
using namespace std;

/* comparing functions for sorting */
bool compare(Aresta a1, Aresta a2) {
    return (a1.ini.x < a2.ini.x);
}

bool compare3(Aresta a1, Aresta a2) {
    return (a1.tri1 < a2.tri1);
}

bool compare4(Ponto v1, Ponto v2) {
    return (v1.index < v2.index);
}

bool compare5(Aresta a1, Aresta a2) {
    return (a1.ini.index < a2.ini.index);
}

/* binary search for intervals */
int binarySearch(vector<int> v, int n, int target) {
    
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if ((mid == 0 || v.at(mid-1) < target) && v.at(mid) == target)
            return mid;
        else if (target > v.at(mid))
            low = mid + 1;
        else
            high = mid - 1;
    }

    return -1;
}

/* calculates dot product for two consecutive edges */
int produtoVetorialPonto(Ponto p1, Ponto p2, Ponto p3) {

	int ax = p1.x - p2.x;
	int ay = p1.y - p2.y;
	int bx = p2.x - p3.x;
	int by = p2.y - p3.y;
	return ((ax * by) - (ay * bx));
}

/* returns true if points p1 and p2 are the same; false if they are different */
bool comparaPontos(Ponto p1, Ponto p2) {
    if (p1.x == p2.x && p1.y == p2.y)
        return true;
    return false;
}

/* A utility function to calculate area of triangle formed by (x1, y1),
(x2, y2) and (x3, y3) */
float area(Ponto p1, Ponto p2, Ponto p3) {
	return abs((p1.x*(p2.y-p3.y) + p2.x*(p3.y-p1.y)+ p3.x*(p1.y-p2.y))/2.0);
}

/* A function to check whether point P(x, y) lies inside the triangle formed
by A(x1, y1), B(x2, y2) and C(x3, y3) */
bool estaDentroTriangulo(vector<Ponto> vertices, Ponto p1, Ponto p2, Ponto p3) {
	/* Calculate area of triangle ABC */

    int i = 0;

	float A = area (p1, p2, p3);

    while (i < vertices.size()) {

        if (comparaPontos(vertices.at(i), p1) || comparaPontos(vertices.at(i), p2) || comparaPontos(vertices.at(i), p3)) {
            i++;
            continue;
        }

        /* Calculate area of triangle PBC */
        float A1 = area (vertices.at(i), p2, p3);

        /* Calculate area of triangle PAC */
        float A2 = area (p1, vertices.at(i), p3);

        /* Calculate area of triangle PAB */
        float A3 = area (p1, p2, vertices.at(i));
            
        /* Check if sum of A1, A2 and A3 is same as A */
        if (A == A1 + A2 + A3)
            return true;
        i++;
    }
    return false;
}

/* checks if it's possible to form a triangle given points first, mid and last, following the ear clipping algorithm */
Aresta* earClipping(Ponto first, Ponto mid, Ponto last, Aresta* edge) {

    if (mid.convexo == true) {
        //printf("Tudo convexo.\n");
        edge->ini = first;
        edge->fim = last;
        return edge;
    }
    
    if (!(first.convexo) && !(mid.convexo) && !(last.convexo)) {
        //printf("nada convexo.\n");
        edge->ini = first;
        edge->fim = last;
        //printf("aqui: %d %d // %d %d // %d %d\n", first.x, first.y, mid.x, mid.y, last.x, last.y);
        return edge;
    }

    return NULL;
}

/* prints Aresta objects (edges) */
void imprimeArestas(vector<Aresta> tri_arestas) {

    printf("a: %d,%d // %d,%d\n", tri_arestas.at(0).ini.x, tri_arestas.at(0).ini.y, tri_arestas.at(0).fim.x, tri_arestas.at(0).fim.y);
    printf("a: %d,%d // %d,%d\n", tri_arestas.at(1).ini.x, tri_arestas.at(1).ini.y, tri_arestas.at(1).fim.x, tri_arestas.at(1).fim.y);
    printf("a: %d,%d // %d,%d\n", tri_arestas.at(2).ini.x, tri_arestas.at(2).ini.y, tri_arestas.at(2).fim.x, tri_arestas.at(2).fim.y);

}

/* calculates each triangle's neighbors */
void calcularVizinhos(vector<Aresta>* all_edges) {
    
    vector<int> v_indices;
    vector<Aresta>::iterator it;
    
    for (it = all_edges->begin(); it != all_edges->end(); ++it) {
        v_indices.push_back(it->ini.x);
    }

    int i_vizinho;
    for (it = all_edges->begin(); it != all_edges->end(); ++it) {

        i_vizinho = it->fim.x;
        int t = binarySearch(v_indices, (all_edges->size() -1), i_vizinho);
        //printf("t: %d\n", t);
            if (t != -1) {
                while (all_edges->at(t).ini.x == all_edges->at(t+1).ini.x && t < (all_edges->size()-1)) {

                    //printf("%d,%d // %d,%d\n", it->ini.x, it->ini.y, it->fim.x, it->fim.y);
                    //printf("%d,%d // %d,%d\n", all_edges->at(t).fim.x, all_edges->at(t).fim.y, all_edges->at(t).ini.x, all_edges->at(t).ini.y);
                    if (it->ini.x == all_edges->at(t).fim.x &&
                    it->ini.y == all_edges->at(t).fim.y && 
                    it->fim.x == all_edges->at(t).ini.x && 
                    it->fim.y == all_edges->at(t).ini.y) {
                        it->tri2 = all_edges->at(t).tri1;
                        all_edges->at(t).tri2 = it->tri1;
                    }

                    t++;
                    if (t >= all_edges->size() -1)
                        break;
                }
            }
    }

}

/* changes a given polygon's orientation (counterclockwise to clockwise) */
void trocaConvexidade(vector<Ponto> *polygon, int size) {

    int i = 0;
    int aux_index;
    for (i = 0; i < (size / 2); i++) {
        Ponto aux = polygon->at(i);
        polygon->at(i) = polygon->at((size-1) - i);
        polygon->at((size-1) - i) = aux;

        aux_index = polygon->at(i).index;
        polygon->at(i).index = polygon->at((size-1) - i).index;
        polygon->at((size-1) - i).index = aux_index;

        polygon->at(i).convexo = !(polygon->at(i).convexo);
        polygon->at((size-1) - i).convexo = !(polygon->at((size-1) - i).convexo);

    }

    polygon->at(i).convexo = !(polygon->at(i).convexo);
}

/* prints the program's output */
void imprimeSaida(int vertices_num, int num_of_triangles, vector<Ponto> polygon_copy, vector<Triangulo> triangulation) {

    printf("%d\n", vertices_num);
    for (int i = 0; i < vertices_num; i++) {
        printf("%d ", polygon_copy.at(i).x);
        printf("%d\n", polygon_copy.at(i).y);
    }

    printf("%d\n", num_of_triangles);
    for (int i = 0; i < num_of_triangles; i++) {

        printf("%d ", triangulation.at(i).vertices.at(0).index);
        printf("%d ", triangulation.at(i).vertices.at(1).index);
        printf("%d ", triangulation.at(i).vertices.at(2).index);

        printf("%d ",  triangulation.at(i).arestas.at(0).tri2);
        printf("%d ",  triangulation.at(i).arestas.at(1).tri2);
        printf("%d\n",  triangulation.at(i).arestas.at(2).tri2);
    }
}