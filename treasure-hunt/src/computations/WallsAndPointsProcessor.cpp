#include "WallsAndPointsProcessor.h"

#include <algorithm>
#include <cassert>
#include <queue>

namespace TH {

WallsAndPointsProcessor::~WallsAndPointsProcessor() {
    delete treasurePoint;
}

void WallsAndPointsProcessor::addWall(const Wall& newWall) {
    walls.push_back(newWall);
}

void WallsAndPointsProcessor::findDoorLocations() {
    //��������� ������ ����� � ������
    for (int i = 0; i < (int)walls.size(); i++) {
        //������ ����� ����������� ������� ����� �� ����� ����������
        for (int j = 0; j < (int)walls.size(); j++) {
            if (i == j) {
                continue;
            }

            //������� ����������� ����� i �� ������ j
            walls[i].findIntersectionPoint(&walls[j]);
        }

        //���������� ������������� ��������� ����� ����������� "����� �����"
        walls[i].sortIntersectionPointsAlongWall();
        //���� ��� �����, ��������������� ������, ������� ����� �������
        walls[i].findDoorPoints();
    }
}

void WallsAndPointsProcessor::calculateInternalStructureOfPyramid(const Point& _treasurePoint) {
    //�������� ������� ��������� ���������, ��� ��� ��� ����������� ��� ���� �������.
    //�.�. ��������� �� ����� �� �� ����� ������, �������� � �������� ���������� �� ��� nullptr.
    treasurePoint = new DoorPointWithSurroundingLines(_treasurePoint, nullptr, nullptr, nullptr);

    //���������� �� ���� ������
    for (const Wall& wall : walls) {
        //� ������ ����� ���������� �� ������� �������, ������� �� ��� ����������� (����� �������������)
        for (auto doorPoint : wall.getDoorPoints()) {
            //���������� ���������� ����� doorPoint. ��� - ��� �������� ������ ������, ��� ���������� �� ������ ��� ����, ����� ������, � ����� ������ ������ ����� ������ �� ����� "doorPoint"
            //��� ��������� � �����, ������� ������ ����� ����� ������� �������.
            //�� ������� ���� �� ���� ���� ���� (��������, "wall1", ����� �������� �� ��� �������� "����� �����", ���� ��� ����� ���� � ����� �������) 
            //� ������ �� ����� ����� ����� ��� ����� (������� �� "��������"), 
            //����������� � ����� doorPoint � ����� ������� (������� ����, ���� �����, �� ������� ��������� doorPoint �������, ��� - ���� ����������).
            //��� ����, ����� ����� "��������" ����� �� ���������� �� ���� ������ ����� ����� � ������ ��� �����, ��������� � ��������������� ����� "doorPoint",
            //������ �����, ��� ��� ������ ��������� � ������ ������ �� �����, �� ������� ��������� doorPoint (������� �� � ������ ������).
            const auto& additionalDoorPoints = doorPoint->wall1->getDoorPoints();
            //��� ��� ���� ������� - ������� ����� ���� ���, ��������������, ����� ���� ���.
            //�������, ������� ��� �������, � ������� "��������" ����� - ���� ��� ������� � �����, ������ ��� ������� � ������ �������.
            //������ ������ ����� ������� ���������� ����������, ������ - ������ �� ��������.
            //DEFAULT_SHORTEST_DISTANCE - ����� �������, ����� �������� ����������  ����� ����� ������� ����� ������
            std::pair<double, DoorPointWithSurroundingLines*> shortestDistance(DEFAULT_SHORTEST_DISTANCE, additionalDoorPoints.front());
            std::pair<double, DoorPointWithSurroundingLines*> secondShortestDistance(DEFAULT_SHORTEST_DISTANCE, additionalDoorPoints.front());

            //����������� �� ���� ������ �� ����� �����
            for (const auto& wall1DoorPoint : additionalDoorPoints) {
                //��������� ���������� �� ��������������� ����� "doorPoint" �� ����� �� ����� �����
                double distance = doorPoint->doorPoint.distance(wall1DoorPoint->doorPoint);
                //���� ���������� ������ ����, ������� �������� � �������� � ������������� �����������...
                if (distance < shortestDistance.first) {
                    //���� ��� �� ���� ��� �� ������ �������� - ���������� �������.
                    if (shortestDistance.first == DEFAULT_SHORTEST_DISTANCE) {
                        shortestDistance = std::pair<double, DoorPointWithSurroundingLines*>(distance, wall1DoorPoint);
                    } else if (wall.arePointsOnTheSameSide(shortestDistance.second->doorPoint, wall1DoorPoint->doorPoint)) {
                        //���� � shortestDistance ��� ��������� �����, � ��, ����������, ����� ����� ����� � ����������� �� doorPoint, �������, ��� ���������,
                        //�� ��� �� ���� ����� ��������� ������ ����� ������, � ������� ���������, ��� ��� ��� ����� ��������� � ����� ������� �� �����, �� ������� ��������� doorPoint
                        //������ ��� ���� ���, �� ��� ������, ��� �� ��� ������������� ������� � ������ �������.
                        shortestDistance = std::pair<double, DoorPointWithSurroundingLines*>(distance, wall1DoorPoint);
                    } else {
                        //���� �� ������ ����, ������, �� ������ ������������� ����� � ������ �������. � ���� ������, ������ ����������� �� �� ������ ���������.
                        //�������� ����� �����������, �� ������ �� ����� ������ ����������, ������, ����� ����� �� �����.
                        secondShortestDistance = std::pair<double, DoorPointWithSurroundingLines*>(distance, wall1DoorPoint);
                        break;
                    }
                } else if (!wall.arePointsOnTheSameSide(shortestDistance.second->doorPoint, wall1DoorPoint->doorPoint)) {
                    //���� �� ������ ����, ������ �� ����������� ��� ����� � ����� ������� �� ����� � ������ �������� ������������� ����� � ������ �������
                    //� ���� ������, ������ �� ��������� �� ��������� - ������ ����������� �� � ������� �� �����
                    secondShortestDistance = std::pair<double, DoorPointWithSurroundingLines*>(distance, wall1DoorPoint);
                    break;
                }
            }

            //������ � ��� ���� ���� �������� ����� doorPoint � ����� �� ����� �����. ���������� ���� � ���, ��� �� ����� "�����������" �������, 
            //������ ��� �������� � ��� ����� ����� (��. createRoom()).
            std::vector<DoorPointWithSurroundingLines*> firstRoom = findMeaningfullPointsInTheRoom(doorPoint, shortestDistance.second);
            //���� � ��� �� ������� �����, �� � ��� ��� �������...
            if (!wall.isEdgeLine()) {
                //...������� ����������� �� �� ����� �� ������
                std::vector<DoorPointWithSurroundingLines*> secondRoom = findMeaningfullPointsInTheRoom(doorPoint, secondShortestDistance.second);

                //���������� �� ��������� �� ����� ������ �����
                firstRoom.insert(firstRoom.end(), secondRoom.begin(), secondRoom.end());
            }

            //��� ��������� ����� ���������� � ���� �� ����� doorPoint
            doorPoint->doorsThatCanBeReached = { firstRoom };
        }
    }
}

int WallsAndPointsProcessor::findShortestWayByBfs() {
    //������� ����� � ������, �� �����������
    //������� �������
    std::queue<DoorPointWithSurroundingLines*> q;
    //�������� � ��� ��������� �������
    q.push(treasurePoint);
    //�������� ��������� ������� ��� �����������
    treasurePoint->used = true;

    //���� �� ������� �������� �����, ����� ������.
    DoorPointWithSurroundingLines* finishPoint = nullptr;
    //�� ��� ���, ���� ������� �� �����
    while (!q.empty()) {
        //��������� ������� �� �������
        auto current = q.front();
        q.pop();

        //����������� �� ���� �� �������� ��������
        for (auto neighbor : current->doorsThatCanBeReached) {
            //���� ������� ���������, �� ��� ��� � �������, ������� ���������� �������� ������ ���� ������� �� ���������
            if (!neighbor->used) {
                //��������� ������� � �������
                q.push(neighbor);
                //�������� �� ��� �����������
                neighbor->used = true;
                //� ���������� �� ������� current ��������� 1 � ����������
                neighbor->distanceFromTreasure = current->distanceFromTreasure + 1;
                //���� ������� ����� �� ������� �����, �� �� ����� ��, ��� ������
                if (neighbor->wallPointBelongs2->isEdgeLine()) {
                    //���������� ��������� �������
                    finishPoint = neighbor;
                    //������� �������, ������ swap � ������ ����������.
                    //������� �����, ������������� ���������� while ���������.
                    std::queue<DoorPointWithSurroundingLines*> empty;
                    std::swap(q, empty);
                    //������� �� �����
                    break;
                }
            }
        }
    }

    assert(finishPoint != nullptr);
    
    return finishPoint->distanceFromTreasure;
}

std::vector<DoorPointWithSurroundingLines*> WallsAndPointsProcessor::findMeaningfullPointsInTheRoom(DoorPointWithSurroundingLines* first, DoorPointWithSurroundingLines* second) {
    //��������� �������, ���������� ��� ��������� ����� � �����, ������� ��� �����������
    std::vector<DoorPointWithSurroundingLines*> result = { first, second };
    //�������� �������, ������� ������� ��� ���������� ������� � ������������ ������ � ����, ��� �� ������� �������.
    //����� ����, ��� fillRoomWithWalls() ����������, � ��� ��������� ������� ������� result ����� ����� ���� "wall1", 
    //����������� �� �� �� �����, ��� � � ������� �������� result ��������� ���� "wallPointBelongs2"
    fillRoomWithWalls(result);

    //��������, ����� �� ��������� � ��������� �������
    bool roomContainsTreasure = true;
    for (int i = 0; i < (int)result.size(); i++) {
        //������� ����� � �������� i � ������� ����� �� ���. 
        DoorPointWithSurroundingLines* nextDoor = nullptr;
        if (i < (int)result.size() - 1) {
            nextDoor = result[i + 1];
        } else {
            nextDoor = result.front();
        }
        
        //"���������" � "����� �� ����� ����� �� ����� � �������� i" ������ ���������� � ����� ������� �� ����� � �������� "i"
        if (!result[i]->wallPointBelongs2->arePointsOnTheSameSide(treasurePoint->doorPoint, nextDoor->doorPoint)) {
            //���� ��� ������� �� ����������� ��� �����-�� ����, �� ��������� � ������� ���.
            roomContainsTreasure = false;
            break;
        }
    }

    //������ ����� ������ �������, �.�. �� ���� ����� ������ � ��� - ����� �� � ����� ������������� ��� ����� � ����� �����.
    result.erase(result.begin());

    //���� ��������� ����� � �������, �� � ���� ������� ��� ���������� ��� ���������� ������ � ����.
    //������� ��� ����� � ��������� ������������ ����� � �����������.
    if (roomContainsTreasure) {
        result.clear();
        result.push_back(treasurePoint);
        //� ���� ����� � ���������� �������� ���� � ���, ��� �� ��� ����� ���������� ����� first
        treasurePoint->doorsThatCanBeReached.push_back(first);
    }

    return result;
}

void WallsAndPointsProcessor::fillRoomWithWalls(std::vector<DoorPointWithSurroundingLines*>& room) const {
    //������� ��������� � ������������� ����������� �����
    const auto& lastDoorPoint = room.back();
    const auto& doorPointBeforeLast = room.at(room.size() - 2);
    Wall* nexLine2Check = nullptr;
    //���� ��������� ����� ���� "�����" �� �������������, �� �� ����� ������������� ���� ����� �� ���������
    if (doorPointBeforeLast->wallPointBelongs2 == lastDoorPoint->wall1) {
        nexLine2Check = lastDoorPoint->wall2;
    } else if (doorPointBeforeLast->wallPointBelongs2 == lastDoorPoint->wall2) {
        //���� ������ - �� ������
        nexLine2Check = lastDoorPoint->wall1;
    } else {
        assert(false);
    }
    
    //���������� ��� ����� �� �����, ������� �� �������������. ������ �� ��� ��������� � ��� �����, ��� ������ � room �� ��������� �����
    const auto& doorPoints = nexLine2Check->getDoorPoints();
    std::pair<double, DoorPointWithSurroundingLines*> shortestDistance(DEFAULT_SHORTEST_DISTANCE, doorPoints.front());
    //��������� �� ���� ������
    for (const auto& middlePoint : doorPoints) {
        //������ ��� �����, ����� ����� ����� ���� � ��� �� ������� �� ��������� �����, ��� � ����� �� ����� ������ �����.
        //���� ��� �� ��� - ��� ����� ��������� � ���� �� � ����� ������� � ��� �� ��������
        bool onTheSameSide = lastDoorPoint->wallPointBelongs2->arePointsOnTheSameSide(room.front()->doorPoint, middlePoint->doorPoint);
        if (!onTheSameSide) {
            continue;
        }

        //���������� ���������� �� ���������� �����, ��� ������ ���� ����������
        double distance = middlePoint->doorPoint.distance(lastDoorPoint->doorPoint);
        if (distance < shortestDistance.first) {
            shortestDistance = std::pair<double, DoorPointWithSurroundingLines*>(distance, middlePoint);
        }
    }

    //��������� ����� � ������
    room.push_back(shortestDistance.second);
    //���� � ������������� ����� ���� �� �������� ���� ��� �� - ��� � ��� � ������ ������, ������, �� �������� ������� � ����� ��������...
    if (shortestDistance.second->wall1 == room.front()->wallPointBelongs2 
     || shortestDistance.second->wall2 == room.front()->wallPointBelongs2) {
        return;
    }
    //...���� �� ��� - ���������� ��������� ������ "�� ��������"
    fillRoomWithWalls(room);
}

}
