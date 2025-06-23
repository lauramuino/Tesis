#include <algorithm>
#include <iostream>
#include <utility>
#include <set>
#include <deque>
#include <cmath>
#include <stdlib.h>
#include "Graph.h"

using namespace std;

Graph::~Graph() = default;

Graph::Graph(Map &m)
{
    totalNodes = 0;
    //initial structure, without neighbour data
    for (int i = 0; i < m.rows(); i++)
    {
        for (int j = 0; j < m.columns(); j++)
        {
            int value = m.at(i, j);
            if (isResource(value) || isWalkable(value))
            {
                nodeToMapIndex[totalNodes] = make_pair(i, j); 
                totalNodes++;
            }
        }
    }

    //filling with neighbour info
    for (int k = 0; k < totalNodes; k++)
    {
        vector<int> indexNeighbours;
        vector<position> neighbours = m.getWalkableNeighbours(nodeToMapIndex[k]);
        for (position neighbour : neighbours)
        {
            for (auto it = nodeToMapIndex.begin(); it != nodeToMapIndex.end(); ++it) {
                if (it->second == neighbour) {
                    indexNeighbours.push_back(it->first); //push index
                }
            }
        }
        adjacencyList[k] = indexNeighbours;
    }

    //there may be some isolated nodes that must be remove
    vector<vector<int> > connectedComponents = getConnectedComponents();
    for (auto cc : connectedComponents) {
        //find if there are non border elements
        int counter = 0;
        for (auto node: cc) {
            position p = nodeToMapIndex[node];
            if (m.isBorder(p.first, p.second)) {
                counter++;
            } else {
                break;
            }
        }

        //if there aren't, delete cc from graph
        if (counter == cc.size()) {
            for (auto node: cc) {
                totalNodes--;
                nodeToMapIndex.erase(node);
                adjacencyList.erase(node);
            }
        }
    }
    vector<vector<int> > connectedComponentss = getConnectedComponents();
}

void Graph::removeEdge(int a, int b)
{
    auto itb = find(adjacencyList[b].begin(), adjacencyList[b].end(), a);
    if (itb != adjacencyList[b].end())
    {
        adjacencyList[b].erase(itb);
    }
    auto ita = find(adjacencyList[a].begin(), adjacencyList[a].end(), b);
    if (ita != adjacencyList[a].end())
    {
        adjacencyList[a].erase(ita);
    }
}

void Graph::removeAllEdges(position p)
{
    for (auto it = nodeToMapIndex.begin(); it != nodeToMapIndex.end(); ++it)
    {
        if (it->second == p) {
            int index = it->first;
            vector<int> aux = adjacencyList[index];
            for (int neighbour : aux)
            {
                removeEdge(index, neighbour);
                removedCuts.push_back(make_pair(index, neighbour));
            }
        }
    }
}

void Graph::makeCuts(solution &s)
{
    for (path cut : s)
    {
        for (position p : cut)
        {
            removeAllEdges(p);
        }
    }
}

void Graph::undoCuts(solution &s)
{
    for (auto cut : removedCuts)
    {
        int a = cut.first;
        int b = cut.second;
        adjacencyList[a].push_back(b);
        adjacencyList[b].push_back(a);
    }
    removedCuts.clear();
}

void Graph::filterPathConnectedComponents(solution &s, vector<vector<int> > &connectedComponents) {
    // count total positions in s
    int ccToDelete = 0;
    for (path camino : s) {
        ccToDelete += camino.size();
    }

    // delete as much cc's with one element as total positions in s
    auto it = connectedComponents.begin();
    while (ccToDelete > 0 && it != connectedComponents.end()) {
        if ((*it).size() == 1) {
            connectedComponents.erase(it);
            ccToDelete--;
        } else {
            it++;
        }
    }
}

vector<vector<position> > Graph::getMapConnectedComponents(solution &s)
{
    makeCuts(s);
    vector<vector<int> > connectedComponents = getConnectedComponents();
    filterPathConnectedComponents(s, connectedComponents);

    vector<vector<position> > result;
    for (auto cc : connectedComponents) {
        vector<position> ccOfMap;
        for (int c : cc) {
            ccOfMap.push_back(nodeToMapIndex[c]);
        }
        result.push_back(ccOfMap); 
    }

    undoCuts(s);
    return result;
}

vector<vector<int> > Graph::getConnectedComponents()
{
    set<int> visited;
    vector<vector<int> > connectedComponents;

    for (auto it = adjacencyList.begin(); it != adjacencyList.end(); ++it) {
        int node = it->first;
        bool notVisited = visited.find(node) == visited.end();

        if (notVisited) {
            vector<int> cc;
            deque<int> toVisit;
            toVisit.push_front(node);
            while(!toVisit.empty()) {
                int actual = toVisit.back();
                toVisit.pop_back();
                //if not already visited
                if(visited.find(actual) == visited.end()) {
                    visited.insert(actual);
                    cc.push_back(actual);
                    //add their non visited neighbours
                    for (int neighbour : adjacencyList[actual]) {
                        if (visited.find(neighbour) == visited.end()) {
                            toVisit.push_front(neighbour);
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