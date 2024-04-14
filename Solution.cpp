#include <algorithm>
#include <limits>
#include <iostream>
#include <cmath>
#include "Solution.h"

Solution::Solution(vector<edge> &s, Graph* g)
{
    startingEdges = s;
    graph = g;
}

bool Solution::notIn(vector<edge> &s, int i, int j)
{
    edge e = make_pair(i, j);
    edge d = make_pair(j, i);

    vector<edge>::iterator it = s.begin();
    while(it != s.end())
    {
        if ((*it) == e || (*it) == d)
        {
            return false;
        }
        it++;
    }
    return true;
}

vector<vector<edge> > Solution::getNeighbourhood(vector<edge> &s)
{
    vector<vector<edge> > neighbours;

    // remove one edge if any
    for (int i = 0; i < s.size(); i++)
    {
        vector<edge> newSolution = s;
        vector<edge>::iterator it = newSolution.begin();
        newSolution.erase(it+i);
        if (newSolution.size() > 0) neighbours.push_back(newSolution);
    }
    
    // add one edge if any
    for (int i = 0; i < graph->nodes(); i++)
    {
        for (int j = i+1; j < graph->nodes(); j++) 
        {
            if (graph->neighbours(i, j) && notIn(s, i, j))
            {
                vector<edge> newSolution = s;
                newSolution.push_back(make_pair(i,j));
                neighbours.push_back(newSolution);
            }
        }
    }
    
    return neighbours;
}

int Solution::objectiveFunction(vector<edge> &s)
{
    doCutsToGraph(s);
    vector<vector<int> > cc = graph->getConnectedComponents();
    undoCutsToGraph(s);

    int ccWithoutResources = 0;
    int highestResourcesOnSameCC = 0;
    int totalArea = 0;
    vector<int> areas;
    for (int i = 0; i < cc.size(); i++)
    {
        totalArea += cc[i].size();
        areas.push_back(cc[i].size());
        int totalResources = 0;
        for (int j = 0; j < cc[i].size(); j++)
        {
            if (graph->isResource(cc[i][j]))  totalResources += 1;
        }
        if (totalResources > highestResourcesOnSameCC)  highestResourcesOnSameCC = totalResources;
        if (totalResources == 0) ccWithoutResources++; 
    }
    int meanArea = totalArea / cc.size();
    int leastSquaresArea = 0;
    for (int a = 0; a < cc.size(); a++)
    {
        leastSquaresArea += pow(meanArea-areas[a], 2);
    }
    
    int resourcesBalanced = highestResourcesOnSameCC - 1 + ccWithoutResources;
    // int separationLength = s.size();

    return leastSquaresArea + resourcesBalanced; //  + separationLength
}

vector<edge> Solution::tabuSearch(int maxIterations, int tabuListSize)
{
    vector<edge> bestSolution = startingEdges;
    vector<edge> currentSolution = startingEdges;
    vector<vector<edge> > tabu_list;
 
    for (int iter = 0; iter < maxIterations; iter++) {
        vector<vector<edge> > neighbours = getNeighbourhood(currentSolution);
        vector<edge> best_neighbour;
        int best_neighbour_fitness = numeric_limits<int>::max();
 
        for(int i = 0; i < neighbours.size(); i++) {
            if (find(tabu_list.begin(), tabu_list.end(), neighbours[i]) == tabu_list.end()) {
                int neighbour_fitness = objectiveFunction(neighbours[i]);
                if (neighbour_fitness < best_neighbour_fitness) {
                    best_neighbour = neighbours[i];
                    best_neighbour_fitness = neighbour_fitness;
                }
            }
        }
 
        if (best_neighbour.empty()) {
            // No non-tabu neighbours found, terminate the search
            break;
        }
 
        currentSolution = best_neighbour;
        tabu_list.push_back(best_neighbour);
        if (tabu_list.size() > tabuListSize) {
            // Remove the oldest entry from the
            // tabu list if it exceeds the size
            tabu_list.erase(tabu_list.begin());
        }
 
        if (objectiveFunction(best_neighbour) < objectiveFunction(bestSolution)) {
            // Update the best solution if the
            // current neighbour is better
            bestSolution = best_neighbour;
        }
    }
 
    return bestSolution;
}

void Solution::showSolution(vector<edge> &s)
{
    for (int i = 0; i < s.size(); i++)
    {
        cout <<"(" << s[i].first << ";" << s[i].second << ") ";
    }
    cout << endl;
}

void Solution::doCutsToGraph(vector<edge> &s)
{
    for (int i = 0; i < s.size(); i++) {
        graph->removeEdge(s[i].first, s[i].second);
    }
}

void Solution::undoCutsToGraph(vector<edge> &s)
{
    for (int i = 0; i < s.size(); i++) {
        graph->addEdge(s[i].first, s[i].second);
    }
}