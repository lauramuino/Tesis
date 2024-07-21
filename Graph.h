#include <iostream>
#include <stdlib.h>
#include "Map.h"

typedef vector<position> path;

struct Node
{
    vector<int> neighbours;
    position mapLocation;
    int value;
};

class Graph {
    int totalNodes;
    vector<Node> adjacencyList;
    vector<vector<int> > paths;

    public:
    Graph(Map &);
    ~Graph();
    bool isResource(int v){return v == 2;}
    bool isWalkable(int v){return v == 1;}
    void FloydWarshallWithPathReconstruction();
    path getMinPath(int u, int v);
};