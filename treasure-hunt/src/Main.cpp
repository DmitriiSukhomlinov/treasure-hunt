#include <iostream>
#include <fstream>
#include <vector>

#include "computations/LineHandler.h"
#include "geometry/Line.h"

int main() {
    int linesNumber = 0;
    std::ifstream fin("input.txt");
    fin >> linesNumber;
    TH::LineHandler lineHandler;
    for (int i = 0; i < linesNumber; i++) {
        double x1 = 0;
        double y1 = 0;
        double x2 = 0;
        double y2 = 0;
        fin >> x1 >> y1 >> x2 >> y2;
        
        lineHandler.addLine(TH::Line(TH::Point(x1, y1), TH::Point(x2, y2)));
    }
    lineHandler.findNodes();

    double xTresure = 0;
    double yTreasure = 0;
    fin >> xTresure >> yTreasure;
    lineHandler.buildAdjacencyMatrix(TH::Point(xTresure, yTreasure));

    auto numberOfDoors = lineHandler.findShortestWayByBfs();

    std::cout << "Number of doors = " << numberOfDoors << std::endl;
}