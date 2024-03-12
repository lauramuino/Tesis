#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include "Graph.h"


//////////////////FUNCIONES AUXILIARES

bool esCaminable(char &c)
{
    // buildable or walkable, for now
    return c == '1';
}

bool esRecurso (char &c)
{
    return c == '2';
}

//////////////////FIN FUNCIONES AUXILIARES

int main() {

    // abro mapa de input
    std::ifstream mapFile;
    mapFile.open ("./maps/test2"); //todo: hardcodeado

    //cargo el mapa en matriz temporal
    int row = 0, nodes = 0, col = 0;
    std::vector<std::vector<int> > map;
    std::vector<std::pair<int, int> > recursos;

    if (mapFile.is_open()) {
        std::string line;

        while ( getline (mapFile,line) )
        {
            col = 0;
            std::vector<int> empty(line.size(), 0);
            map.push_back(empty);
            for (char& c : line) {
                if (esCaminable(c)) {
                    nodes++;
                    map[row][col] = nodes; //guardo el "numero" de nodo
                } else if (esRecurso(c)) {
                    nodes++;
                    map[row][col] = nodes;
                    //lo mismo pero los tengo en una lista para distinguirlos
                    recursos.push_back(std::make_pair(row,col));
                }
                col++;
            }
            row++;
        }
        mapFile.close();

       std::cout << "cantidad de filas: " << row << std::endl;
       std::cout << "cantidad de columnas: " << col << std::endl;
       std::cout << "cantidad de nodos: " << nodes << std::endl;

    } else {
        std::cout << "Unable to open file";
        return 0;
    }

    // mapa de input a grafo
    Graph wakableTiles(nodes);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            //chequeo de vecino arriba
            if (map[i][j] > 0 && i > 0 && map[i-1][j] > 0) {
                wakableTiles.addEdge(map[i][j] -1, map[i-1][j] -1);
            }
            //chequeo de vecino izquierda
            if (map[i][j] > 0 && j > 0 && map[i][j-1] > 0) {
                wakableTiles.addEdge(map[i][j] -1, map[i][j-1] -1);
            }
        }
    }


    //por ahora, el corte hardcodeado ..
    wakableTiles.removeEdge(0,4);

    //tabu search

    

    return 0;
}