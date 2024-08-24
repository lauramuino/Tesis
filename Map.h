#include <vector>
#include <utility>
#include <fstream>

using namespace std;

typedef pair<int, int> position;
typedef vector<position> path;

class Map {
    vector<vector<int> > positions;
    vector<pair<int,int> > borderPositions;
    int totalRows;
    int totalColumns;
    int totalResources;
    bool inRange(pair<int,int>);

    public:
    Map(ifstream &);
    ~Map();
    bool isBorder(int i, int j);
    int rows() {return totalRows;}
    int columns() {return totalColumns;}
    int resources() {return totalResources;}
    int at(int i, int j){return positions[i][j];}
    vector<position> getBorders();
    vector<position> getWalkableNeighbours(position);
    path getPathBetween(position, position);
};