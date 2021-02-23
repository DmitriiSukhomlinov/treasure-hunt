#include "Point.h"

#include <cmath>

namespace TH {

Point::Point(double _x, double _y) : x(_x), y(_y) {}

bool Point::operator==(const Point& other) const {
    bool xEquals = round(this->x) == round(other.x);
    bool yEquals = round(this->y) == round(other.y);
    return xEquals && yEquals;
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

bool Point::isValid() const {
    return x != -1.0;
}

const int Point::round(double v) {
    return (int)(v * 10000);
}

}