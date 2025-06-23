#include <vector>

using namespace std;

typedef pair<int, int> position;
typedef vector<position> path;

class Map {
    vector<vector<int> > positions;
    vector<position> borderPositions;
    vector<position> resourcesPositions;
    vector<vector<position> > resourceClusters;
    int totalRows;
    int totalColumns;
    int totalResources;
    bool inRange(position);

    public:
    Map(ifstream &);
    ~Map();
    bool isBorder(int i, int j);
    bool isUnbuildable(position p) {return positions[p.first][p.second] == 0;}
    int resourceClustersCount() {return resourceClusters.size();}
    vector<vector<position> > getResourceClusters() {return resourceClusters;}
    void computeResourceClusters();
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