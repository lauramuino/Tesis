#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>

using namespace std;

typedef vector<vector<vector<int> > > image;

// Function to map matrix values to colors
void getColor(int value, int& r, int& g, int& b) {
    switch (value) {
        case 0:  r = 105;  g = 105;  b = 105;  break; // grey for non-wakable
        case 1:  r = 0;    g = 0;    b = 0;    break; // black wakable/buildable
        case 2:  r = 0;    g = 255;  b = 0;    break; // green resourses
        default: r = 255;  g = 255;  b = 255;  break; // white should never happen
    }
}

// Function to draw a filled rectangle
void drawRectangle(image& output, int x, int y, int size, int r, int g, int b) {
    for (int i = y; i < y + size; ++i) {
        for (int j = x; j < x + size; ++j) {
             if (i >= 0 && i < output.size() && j >= 0 && j < output[0].size()) {
                if (i==y || i == y+size-1 || j==x || j==x+size-1)
                {
                    //drawing edges
                    output[i][j][0] = 255;
                    output[i][j][1] = 255;
                    output[i][j][2] = 255;
                } else{
                    output[i][j][0] = r;
                    output[i][j][1] = g;
                    output[i][j][2] = b;
                }
            }
        }
    }
}

// Function to draw a line between two points (Bresenham)
void drawLine(image& output, int x1, int y1, int x2, int y2, int r, int g, int b) {
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (true) {
        if (x1 >= 0 && x1 < output[0].size() && y1 >= 0 && y1 < output.size()) {
            output[y1][x1][0] = r;
            output[y1][x1][1] = g;
            output[y1][x1][2] = b;
        }
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}

// Function to convert HSV to RGB
void HSVtoRGB(float h, float s, float v, int &r, int &g, int &b) {
    float c = v * s;
    float x = c * (1 - fabs(fmod(h * 6, 2) - 1));
    float m = v - c;
    
    float r1, g1, b1;
    
    if (h < 1.0/6)      { r1 = c, g1 = x, b1 = 0; }
    else if (h < 2.0/6) { r1 = x, g1 = c, b1 = 0; }
    else if (h < 3.0/6) { r1 = 0, g1 = c, b1 = x; }
    else if (h < 4.0/6) { r1 = 0, g1 = x, b1 = c; }
    else if (h < 5.0/6) { r1 = x, g1 = 0, b1 = c; }
    else                { r1 = c, g1 = 0, b1 = x; }
    
    r = static_cast<int>((r1 + m) * 255);
    g = static_cast<int>((g1 + m) * 255);
    b = static_cast<int>((b1 + m) * 255);
}

// Function to generate n distinct RGB colors
vector<vector<int>> generateDistinctColors(int n) {
    vector<vector<int>> colors;
    for (int i = 0; i < n; i++) {
        float hue = static_cast<float>(i) / n; // Evenly spaced hues
        int r, g, b;
        HSVtoRGB(hue, 1.0, 1.0, r, g, b);
        colors.push_back({r, g, b});
    }
    return colors;
}

void print(vector<vector<int> > &matrix, vector<vector<pair<int,int>>>& resourceClusters, vector<vector<pair<int, int> > >& sol, string fileName) {

    // Image parameters
    const int cellSize = 50; // Size of each square
    const int rows = matrix.size();
    const int cols = matrix[0].size();
    const int imgWidth = cols * cellSize;
    const int imgHeight = rows * cellSize;
    auto colors = generateDistinctColors(resourceClusters.size());

    // Create an empty image (RGB)
    image output(imgHeight, std::vector<std::vector<int>>(imgWidth, std::vector<int>(3, 255)));

    // Draw the matrix
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int r, g, b;
            int cliqueIndex = -1;
            for (int cl = 0; cl < resourceClusters.size(); cl++)
            {
                auto it = find(resourceClusters[cl].begin(), resourceClusters[cl].end(), make_pair(i, j));
                if (it != std::end(resourceClusters[cl])) {
                    cliqueIndex = cl;
                    break;
                }
            }
            if (cliqueIndex != -1)
            {
                r = colors[cliqueIndex][0];
                g = colors[cliqueIndex][1]; 
                b = colors[cliqueIndex][2];
            } else {
                getColor(matrix[i][j], r, g, b);
            }

            drawRectangle(output, j * cellSize, i * cellSize, cellSize, r, g, b);
        }
    }

    // Draw lines connecting positions of the solution
    for (size_t i = 0; i < sol.size(); ++i) {
        for (size_t j = 0; j < sol[i].size()-1; j++)
        {
            pair<int,int> init = sol[i][j];
            pair<int,int> end  = sol[i][j+1];
            drawLine(output, init.second*cellSize + cellSize/2, init.first*cellSize + cellSize/2,
                 end.second*cellSize + cellSize/2, end.first*cellSize + cellSize/2, 255, 0, 0);
        }
        if (sol[i].size() == 1)
        {
            pair<int,int> init = sol[i][0];
            drawLine(output, init.second*cellSize + cellSize/4, init.first*cellSize + cellSize/2,
                     init.second*cellSize + 3*cellSize/4 , init.first*cellSize + cellSize/2, 255, 0, 0);
        }        
    }

    // Save the image as a PPM file
    std::ofstream outFile(fileName + ".ppm");
    outFile << "P3\n" << imgWidth << " " << imgHeight << "\n255\n";
    for (const auto& row : output) {
        for (const auto& pixel : row) {
            outFile << pixel[0] << " " << pixel[1] << " " << pixel[2] << " ";
        }
        outFile << "\n";
    }
    outFile.close();

    std::cout << "Image saved as " + fileName << std::endl;
}
