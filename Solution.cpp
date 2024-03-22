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

bool Solution::notInCC(int node, vector<int> &cc)
{
    for (int i = 0; i < cc.size(); i++)
    {
        if (cc[i] == node)
        {
            return false;
        }
    }
    return true;
}

void Solution::removeOldEdges(vector<edge> &cc, int node)
{
    vector<edge>::iterator it = cc.begin();

    while(it != cc.end())
    {
        if((*it).first == node || (*it).second == node){
            it = cc.erase(it);
        } else {
            ++it;
        } 
    }
}

vector<vector<edge> > Solution::getNeighbourhood(vector<edge> &s)
{
    vector<vector<edge> > neighbours;
    doCutsToGraph(s);
    vector<vector<int> > cc = graph->getConnectedComponents();
    undoCutsToGraph(s);

    // for every cc, we add add a neighbouring node from other cc when possible
    for (int i = 0; i < cc.size(); i++)
    {
        for (int j = 0; j < cc[i].size(); j++)
        {
            vector<int> nodes = graph->getNeighbours(cc[i][j]);
            //we want the ones outside de cc that are not resources
            for (int k = 0; k < nodes.size(); k++)
            {
                bool isNotResource = !graph->isResource(nodes[k]);
                bool kNotInCC_i = notInCC(nodes[k], cc[i]);
                if (isNotResource && kNotInCC_i)
                {
                    vector<edge> newSolution = startingEdges;
                    //delete nodes[k] edges
                    removeOldEdges(newSolution, nodes[k]);//problema: puede generar mas cc de las deseadas
                    //add node with cc[i][j] 
                    newSolution.push_back(make_pair(cc[i][j], nodes[k])); //problema: puede que genere un ciclo y pierda componentes conexas
                    
                    bool isNotIncluded = find(neighbours.begin(), neighbours.end(), newSolution) == neighbours.end();
                    //if solution generates n-1 cc, with a resource in each, save solution
                    if (isNotIncluded && isValidCut(newSolution)) {
                        neighbours.push_back(newSolution);
                    }
                }
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
    for (int i = 0; i < cc.size(); i++)
    {
        int totalResources = 0;
        for (int j = 0; j < cc[i].size(); j++)
        {
            if (graph->isResource(cc[i][j]))
            {
               totalResources += 1;
            }
        }
        if (totalResources > highestResourcesOnSameCC)
        {
            highestResourcesOnSameCC = totalResources;
        }
        if (totalResources == 0)
        {
            ccWithoutResources++;
        }
    }
    return highestResourcesOnSameCC - 1 + ccWithoutResources;
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
        if (tabu_list.size() > tabu_list_size) {
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

bool Solution::isValidCut(vector<edge> &s)
{
    doCutsToGraph(s);
    vector<vector<int> > cc = graph->getConnectedComponents();
    undoCutsToGraph(s);
    
    //check if every cc has one resource
    bool everyCCHasResource = true;
    for (int i = 0; i < cc.size(); i++) {
        int j = 0;
        bool resourceFound = false;
        while (j < cc[i].size() && !resourceFound) {
            resourceFound = graph->isResource(cc[i][j]);
            j++;
        }
        everyCCHasResource = everyCCHasResource && resourceFound;
    }
    int wantedCCCount = graph->resources();
    int ccCount = cc.size();
    //debug 
    if (ccCount != wantedCCCount)
    {
        cout << "Se generaron " << ccCount << "cc's en lugar de " << wantedCCCount << endl;
    }
    

    return (ccCount == wantedCCCount) && everyCCHasResource;
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