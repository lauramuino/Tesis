#include "Graph.h"

using namespace std;

typedef pair<int, int> edge;

class Solution {
    vector<edge> startingEdges;
    Graph* graph;

    void doCutsToGraph(vector<edge>&);
    void undoCutsToGraph(vector<edge>&);
    int objectiveFunction(vector<edge> &cc);
    bool notInCC(int node, vector<int> &cc);
    void removeOldEdges(vector<edge> &cc, int);

    public: 
    Solution(vector<edge>&, Graph*);
    vector<edge> tabuSearch(int, int);
    vector<vector<edge> > getNeighbourhood(vector<edge> &s);
    bool isValidCut(vector<edge> &s );
    void showSolution(vector<edge> &s);
};