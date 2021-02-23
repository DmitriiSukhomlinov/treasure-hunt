#pragma once

#include "Point.h"

#include <vector>

namespace TH {

class Line;

struct PointBetweenLines {
    PointBetweenLines(const Point& point, Line* linePointBelongs2, Line* line1, Line* line2);

    Point point;
    Line* linePointBelongs2 = nullptr;
    Line* line1 = nullptr;
    Line* line2 = nullptr;
    std::vector<PointBetweenLines*> adjacentGraphPoints;

    bool used = false;
    int distanceFromTreasure = 0;
};

}