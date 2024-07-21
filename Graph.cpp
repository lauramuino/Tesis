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
        vector<position> neighbours = m.getWalkableNeighbours(adjacencyList[k].mapLocation);
        for (int h = 0; h < neighbours.size(); h++)
        {
            int index = translation[neighbours[h]];
            adjacencyList[k].neighbours.push_back(index);
        }
    }
}

Graph::~Graph() = default;

void showMatrix(vector<vector<int> > &m)
{
    for (int i = 0; i < m.size(); i++)
    {
        for (int j = 0; j < m[0].size(); j++)
        {
            cout << m[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void Graph::FloydWarshallWithPathReconstruction()
{
    int maxDistance = this->totalNodes * 2;
    vector<vector<int> > distance(this->totalNodes, vector<int>(this->totalNodes, maxDistance));
    vector<vector<int> > previous(this->totalNodes, vector<int>(this->totalNodes, -1));
    for (int i = 0; i < this->totalNodes; i++) {
        distance[i][i] = 0;
        previous[i][i] = i;
        vector<int> edges = this->adjacencyList[i].neighbours;
        for (int j = 0; j < edges.size(); j++) {
            //edge i -> j
            distance[i][edges[j]] = 1;
            previous[i][edges[j]] = i;
        }
        
    }

    for (int k = 0; k < this->totalNodes; k++) {
        for (int i = 0; i < this->totalNodes; i++) {
            for (int j = 0; j < this->totalNodes; j++) {
                if (distance[i][j] > distance[i][k] + distance[k][j]) {
                    distance[i][j] = distance[i][k] + distance[k][j];
                    previous[i][j] = previous[k][j];
                }
            }
        }
    }

    this->paths = previous;
}


path Graph::getMinPath(int u, int v) //recieves graph indexes
{
    path res;
    if (this->paths[u][v] == 0) {
        return {};
    }

    res.insert(res.begin(), this->adjacencyList[v].mapLocation);
    int idx = v;

    while (idx != u) {
        idx = this->paths[u][idx];
        res.insert(res.begin(), this->adjacencyList[idx].mapLocation);
    }
    
    return res; //returns map indexes
}