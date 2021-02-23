#include "Line.h"

#include <algorithm>

namespace TH {

const Line Line::TOP(Point(0, 100), Point(100, 100));
const Line Line::RIGHT(Point(100, 0), Point(100, 100));
const Line Line::BOTTOM(Point(0, 0), Point(100, 0));
const Line Line::LEFT(Point(0, 0), Point(0, 100));

Line::Line(const Point& _point1, const Point& _point2) 
    : point1(_point1), point2(_point2) {

    auto x1 = point1.getX();
    auto y1 = point1.getY();
    auto x2 = point2.getX();
    auto y2 = point2.getY();

    a = y1 - y2;
    b = x2 - x1;
    c = x1 * y2 - x2 * y1;
}

Line::~Line() {
    for (auto m : middlePoints) {
        delete m;
    }
}

Line::Line(const Line& other) 
    : Line(other.point1, other.point2) {
    this->crossoverPointsAndLines = other.crossoverPointsAndLines;
    for (auto m : other.middlePoints) {
        this->middlePoints.push_back(new PointBetweenLines(*m));
    }
}

Line::Line(Line&& other) noexcept 
    : Line(other.point1, other.point2) {
    this->crossoverPointsAndLines = other.crossoverPointsAndLines;
    for (auto m : other.middlePoints) {
        this->middlePoints.push_back(new PointBetweenLines(*m));
    }
    other.middlePoints.clear();
}

Line& Line::operator=(const Line& other) {
    if (*this == other) {
        return *this;
    }

    this->point1 = other.point1;
    this->point2 = other.point2;
    this->a = other.a;
    this->b = other.b;
    this->c = other.c;
    this->crossoverPointsAndLines = other.crossoverPointsAndLines;
    for (auto m : other.middlePoints) {
        this->middlePoints.push_back(new PointBetweenLines(*m));
    }

    return *this;
}

Line& Line::operator=(Line&& other) noexcept {
    if (*this == other) {
        return *this;
    }

    this->point1 = other.point1;
    this->point2 = other.point2;
    this->a = other.a;
    this->b = other.b;
    this->c = other.c;
    this->crossoverPointsAndLines = other.crossoverPointsAndLines;
    for (auto m : other.middlePoints) {
        this->middlePoints.push_back(new PointBetweenLines(*m));
    }
    other.middlePoints.clear();

    return *this;
}

bool Line::operator==(const Line& other) const {
    return this->point1 == other.point1 && this->point2 == other.point2;
}

void Line::findCrossoverPoint(Line* other) {
    auto point = calculateCrossoverPoint(other);
    if (!point.isValid()) {
        return;
    }

    std::pair<Point, Line*> p(point, other);
    crossoverPointsAndLines.push_back(p);
}

Point Line::calculateCrossoverPoint(Line* other) const {
    const auto determinant = this->a * other->b - this->b * other->a;
    if (determinant == 0) {
        //Матрица вырождена, линии параллельны
        return Point();
    }

    const double xCrossoverPoint = (this->b * other->c - other->b * this->c) / determinant;
    const double yCrossoverPoint = (other->a * this->c - this->a * other->c) / determinant;
    if (xCrossoverPoint < MIN_COORDINATE || xCrossoverPoint > MAX_COORDINATE
        || yCrossoverPoint < MIN_COORDINATE || yCrossoverPoint > MAX_COORDINATE) {
        //Точка пересечения находится вне квадрата, в котором мы ищем решения
        return Point();
    }

    return Point(xCrossoverPoint, yCrossoverPoint);
}

void Line::sortCrossoverPointsAlongLine() {
    std::sort(crossoverPointsAndLines.begin(), crossoverPointsAndLines.end(), [&](const std::pair<Point, Line*>& first, const std::pair<Point, Line*>& second) -> bool {
        auto distanceFromPoint1ToFirst = point1.distance(first.first);
        auto distanceFromPoint1ToSecond = point1.distance(second.first);
        return distanceFromPoint1ToFirst < distanceFromPoint1ToSecond;
    });
}

void Line::findMiddlePoints() {
    for (int i = 0; i < (int)crossoverPointsAndLines.size() - 1; i++) {
        const auto& crossoverPointI = crossoverPointsAndLines[i].first;
        const auto& crossoverPointIPlus1 = crossoverPointsAndLines[i + 1].first;
        double xMiddlePoint = (crossoverPointI.getX() + crossoverPointIPlus1.getX()) / 2;
        double yMiddlePoint = (crossoverPointI.getY() + crossoverPointIPlus1.getY()) / 2;

        Point middlePoint(xMiddlePoint, yMiddlePoint);
        auto pbl = new PointBetweenLines(middlePoint, this, crossoverPointsAndLines[i].second, crossoverPointsAndLines[i + 1].second);
        middlePoints.push_back(pbl);
    }
}

const std::vector<PointBetweenLines*>& Line::getMiddlePoints() const {
    return middlePoints;
}

bool Line::isEdgeLine() const {
    return *this == Line::TOP || 
           *this == Line::RIGHT ||
           *this == Line::BOTTOM ||
           *this == Line::LEFT;
}

bool Line::arePointsOnTheSameSide(const Point& p1, const Point& p2) const {
    auto v1 = a * p1.getX() + b * p1.getY() + c;
    auto v2 = a * p2.getX() + b * p2.getY() + c;
    return (v1 > 0 && v2 > 0) || (v1 < 0 && v2 < 0);
}

}