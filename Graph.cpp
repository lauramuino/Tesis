#include <map>
#include "Graph.h"

using namespace std;

Graph::Graph(Map & m)
{
    totalNodes = m.rows() * m.columns() - m.unwakableTiles();
    int nodeCount = 0;
    map<position, int> translation; //given a position on the map, i save the corresponding node on the graph
    
    //initial structure, without neighbour data
    for (int i = 0; i < m.rows(); i++)
    {
        for (int j = 0; j < m.columns(); j++)
        {
            int value = m.at(i, j);
            if (isResource(value) || isWalkable(value))
            {
                Node n;
                position p =  make_pair(i, j);
                translation[p] = nodeCount; 
                n.mapLocation = p;
                n.value = value;
                adjacencyList.push_back(n);
                nodeCount++;
            }
        }
    }

    if (nodeCount != totalNodes) {
        cout << "Error at Graph initialization: number of nodes mismatch" << endl;
        exit(EXIT_FAILURE);
    }
     
    //filling with neighbour info
    for (int k = 0; k < totalNodes; k++)
    {
        Node n = adjacencyList[k];
        vector<position> neighbours = m.getWalkableNeighbours(n.mapLocation);
        for (int h = 0; h < neighbours.size(); h++)
        {
            int index = translation[neighbours[h]];
            n.neighbours.push_back(index);
        }   
    }
}

Graph::~Graph() = default;
