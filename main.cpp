#include <iostream>
#include <utility>
#include "Solution.h"

using namespace std;

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cout << "Missing parameters. The arguments are:" << endl;
        cout << "<map>" << endl;
        cout << "<map>: path of input map"<< endl;
        return EXIT_FAILURE;
    }
    
    ifstream mapFile;
    mapFile.open(argv[1]);
    
    vector<vector<int> > map;
    if (mapFile.is_open()) {
        Map mapa = Map(mapFile);
        vector<path> initialSolution;
        
    } else {
        cout << "Unable to open map file.";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}