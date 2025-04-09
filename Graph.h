#include <iostream>
#include <stdlib.h>
#include <utility>
#include <set>
#include <deque>
#include "Map.h"

typedef vector<path> solution;

class Graph {
    int totalNodes;
    vector<vector<int> > adjacencyList;
    vector<position> nodeToMapIndex; //indexes are node numbers, elements are map positions
    vector<pair<int, int> > removedCuts;
    void makeCuts(solution &s);
    void undoCuts(solution &s);
    void removeAllEdges(position);
    void removeEdge(int a, int b);
    vector<vector<int> > getConnectedComponents();
    void filterPathConnectedComponents(solution s, vector<vector<int> > &connectsdComponents);

    public:
    Graph(Map &m);
    ~Graph();
    bool isResource(int v){return v == 2;}
    bool isWalkable(int v){return v == 1;}
    vector<vector<position> > getMapConnectedComponents(solution &s);
};