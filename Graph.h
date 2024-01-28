#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

class Graph {
    int numVertices;
    std::vector<std::vector<int> > adjMatrix;
    bool neightbours(int, int);

    public:
    explicit Graph(int n);
    ~Graph();
    void addEdge(int v, int w);
    std::vector<std::vector<int> > getConnectedComponents();
    void showGraph();

    int nodes() {
        return numVertices;
    }
};


#endif //GRAPH_H
