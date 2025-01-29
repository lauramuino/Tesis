#include <iostream>
#include <utility>
#include "Solution.h"
#include <cstring>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {

    if (argc < 4) {
        cout << "Missing parameters. The arguments are:" << endl;
        cout << "<map>: path of input map"<< endl;
        cout << "<maxIter>: maximum iterations for Tabu Search"<< endl;
        cout << "<listSize>: size of Tabu Search's list"<< endl;

        cout << "(optional) <inputFile>: path of starting solution, otherwise backtracking will be used" << endl;
        return EXIT_FAILURE;
    }
    
    ifstream mapFile;
    mapFile.open(argv[1]);
    
    if (mapFile.is_open()) {
        Map mapa = Map(mapFile);

        int maxIterations = atoi(argv[2]);
        int tabuListSize = atoi(argv[3]);
        string initialSolPath = (argc == 5) ? argv[4] : "";

        try
        {
            solution bestSolution = tabuSearch(maxIterations, tabuListSize, mapa, initialSolPath);
            string fileName = argv[1]; 
            fileName = fileName + "_it" + to_string(maxIterations) + "_size" + to_string(tabuListSize) + "_solution";
            const char* newFileName = fileName.c_str();
            mapa.drawSolution(bestSolution, newFileName);
        }
        catch(const std::exception& e)
        {
            cout << e.what() << '\n';
        }
        
    } else {
        cout << "Unable to open map file.";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}