#pragma once

namespace TH {

/*
 * \class Point
 * \brief ����� � ��������� ������������ � ������������ "x" � "y"
 */
class Point {
public:
    /*
     * �����������.
     * @param x ���������� �� ��� X.
     * @param y ���������� �� ��� Y.
     */
    Point(double x, double y);

    /**
     * �������� ���������, ������������, ����� ��� ����� ��� ���.
     * @param other ������, � ������� ���������� ��� ������ this
     * @return true ���� �����, false �����
     */
    bool operator==(const Point& other) const;

    /**
     * ������� �������� "x"
     * @return �������� �� ��� X
     */
    double getX() const;
    /**
     * ������� �������� "y"
     * @return �������� �� ��� Y
     */
    double getY() const;
    /**
     * ���������� ���������� ����� ����� ������� �� ���������
     * @param other ������, ���������� �� �������� ����� �����
     * @return ���������� �� ����� this �� ����� other
     */
    double distance(const Point& other) const;

private:
    /**
     * ���������� �� ��� �
     */
    double x = -1.0;
    /**
     * ���������� �� ��� Y
     */
    double y = -1.0;
};

}