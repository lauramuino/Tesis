#include <algorithm>
#include <cmath>
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
                if (isResource(value)) resources.insert(totalNodes);
                
                position p =  make_pair(i, j);
                nodeToMapIndex.push_back(p); 
                totalNodes++;
            }
        }
    }

    //filling with neighbour info
    for (int k = 0; k < totalNodes; k++)
    {
        vector<int> aux;
        vector<position> neighbours = m.getWalkableNeighbours(nodeToMapIndex[k]);
        for (position neighbour : neighbours)
        {
            auto it = find(nodeToMapIndex.begin(), nodeToMapIndex.end(), neighbour);
            if (it != std::end(nodeToMapIndex)) {
               int index = it - nodeToMapIndex.begin();
                aux.push_back(index);
            }
        }
        adjacencyList.push_back(aux);
    }
}

void Graph::removeNeighbourAFromB(int a, int b)
{
    auto it = find(adjacencyList[b].begin(), adjacencyList[b].end(), a);
    if (it != adjacencyList[b].end())
    {
        adjacencyList[b].erase(it);
    }
}

void Graph::removeEdge(int a, int b)
{
    removeNeighbourAFromB(a, b);
    removeNeighbourAFromB(b, a);
}

void removeDiagonal(position p_a, position p_b, int a, int b)
{
    bool sameHorizontal = p_a.first == p_b.first;
    bool sameVertical = p_a.second == p_b.second;

    if (!sameHorizontal && !sameVertical)
    {
        /* it's a diagonal, should remove also de inverted one */

    }
    
}

void Graph::makeCuts(solution &s)
{
    for (int i = 0; i < s.size(); i++)
    {
        for (int j = 0; j < s[i].size()-1; j++)
        {
            auto it = find(nodeToMapIndex.begin(), nodeToMapIndex.end(), s[i][j]);
            if (it != nodeToMapIndex.end())
            {
                pair init = s[i][j]; //coordenadas en el mapa
                pair end = s[i][j+1];

                int a = it - nodeToMapIndex.begin();
                it = find(nodeToMapIndex.begin(), nodeToMapIndex.end(), end);
                if (it != nodeToMapIndex.end())
                {
                    int b = it - nodeToMapIndex.begin();
                    removeEdge(a, b);
                    removedCuts.push_back(make_pair(a, b));
                   // remove inverse diagonal if there's one
                    bool sameHorizontal = init.first == end.first;
                    bool sameVertical = init.second == end.second;
                    if (!sameHorizontal && !sameVertical)
                    {
                        pair diagInit = make_pair(init.first, end.second);
                        pair diagEnd = make_pair(end.first, init.second);
                        it = find(nodeToMapIndex.begin(), nodeToMapIndex.end(), diagInit);
                        if (it != nodeToMapIndex.end())
                        {
                            int c = it - nodeToMapIndex.begin();
                            it = find(nodeToMapIndex.begin(), nodeToMapIndex.end(), diagEnd);
                            if (it != nodeToMapIndex.end())
                            {
                                int d = it - nodeToMapIndex.begin();
                                removeEdge(c, d);
                                removedCuts.push_back(make_pair(c, d));
                            }
                        }
                    }
                }
            }
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

vector<int> Graph::getInfoOfCutsMadeBy(solution &s)
{
    makeCuts(s);
    vector<vector<int> > connectedComponents = getConnectedComponents();
    int meanArea = 0, highestResourcesOnSameCC = 0, ccWithoutResources = 0;
    int countOfCC = connectedComponents.size();
    for (auto cc : connectedComponents) {
        meanArea += cc.size();
        int cantResources = 0;
        for (int c : cc) {
            if (resources.find(c) != resources.end()) cantResources++;
        }
        if (highestResourcesOnSameCC < cantResources) highestResourcesOnSameCC = cantResources;
        if (cantResources == 0) ccWithoutResources++;
        
    }
    meanArea = meanArea / connectedComponents.size();

    int leastSquaresArea = 0;
    for (auto cc : connectedComponents)
    {
        leastSquaresArea += pow(meanArea-cc.size(), 2);
    }

    undoCuts(s);
    return {leastSquaresArea, highestResourcesOnSameCC, ccWithoutResources, countOfCC};
}

vector<vector<int> > Graph::getConnectedComponents()
{
    set<int> visited;
    vector<vector<int> > connectedComponents;

   for (int v = 0; v < totalNodes; v++) {
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