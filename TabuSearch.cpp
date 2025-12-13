#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>
#include <numeric>
#include <filesystem>
#include "TabuSearch.h"

TabuSearch::TabuSearch(int maxIterations, int tabuListSize, Map &map, string initialSolPath, double cutsThreshold, double lengthsThreshold, int neighbourDistance) : maxIterations(maxIterations), tabuListSize(tabuListSize), mapa(map), initialSolPath(initialSolPath), grafo(Graph(mapa)), cutsThreshold(cutsThreshold), lengthsThreshold(lengthsThreshold), neighbourDistance(neighbourDistance) {}

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
            if (nuevoCorte.size() != 0 && !corteEstaEnSolucion(nuevoCorte, s))
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
            // share same position
            if (a[i] == b[j] ) {
                return true;
            }
            if (positionsTouch(a[i], b[j]) && i+1<a.size() && j+1<b.size() && positionsTouch(a[i+1], b[j+1])) //TODO: not so sure about this, check later
            {
                return true;
            }
        } 
    }
    return false;
}

bool TabuSearch::hayCruces(solution &s)
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

//obtencion de vecindario completo, por ahora lo dejo comentado porque genera muchos elementos 
/*vector<solution> TabuSearch::getNeighbourhood(solution &s)
{ 
    vector<solution> neighbours;
    vector<path> cortes = cortesQueNoEstanEn(s);

    for (int i = 0; i < cortes.size(); i++)
    {
        for (int j = 0; j < s.size(); j++)
        {
            //interchanging cuts
            solution newSolution = s; 
            newSolution[j] = cortes[i];
            if (esSolucionValida(newSolution))
                neighbours.push_back(newSolution);
            
            //reducing cuts
            solution newSolution2 = s;
            newSolution2.erase(newSolution2.begin() + j);
            if (esSolucionValida(newSolution2))
                neighbours.push_back(newSolution2);
        }
        
    }
    
    return neighbours;
}*/

vector<solution> TabuSearch::getNeighbourhood(solution &solucion)
{
    vector<solution> neighbourhood;
    for (int k = 0; k < solucion.size(); k++) {
        position extremo_a = solucion[k][0];
        position extremo_b = solucion[k][solucion[k].size()-1];
        
        //busco alrededor de los bordes, un cuadrado de distancia neighbourDistance, por bordes
        vector<position> losDeA;
        for (int i = extremo_a.first - neighbourDistance; i <= extremo_a.first + neighbourDistance; i++) {
            for (int j = extremo_a.second - neighbourDistance; j <= extremo_a.second + neighbourDistance; j++) {
                if (i >= 0 and i < this->mapa.rows() and j >= 0 and j < this->mapa.columns()) {//chequeo no irme de rango
                    if (max(abs(i - extremo_a.first), abs(j - extremo_a.second)) == neighbourDistance) { //chequeo distancia
                        if (this->mapa.isBorder(i,j)) { //chequeo q sea borde
                            losDeA.push_back(make_pair(i,j));
                        }
                    }
                }
            }
        }

        vector<position> losDeB;
        for (int i = extremo_b.first - neighbourDistance; i <= extremo_b.first + neighbourDistance; i++) {
            for (int j = extremo_b.second - neighbourDistance; j <= extremo_b.second + neighbourDistance; j++) {
                if (i >= 0 and i < this->mapa.rows() and j >= 0 and j < this->mapa.columns()) {//chequeo no irme de rango
                    if (max(abs(i - extremo_b.first), abs(j - extremo_b.second)) == neighbourDistance) { //chequeo distancia
                        if (this->mapa.isBorder(i,j)) { //chequeo q sea borde
                            losDeB.push_back(make_pair(i,j));
                        }
                    }
                }
            }
        }

        //combinar los puntos bordes encontrados para formar cortes
        for (int i = 0; i < losDeA.size(); i++) { //todo: change to foreach
            for (int j = 0; j < losDeB.size(); j++) {
                if (losDeA[i] == make_pair(24, 42) && j == 0) {
                    path p = this->mapa.getPathBetween(losDeA[i], losDeB[j]);
                    //interchanging cuts
                    solution newSolution = solucion; 
                    newSolution[k] = p;
                    if (esSolucionValida(newSolution)) {
                        neighbourhood.push_back(newSolution);
                    }
                    string path = std::filesystem::current_path().string().c_str();
                    path = path + "/aChequear";
                    mapa.drawSolution(newSolution, path.c_str());
                    return neighbourhood;
                }
            }
        }
    }
    return neighbourhood;
}

