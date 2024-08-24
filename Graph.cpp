#include <algorithm>
#include <cmath>
#include "Graph.h"

using namespace std;

Graph::~Graph() = default;

Graph::Graph(Map &m)
{
    int nodeCount = 0;
    vector<position> translation; //given a node number, get its corresponding position on the map
    
    //initial structure, without neighbour data
    for (int i = 0; i < m.rows(); i++)
    {
        for (int j = 0; j < m.columns(); j++)
        {
            int value = m.at(i, j);
            if (isResource(value) || isWalkable(value))
            {
                if (isResource(value)) resources.insert(nodeCount);
                
                position p =  make_pair(i, j);
                translation.push_back(p); 
                nodeCount++;
            }
        }
    }
    nodeToMapIndex = translation;
    totalNodes = nodeCount;
     
    //filling with neighbour info
    for (int k = 0; k < totalNodes; k++)
    {
        vector<position> neighbours = m.getWalkableNeighbours(translation[k]);
        for (position neighbour : neighbours)
        {
            auto it = find(translation.begin(), translation.end(), neighbour);
            if (it != std::end(translation)) {
               int index = it - translation.begin();
                adjacencyList[k].push_back(index);
            }
        }
    }
}

void Graph::removeNeighbourAFromB(int a, int b)
{
    for (int i = 0; i < adjacencyList[b].size(); i++)
    {
        auto it = find(adjacencyList[b].begin(), adjacencyList[b].end(), a);
        if (it != adjacencyList[b].end())
        {
            adjacencyList[b].erase(it);
        }
    }
}

void Graph::removeEdge(int a, int b)
{
    removeNeighbourAFromB(a, b);
    removeNeighbourAFromB(b, a);
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
                int a = it - nodeToMapIndex.begin();
                it = find(nodeToMapIndex.begin(), nodeToMapIndex.end(), s[i][j+1]);
                if (it != nodeToMapIndex.end())
                {
                    int b = it - nodeToMapIndex.begin();
                    removeEdge(a, b);
                }
            }
        }
    }
}

void Graph::undoCuts(solution &s)
{
    for (int i = 0; i < s.size(); i++)
    {
        for (int j = 0; j < s[i].size()-1; j++)
        {
             auto it = find(nodeToMapIndex.begin(), nodeToMapIndex.end(), s[i][j]);
            if (it != nodeToMapIndex.end())
            {
                int a = it - nodeToMapIndex.begin();
                it = find(nodeToMapIndex.begin(), nodeToMapIndex.end(), s[i][j+1]);
                if (it != nodeToMapIndex.end())
                {
                    int b = it - nodeToMapIndex.begin();
                    adjacencyList[a].push_back(b);
                    adjacencyList[b].push_back(a);
                }
            }
        }
    }
}

vector<int> Graph::getInfoOfCutsMadeBy(solution &s)
{
    makeCuts(s);
    vector<vector<int> > connectedComponents = getConnectedComponents();
    int meanArea = 0, highestResourcesOnSameCC = 0, ccWithoutResources = 0;
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
    return {leastSquaresArea, highestResourcesOnSameCC, ccWithoutResources};
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