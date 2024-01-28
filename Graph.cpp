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

   for (int v = 0; v < numVertices; v++) {
        bool notVisited = visited.find(v) == visited.end();

        if (notVisited) {
            std::vector<int> cc;
            std::deque<int> toVisit;
            toVisit.push_front(v);

            while(!toVisit.empty()) {
                int actual = toVisit.back();
                toVisit.pop_back();
                //if not already visited
                if(visited.find(actual) == visited.end()) {
                    visited.insert(actual);
                    cc.push_back(actual);
                    //add their non visited neighbours
                    for (int j = 0; j < numVertices; j++) {
                        if (neightbours(actual, j) && visited.find(j) == visited.end()) {
                            toVisit.push_front(j);
                        }
                    }
                }
            }

            //add new cc
            connectedComponents.push_back(cc);
        }
   }
   return connectedComponents;
}

bool Graph::neightbours(int i, int j) {
    return this->adjMatrix[i][j];
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