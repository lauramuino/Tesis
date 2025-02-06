#include <iostream>
#include <utility>
#include "TabuSearch.h"
#include <cstring>
#include <string>
#include <filesystem>

using namespace std;

string getOutputFilename(string inputFilePath, int maxIterations, int tabuListSize)
{
    string mapName = inputFilePath.substr(inputFilePath.find_last_of("/"), inputFilePath.size() -1);
    string path = std::filesystem::current_path().string() + "/output" + mapName;
    string fileName = path + "_it" + to_string(maxIterations) + "_size" + to_string(tabuListSize) + "_solution";
    return fileName;
}

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
            solution bestSolution = TabuSearch(maxIterations, tabuListSize, mapa, initialSolPath).run();
            auto outputFilename = getOutputFilename(argv[1], maxIterations, tabuListSize);
            mapa.drawSolution(bestSolution, outputFilename.c_str());
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