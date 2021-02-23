#pragma once

#include "Point.h"
#include "PointBetweenLines.h"

#include <vector>

namespace TH {

class Line {
public:
    Line(const Point& point1, const Point& point2);
    ~Line();
    Line(const Line& other);
    Line(Line&& other) noexcept;
    Line& operator=(const Line& other);
    Line& operator=(Line&& other) noexcept;


    bool operator==(const Line& other) const;

    void findCrossoverPoint(Line* other);
    Point calculateCrossoverPoint(Line* other) const;
    void sortCrossoverPointsAlongLine();
    void findMiddlePoints();

    const std::vector<PointBetweenLines*>& getMiddlePoints() const;
    bool isEdgeLine() const;
    bool arePointsOnTheSameSide(const Point& p1, const Point& p2) const;

    static const Line TOP;
    static const Line RIGHT;
    static const Line BOTTOM;
    static const Line LEFT;

private:
    static constexpr double MIN_COORDINATE = 0.0;
    static constexpr double MAX_COORDINATE = 100.0;

    Point point1;
    Point point2;

    /*
     * Уравнение прямой по двум точкам имеет общий вид:
     * (y1 - y2)x + (x2 - x1)y + (x1*y2 - x2*y1) = 0 или
     * ax + by + c = 0
     * Получаем, что 
     * (y1 - y2) = a
     * (x2 - x1) = b
     * (x1*y2 - x2*y1) = c
     */
    double a = 0;
    double b = 0;
    double c = 0;

    std::vector<std::pair<Point, Line*>> crossoverPointsAndLines;

    std::vector<PointBetweenLines*> middlePoints;
};

}
