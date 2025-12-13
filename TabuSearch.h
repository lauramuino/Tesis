#include "Graph.h"

class TabuSearch {
    public:
        TabuSearch(int maxIterations, int tabuListSize, Map &map, string initialSolPath, double cutsThreshold, double lengthsThreshold, int neighbourDistance);
        solution run();
    private:
        int maxIterations, tabuListSize;
        int neighbourDistance;
        double cutsThreshold, lengthsThreshold;
        Map &mapa;
        Graph grafo;
        string initialSolPath;

        double objectiveFunction(solution &s);
        
        solution getInitialSolution();
        solution getInitialSolutionDoingBacktracking();
        solution getInitialSolutionFromFile(const char* filename);
        vector<solution> getNeighbourhood(solution &s);
        bool esSolucionValida(solution &s);
        vector<path> cortesQueNoEstanEn(solution &s);
        bool corteEstaEnSolucion(path &cutA, solution &solucion);
        bool positionsTouch(position a, position b);
        bool caminosQueSeCruzan(path a, path b);
        bool backtracking(solution &s, const vector<path> &cuts, int cutsNeeded, int startIndex);
        void printSolution(solution& s);
        bool hayCruces(solution &s);

        vector<double> getInfoOfCutsMadeBy(solution &s);
};