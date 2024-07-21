#include <iostream>
#include <utility>
#include "Solution.h"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cout << "Missing path of input map." << endl;
        return EXIT_FAILURE;
    }
    
    ifstream mapFile;
    mapFile.open(argv[1]);
    
    vector<vector<int> > map;
    if (mapFile.is_open()) {
        Map mapa = Map(mapFile);
        Graph grafo = Graph(mapa);
        grafo.FloydWarshallWithPathReconstruction();
        path a = grafo.getMinPath(2,3);
        showPath(a);
        //puedo establecer en base a la # de recursos
        //cuantos cortes (o pares de nodos) necesito
        //como elijo los puntos iniciales?
        //como armo la vecindad?
        //como decido cual es mejor

    } else {
        cout << "Unable to open map file.";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}