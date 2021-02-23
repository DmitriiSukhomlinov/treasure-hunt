#include <iostream>
#include <fstream>
#include <vector>

#include "computations/WallsAndPointsProcessor.h"
#include "geometry/Wall.h"

/*
 * ���������� �������, ��� ���, 4 ����, 17121
 * 
 * ����� ���� ������� - ����������, ������� ������ ������������ �� ���� ���������� ������� �����.
 * ����� � ������ ��������� - ��� ��� �������. ��� �� �������� �������� �����, ��� ��������� ���������.
 * ������ ������ ������� ��� ����� ��������� ����� �����, ������� ����� �����. ���������� - ������� � ����������, ��� ���������� ��������� ����� ������ � ��������������� ���������.
 * ����� ��������� ���� ���� � ��������� ����� � ����� ����� ��������� ���� �� �������, ������� �������� ��������������� ���������, �� ����� �� ������, ����������� �� "������� ������" ��������.
 * ����������� ����� ������������ ����� "� ������", �.�. ��� ����� ������ ������ �� ��������� ���� �������� ����������, � ��� ������ "� �������" ���������� ������� ����� ��� ����, � ����� �������� ��.
 * 
 */

int main() {
    int wallsNumber = 0;
    std::ifstream fin("input.txt");
    //��������� ���������� ����
    fin >> wallsNumber;
    //������� ����������, ������� ����� ���������� ����� ������������
    TH::WallsAndPointsProcessor processor;
    for (int i = 0; i < wallsNumber; i++) {
        double x1 = 0;
        double y1 = 0;
        double x2 = 0;
        double y2 = 0;
        //��������� ���������� �����
        fin >> x1 >> y1 >> x2 >> y2;
        
        //��������� ����� ����� � ����������
        processor.addWall(TH::Wall(TH::Point(x1, y1), TH::Point(x2, y2)));
    }
    //������� �������������� �������������� ���� ������ � ������
    processor.findDoorLocations();

    double xTresure = 0;
    double yTreasure = 0;
    //��������� ���������� ���������
    fin >> xTresure >> yTreasure;
    //������������ ������������ ���� �� ��������� ����� ������������ �����, �.�. ������ ����
    processor.calculateInternalStructureOfPyramid(TH::Point(xTresure, yTreasure));

    //��������� ����� � ������ ������� ���������� ���� � ����� �� �������, ������� �������� ������������ ���������, �� ����� �������, ������� �� ������� ������.
    auto numberOfDoors = processor.findShortestWayByBfs();

    std::cout << "Number of doors = " << numberOfDoors << std::endl;

    return 0;
}