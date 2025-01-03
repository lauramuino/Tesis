#include <iostream>
#include <utility>
#include "Solution.h"
#include <cstring>
#include <string>

using namespace std;

void printSolution(solution& s)
{
    for (auto path: s)
    {
        for (auto position: path)
        {
            cout << "(" << position.first << ", " << position.second << ") ";
        }
        cout  << endl;
    }
    cout << endl;
}

int main(int argc, char* argv[]) {

    if (argc < 3) {
        cout << "Missing parameters. The arguments are:" << endl;
        cout << "<map>: path of input map"<< endl;
        cout << "<maxIter>: maximum iterations for Tabu Search"<< endl;
        cout << "<listSize>: size of Tabu Search's list"<< endl;
        return EXIT_FAILURE;
    }
    
    ifstream mapFile;
    mapFile.open(argv[1]);
    
    if (mapFile.is_open()) {
        Map mapa = Map(mapFile);

        int maxIterations = atoi(argv[2]);
        int tabuListSize = atoi(argv[3]);
        solution bestSolution = tabuSearch(maxIterations, tabuListSize, mapa);

        printSolution(bestSolution);

        string fileName = argv[1]; 
        fileName = fileName + "_solution";
        const char* newFileName = fileName.c_str();
        mapa.drawSolution(bestSolution, newFileName);
    } else {
        cout << "Unable to open map file.";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}