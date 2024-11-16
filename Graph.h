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
    vector<position> nodeToMapIndex; //given a node number, get its corresponding position on the map
    set<int> resources;
    void makeCuts(solution &s);
    void undoCuts(solution &s);
    void removeEdge(int, int);
    void removeNeighbourAFromB(int a, int b);
    vector<vector<int> > getConnectedComponents();

    public:
    Graph(Map &m);
    ~Graph();
    bool isResource(int v){return v == 2;}
    bool isWalkable(int v){return v == 1;}
    vector<int> getInfoOfCutsMadeBy(solution &s);
};