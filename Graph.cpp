#include "Graph.h"
#include <iostream>
#include <set>
#include <deque>


Graph::Graph(int n) {
    numVertices = n;
    adjMatrix = std::vector(n, std::vector(n,0)); //todo: check adjacency lists
}

Graph::~Graph() = default;

void Graph::addEdge(int v, int w) {
    adjMatrix[v][w] = 1;
    adjMatrix[w][v] = 1;
}

std::vector<std::vector<int> > Graph::getConnectedComponents() {
    std::set<int> visited;
    std::vector<std::vector<int> > connectedComponents;
    std::vector<int> cc;
    connectedComponents.push_back(cc);

   for (int v = 0; v < numVertices; v++) {
        bool notVisited = visited.find(v) == visited.end();
        if (notVisited) {
            std::deque<int> toVisit;
            toVisit.push_front(v);
            cc.push_back(v);

            while(!toVisit.empty()) {
                int actual = toVisit.back();
                toVisit.pop_back();
                visited.insert(actual);
                //add their non visited neighbours
                for (int j = 0; j<numVertices && j!=actual && this->adjMatrix[actual][j] && visited.find(j) == visited.end(); j++) {
                    toVisit.push_front(j);
                    cc.push_back(j);
                }
            }

            //if is not the end, add new cc
            if (v < numVertices-1) {
                std::vector<int> aux;
                cc = aux;
                connectedComponents.push_back(cc);
            }
        }
   }
   return connectedComponents;
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