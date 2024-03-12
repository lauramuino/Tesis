#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <set>

typedef std::pair<int, int> edge;
typedef std::vector<edge> solution;

class Graph {
    int numVertices;
    std::set<int> resources;
    std::vector<std::vector<int> > adjMatrix;


    public:
    explicit Graph(int, std::set<int>);
    ~Graph();
    void addEdge(int, int);
    void removeEdge(int, int);
    bool neightbours(int, int);
    std::vector<edge> getNeightbours(int, int);
    std::vector<std::vector<int> > getConnectedComponents();
    void showGraph();
    int simulateCutAndCountConnectedComponentes(solution);
    int getSquareDifferenceOfAreaOfConnectedComponentes(solution);
    bool isValidSolution(solution, int);

    int nodes() { return numVertices; }
};


#endif //GRAPH_H
