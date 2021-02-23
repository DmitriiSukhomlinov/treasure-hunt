#pragma once

#include "Point.h"

#include <vector>

namespace TH {

class Wall;

/*
 * \struct DoorPointWithSurroundingLines
 * \brief ��������� ������, ��������, doorPoint - ���������� �������� ������, linePointBelongs2 - �����, ������� ��� ����� �����������, wall1 � wall2 - ����� ����� � ������,
 * doorsThatCanBeReached - �����, ������� ����� ���������� �� ����� doorPoint, �.�. �����, ������� ��������� � ��� �� ��������, ��� � ����� doorPoint 
 * (������� ����, ���� � ��� "�������" ����� � ������� ���, ���� ����� "����������").
 * ��� �� �������� ����, ����������� ��� ������ � ������: used - ���� �� ������� ��������� � distanceFromTreasure - ���������� ����, ������� ����� �������, ����� ��������� �� ���������. 
 */
struct DoorPointWithSurroundingLines {
     /*
     * �����������.
     * @param point ���������� �����.
     * @param linePointBelongs2 ��������� �� �����, �� ������� ��� ����� �����.
     * @param wall1 ��������� �� ����� � ����� �������
     * @param wall1 ��������� �� ����� � ������ ������
     */
    DoorPointWithSurroundingLines(const Point& doorPoint, Wall* wallPointBelongs2, Wall* wall1, Wall* wall2);

    /*
     * ���������� �����.
     */
    Point doorPoint;
    /* 
     * ��������� �� �����, �� ������� ��� ����� �����.
     */
    Wall* wallPointBelongs2 = nullptr;
    /*
     * ��������� �� ����� � ����� �������
     */
    Wall* wall1 = nullptr;
    /*
     * ��������� �� ����� � ������ ������
     */
    Wall* wall2 = nullptr;
    /*
     * ����� �� ������, � � ������� ��������� ����� � ������������ doorPoint (������� ����, ���� ����� wallPointBelongs2 ������� � ��� - ���� ����������)
     */
    std::vector<DoorPointWithSurroundingLines*> doorsThatCanBeReached;

    /*
     * ��� ������ � ������ - ���� �� ������� ����������, �.�. ��� ������ ��� ������.
     */
    bool used = false;
    /*
     * ��� ������ � ������ - ���������� ������, ������� ���� �������, ����� ��������� �� ��������� �� doorPoint (������� ��������� doorPoint).
     */
    int distanceFromTreasure = 0;
};

}