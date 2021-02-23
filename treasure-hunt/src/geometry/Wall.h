#pragma once

#include "DoorPointWithSurroundingLines.h"
#include "Point.h"

#include <vector>

namespace TH {

/*
 * \class Wall
 * \brief �����, �������������� �� ���� �����. ����������, ����� - ��� �����, � ����� ����� ��������� �� ���� ������.
 */
class Wall {
public:
    /*
     * �����������.
     * @param point1 ������ �����, �� ������� ����� ��������� ����� (�����).
     * @param point2 ����� �����, �� ������� ����� ��������� ����� (�����).
     */
    Wall(const Point& point1, const Point& point2);
    /*
     * ����������.
     */
    ~Wall();
    /*
     * ����������� �����������.
     * @param other ����������� ������ �� ������, ������� ����� �����������.
     */
    Wall(const Wall& other);
    /*
     * ����������� �����������.
     * @param other r-value ������ �� ������, ������� ����� �����������.
     */
    Wall(Wall&& other) noexcept;
    /*
     * �������� ������������ ������������.
     * @param other ����������� ������ �� ������, ������� ����� �����������.
     * @return ������������� ������
     */
    Wall& operator=(const Wall& other);
    /*
     * �������� ������������ ������������.
     * @param other r-value ������ �� ������, ������� ����� �����������.
     * @return ������������ ������
     */
    Wall& operator=(Wall&& other) noexcept;
    /*
     * �������� ���������.
     * @param other ������, � ������� ���������� ��� ������ this
     * @return true ���� �����, false �����
     */
    bool operator==(const Wall& other) const;

    /*
     * ����� ����� ����������� ������� ����� ������, ������������ ����������.
     * ����� ����������� � ���� �� ������ other ��������� � ������ intersectionPointsAndWallsTheyIntersectsOn
     * @param other ��������� �� ������ Wall, � ������� ����� ����� ����� �����������
     */
    void findIntersectionPoint(Wall* other);
    /*
     * ����� ����, ��� ����� ����������� �� ����� ������� ���� ������� �� ����� ������������� �� ������� "����� �����".
     * ��� ����� ��� ����, ����� ����� ���� ����� ����� � ��������� i � i+1 � ���� ��������, ��� ����� ����� ����� ������� ��� ��� �����.
     * �.�. ��� ��� ����� ���-�� ������������ �� ���� ���� ����� �������.
     */
    void sortIntersectionPointsAlongWall();
    /*
     * ����� ��� �����, ���������� �������������� �������, ������� ����� ������ ��� ����, ����� ������ � ����������.
     * ��� ����� �� ����� ���� ��������� � ��������������� "����� �����" ����� (� ��������� i � i+1) � ������� �������� ����� ����.
     * ��������� ����� ��������� � doorPoints ������ �� ������, ������� ��� ����������� � ����� �������� �������, ����� �������� ��������� ��� ������� �����.
     */
    void findDoorPoints();

    /*
     * ���������� ������ �����-������.
     * @return ����������� ������ �� ������ doorPoints.
     */
    const std::vector<DoorPointWithSurroundingLines*>& getDoorPoints() const;
    /*
     * ���������, �������� �� ������� ����� "�������".
     * @return true ���� ����� �������, false ���� ����������.
     */
    bool isEdgeLine() const;
    /*
     * ���������, ����� �� ��� ����� � ����� ������� �� ����� (����������, � �������������� ������, ������) ��� ���.
     * @param p1 ������ �����.
     * @param p2 ������ �����.
     * @return true ���� ����� ����� � ����� ������� �� �����, false � ��������� ������.
     */
    bool arePointsOnTheSameSide(const Point& p1, const Point& p2) const;

    /*
     * ������� ������� �����.
     */
    static const Wall TOP;
    /*
     * ������ ������� �����.
     */
    static const Wall RIGHT;
    /*
     * ������ ������� �����.
     */
    static const Wall BOTTOM;
    /*
     * ����� ������� �����.
     */
    static const Wall LEFT;

private:
    /*
     * ����� 1, �� ������� ��������� ����� (�����).
     */
    Point point1;
    /*
     * ����� 2, �� ������� ��������� ����� (�����).
     */
    Point point2;

    /*
     * ��������� ������ �� ���� ������ ����� ����� ���:
     * (y1 - y2)x + (x2 - x1)y + (x1*y2 - x2*y1) = 0 ���
     * ax + by + c = 0
     * ��������, ��� 
     * (y1 - y2) = a
     * (x2 - x1) = b
     * (x1*y2 - x2*y1) = c
     */

     /*
     * ax + by + c = 0
     * ��� "a" � ���� ��������� ������. 
     */
    double a = 0;
    /*
     * ax + by + c = 0
     * ��� "b" � ���� ��������� ������. 
     */
    double b = 0;
    /*
     * ax + by + c = 0
     * ��� "c" � ���� ��������� ������.
     */
    double c = 0;

    /*
     * ������ ��� ���������� ����������:
     * second - ��������� �� ������ ����� (�� ���������� this).
     * first - ����� ����������� ����� this �� ������ second.
     */
    std::vector<std::pair<Point, Wall*>> intersectionPointsAndWallsTheyIntersectsOn;

    /*
     * ������, ���������� ����� (� ����������� � ��� ������, ��. �������� DoorPointWithSurroundingLines), ������� �������� ������� � ����� this.
     */
    std::vector<DoorPointWithSurroundingLines*> doorPoints;
};

}
