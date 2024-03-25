#include "Graph.h"

using namespace std;

typedef pair<int, int> edge;

class Solution {
    vector<edge> startingEdges;
    Graph* graph;

    void doCutsToGraph(vector<edge>&);
    void undoCutsToGraph(vector<edge>&);
    int objectiveFunction(vector<edge> &cc);
    bool notIn(vector<edge> &s, int i, int j);
    vector<vector<edge> > getNeighbourhood(vector<edge> &s);

    public: 
    Solution(vector<edge>&, Graph*);
    vector<edge> tabuSearch(int, int);
    void showSolution(vector<edge> &s);
};