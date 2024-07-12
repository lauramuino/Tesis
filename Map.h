#include <vector>
#include <fstream>

using namespace std;

typedef pair<int, int> position;

class Map {
    vector<vector<int> > positions;
    int totalRows;
    int totalColumns;
    int totalUnwakableTiles;
    void FloydWarshallWithPathReconstruction();

    public:
    Map(ifstream &);
    ~Map();
    bool isBorder(int i, int j);
    int rows() {return totalRows;}
    int columns() {return totalColumns;}
    int unwakableTiles() {return totalUnwakableTiles;}
    int at(int i, int j){return positions[i][j];}
    vector<position> getWalkableNeighbours(position);
};