#include "Graph.h"
#include <iostream>
using namespace std;


Graph::Graph(int n) {
    numVertices = n;
    adjMatrix = vector(n, vector(n,0)); //todo: check adjacency lists
}

Graph::~Graph() = default;

void Graph::addEdge(int v, int w) {
    adjMatrix[v][w] = 1;
    adjMatrix[w][v] = 1;
}

void Graph::getConnectedComponents() {  //todo: check and adapt
    // Mark all the vertices as not visited
//    bool* visited = bool[V];
//    for (int v = 0; v < numVertices; v++)
//        visited[v] = false;

//    for (int v = 0; v < numVertices; v++) {
//        if (visited[v] == false) {
//            // print all reachable vertices
//            // from v
//            DFSUtil(v, visited);
//
//            cout << "\n";
//        }
//    }
} //todo: hacer que funcione

void Graph::DFSUtil(int v, bool visited[]) {  //todo: check and adapt
    // Mark the current node as visited and print it
    visited[v] = true;
    cout << v << " ";

    // Recur for all the vertices
    // adjacent to this vertex
//    list<int>::iterator i;
//    for (i = adjLists[v].begin(); i != adjLists[v].end(); ++i)
//        if (!visited[*i])
//            DFSUtil(*i, visited);
}

void Graph::showGraph() { //todo: mejorar?
    for (int i = 0; i < nodes(); ++i) {
        for (int j = 0; j < nodes(); ++j) {
            std::cout << adjMatrix[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}