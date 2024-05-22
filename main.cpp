#include <iostream>
#include <utility>
#include "Solution.h"
#include "Map.h"

typedef pair<int, int> position;

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cout << "Missing path of input map." << endl;
        return EXIT_FAILURE;
    }
    
    ifstream mapFile;
    mapFile.open(argv[1]);
    
    vector<vector<int> > map;
    if (mapFile.is_open()) {
        //loading map
        Map map = Map(mapFile);
    } else {
        cout << "Unable to open map file.";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}