#include <iostream>
#include <fstream>
#include <utility>
#include "TabuSearch.h"


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
    std::set<int> recursos;

    if (mapFile.is_open()) {
        std::string line;

        while ( getline (mapFile,line) )
        {
            col = 0;
            std::vector<int> empty(line.size(), 0);
            map.push_back(empty);
            for (char& c : line) {
                if (esCaminable(c) || esRecurso(c)) {
                    nodes++;
                    map[row][col] = nodes; //guardo el "numero" de nodo
                    if (esRecurso(c)) {
                        recursos.insert(nodes); //lista para distinguir los que son recursos
                    }
                }
                col++;
            }
            row++;
        }
        mapFile.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    // mapa de input a grafo
    Graph wakableTiles(nodes, recursos);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            //chequeo de vecino arriba
            if (map[i][j] > 0 && i > 0 && map[i-1][j] > 0) {
                wakableTiles.addEdge(map[i][j] -1, map[i-1][j] -1); //los nodos se cuentan desde uno, porque las posiciones 0 representa una parte de mapa no caminable
            }
            //chequeo de vecino izquierda
            if (map[i][j] > 0 && j > 0 && map[i][j-1] > 0) {
                wakableTiles.addEdge(map[i][j] -1, map[i][j-1] -1);
            }
        }
    }


    // corte inicial hardcodeado ..
    std::vector<std::pair<int,int> > initial_solution = {std::make_pair(4-1,6-1), std::make_pair(6-1,7-1)};

    //tabu search
    solution best = tabu_search(initial_solution, 50, 100, &wakableTiles);
    
    showSolution(best);

    return 0;
}