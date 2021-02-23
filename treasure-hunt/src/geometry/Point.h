#pragma once

namespace TH {

class Point {
public:
    Point() = default;
    Point(double x, double y);

    bool operator==(const Point& other) const;

    double getX() const;
    double getY() const;
    double distance(const Point& other) const;

    bool isValid() const;

private:
    static const int round(double v);

    double x = -1.0;
    double y = -1.0;
};

}