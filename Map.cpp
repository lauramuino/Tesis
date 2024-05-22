#include "Map.h"

Map::Map(ifstream & f) {
    int row = 0, col = 0;
    string line;
    while ( getline(f, line) ) {
        col = 0;
        vector<int> empty(line.size(), 0);
        positions.push_back(empty);
        for (char& c : line) {
            positions[row][col] = c - '0';
            col++;
        }
        row++;
    }
    f.close();
}

bool Map::isBorder(int i, int j) {
    int max_row_index = positions.size() - 1;
    int max_col_index = positions[0].size() - 1;
    
    //a matrix border
    if (max_row_index == i || 0 == i || 0 == j || max_col_index == j) {
        return positions[i][j] == 1;
    }

    bool up    = positions[i-1][j] == 0;
    bool down  = positions[i+1][j] == 0;
    bool left  = positions[i][j-1] == 0;
    bool right = positions[i][j+1] == 0;
    bool sides = up || down || left || right;

    bool d_left_up    = positions[i-1][j-1] == 0;
    bool d_left_down  = positions[i-1][j+1] == 0;
    bool d_right_up   = positions[i+1][j-1] == 0;
    bool d_right_down = positions[i+1][j+1] == 0;
    bool diagonals    = d_left_up || d_left_down || d_right_down || d_right_up;

    //if there's any unbuildable square, is border
    return (sides || diagonals) && (positions[i][j] == 1);
}