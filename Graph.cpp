#include "Graph.h"
#include <iostream>
#include <deque>

Graph::Graph(int n, set<int> r)
{
    resourceBag = r;
    numVertices = n;
    adjMatrix = vector(n, vector(n,0));
}

Graph::~Graph() = default;

void Graph::addEdge(int v, int w)
{
    adjMatrix[v][w] = 1;
    adjMatrix[w][v] = 1;
}

void Graph::removeEdge(int v, int w)
{
    adjMatrix[v][w] = 0;
    adjMatrix[w][v] = 0;
}

vector<vector<int> > Graph::getConnectedComponents()
{
    set<int> visited;
    vector<vector<int> > connectedComponents;

   for (int v = 0; v < numVertices; v++) {
        bool notVisited = visited.find(v) == visited.end();

        if (notVisited) {
            vector<int> cc;
            deque<int> toVisit;
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

bool Graph::neightbours(int v, int w)
{
    return this->adjMatrix[v][w];
}

bool Graph::isResource(int v)
{
    return ( resourceBag.find(v) != resourceBag.end() );
}

vector<int> Graph::getNeighbours(int v)
{
    vector<int> res;

    for (int i = 0; i < numVertices; i++)
    {
        if ( neightbours(i, v)) res.push_back(i);
    }

    return res;
}

void Graph::showGraph()
{
    for (int i = 0; i < nodes(); ++i) {
        for (int j = 0; j < nodes(); ++j) {
            cout << adjMatrix[i][j];
        }
        cout << endl;
    }
    cout << endl;
}