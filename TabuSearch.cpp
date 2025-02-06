#include <algorithm>
#include <iostream>
#include <cmath>
#include <numeric>
#include <filesystem>
#include "TabuSearch.h"

TabuSearch::TabuSearch(int maxIterations, int tabuListSize, Map &map, string initialSolPath) : maxIterations(maxIterations), tabuListSize(tabuListSize), mapa(map), initialSolPath(initialSolPath), grafo(Graph(mapa)) {}

void TabuSearch::printSolution(solution& s)
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

bool TabuSearch::corteEstaEnSolucion(path &cutA, solution &solucion)
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

vector<path> TabuSearch::cortesQueNoEstanEn(solution &s) 
{
    vector<path> cortes;
    for (int i = 0; i < mapa.borders(); i++)
    {
        for (int j = i+1; j < mapa.borders(); j++)
        {
            path nuevoCorte = mapa.getPathBetween(mapa.getBorderAt(i), mapa.getBorderAt(j));
            if (!corteEstaEnSolucion(nuevoCorte, s))
            {
                cortes.push_back(nuevoCorte);
            }      
        }
    }
    return cortes;
}

bool TabuSearch::positionsTouch(position a, position b)
{
    bool mismaFila = (a.first == b.first);
    bool columnasContiguas = abs(a.second - b.second) == 1;
    return mismaFila && columnasContiguas;
}

bool TabuSearch::caminosQueSeCruzan(path a, path b)
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

bool TabuSearch::hayPosicionesNoCaminables(path &cut)
{
    for (int i = 0; i < cut.size(); i++)
    {
        if (mapa.at(cut[i].first, cut[i].second) == 0)
        {
            return true;
        }
    }
    return false;
}

bool TabuSearch::hayCrucesOPosicionesNoCaminables(solution &s)
{
    for (int i = 0; i < s.size(); i++)
    {
        if (hayPosicionesNoCaminables(s[i]))
            return true;

        for (int j = i+1; j < s.size(); j++)
        {
            if (caminosQueSeCruzan(s[i], s[j]))
                return true;
        }
    }
    
    return false;
}

vector<solution> TabuSearch::getNeighbourhood(solution &s)
{ 
    vector<solution> neighbours;
    vector<path> cortes = cortesQueNoEstanEn(s);

    for (int i = 0; i < cortes.size(); i++)
    {
        for (int j = 0; j < s.size(); j++)
        {
            solution newSolution = s; 
            newSolution[j] = cortes[i];
            if (!hayCrucesOPosicionesNoCaminables(newSolution))
                neighbours.push_back(newSolution);
        }
        
    }
    
    return neighbours;
}

double TabuSearch::objectiveFunction(solution &s)
{
    double averageCutSizes = 0;
    for (path p : s)
    {
        averageCutSizes += (double) p.size();
    }
    averageCutSizes = averageCutSizes / s.size();
    
    vector<double> info = grafo.getInfoOfCutsMadeBy(s);
    double leastSquaresArea = info[0];
    double highestResourcesOnSameCC = info[1];
    double ccWithoutResources = info[2];

    double resourcesBalanced = highestResourcesOnSameCC - 1 + ccWithoutResources;

    return leastSquaresArea + resourcesBalanced + averageCutSizes;
}

bool TabuSearch::esSolucionParcialValida(solution &s)
{
    if (hayCrucesOPosicionesNoCaminables(s))
        return false;
    
    vector<double> info = grafo.getInfoOfCutsMadeBy(s);
    double ccWithoutResources = info[2];
    double countOfCC = info[3];
    
    if (countOfCC != s.size() + 1)
        return false;

    if (ccWithoutResources > 0)
        return false;

    return true;
}

solution TabuSearch::getInitialSolutionFromFile(const char* filename)
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

void TabuSearch::backtracking(solution &s, vector<path> &cuts, int cutsNeeded)
{
    if (cutsNeeded == s.size()) {
        return;
    }
    for (int i = 0; i < cuts.size(); i++)
    {
        s.push_back(cuts[i]);
        if (esSolucionParcialValida(s))
        { 
            cuts.erase(cuts.begin() + i);
            backtracking(s, cuts, cutsNeeded);
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

solution TabuSearch::getInitialSolutionDoingBacktracking()
{
    solution s;
    vector<path> cuts = cortesQueNoEstanEn(s);
    backtracking(s, cuts, mapa.resources()-1);
    return s;
}

solution TabuSearch::getInitialSolution(string initialSolPath)
{
    solution bestSolution;
    string path = std::filesystem::current_path().string().c_str();
    
    if (initialSolPath.empty())
    {
        bestSolution = getInitialSolutionDoingBacktracking();
        path.append("/output/initialSolutionBacktracking");
    } else {
        bestSolution = getInitialSolutionFromFile(initialSolPath.c_str());
        path.append("/output/initialSolutionManual");
    }

    mapa.drawSolution(bestSolution, path.substr(0, path.size()).c_str());

    return bestSolution;
}

solution TabuSearch::run()
{
    solution bestSolution = getInitialSolution(initialSolPath);

    solution currentSolution = bestSolution;
    vector<solution> tabu_list;
 
    for (int iter = 0; iter < maxIterations; iter++) {
        vector<solution> neighbours = getNeighbourhood(currentSolution);
        solution best_neighbour;
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
            // Remove the oldest entry from the tabu list if it exceeds the size
            tabu_list.erase(tabu_list.begin());
        }
 
        if (objectiveFunction(best_neighbour) < objectiveFunction(bestSolution)) {
            // Update the best solution if the current neighbour is better
            bestSolution = best_neighbour;
        }
    }
 
    cout << "Best solution found: " << endl;
    printSolution(bestSolution);
    return bestSolution;
}