#include "LineHandler.h"

#include <algorithm>
#include <cassert>
#include <queue>

namespace TH {
LineHandler::~LineHandler() {
    delete treasurePoint;
}

void LineHandler::addLine(const Line& newLine) {
    lines.push_back(newLine);
}

void LineHandler::findNodes() {
    for (int i = 0; i < (int)lines.size(); i++) {
        for (int j = 0; j < (int)lines.size(); j++) {
            if (i == j) {
                continue;
            }

            lines[i].findCrossoverPoint(&lines[j]);
        }

        lines[i].sortCrossoverPointsAlongLine();
        lines[i].findMiddlePoints();
    }
}

void LineHandler::buildAdjacencyMatrix(const Point& _treasurePoint) {
    treasurePoint = new PointBetweenLines(_treasurePoint, nullptr, nullptr, nullptr);

    for (const Line& line : lines) {
        for (auto middlePoint : line.getMiddlePoints()) {
            const auto& additionalMiddlePoints = middlePoint->line1->getMiddlePoints();
            std::pair<double, PointBetweenLines*> shortestDistance(DEFAULT_SHORTEST_DISTANCE, additionalMiddlePoints.front());
            std::pair<double, PointBetweenLines*> secondShortestDistance(DEFAULT_SHORTEST_DISTANCE, additionalMiddlePoints.front());

            for (const auto& additionalMiddlePoint : additionalMiddlePoints) {
                double distance = middlePoint->point.distance(additionalMiddlePoint->point);
                if (distance < shortestDistance.first) {
                    if (shortestDistance.first == DEFAULT_SHORTEST_DISTANCE) {
                        shortestDistance = std::pair<double, PointBetweenLines*>(distance, additionalMiddlePoint);
                    } else if (line.arePointsOnTheSameSide(shortestDistance.second->point, additionalMiddlePoint->point)) {
                        shortestDistance = std::pair<double, PointBetweenLines*>(distance, additionalMiddlePoint);
                    } else {
                        secondShortestDistance = std::pair<double, PointBetweenLines*>(distance, additionalMiddlePoint);
                        break;
                    }
                } else if (!line.arePointsOnTheSameSide(shortestDistance.second->point, additionalMiddlePoint->point)) {
                    secondShortestDistance = std::pair<double, PointBetweenLines*>(distance, additionalMiddlePoint);
                    break;
                }
            }

            std::vector<PointBetweenLines*> firstRoom = createRoom(middlePoint, shortestDistance.second);
            if (!line.isEdgeLine()) {
                std::vector<PointBetweenLines*> secondRoom = createRoom(middlePoint, secondShortestDistance.second);

                firstRoom.insert(firstRoom.end(), secondRoom.begin(), secondRoom.end());
            }

            middlePoint->adjacentGraphPoints = { firstRoom };
        }
    }
}

int LineHandler::findShortestWayByBfs() {
    std::queue<PointBetweenLines*> q;
    q.push(treasurePoint);
    treasurePoint->used = true;

    PointBetweenLines* finishPoint = nullptr;
    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        for (auto neighbor : current->adjacentGraphPoints) {
            if (!neighbor->used) {
                q.push(neighbor);
                neighbor->used = true;
                neighbor->distanceFromTreasure = current->distanceFromTreasure + 1;
                if (neighbor->linePointBelongs2->isEdgeLine()) {
                    finishPoint = neighbor;
                    std::queue<PointBetweenLines*> empty;
                    std::swap(q, empty);
                    break;
                    
                }
            }
        }
    }

    assert(finishPoint != nullptr);
    
    return finishPoint->distanceFromTreasure;
}

std::vector<PointBetweenLines*> LineHandler::createRoom(PointBetweenLines* first, PointBetweenLines* second) {
    std::vector<PointBetweenLines*> result = { first };
    result.push_back(second);
    fillRoom(result);
    result.erase(result.begin());
    bool correct = true;
    for (auto pointFromRoom : result) {
        if (!first->linePointBelongs2->arePointsOnTheSameSide(pointFromRoom->point, treasurePoint->point)) {
            correct = false;
            break;
        }

        if (!pointFromRoom->linePointBelongs2->arePointsOnTheSameSide(first->point, treasurePoint->point)) {
            correct = false;
            break;
        }
    }
    if (correct) {
        result.clear();
        result.push_back(treasurePoint);
        treasurePoint->adjacentGraphPoints.push_back(first);
    }

    return result;
}

void LineHandler::fillRoom(std::vector<PointBetweenLines*>& room) const {
    const auto& lastPoint = room.back();
    const auto& pointBeforeLast = room.at(room.size() - 2);
    Line* nexLine2Check = nullptr;
    if (pointBeforeLast->linePointBelongs2 == lastPoint->line1) {
        nexLine2Check = lastPoint->line2;
    } else if (pointBeforeLast->linePointBelongs2 == lastPoint->line2) {
        nexLine2Check = lastPoint->line1;
    } else {
        assert(false);
    }

    const auto& middlePoints = nexLine2Check->getMiddlePoints();
    std::pair<double, PointBetweenLines*> shortestDistance(DEFAULT_SHORTEST_DISTANCE, middlePoints.front());
    for (const auto& middlePoint : middlePoints) {
        bool onTheSameSide = lastPoint->linePointBelongs2->arePointsOnTheSameSide(room.front()->point, middlePoint->point);
        if (!onTheSameSide) {
            continue;
        }

        double distance = middlePoint->point.distance(lastPoint->point);
        if (distance < shortestDistance.first) {
            shortestDistance = std::pair<double, PointBetweenLines*>(distance, middlePoint);
        }
    }

    room.push_back(shortestDistance.second);
    if (shortestDistance.second->line1 == room.front()->linePointBelongs2 
     || shortestDistance.second->line2 == room.front()->linePointBelongs2) {
        return;
    }
    fillRoom(room);
}

}
