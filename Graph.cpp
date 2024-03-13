#include "Graph.h"
#include <iostream>
#include <deque>
#include <cmath>


Graph::Graph(int n, std::set<int> recursos) {
    resources = recursos;
    numVertices = n;
    adjMatrix = std::vector(n, std::vector(n,0)); //todo: check adjacency lists
}

Graph::~Graph() = default;

void Graph::addEdge(int v, int w) {
    adjMatrix[v][w] = 1;
    adjMatrix[w][v] = 1;
}

void Graph::removeEdge(int v, int w) {
    adjMatrix[v][w] = 0;
    adjMatrix[w][v] = 0;
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

std::vector<edge> Graph::getNeightbours(int i, int j) {
    std::vector<edge> res;

    for (int idx = 0; idx < numVertices; idx++)
    {
        if(idx!=j && neightbours(i, idx)) res.push_back(std::make_pair(i,idx));
    }
    
    for (int idx = 0; idx < numVertices; idx++)
    {
        if(idx!=i && neightbours(j, idx)) res.push_back(std::make_pair(j,idx));
    }

    return res;
}

bool Graph::isValidSolution(solution s, int countOfCC) {
    for (int i = 0; i < s.size(); i++) {
        removeEdge(s[i].first, s[i].second);
    }
    std::vector<std::vector<int> > cc = getConnectedComponents();
    
    //check if every cc has one resource
    bool everyCCHasResource = true;
    for (int i = 0; i < cc.size(); i++)
    {
        int j = 0;
        bool resourceFound = false;
        while (j< cc[i].size() && !resourceFound) {
            if (resources.find(cc[i][j]) != resources.end()) {
                resourceFound = true;
            }
            j++;
        }
        everyCCHasResource = everyCCHasResource && resourceFound;
    }
     ;
    for (int i = 0; i < s.size(); i++) {
        addEdge(s[i].first, s[i].second);
    }
    return (cc.size() == countOfCC) && everyCCHasResource;
}

int Graph::simulateCutAndCountConnectedComponentes(solution s) {
    for (int i = 0; i < s.size(); i++) {
        removeEdge(s[i].first, s[i].second);
    }
    int res = getConnectedComponents().size();
    for (int i = 0; i < s.size(); i++) {
        addEdge(s[i].first, s[i].second);
    }
    return res;
}

int Graph::getSquareDifferenceOfAreaOfConnectedComponentes(solution s) {
    for (int i = 0; i < s.size(); i++) {
        removeEdge(s[i].first, s[i].second);
    }
    
    std::vector<std::vector<int> > cc = getConnectedComponents();

    int meanArea = 0;
    int totalAreas = cc.size();
    for (int i = 0; i < totalAreas; i++)
    {
        int area = cc[i].size();
    }
    meanArea = meanArea/totalAreas;

    int res = 0;
    for (int i = 0; i < totalAreas; i++)
    {
        res = res + pow(meanArea - cc[i].size(), 2);
    }

    for (int i = 0; i < s.size(); i++) {
        addEdge(s[i].first, s[i].second);
    }
    return res;
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