#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>
#include <numeric>
#include <filesystem>
#include "TabuSearch.h"
#include <queue>

struct Node {
    int r, c, colorID;
};

TabuSearch::TabuSearch(int maxIterations, int tabuListSize, Map &map, string initialSolPath, double cutsThreshold, double lengthsThreshold, int neighbourDistance) : maxIterations(maxIterations), tabuListSize(tabuListSize), mapa(map), initialSolPath(initialSolPath), cutsThreshold(cutsThreshold), lengthsThreshold(lengthsThreshold), neighbourDistance(neighbourDistance) {}

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
    
    vector<double> info = this->getPartitionStatsOptimized(s);
    double leastSquaresArea = info[0];
    double highestResourcesOnSameCC = info[1];
    double ccWithoutResources = info[2];

    double resourcesBalanced = highestResourcesOnSameCC - 1 + ccWithoutResources;

    return leastSquaresArea + resourcesBalanced + cutsAndLengthsBalance;
}

vector<double> TabuSearch::getPartitionStatsOptimized(solution& s)
{
    int rows = mapa.rows();
    int cols = mapa.columns();

    // 1. Mark cuts on a lookup grid for O(1) access
    vector<vector<bool>> isCut(rows, vector<bool>(cols, false));
    for (const path& p : s) {
        for (const position& pos : p) {
            isCut[pos.first][pos.second] = true;
        }
    }

    // 2. Full Scan BFS to identify all components
    // componentMap stores the Component ID for each cell (-1 if unvisited/wall)
    vector<vector<int>> componentMap(rows, vector<int>(cols, -1));
    vector<int> componentSizes;
    int compId = 0;

    // Directions for neighbors
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // If it's a valid starting point (Walkable, Not a Cut, Not Visited)
            if (!mapa.isUnbuildable(make_pair(i, j)) && !isCut[i][j] && componentMap[i][j] == -1) {
                
                // Start BFS for this component
                int currentSize = 0;
                queue<pair<int, int>> q;
                
                q.push({i, j});
                componentMap[i][j] = compId;
                currentSize++;

                while (!q.empty()) {
                    pair<int, int> curr = q.front();
                    q.pop();

                    for (int k = 0; k < 4; k++) {
                        int nr = curr.first + dr[k];
                        int nc = curr.second + dc[k];

                        // Check Bounds
                        if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;

                        // Check Walkability and if already visited
                        if (!mapa.isUnbuildable(make_pair(nr, nc)) && !isCut[nr][nc] && componentMap[nr][nc] == -1) {
                            componentMap[nr][nc] = compId;
                            currentSize++;
                            q.push({nr, nc});
                        }
                    }
                }
                
                // Finished one component
                componentSizes.push_back(currentSize);
                compId++;
            }
        }
    }

    // 3. Analyze Resource Clusters
    // We check which component each resource cluster falls into.
    vector<vector<position>> resourceClusters = mapa.getResourceClusters();
    vector<int> fullClustersInComp(compId, 0); 
    vector<bool> compHasAnyResource(compId, false);

    for (const auto& cluster : resourceClusters) {
        if (cluster.empty()) continue;

        // Check the component ID of the first resource in the cluster
        position first = cluster[0];
        
        // If the resource is covered by a cut, it's considered destroyed/isolated
        if (isCut[first.first][first.second]) continue;

        int cID = componentMap[first.first][first.second];
        
        // If cID is -1, it means it's a wall or unreachable (shouldn't happen for valid resources)
        if (cID == -1) continue;

        // Verify if the ENTIRE cluster is inside this same component
        bool isFullCluster = true;
        for (size_t k = 1; k < cluster.size(); ++k) {
            position p = cluster[k];
            // If part of the cluster is cut or in a different component (impossible if connected, but good to check)
            if (isCut[p.first][p.second] || componentMap[p.first][p.second] != cID) {
                isFullCluster = false;
                break;
            }
        }

        if (isFullCluster) {
            fullClustersInComp[cID]++;
            compHasAnyResource[cID] = true; // Mark that this component has at least one valid cluster
        }
    }

    // 4. Calculate Final Metrics
    double highestResourcesOnSameCC = 0;
    double ccWithoutResources = 0;

    for (int c = 0; c < compId; ++c) {
        if (fullClustersInComp[c] > highestResourcesOnSameCC) {
            highestResourcesOnSameCC = fullClustersInComp[c];
        }
        // If the component has NO full clusters, increment empty counter
        if (!compHasAnyResource[c]) {
            ccWithoutResources++;
        }
    }

    double meanArea = 0;
    if (!componentSizes.empty()) {
        double totalArea = 0;
        for (int s : componentSizes) totalArea += s;
        meanArea = totalArea / componentSizes.size();
    }

    double leastSquaresArea = 0;
    for (int s : componentSizes) {
        leastSquaresArea += pow(meanArea - s, 2.0);
    }

    return {leastSquaresArea, highestResourcesOnSameCC, ccWithoutResources};
}

