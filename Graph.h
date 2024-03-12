#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

typedef std::pair<int, int> edge;
typedef std::vector<edge> solution;

class Graph {
    int numVertices;
    std::vector<std::vector<int> > adjMatrix;
    bool neightbours(int, int);

    public:
    explicit Graph(int n);
    ~Graph();
    void addEdge(int v, int w);
    void removeEdge(int v, int w);
    bool neightbours(int v, int w);
    std::vector<edge> getNeightbours(int i, int j);
    std::vector<std::vector<int> > getConnectedComponents();
    void showGraph();
    int simulateCutAndCountConnectedComponentes(solution);
    int getSquareDifferenceOfAreaOfConnectedComponentes(solution);

    int nodes() { return numVertices; }
};


#endif //GRAPH_H
