#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

class Graph {
    int numVertices;
    std::vector<std::vector<int> > adjMatrix;
    void DFSUtil(int v, bool visited[]);

    public:
    explicit Graph(int n);
    ~Graph();
    void addEdge(int v, int w);
    void getConnectedComponents();
    void showGraph();

    int nodes() {
        return numVertices;
    }
};


#endif //GRAPH_H
