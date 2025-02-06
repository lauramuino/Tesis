#include "Graph.h"

class TabuSearch {
    public:
        TabuSearch(int maxIterations, int tabuListSize, Map &map, string initialSolPath);
        solution run();
    private:
        int maxIterations, tabuListSize;
        Map &mapa;
        Graph grafo;
        string initialSolPath;

        int objectiveFunction(solution &s);
        
        solution getInitialSolution(string initialSolPath);
        solution getInitialSolutionDoingBacktracking();
        solution getInitialSolutionFromFile(const char* filename);
        vector<solution> getNeighbourhood(solution &s);
        bool esSolucionParcialValida(solution &s);
        vector<path> cortesQueNoEstanEn(solution &s);
        bool corteEstaEnSolucion(path &cutA, solution &solucion);
        bool positionsTouch(position a, position b);
        bool caminosQueSeCruzan(path a, path b);
        void backtracking(solution &s, vector<path> &cuts, int cutsNeeded);
        void printSolution(solution& s);
        bool hayCruces(solution &s);
};