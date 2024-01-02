#include <iostream>
#include <fstream>
#include <vector>
#include "Graph.h"

int main() {

    // abro mapa de input
    std::ifstream mapFile;
    mapFile.open ("./maps/test1"); //todo: hardcodeado

    //cargo el mapa en matriz temporal //todo: ver si puedo mejorar esto
    int row = 0, nodes = 0, col = 0;
    std::vector<std::vector<int> > map;
    if (mapFile.is_open()) {
        std::string line;

        while ( getline (mapFile,line) )
        {
            col = 0;
            std::vector<int> empty(line.size(), 0);
            map.push_back(empty);
            for (char& c : line) {
                if (c == 'O') {
                    nodes++;
                    map[row][col] = nodes; //guardo el "numero" de nodo
                }
                col++;
            }
            row++;
        }
        mapFile.close();

//        std::cout << "cantidad de filas: " << row << std::endl;
//        std::cout << "cantidad de columnas: " << col << std::endl;
//        std::cout << "cantidad de nodos: " << nodes << std::endl;

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

    wakableTiles.showGraph();

    return 0;
}