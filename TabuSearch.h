#include "Graph.h"

class TabuSearch {
    public:
        TabuSearch(int maxIterations, int tabuListSize, Map &map, string initialSolPath, double cutsThreshold, double lengthsThreshold);
        solution run();
    private:
        int maxIterations, tabuListSize;
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
        bool esSolucionParcialValida(solution &s, int resources);
        vector<path> cortesQueNoEstanEn(solution &s);
        bool corteEstaEnSolucion(path &cutA, solution &solucion);
        bool positionsTouch(position a, position b);
        bool caminosQueSeCruzan(path a, path b);
        void backtracking(solution &s, vector<path> &cuts, int cutsNeeded);
        void writeSolution(solution& s);
        bool hayCruces(solution &s);

        vector<double> getInfoOfCutsMadeBy(solution &s);
};