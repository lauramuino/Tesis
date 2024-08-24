#include <algorithm>
#include <iostream>
#include <cmath>
#include "Solution.h"

bool noEstaEn(path p, solution &s)
{
    for (int i = 0; i < s.size(); i++)
    {
        bool notTheBegining = (p[0] != s[i][0]) && (p[p.size()-1] != s[i][0]);
        int lastIndexOfSAti = s[i].size()-1;
        bool notTheEnd = (p[0] != s[i][lastIndexOfSAti]) && (p[p.size()-1] != s[i][lastIndexOfSAti]);
        if (notTheBegining && notTheEnd) {
            return false;
        }
    }
    
    return true;
}

vector<path> cortesQueNoEstanEn(solution &s, Map &m) 
{
    vector<position> borders = m.getBorders();
    vector<path> cortes;
    for (int i = 0; i < borders.size(); i++)
    {
        for (int j = i+1; j < borders.size(); j++)
        {
            path nuevoCorte = m.getPathBetween(borders[i], borders[j]);
            if (noEstaEn(nuevoCorte, s))
            {
                cortes.push_back(nuevoCorte);
            }      
        }
    }
    return cortes;
}

bool caminosQueSeCruzan(path a, path b)
{
    for (int i = 0; i < a.size(); i++)
    {
        for (int j = 0; j < b.size(); j++)
        {
            if (a[i] == b[j]) {
                return true;
            }
        } 
    }
    return false;
}

bool noHayCruces(solution &s)
{
    for (int i = 0; i < s.size(); i++)
    {
        for (int j = i+1; j < s.size(); j++)
        {
            if (caminosQueSeCruzan(s[i], s[j]))
                return true;
        }
    }
    
    return false;
}

vector<solution> getNeighbourhood(solution &s, Map &map)
{ 
    vector<solution> neighbours;
    vector<path> cortes = cortesQueNoEstanEn(s, map);

    for (int i = 0; i < cortes.size(); i++)
    {
        for (int j = 0; j < s.size(); j++)
        {
            solution newSolution = s; 
            newSolution[j] = cortes[i];
            if (noHayCruces(newSolution))
                neighbours.push_back(newSolution);
        }
        
    }
    
    return neighbours;
}

int objectiveFunction(solution &s, Graph &grafo)
{
    int averageCutSizes = 0;
    for (int i = 0; i < s.size(); i++)
    {
        averageCutSizes += s[i].size();
    }
    averageCutSizes = averageCutSizes / s.size();
    
    vector<int> info = grafo.getInfoOfCutsMadeBy(s);
    int leastSquaresArea = info[0];
    int highestResourcesOnSameCC = info[1] ;
    int ccWithoutResources = info[2];

    int resourcesBalanced = highestResourcesOnSameCC - 1 + ccWithoutResources;

    return leastSquaresArea + resourcesBalanced + averageCutSizes;
}

solution tabuSearch(int maxIterations, int tabuListSize, Map &map, solution &initialSolution)
{
    solution bestSolution = initialSolution;
    solution currentSolution = initialSolution;
    vector<solution> tabu_list;
    Graph grafo = Graph(map);
 
    for (int iter = 0; iter < maxIterations; iter++) {
        vector<solution> neighbours = getNeighbourhood(currentSolution, map);
        solution best_neighbour;
        int best_neighbour_fitness = numeric_limits<int>::max();
 
        for(int i = 0; i < neighbours.size(); i++) {
            if (find(tabu_list.begin(), tabu_list.end(), neighbours[i]) == tabu_list.end()) {
                int neighbour_fitness = objectiveFunction(neighbours[i], grafo);
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
            // Remove the oldest entry from the tabu list if it exceeds the size
            tabu_list.erase(tabu_list.begin());
        }
 
        if (objectiveFunction(best_neighbour, grafo) < objectiveFunction(bestSolution, grafo)) {
            // Update the best solution if the current neighbour is better
            bestSolution = best_neighbour;
        }
    }
 
    return bestSolution;
}