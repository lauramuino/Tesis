#include <vector>
#include <fstream>

using namespace std;

class Map {
    vector<vector<int> > positions;

    public:
    Map(ifstream &);
    bool isBorder(int i, int j);
};