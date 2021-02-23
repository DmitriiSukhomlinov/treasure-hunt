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


int main(int argc, char* argv[]) {
    //���� ������
    if (argc == 1) {
        std::cout << "Treasure Hunt." << std::endl;
        std::cout << std::endl;
        std::cout << "[OPTION] - Path to the input file. The only argument." << std::endl;
        std::cout << "Example: \"treasure-hunt.exe C:\\inputs\\input.txt\"." << std::endl;
        return 0;
    }

    //���������� ������ ���� 2
    if (argc != 2) {
        std::cout << "Incorrect input." << std::endl;
        return 1 ;
    }

    //� ����� �� ������ �������� "�� ������", ����� �������, ��� � ��� ������ ���������� ������ �������� �����.
    int wallsNumber = 0;
    std::ifstream fin(argv[1]);
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
    fin.close();
    //������������ ������������ ���� �� ��������� ����� ������������ �����, �.�. ������ ����
    processor.calculateInternalStructureOfPyramid(TH::Point(xTresure, yTreasure));

    //��������� ����� � ������ ������� ���������� ���� � ����� �� �������, ������� �������� ������������ ���������, �� ����� �������, ������� �� ������� ������.
    auto numberOfDoors = processor.findShortestWayByBfs();

    std::cout << "Number of doors = " << numberOfDoors << std::endl;

    return 0;
}