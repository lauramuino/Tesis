#include <iostream>
#include <stdlib.h>
#include "Map.h"

struct Node
{
    vector<int> neighbours;
    position mapLocation;
    int value;
};

class Graph {
    int totalNodes;
    vector<Node> adjacencyList;

    public:
    Graph(Map &);
    ~Graph();
};