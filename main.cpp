#include <iostream>
#include <fstream>
#include <utility>
#include "TabuSearch.h"
#include <cstring>
#include <string>
#include <filesystem>

using namespace std;

string floatNumberToString(double number)
{
    string str = to_string(number);
    str.erase(str.find_last_not_of('0') + 1, string::npos);
    if (str.back() == '.') {
        str.pop_back();
    }
    return str;
}

string getOutputFilename(string inputFilePath, int maxIterations, int tabuListSize, double cutsThreshold, double lengthsThreshold)
{
    string mapName = inputFilePath.substr(inputFilePath.find_last_of("/"), inputFilePath.size() -1);

    string fileName = mapName + "_it" + to_string(maxIterations) + "_tabuListSize" + to_string(tabuListSize);
    fileName = fileName + "_cutsTh" + floatNumberToString(cutsThreshold) + "_lengthsTh" + floatNumberToString(lengthsThreshold);
    fileName = fileName + "_solution";

    string path = std::filesystem::current_path().string() + "/output";

    return path + fileName;
}

void printHelp()
{
    cout << "ERROR: MISSING PARAMETERS" << endl << endl;
    cout << "Expected arguments are:" << endl;
    cout << "   <map>:      path of input map"<< endl;
    cout << "   <maxIter>:  maximum iterations for Tabu Search"<< endl;
    cout << "   <listSize>: size of Tabu Search's list"<< endl;

    cout << "   (optional) <cutsThreshold>: threshold for cuts, default is 0.3" << endl;
    cout << "   (optional) <lengthsThreshold>: threshold for lengths, default is 1" << endl;
    cout << "   (optional) <inputFile>: path of starting solution, otherwise backtracking will be used" << endl;
}

int main(int argc, char* argv[]) {

    if (argc < 4) {
        printHelp();
        return EXIT_FAILURE;
    }
    
    ifstream mapFile;
    mapFile.open(argv[1]);
    
    if (mapFile.is_open()) {
        Map mapa = Map(mapFile);

        int maxIterations = atoi(argv[2]);
        int tabuListSize = atoi(argv[3]);

        double cutsThreshold, lengthsThreshold;
        if (argc == 6) {
            cutsThreshold = atof(argv[4]);
            lengthsThreshold = atof(argv[5]);
        } else {
            cutsThreshold = 0.3;
            lengthsThreshold = 1;
        }

        int neighbourDistance = 5; //TODO: parameters must be taken from file, too tedious to recompile every time

        string initialSolPath = (argc == 7) ? argv[6] : "";

        try {
            solution bestSolution = TabuSearch(maxIterations, tabuListSize, mapa, initialSolPath, cutsThreshold, lengthsThreshold, neighbourDistance).run();
            auto outputFilename = getOutputFilename(argv[1], maxIterations, tabuListSize, cutsThreshold, lengthsThreshold);
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