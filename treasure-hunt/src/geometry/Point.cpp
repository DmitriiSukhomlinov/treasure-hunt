#include "Point.h"

#include <cmath>

namespace TH {

Point::Point(double _x, double _y) : x(_x), y(_y) {}

bool Point::operator==(const Point& other) const {
    return this->x == other.x && this->y == other.y;
}

double Point::getX() const {
    return x;
}

double Point::getY() const {
    return y;
}

double Point::distance(const Point& other) const {
    return std::hypot(this->x - other.x, this->y - other.y);
}

}