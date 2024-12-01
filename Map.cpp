#include "Map.h"
#include "Printing.h"

Map::Map(ifstream & f) 
{
    int row = 0, col = 0, resources = 0;
    string line;
    while ( getline(f, line) ) {
        col = 0;
        vector<int> empty(line.size(), 0);
        positions.push_back(empty);
        for (char& c : line) {
            positions[row][col] = c - '0';
            if (positions[row][col] == 2) {
                resources++;
            }       
            col++;
        }
        row++;
    }
    f.close();

    totalColumns = col;
    totalRows = row;
    totalResources = resources;
    
    //se completa info sobre bordes
    for (int i = 0; i < positions.size(); i++)
    {
        for (int j = 0; j < positions[i].size(); j++)
        {
            if(isBorder(i, j) && positions[i][j] != 2) {
                borderPositions.push_back(make_pair(i,j));
            }
        }
    }
}

Map::~Map() = default;

bool Map::isBorder(int i, int j) 
{
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

bool Map::inRange(pair<int, int> p)
{
    bool xInRange = p.first >=0 && p.first <= positions.size() - 1;
    bool yInRange = p.second >=0 && p.second <= positions[0].size() -1;
    return xInRange && yInRange;
}

vector<position> Map::getWalkableNeighbours(position p)
{
    vector<position> walkableNeighbours;

    for (int a = -1; a <= 1; a++) {
        for (int b = -1; b <= 1; b++) {
            pair<int,int> v = make_pair(p.first + a, p.second + b);
            if (v != p && inRange(v) && this->at(v.first, v.second) != 0) {
                walkableNeighbours.push_back(v);
            }
        }       
    }
    
    return walkableNeighbours;
}

//Bresenham's line algorithm (https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm)
path Map::getPathBetween(position x, position y) {
    int dx = abs(y.first - x.first);
    int dy = abs(y.second - x.second);
    int sx = (x.first < y.first) ? 1 : -1;
    int sy = (x.second < y.second) ? 1 : -1;
    int err = dx - dy;

    path result;

    while (x != y ) {

        if (this->inRange(x)) {
            result.push_back(x);
        }

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x.first += sx;
        }
        if (e2 < dx) {
            err += dx;
            x.second += sy;
        }
    }
    result.push_back(x);

    return result;
}

void Map::drawSolution(vector<path> &s, const char* filename)
{
    print(this->positions, s, filename);
}