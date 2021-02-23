#pragma once

#include "Point.h"

#include <vector>

namespace TH {

class Wall;

/*
 * \struct DoorPointWithSurroundingLines
 * \brief Структура данных, хранящая, doorPoint - координаты дверного проема, linePointBelongs2 - стену, которая это точка принадлежит, wall1 и wall2 - стены следа и справа,
 * doorsThatCanBeReached - двери, которые можно достигнуть из двери doorPoint, т.е. двери, которые находятся в тех же комнатах, что и дверь doorPoint 
 * (комната одна, если у нас "боковая" стена и комнаты две, если стена "внутренняя").
 * Так же содержит поля, необходимые для поиска в ширину: used - была ли вершина развёрнута и distanceFromTreasure - количество стен, которые нужно сломать, чтобы добраться до сокровища. 
 */
struct DoorPointWithSurroundingLines {
     /*
     * Конструктор.
     * @param point координата двери.
     * @param linePointBelongs2 указатель на стену, на которой эта дверь лежит.
     * @param wall1 указатель на стену с одной стороны
     * @param wall1 указатель на стену с другой сторон
     */
    DoorPointWithSurroundingLines(const Point& doorPoint, Wall* wallPointBelongs2, Wall* wall1, Wall* wall2);

    /*
     * Координата двери.
     */
    Point doorPoint;
    /* 
     * Указатель на стену, на которой эта дверь лежит.
     */
    Wall* wallPointBelongs2 = nullptr;
    /*
     * Указатель на стену с одной стороны
     */
    Wall* wall1 = nullptr;
    /*
     * Указатель на стену с другой сторон
     */
    Wall* wall2 = nullptr;
    /*
     * Двери из комнат, в в которых находится дверь с координатами doorPoint (комната одна, если стена wallPointBelongs2 боковая и две - если внутренняя)
     */
    std::vector<DoorPointWithSurroundingLines*> doorsThatCanBeReached;

    /*
     * Для поиска в ширину - была ли вершина развернута, т.е. уже учтена при поиске.
     */
    bool used = false;
    /*
     * Для поиска в ширину - количество дверей, которые надо сломать, чтобы добраться от сокровища до doorPoint (включая сломанную doorPoint).
     */
    int distanceFromTreasure = 0;
};

}