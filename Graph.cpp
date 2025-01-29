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
    auto it = find(nodeToMapIndex.begin(), nodeToMapIndex.end(), p);
    if (it!=nodeToMapIndex.end())
    {
        int index = it - nodeToMapIndex.begin();
        vector<int> aux = adjacencyList[index];
        for (int neighbour : aux)
        {
            removeEdge(index, neighbour);
            removedCuts.push_back(make_pair(index, neighbour));
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

void Graph::filterPathConnectedComponents(solution s, vector<vector<int> > &connectsdComponents)
{
    vector<int> ccToDelete;
    //recorro todos los caminos
    for (auto is = s.begin(); is != s.end(); is++)
    {
        path p = *is;
        //para cada camino, recorro sus posiciones (del map)
        for (auto ip = p.begin(); ip != p.end(); ip++)
        {
            position pos = *ip;
            auto it = find(nodeToMapIndex.begin(), nodeToMapIndex.end(), pos);
            //obtain index of position in the nodeToMapIndex array
            int nodeNumber = it - nodeToMapIndex.begin();
            ccToDelete.push_back(nodeNumber);
        }
    }

    //foreach element of ccTodelete, find its aparition in conectsdComponents and delete it
    for (int nodeNumber : ccToDelete)
    {
        for (auto it = connectsdComponents.begin(); it != connectsdComponents.end(); it++)
        {
            vector<int> cc = *it;
            auto it2 = find(cc.begin(), cc.end(), nodeNumber);
            if (it2 != cc.end())
            {
                connectsdComponents.erase(it);
                break;
            }
        }
    }
    
}

vector<int> Graph::getInfoOfCutsMadeBy(solution &s)
{
    makeCuts(s);
    vector<vector<int> > connectedComponents = getConnectedComponents();
    filterPathConnectedComponents(s, connectedComponents);

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
    meanArea = meanArea / countOfCC;

    int leastSquaresArea = 0;
    for (auto cc : connectedComponents)
    {
        leastSquaresArea += pow(meanArea-cc.size(), 2);
    }

    undoCuts(s);
    //resto los cortes del conteo de componentes conexas
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