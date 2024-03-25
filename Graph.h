#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <set>

using namespace std;

class Graph {
    int numVertices;
    set<int> resourceBag;
    vector<vector<int> > adjMatrix;

    public:
    Graph(int, set<int>);
    ~Graph();
    void addEdge(int, int);
    void removeEdge(int, int);
    bool neighbours(int, int);
    bool isResource(int);
    vector<int> getNeighbours(int);
    vector<vector<int> > getConnectedComponents();
    void showGraph();
    int nodes() { return numVertices; }
    int resources() { return resourceBag.size(); }
};


#endif //GRAPH_H