#include "Map.h" //matrix
#include <unordered_map> //dictionary

typedef vector<path> solution;

class Graph {
    int totalNodes;
    std::unordered_map<int, vector<int> > adjacencyList;
    std::unordered_map<int, position> nodeToMapIndex; //indexes are node numbers, elements are map positions
    vector<position> removedCuts;

    void makeCuts(solution &s);
    void undoCuts(solution &s);
    void removeAllEdges(position);
    void removeEdge(int a, int b);
    void filterPathConnectedComponents(solution &s, vector<vector<int> > &connectsdComponents);
    bool isResource(int v){return v == 2;}
    bool isWalkable(int v){return v == 1;}
    vector<vector<int> > getConnectedComponents();

    public:
    Graph(Map &m);
    ~Graph();
    vector<vector<position> > getMapConnectedComponents(solution &s);
};