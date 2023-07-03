#include <iostream>
#include <iterator>
#include <vector>
#include "poligonos.h"
using namespace std;

Ponto::Ponto(int x, int y, bool convexo, int index)
	:x{x}, y{y}, convexo{convexo}, index{index} {}

Aresta::Aresta(Ponto ini, Ponto fim)
	:ini{ini}, fim{fim} {}

Triangulo::Triangulo(vector<Ponto> vertices, vector<Aresta> arestas, vector<Triangulo> vizinhos, int tri_index)
	:vertices{vertices}, arestas{arestas}, vizinhos{vizinhos}, tri_index{tri_index} {}

int main() {

    int vertices_num;
	int x, y;
	vector<Ponto> polygon;
    int soma_convexo = 0;

    fscanf(stdin, "%d", &vertices_num);
    fscanf(stdin, "%d", &x);
    fscanf(stdin, "%d", &y);
    Ponto v_first{x, y, true, 1};
    fscanf(stdin, "%d", &x);
    fscanf(stdin, "%d", &y);
    Ponto v_mid{x, y, true, 2};
    polygon.push_back(v_first);

    // constroi a lista de vértices e define as propriedades de cada um
	for (int i = 1; i < vertices_num; i++) {
		
		fscanf(stdin, "%d", &x);
        fscanf(stdin, "%d", &y);
        Ponto v_last{x,y, true, i+2};
        if (produtoVetorialPonto(v_first, v_mid, v_last) > 0)
            v_mid.convexo = false;
        
        soma_convexo = soma_convexo + v_mid.convexo;
        polygon.push_back(v_mid);
        v_first = v_mid;
        v_mid = v_last;
	}

    int size = polygon.size();

    // primeiro vertice
    if (produtoVetorialPonto(polygon.at(size - 1), polygon.at(0), polygon.at(1)) > 0)
            polygon.at(0).convexo = false;
    
    // ultimo vertice
    if (produtoVetorialPonto(polygon.at(size - 2), polygon.at(size - 1), polygon.at(0)) > 0)
            polygon.at(size - 1).convexo = false;
    
    // conta quantos vertices são convexos/nao convexos
    soma_convexo = soma_convexo + polygon.at(0).convexo + polygon.at(size - 1).convexo - 1;
    
    // se mais da metade é nao convexa, entende-se que o poligono foi dado no sentido horario
    // troca-se o sentido de leitura do poligono
    if (soma_convexo < (size / 2))
        trocaConvexidade(&polygon, size);

    Aresta* a_ptr = NULL;
    Ponto o{0,0, true, 0};
    Aresta e{o,o};

    int i = 0;
    bool inside = false;
    vector<Ponto> polygon_copy = polygon;
    vector<Triangulo> triangulation;
    int num_of_triangles = 0;

    while (polygon.size() > 2) {
        
        if (i >= polygon.size())
            i = 0;

        if (i == 0) {
            a_ptr = earClipping(polygon.at(polygon.size() - 1), polygon.at(i), polygon.at(i+1), &e);
            inside = estaDentroTriangulo(polygon, polygon.at(polygon.size() - 1), polygon.at(i), polygon.at(i+1));
            //printf("escolhido: %d %d, %d %d, %d %d\n", polygon.at(polygon.size() - 1).x, polygon.at(polygon.size() - 1).y, polygon.at(i).x, polygon.at(i).y, polygon.at(i+1).x, polygon.at(i+1).y);
            //printf("INSIDE: %d\n", inside);
        }

        if (i == (polygon.size() - 1)) {
            a_ptr = earClipping(polygon.at(i-1), polygon.at(i), polygon.at(0), &e);
            //printf("escolhido: %d %d, %d %d, %d %d\n", polygon.at(i-1).x, polygon.at(i-1).y, polygon.at(i).x, polygon.at(i).y, polygon.at(0).x, polygon.at(0).y);
            inside = estaDentroTriangulo(polygon, polygon.at(i-1), polygon.at(i), polygon.at(0));
            //printf("INSIDE: %d\n", inside);

        }

        if (i != 0 && i != (polygon.size() - 1)) {
            a_ptr = earClipping(polygon.at(i-1), polygon.at(i), polygon.at(i+1), &e);
            //printf("escolhido: %d %d, %d %d, %d %d\n", polygon.at(i-1).x, polygon.at(i-1).y, polygon.at(i).x, polygon.at(i).y, polygon.at(i+1).x, polygon.at(i+1).y);
            inside = estaDentroTriangulo(polygon, polygon.at(i-1), polygon.at(i), polygon.at(i+1));
            //printf("INSIDE: %d\n", inside);
        }

        if (a_ptr != NULL && inside == false) {
            num_of_triangles++;
            Aresta a_obj{a_ptr->ini, a_ptr->fim};
            vector<Ponto> tri_vertices = {a_ptr->ini, polygon.at(i), a_ptr->fim};
            vector<Aresta> tri_arestas = {{a_ptr->ini, polygon.at(i)}, {polygon.at(i), a_ptr->fim}, {a_ptr->fim, a_ptr->ini}};
            
            tri_arestas.at(0).tri1 = num_of_triangles;
            tri_arestas.at(1).tri1 = num_of_triangles;
            tri_arestas.at(2).tri1 = num_of_triangles;
            tri_arestas.at(0).tri2 = 0;
            tri_arestas.at(1).tri2 = 0;
            tri_arestas.at(2).tri2 = 0;
            vector<Triangulo> tri_vizinhos;
            Triangulo tri{tri_vertices, tri_arestas, tri_vizinhos, num_of_triangles};
            triangulation.push_back(tri);
            //printf("\narestas:\n");
            //imprimeArestas(tri_arestas);
            //printf("\n");
            polygon.erase(polygon.begin()+i);
        }
        
        i++;
        
    }

    vector<Triangulo>::iterator it;
    vector<Aresta> all_edges;
    for (it = triangulation.begin(); it != triangulation.end(); ++it) {
        all_edges.push_back(it->arestas.at(0));
        all_edges.push_back(it->arestas.at(1));
        all_edges.push_back(it->arestas.at(2));
    }
    sort(all_edges.begin(), all_edges.end(), compare);

    calcularVizinhos(&all_edges);

    sort(all_edges.begin(), all_edges.end(), compare3);

    //for (int i = 0; i < 9; i++)
    //    printf("a: %d,%d // %d,%d // %d %d\n", all_edges.at(i).ini.x, all_edges.at(i).ini.y, all_edges.at(i).fim.x, all_edges.at(i).fim.y, all_edges.at(i).tri1, all_edges.at(i).tri2);
    
    int j = 0;
    // definindo qual aresta pertence a qual triangulo
    for (int i = 0; i < num_of_triangles; i++) {
        triangulation.at(i).arestas.at(0) = all_edges.at(j);
        j++;
        triangulation.at(i).arestas.at(1) = all_edges.at(j);
        j++;
        triangulation.at(i).arestas.at(2) = all_edges.at(j);
        j++;
    }

    // ordenando a lista de vertices e de arestas dos triangulos
    for (int i = 0; i < num_of_triangles; i++) {

        sort(triangulation.at(i).vertices.begin(), triangulation.at(i).vertices.end(), compare4);
        sort(triangulation.at(i).arestas.begin(), triangulation.at(i).arestas.end(), compare5);
    }

    // trocando as posições para que a saída esteja formatada corretamente
    for (int i = 0; i < num_of_triangles; i++) {
        Aresta aux = triangulation.at(i).arestas.at(0); 
        triangulation.at(i).arestas.at(0) = triangulation.at(i).arestas.at(1);
        triangulation.at(i).arestas.at(1) = triangulation.at(i).arestas.at(2);
        triangulation.at(i).arestas.at(2) = aux;
    }
    
    imprimeSaida(vertices_num, num_of_triangles, polygon_copy, triangulation);

	return 0;
}