double TabuSearch::objectiveFunction(solution &s)
{
    double longitudes = 0;
    for (path p : s)
    {
        longitudes += (double) p.size();
    }
    double cutsAndLengthsBalance = s.size() * cutsThreshold + longitudes * lengthsThreshold;
    
    vector<double> info = this->getInfoOfCutsMadeBy(s);
    double leastSquaresArea = info[0];
    double highestResourcesOnSameCC = info[1];
    double ccWithoutResources = info[2];

    double resourcesBalanced = highestResourcesOnSameCC - 1 + ccWithoutResources;

    return leastSquaresArea + resourcesBalanced + cutsAndLengthsBalance;
}

vector<double> TabuSearch::getInfoOfCutsMadeBy(solution& s)
{
    vector<vector<position> > connectedComponents = this->grafo.getMapConnectedComponents(s);
    vector<vector<position> > resourceClusters = this->mapa.getResourceClusters();

    double meanArea = 0, highestResourcesOnSameCC = 0, ccWithoutResources = 0;

    for (vector<position> cc : connectedComponents) {
        meanArea += (double)cc.size();
        int countOfClusterResources = 0;
        
        //check if connected component contains a particular resource cluster
        for (vector<position> resourceCluster : resourceClusters) {
            int counter = 0;
            for (position resource : resourceCluster) {
                if (find(cc.begin(), cc.end(), resource) != cc.end()) {
                    counter++;
                }
            }
            if (counter == resourceCluster.size()) {
                countOfClusterResources++;
            }
        }   

        if (highestResourcesOnSameCC < countOfClusterResources) highestResourcesOnSameCC = countOfClusterResources;
        if (countOfClusterResources == 0) ccWithoutResources++;
        
    }
    meanArea = meanArea / connectedComponents.size();

    double leastSquaresArea = 0;
    for (auto cc : connectedComponents)
    {
        leastSquaresArea += pow(meanArea-cc.size(), 2.0);
    }

    return {leastSquaresArea, highestResourcesOnSameCC, ccWithoutResources};
}

bool TabuSearch::esSolucionValida(solution &s)
{
    if (hayCruces(s))
        return false;
    
    vector<double> info = this->getInfoOfCutsMadeBy(s);
    double ccWithoutResources = info[2];
    double highestResourcesOnSameCC = info[1];

    if (ccWithoutResources > 0)
        return false;
    
    if (highestResourcesOnSameCC != 1)
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
        int x1, y1, x2, y2;
        file >> x1 >> y1 >> x2 >> y2;
        path cut = mapa.getPathBetween(make_pair(x1, y1), make_pair(x2, y2));
        s.push_back(cut);
    }
    return s;  
}

bool TabuSearch::backtracking(solution &s, const vector<path> &cuts, int cutsNeeded, int startIndex)
{
    cout << "index is " << startIndex << endl;
    if (s.size() == cutsNeeded) {
        cout << "retured true" << endl;
        return true; 
    }

    for (int i = startIndex; i < cuts.size(); ++i) {
        cout << "i is " << i << endl;
        s.push_back(cuts[i]);

        if (esSolucionValida(s)) {
            if (backtracking(s, cuts, cutsNeeded, i + 1)) {
                return true; // Found a solution, bubble up true
            }
        }

        s.pop_back();
    }

    return false;
}

solution TabuSearch::getInitialSolutionDoingBacktracking()
{
    solution s;
    vector<path> cuts = cortesQueNoEstanEn(s);
    cout << "size of cuts " << cuts.size() << endl;
    backtracking(s, cuts, mapa.resourceClustersCount()-1, 0);
    return s;
}

solution TabuSearch::getInitialSolution()
{
    solution bestSolution;
    string path = std::filesystem::current_path().string().c_str();
    
    if (this->initialSolPath.empty())
    {
        cout << "Building Initial solution by backtracking..." << endl;
        bestSolution = getInitialSolutionDoingBacktracking();
        path.append("/output/initialSolutionBacktracking");
    } else {
        cout << "Building Initial solution from file..." << endl;
        bestSolution = getInitialSolutionFromFile(this->initialSolPath.c_str());
        path.append("/output/initialSolutionManual");
    }

    cout << "Initial solution" << endl;
    // printSolution(bestSolution);
    // mapa.drawSolution(bestSolution, path.substr(0, path.size()).c_str());

    return bestSolution;
}

solution TabuSearch::run()
{
    solution bestSolution = getInitialSolution();

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
  
    cout << "Best solution found" << endl;
    //printSolution(bestSolution);
    return bestSolution;
}