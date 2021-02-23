#pragma once

#include <vector>

#include "../geometry/Line.h"

namespace TH {

class LineHandler {
public:
    LineHandler() = default;
    ~LineHandler();
    LineHandler(const LineHandler& other) = delete;
    LineHandler(LineHandler&& other) noexcept = delete;
    LineHandler& operator=(const LineHandler& other) = delete;
    LineHandler& operator=(LineHandler&& other) noexcept = delete;

    void addLine(const Line& newLine);

    void findNodes();

    void buildAdjacencyMatrix(const Point& treasurePoint);

    int findShortestWayByBfs();

private:
    std::vector<PointBetweenLines*> createRoom(PointBetweenLines* first, PointBetweenLines* second);

    void fillRoom(std::vector<PointBetweenLines*>& room) const;


    /*
     * Набор линий, по-умолчанию включаем границы квадрата
     */
    std::vector<Line> lines = { Line::TOP,
                                Line::RIGHT,
                                Line::BOTTOM,
                                Line::LEFT };

    PointBetweenLines* treasurePoint = nullptr;

    static constexpr double DEFAULT_SHORTEST_DISTANCE = 1000;
};

}

