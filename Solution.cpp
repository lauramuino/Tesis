#include <algorithm>
#include <iostream>
#include <cmath>
#include <numeric>
#include <filesystem>
#include "Solution.h"

void printSolution(solution& s)
{
    for (auto path: s)
    {
        for (auto position: path)
        {
            cout << "(" << position.first << ", " << position.second << ") ";
        }
        cout  << endl;
    }
    cout << endl;
}

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

bool positionsTouch(position a, position b)
{
    bool mismaFila = (a.first == b.first);
    bool columnasContiguas = abs(a.second - b.second) == 1;
    return mismaFila && columnasContiguas;
}

bool caminosQueSeCruzan(path a, path b)
{
    for (int i = 0; i < a.size(); i++)
    {
        for (int j = 0; j < b.size(); j++)
        {
            if (a[i] == b[j] ) {
                return true;
            }
            if (positionsTouch(a[i], b[j]) && i+1<a.size() && j+1<b.size() && positionsTouch(a[i+1], b[j+1]))
            {
                return true;
            }
            
        } 
    }
    return false;
}

bool hayCruces(solution &s)
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
            if (!hayCruces(newSolution))
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

bool esSolucionParcialValida(solution &s, Graph &g)
{
    if (hayCruces(s))
    {
        return false;
    }
    vector<int> info = g.getInfoOfCutsMadeBy(s);
    int ccWithoutResources = info[2];
    int countOfCC = info[3];
    if (countOfCC != s.size() + 1)
    {
        return false;
    }
    if (ccWithoutResources > 0)
    {
        return false;
    }
    return true;
}

solution getInitialSolutionFromFile(const char* filename)
{
    solution s;
    ifstream file;
    file.open(filename);
    if (!file.is_open())
    {
        cout << "Error opening file" << endl;
       return s;
    }
    int n; 
    file >> n;
    for (int i = 0; i < n; i++)
    {
        int m;
        file >> m;
        path cut;
        for (int j = 0; j < m; j++)
        {
            int x, y;
            file >> x >> y;
            cut.push_back(make_pair(x, y));
        }
        s.push_back(cut);
    }
    return s;  
}

void backtracking(solution &s, vector<path> &cuts, Graph &g, int cutsNeeded)
{
    if (cutsNeeded == s.size()) {
        return;
    }
    for (int i = 0; i < cuts.size(); i++)
    {
        s.push_back(cuts[i]);
        if (esSolucionParcialValida(s, g))
        { 
            cuts.erase(cuts.begin() + i);
            backtracking(s, cuts, g, cutsNeeded);
            if (cutsNeeded == s.size()) { 
                return;
            }
            cuts.insert(cuts.begin() + i, s.back());
        } else {
            s.pop_back();
        }
    }
    if (s.size() != cutsNeeded)
    { 
        throw std::runtime_error("No se pudo encontrar una solucion inicial");
    }
    
}

solution getInitialSolutionDoingBacktracking(Map &m, Graph &grafo)
{
    solution s;
    vector<path> cuts = cortesQueNoEstanEn(s, m);
    backtracking(s, cuts, grafo, m.resources()-1);
    return s;
}

solution getInitialSolution(string initialSolPath, Map &m, Graph &g)
{
    solution bestSolution;
    string path = std::filesystem::current_path().string().c_str();
    
    if (initialSolPath.empty())
    {
        bestSolution = getInitialSolutionDoingBacktracking(m, g);
        path.append("/output/initialSolutionBacktracking");
    } else {
        bestSolution = getInitialSolutionFromFile(initialSolPath.c_str());
        path.append("/output/initialSolutionManual");
    }

    m.drawSolution(bestSolution, path.substr(0, path.size()).c_str());

    return bestSolution;
}

solution tabuSearch(int maxIterations, int tabuListSize, Map &map, string initialSolPath)
{
    Graph grafo = Graph(map);
    solution bestSolution = getInitialSolution(initialSolPath, map, grafo);

    solution currentSolution = bestSolution;
    vector<solution> tabu_list;
 
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
 
    cout << "Best solution found: " << endl;
    printSolution(bestSolution);
    return bestSolution;
}