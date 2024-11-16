#include <algorithm>
#include <iostream>
#include <cmath>
#include <numeric>
#include <ctime>
#include "Solution.h"

bool corteEstaEnSolucion(path &cutA, solution &solucion)
{
    int lastIndexOfCutA = cutA.size() - 1;
    for (path cutB : solucion)
    {
        int lastIndexOfCutB = cutB.size() - 1;
        bool isTheSameCut = (cutA[0] == cutB[0]) && (cutA[lastIndexOfCutA] == cutB[lastIndexOfCutB]);
        bool isReversedCut = (cutA[0] == cutB[lastIndexOfCutB]) && (cutA[lastIndexOfCutA] == cutB[0]);
        if (isTheSameCut || isReversedCut) {
            return true;
        }
    }
    
    return false;
}

vector<path> cortesQueNoEstanEn(solution &s, Map &m) 
{
    vector<path> cortes;
    for (int i = 0; i < m.borders(); i++)
    {
        for (int j = i+1; j < m.borders(); j++)
        {
            path nuevoCorte = m.getPathBetween(m.getBorderAt(i), m.getBorderAt(j));
            if (!corteEstaEnSolucion(nuevoCorte, s))
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

bool noHayCruces(solution &s) //todo: no funciona como deberia
{
    for (int i = 0; i < s.size(); i++)
    {
        for (int j = i+1; j < s.size(); j++)
        {
            if (caminosQueSeCruzan(s[i], s[j]))
                return false;
        }
    }
    
    return true;
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

solution buildInitialSolution(Map &m)
{
    int nededCuts = m.resources()-1;
    solution initialSolution;

    //vector con lista de indices de ptos borde
    vector<int> bordersIndex(m.borders());
    //colocar numeros del 0 a m.borders-1
    iota(bordersIndex.begin(), bordersIndex.end(), 0);
    std::srand(std::time(0));

    while (initialSolution.size() != nededCuts)
    {
        //tomar dos puntos bordes no usados
        int random_pos = std::rand() % bordersIndex.size();
        int random_index = bordersIndex[random_pos];

        position x = m.getBorderAt(random_index);
        bordersIndex.erase(bordersIndex.begin() + random_pos);

        random_pos = std::rand() % bordersIndex.size();
        random_index = bordersIndex[random_pos];
        
        position y = m.getBorderAt(random_index);
        bordersIndex.erase(bordersIndex.begin() + random_pos);

        path a = m.getPathBetween(x, y);
        //todo: chequear que el camino sea valido
        initialSolution.push_back(a);
    }
    return initialSolution;
}