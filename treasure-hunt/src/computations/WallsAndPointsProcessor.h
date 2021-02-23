#pragma once

#include <vector>

#include "../geometry/Wall.h"

namespace TH {

/*
 * \class WallsAndPointsProcessor
 * \brief Класс, который проводит вычисления. Основными объектами, с которыми работает данный класс являются стены и точки, что и отражено в названии.
 */
class WallsAndPointsProcessor {
public:
    /*
     * Конструктор по-умолчанию.
     */
    WallsAndPointsProcessor() = default;
    /*
     * Деструктор.
     */
    ~WallsAndPointsProcessor();
    /*
     * Конструкторы копирования и перемещения, а так же операторы присваивания копированием и перемещением удалены,
     * т.к. копировать этот объект не имеет смысла - он один на всё время выполнения программы.
     */
    WallsAndPointsProcessor(const WallsAndPointsProcessor& other) = delete;
    WallsAndPointsProcessor(WallsAndPointsProcessor&& other) noexcept = delete;
    WallsAndPointsProcessor& operator=(const WallsAndPointsProcessor& other) = delete;
    WallsAndPointsProcessor& operator=(WallsAndPointsProcessor&& other) noexcept = delete;

    /**
     * Добавить новую стену в обработчик.
     * @param newWall новая стена.
     */
    void addWall(const Wall& newWall);

    /**
     * Найти всевозможные двери, которые только можно сделать в хранилище.
     * Из условия задачи, двери можно пробивать в стене только в точке, являющейся срединной в стене в промежутке, зажатом между двумя соседними стенами.
     */
    void findDoorLocations();

    /**
     * Теперь, когда мы знаем местоположение всех дверей, мы можем найти всевозможные варианты "как мы можем добраться от одной двери к другой".
     * Говоря чуть более математическим языком - у нас есть все вершины графа, теперь нужно найти все его ребра.
     * @pararm treasurePoint точка, представляющая из себя координаты искомого сокровища
     */
    void calculateInternalStructureOfPyramid(const Point& treasurePoint);

    /**
     * У нас есть полноценный граф и теперь с помощью поиска в ширину нужно найти, сколько стен нужно проломить, чтобы добраться от местоположения сокровищ до любой из внешних стен.
     */
    int findShortestWayByBfs();

private:
    /**
     * У нас есть первая и вторая точка комнаты, теперь перебирая стены слева (условно говоря, конкретно - мы будем брать всегда "wall1") 
     * нам нужно добраться обратно до стены, хранящейся в first.
     * Если вдруг окажется, что в этой комнате лежит сокровище, то единственным элементом списка будет объект DoorPointWithSurroundingLines с координатами сокровища.
     * @pararm first хранит информацию о двери (точке), с которой мы начали отсчет.
     * @pararm second вторая дверь, она определяет в какую сторону пойдет комната (поскольку, если стена, на которой лежит точка из first не является внешней, то есть два варианта комнат).
     * @return возвращает вектор объектов, хранящих координаты дверей, находящихся в одной комнате (а так же стены, на которых эта дверь находится и т.д, см DoorPointWithSurroundingLines). Не включая стартовый объект first.
     */
    std::vector<DoorPointWithSurroundingLines*> findMeaningfullPointsInTheRoom(DoorPointWithSurroundingLines* first, DoorPointWithSurroundingLines* second);

    /**
     * Рекурсивная функция, ищущая необходимый дверной проем на двери слева (условно - слева, по факту - "wall1").
     * @pararm room вектор объектов, содержащих двери, передается по ссылке.
     */
    void fillRoomWithWalls(std::vector<DoorPointWithSurroundingLines*>& room) const;

    /*
     * Набор стен, по-умолчанию включаем внешние стены пирамиды
     */
    std::vector<Wall> walls = { Wall::TOP,
                                Wall::RIGHT,
                                Wall::BOTTOM,
                                Wall::LEFT };

    /*
     * Координаты сокровища.
     */
    DoorPointWithSurroundingLines* treasurePoint = nullptr;

    /*
     * Расстояние между двумя дверьми по-умолчанию. Так как мы делаем проверку на уменьшение, то оно заведомо очень большое, чтобы любое реальное значение было меньше.
     */
    static constexpr double DEFAULT_SHORTEST_DISTANCE = 1000;
};

}

