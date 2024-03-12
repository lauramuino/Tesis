#include <algorithm>
#include <limits>
#include <iostream>
#include "TabuSearch.h"

std::vector<std::pair<int, int> > copyWithoutIth(const std::vector<std::pair<int, int> >& s, int i) {
    std::vector<std::pair<int, int> > vec2; 
    for (int j = 0; j < s.size() && j !=i ; j++) {
        vec2.push_back(s[j]);
    } 
    return vec2;
}

std::vector<solution> get_neighbors(const solution& s, Graph* g)
{
    //a solution is list of edges of the graph
    // that produces n connected components
    std::vector<std::vector<std::pair<int, int> > > neighbours;
    int resComponents = g->simulateCutAndCountConnectedComponentes(s);

    for (size_t i = 0; i < s.size(); i++) {
        //take out element of solution list
        solution new_solution = copyWithoutIth(s, i);
        // add neighbour edges of i, if they are not already
        solution next_edges =  g->getNeightbours(s[i].first, s[i].second);
        for (int j = 0; j < next_edges.size(); j++) {
            if(std::find(new_solution.begin(), new_solution.end(), next_edges[j]) == new_solution.end() ) {
                new_solution.push_back(next_edges[j]);
            }
        }

        bool isNotIncluded = std::find(neighbours.begin(), neighbours.end(), new_solution) == neighbours.end();
        //si la solucion genera n-1 componentes conexas con al menos 1 recurso, y no es igual a ninguna otra, agregar
        if (isNotIncluded && g->isValidSolution(new_solution, resComponents)) {
            neighbours.push_back(new_solution);
        }
    }
    return neighbours;
}

int objective_function(const solution& s, Graph* g)
{
    // the length of separations should be minimum
    int length = s.size();
    // not sure about the paper definition, using this for now
    int squaresArea = g->getSquareDifferenceOfAreaOfConnectedComponentes(s);

    return length + squaresArea;
}

solution tabu_search(const solution& initial_solution, int max_iterations, int tabu_list_size, Graph* g) {
    solution best_solution = initial_solution;
    solution current_solution = initial_solution;
    std::vector<solution> tabu_list;
 
    for (int iter = 0; iter < max_iterations; iter++) {
        std::vector<solution> neighbors = get_neighbors(current_solution, g);
        solution best_neighbor;
        int best_neighbor_fitness = std::numeric_limits<int>::max();
 
        for (const solution& neighbor : neighbors) {
            if (std::find(tabu_list.begin(), tabu_list.end(), neighbor) == tabu_list.end()) {
                int neighbor_fitness = objective_function(neighbor, g);
                if (neighbor_fitness < best_neighbor_fitness) {
                    best_neighbor = neighbor;
                    best_neighbor_fitness = neighbor_fitness;
                }
            }
        }
 
        if (best_neighbor.empty()) {
            // No non-tabu neighbors found, terminate the search
            break;
        }
 
        current_solution = best_neighbor;
        tabu_list.push_back(best_neighbor);
        if (tabu_list.size() > tabu_list_size) {
            // Remove the oldest entry from the
            // tabu list if it exceeds the size
            tabu_list.erase(tabu_list.begin());
        }
 
        if (objective_function(best_neighbor, g) < objective_function(best_solution, g)) {
            // Update the best solution if the
            // current neighbor is better
            best_solution = best_neighbor;
        }
    }
 
    return best_solution;
}

void showSolution(solution s) {
    for (int i = 0; i < s.size(); i++)
    {
        std::cout <<"(" << s[i].first << ";" << s[i].second << ") ";
    }
    std::cout << std::endl;
}