bool TabuSearch::checkPartitionOptimized(Map &mapa, solution &cuts) {
    int rows = mapa.rows();
    int cols = mapa.columns();
    
    // 1. Mark cuts on a fast lookup grid (0: empty, 1: cut)
    vector<vector<bool>> isCut(rows, vector<bool>(cols, false));
    for (const path& cut : cuts) {
        for (const position& p : cut) {
            isCut[p.first][p.second] = true;
        }
    }

    // 2. Initialize visited grid with 0
    // visited[r][c] stores the ColorID of the resource that reached it
    vector<vector<int>> visited(rows, vector<int>(cols, 0));
    queue<Node> q;

    // 3. Get Resource Clusters and initialize BFS
    // We treat each cluster as a unique "source" with a unique ID (starting at 1)
    vector<vector<position>> clusters = mapa.getResourceClusters();
    for (int i = 0; i < clusters.size(); i++) {
        int colorID = i + 1; 
        for (const position& p : clusters[i]) {
            // If a resource is underneath a cut, it's effectively isolated/destroyed
            if (isCut[p.first][p.second]) continue; 
            
            visited[p.first][p.second] = colorID;
            q.push({p.first, p.second, colorID});
        }
    }

    // 4. Multi-Source BFS
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!q.empty()) {
        Node current = q.front();
        q.pop();

        for (int i = 0; i < 4; i++) {
            int nr = current.r + dr[i];
            int nc = current.c + dc[i];

            // Check boundaries
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;

            // Check if it's a static wall or a dynamic cut
            if (mapa.isUnbuildable(make_pair(nr, nc)) || isCut[nr][nc]) continue;

            int neighborColor = visited[nr][nc];

            if (neighborColor == 0) {
                // Found unvisited cell, claim it
                visited[nr][nc] = current.colorID;
                q.push({nr, nc, current.colorID});
            } else if (neighborColor != current.colorID) {
                // COLLISION: Two different resource clusters met.
                // This means the cuts did NOT separate them.
                return false; 
            }
        }
    }

    // 5. Check for Empty Regions (Orphans)
    // Any walkable cell that is NOT a cut and has visited == 0 means
    // it belongs to a region with NO resources.
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (!mapa.isUnbuildable(make_pair(i, j)) && !isCut[i][j]) {
                if (visited[i][j] == 0) {
                    return false; // Found region without resources
                }
            }
        }
    }

    return true; // All checks passed
}

bool TabuSearch::esSolucionValida(solution &s)
{
    if (hayCruces(s))
        return false;
    
    return checkPartitionOptimized(this->mapa, s);
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
    if (s.size() == cutsNeeded) {
        cout << "returned true at " << startIndex << endl;
        return true; 
    }

    for (int i = startIndex; i < cuts.size(); ++i) {
        s.push_back(cuts[i]);

        if (esSolucionValida(s)) {
            if (backtracking(s, cuts, cutsNeeded, i + 1)) {
                cout << "returned true at i = " << i << endl;
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