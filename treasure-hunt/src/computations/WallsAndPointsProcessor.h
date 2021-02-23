#pragma once

#include <vector>

#include "../geometry/Wall.h"

namespace TH {

/*
 * \class WallsAndPointsProcessor
 * \brief �����, ������� �������� ����������. ��������� ���������, � �������� �������� ������ ����� �������� ����� � �����, ��� � �������� � ��������.
 */
class WallsAndPointsProcessor {
public:
    /*
     * ����������� ��-���������.
     */
    WallsAndPointsProcessor() = default;
    /*
     * ����������.
     */
    ~WallsAndPointsProcessor();
    /*
     * ������������ ����������� � �����������, � ��� �� ��������� ������������ ������������ � ������������ �������,
     * �.�. ���������� ���� ������ �� ����� ������ - �� ���� �� �� ����� ���������� ���������.
     */
    WallsAndPointsProcessor(const WallsAndPointsProcessor& other) = delete;
    WallsAndPointsProcessor(WallsAndPointsProcessor&& other) noexcept = delete;
    WallsAndPointsProcessor& operator=(const WallsAndPointsProcessor& other) = delete;
    WallsAndPointsProcessor& operator=(WallsAndPointsProcessor&& other) noexcept = delete;

    /**
     * �������� ����� ����� � ����������.
     * @param newWall ����� �����.
     */
    void addWall(const Wall& newWall);

    /**
     * ����� ������������ �����, ������� ������ ����� ������� � ���������.
     * �� ������� ������, ����� ����� ��������� � ����� ������ � �����, ���������� ��������� � ����� � ����������, ������� ����� ����� ��������� �������.
     */
    void findDoorLocations();

    /**
     * ������, ����� �� ����� �������������� ���� ������, �� ����� ����� ������������ �������� "��� �� ����� ��������� �� ����� ����� � ������".
     * ������ ���� ����� �������������� ������ - � ��� ���� ��� ������� �����, ������ ����� ����� ��� ��� �����.
     * @pararm treasurePoint �����, �������������� �� ���� ���������� �������� ���������
     */
    void calculateInternalStructureOfPyramid(const Point& treasurePoint);

    /**
     * � ��� ���� ����������� ���� � ������ � ������� ������ � ������ ����� �����, ������� ���� ����� ���������, ����� ��������� �� �������������� �������� �� ����� �� ������� ����.
     */
    int findShortestWayByBfs();

private:
    /**
     * � ��� ���� ������ � ������ ����� �������, ������ ��������� ����� ����� (������� ������, ��������� - �� ����� ����� ������ "wall1") 
     * ��� ����� ��������� ������� �� �����, ���������� � first.
     * ���� ����� ��������, ��� � ���� ������� ����� ���������, �� ������������ ��������� ������ ����� ������ DoorPointWithSurroundingLines � ������������ ���������.
     * @pararm first ������ ���������� � ����� (�����), � ������� �� ������ ������.
     * @pararm second ������ �����, ��� ���������� � ����� ������� ������ ������� (���������, ���� �����, �� ������� ����� ����� �� first �� �������� �������, �� ���� ��� �������� ������).
     * @return ���������� ������ ��������, �������� ���������� ������, ����������� � ����� ������� (� ��� �� �����, �� ������� ��� ����� ��������� � �.�, �� DoorPointWithSurroundingLines). �� ������� ��������� ������ first.
     */
    std::vector<DoorPointWithSurroundingLines*> findMeaningfullPointsInTheRoom(DoorPointWithSurroundingLines* first, DoorPointWithSurroundingLines* second);

    /**
     * ����������� �������, ������ ����������� ������� ����� �� ����� ����� (������� - �����, �� ����� - "wall1").
     * @pararm room ������ ��������, ���������� �����, ���������� �� ������.
     */
    void fillRoomWithWalls(std::vector<DoorPointWithSurroundingLines*>& room) const;

    /*
     * ����� ����, ��-��������� �������� ������� ����� ��������
     */
    std::vector<Wall> walls = { Wall::TOP,
                                Wall::RIGHT,
                                Wall::BOTTOM,
                                Wall::LEFT };

    /*
     * ���������� ���������.
     */
    DoorPointWithSurroundingLines* treasurePoint = nullptr;

    /*
     * ���������� ����� ����� ������� ��-���������. ��� ��� �� ������ �������� �� ����������, �� ��� �������� ����� �������, ����� ����� �������� �������� ���� ������.
     */
    static constexpr double DEFAULT_SHORTEST_DISTANCE = 1000;
};

}

