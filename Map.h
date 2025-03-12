#include <vector>
#include <utility>
#include <fstream>
#include <iostream>

using namespace std;

typedef pair<int, int> position;
typedef vector<position> path;

class Map {
    vector<vector<int> > positions;
    vector<position> borderPositions;
    vector<position> resourcesPositions;
    int totalRows;
    int totalColumns;
    int totalResources;
    bool inRange(position);
    vector<vector<position> > getResourceClusters();

    public:
    Map(ifstream &);
    ~Map();
    bool isBorder(int i, int j);
    int borders() {return borderPositions.size();}
    position getBorderAt(int i) {return borderPositions[i];}
    vector<position> getBorders() {return borderPositions;}
    vector<position> getResources() {return resourcesPositions;}
    int rows() {return totalRows;}
    int columns() {return totalColumns;}
    int resources() {return totalResources;}
    int at(int i, int j){return positions[i][j];}
    vector<position> getWalkableNeighbours(position);
    path getPathBetween(position, position);
    void drawSolution(vector<path>&, const char*);
};