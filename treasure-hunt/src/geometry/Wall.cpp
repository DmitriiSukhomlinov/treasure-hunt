#include "Wall.h"

#include <algorithm>

namespace TH {

const Wall Wall::TOP(Point(0, 100), Point(100, 100));
const Wall Wall::RIGHT(Point(100, 0), Point(100, 100));
const Wall Wall::BOTTOM(Point(0, 0), Point(100, 0));
const Wall Wall::LEFT(Point(0, 0), Point(0, 100));

Wall::Wall(const Point& _point1, const Point& _point2) 
    : point1(_point1), point2(_point2) {

    //�� ���� ������ ������� ������������ ��������� ������
    auto x1 = point1.getX();
    auto y1 = point1.getY();
    auto x2 = point2.getX();
    auto y2 = point2.getY();

    a = y1 - y2;
    b = x2 - x1;
    c = x1 * y2 - x2 * y1;
}

Wall::~Wall() {
    for (auto m : doorPoints) {
        delete m;
    }
}

Wall::Wall(const Wall& other) 
    : Wall(other.point1, other.point2) {
    this->intersectionPointsAndWallsTheyIntersectsOn = other.intersectionPointsAndWallsTheyIntersectsOn;
    //�������� ������ �������, �������� ������
    for (auto m : other.doorPoints) {
        this->doorPoints.push_back(new DoorPointWithSurroundingLines(*m));
    }
}

Wall::Wall(Wall&& other) noexcept 
    : Wall(other.point1, other.point2) {
    this->intersectionPointsAndWallsTheyIntersectsOn = other.intersectionPointsAndWallsTheyIntersectsOn;
    //���������� ������ ���������� �������...
    for (auto m : other.doorPoints) {
        this->doorPoints.push_back(m);
    }
    //... � ������� �������� ��������� ������
    other.doorPoints.clear();
}

Wall& Wall::operator=(const Wall& other) {
    if (*this == other) {
        //�������� �� ����������������
        return *this;
    }

    this->point1 = other.point1;
    this->point2 = other.point2;
    this->a = other.a;
    this->b = other.b;
    this->c = other.c;
    this->intersectionPointsAndWallsTheyIntersectsOn = other.intersectionPointsAndWallsTheyIntersectsOn;
    //�������� ������ �������, �������� ������
    for (auto m : other.doorPoints) {
        this->doorPoints.push_back(new DoorPointWithSurroundingLines(*m));
    }

    return *this;
}

Wall& Wall::operator=(Wall&& other) noexcept {
    if (*this == other) {
        //�������� �� ����������������
        return *this;
    }

    this->point1 = other.point1;
    this->point2 = other.point2;
    this->a = other.a;
    this->b = other.b;
    this->c = other.c;
    this->intersectionPointsAndWallsTheyIntersectsOn = other.intersectionPointsAndWallsTheyIntersectsOn;
    //���������� ������ ���������� �������...
    for (auto m : other.doorPoints) {
        this->doorPoints.push_back(m);
    }
    //... � ������� �������� ��������� ������
    other.doorPoints.clear();

    return *this;
}

bool Wall::operator==(const Wall& other) const {
    //��� ��������� ���������� �������, ��� ����������� ����� ���� �����.
    return this->point1 == other.point1 && this->point2 == other.point2;
}

void Wall::findIntersectionPoint(Wall* other) {
    //����������, ����� ������ ������� �� ���� �������� ��������� ������ ����:
    // (a1 b1)  x = c1
    // (a2 b2)  y = c2
    //������� ������������ �������
    const auto determinant = this->a * other->b - this->b * other->a;
    if (determinant == 0) {
        //������� ���������, ����� �����������
        return;
    }

    //������� ����� �����������
    const double xIntersectionPoint = (this->b * other->c - other->b * this->c) / determinant;
    const double yIntersectionPoint = (other->a * this->c - this->a * other->c) / determinant;

    if (xIntersectionPoint < 0 || xIntersectionPoint > 100
        || yIntersectionPoint < 0 || yIntersectionPoint > 100) {
        //����� ����������� ��������� ��� ��������, � ������� �� ���� �������
        return;
    }

    auto point = Point(xIntersectionPoint, yIntersectionPoint);

    //��������� � ��������� ����� � �����, ��� ����������� � ������� ��� ����� ����������
    std::pair<Point, Wall*> p(point, other);
    intersectionPointsAndWallsTheyIntersectsOn.push_back(p);
}

void Wall::sortIntersectionPointsAlongWall() {
    //���������� ��������� ������� ����������� ����������
    //����������� �������������� ��������� - O(N*logN)
    std::sort(intersectionPointsAndWallsTheyIntersectsOn.begin(), intersectionPointsAndWallsTheyIntersectsOn.end(), 
        [&](const std::pair<Point, Wall*>& first, const std::pair<Point, Wall*>& second) -> bool {
        //���������� ���������� �� ����� �� �����, ������� �� ������ �����. ���� ��� ����� point1.
        auto distanceFromPoint1ToFirst = point1.distance(first.first);
        auto distanceFromPoint1ToSecond = point1.distance(second.first);
        return distanceFromPoint1ToFirst < distanceFromPoint1ToSecond;
    });
}

void Wall::findDoorPoints() {
    //����������� �� ���� ������ ����������� ���� ����� � ������� ������� (���� ������ �� size()-1, �.�. � ����� ����� ���� i+1)
    for (int i = 0; i < (int)intersectionPointsAndWallsTheyIntersectsOn.size() - 1; i++) {
        //����� ����� ����������� � ��������� i � i+1.
        //�.�. ��� ���� �������������, �������������� ��� ���� ����� �������.
        const auto& crossoverPointI = intersectionPointsAndWallsTheyIntersectsOn[i].first;
        const auto& crossoverPointIPlus1 = intersectionPointsAndWallsTheyIntersectsOn[i + 1].first;
        //������� ���������� ����� ���������� ����� ����.
        double xMiddlePoint = (crossoverPointI.getX() + crossoverPointIPlus1.getX()) / 2;
        double yMiddlePoint = (crossoverPointI.getY() + crossoverPointIPlus1.getY()) / 2;

        Point doorPoint(xMiddlePoint, yMiddlePoint);
        //������ ����� - ���������� ����� - ��� ����������� ����������� �����:
        //1. �� ����� ����� ����� ��� �����. � ����� ������, �� ��� ����� ���� �� this, ������� ��������� ��������� �� ���.
        //2. ����� ����� � ������ - �.�. ����� ������ ����� ������� ��������� ���� �����. ��������� �� ��� ��� �� ���������.
        auto dpwsl = new DoorPointWithSurroundingLines(doorPoint, this, intersectionPointsAndWallsTheyIntersectsOn[i].second, intersectionPointsAndWallsTheyIntersectsOn[i + 1].second);
        doorPoints.push_back(dpwsl);
    }
}

const std::vector<DoorPointWithSurroundingLines*>& Wall::getDoorPoints() const {
    return doorPoints;
}

bool Wall::isEdgeLine() const {
    return *this == Wall::TOP || 
           *this == Wall::RIGHT ||
           *this == Wall::BOTTOM ||
           *this == Wall::LEFT;
}

bool Wall::arePointsOnTheSameSide(const Point& p1, const Point& p2) const {
    //����� ����� � ����� ������� �� ������, ���� ��� ������������ �� � ��������� ������ � ����������� ����� ���������� �����.
    auto v1 = a * p1.getX() + b * p1.getY() + c;
    auto v2 = a * p2.getX() + b * p2.getY() + c;
    return (v1 > 0 && v2 > 0) || (v1 < 0 && v2 < 0);
}

}