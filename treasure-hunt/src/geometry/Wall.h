#pragma once

#include "DoorPointWithSurroundingLines.h"
#include "Point.h"

#include <vector>

namespace TH {

/*
 * \class Wall
 * \brief Класс, представляющий из себя стену. Фактически, стена - это линия, а линию можно построить по двум точкам.
 */
class Wall {
public:
    /*
     * Конструктор.
     * @param point1 Первая точка, по которой можно построить линию (стену).
     * @param point2 Вторя точка, по которой можно построить линию (стену).
     */
    Wall(const Point& point1, const Point& point2);
    /*
     * Деструктор.
     */
    ~Wall();
    /*
     * Конструктор копирования.
     * @param other константная ссылка на объект, который нужно скопировать.
     */
    Wall(const Wall& other);
    /*
     * Конструктор перемещения.
     * @param other r-value ссылка на объект, который нужно переместить.
     */
    Wall(Wall&& other) noexcept;
    /*
     * Оператор присваивания копированием.
     * @param other константная ссылка на объект, который нужно скопировать.
     * @return скопированный объект
     */
    Wall& operator=(const Wall& other);
    /*
     * Оператор присваивания перемещением.
     * @param other r-value ссылка на объект, который нужно переместить.
     * @return перемещенный объект
     */
    Wall& operator=(Wall&& other) noexcept;
    /*
     * Оператор сравнения.
     * @param other объект, с которым сравниваем наш объект this
     * @return true если равны, false иначе
     */
    bool operator==(const Wall& other) const;

    /*
     * Найти точку пересечения текущей стены стеной, передаваемой аргументом.
     * Точка пересечения в паре со стеной other заносится в список intersectionPointsAndWallsTheyIntersectsOn
     * @param other указатель на объект Wall, с которым нужно найти точку пересечения
     */
    void findIntersectionPoint(Wall* other);
    /*
     * После того, как точки пересечения со всеми стенами были найдены их нужно отсортировать по порядку "вдоль стены".
     * Это нужно для того, чтобы можно было взять точки с индексами i и i+1 и быть увереным, что между этими двумя точками нет еще одной.
     * Т.е. эти две точки как-бы представляют из себя углы одной комнаты.
     */
    void sortIntersectionPointsAlongWall();
    /*
     * Найти все точки, являющиеся потенциальными дверями, которые нужно выбить для того, чтобы пройти к сокровищам.
     * Для этого мы берем пару найденных и отсортированных "вдоль стены" точек (с индексами i и i+1) и находим середину между ними.
     * Найденную точку добавляем в doorPoints вместе со стеной, которой она принадлежит и двумя боковыми стенами, между которыми находится эта дверная точка.
     */
    void findDoorPoints();

    /*
     * Возвращает вектор точек-дверей.
     * @return Константная ссылка на вектор doorPoints.
     */
    const std::vector<DoorPointWithSurroundingLines*>& getDoorPoints() const;
    /*
     * Проверяет, является ли текущая стена "боковой".
     * @return true если стена боковая, false если внутренняя.
     */
    bool isEdgeLine() const;
    /*
     * Проверяет, лежат ли две точки с одной стороны от стены (являющейся, в геометрическом смысле, линией) или нет.
     * @param p1 Первая точка.
     * @param p2 Вторая точка.
     * @return true если точки лежат с одной стороны от стены, false в противном случае.
     */
    bool arePointsOnTheSameSide(const Point& p1, const Point& p2) const;

    /*
     * Верхняя внешняя стена.
     */
    static const Wall TOP;
    /*
     * Правая внешняя стена.
     */
    static const Wall RIGHT;
    /*
     * Нижняя внешняя стена.
     */
    static const Wall BOTTOM;
    /*
     * Левая внешняя стена.
     */
    static const Wall LEFT;

private:
    /*
     * Точка 1, по которой строилась стена (линия).
     */
    Point point1;
    /*
     * Точка 2, по которой строилась стена (линия).
     */
    Point point2;

    /*
     * Уравнение прямой по двум точкам имеет общий вид:
     * (y1 - y2)x + (x2 - x1)y + (x1*y2 - x2*y1) = 0 или
     * ax + by + c = 0
     * Получаем, что 
     * (y1 - y2) = a
     * (x2 - x1) = b
     * (x1*y2 - x2*y1) = c
     */

     /*
     * ax + by + c = 0
     * Это "a" в этом уравнении прямой. 
     */
    double a = 0;
    /*
     * ax + by + c = 0
     * Это "b" в этом уравнении прямой. 
     */
    double b = 0;
    /*
     * ax + by + c = 0
     * Это "c" в этом уравнении прямой.
     */
    double c = 0;

    /*
     * Вектор пар следующего содержания:
     * second - указатель на другую стену (не являющуюся this).
     * first - точка перемечения стены this со стеной second.
     */
    std::vector<std::pair<Point, Wall*>> intersectionPointsAndWallsTheyIntersectsOn;

    /*
     * Вектор, содержащий точки (и прилегающие к ним данные, см. описание DoorPointWithSurroundingLines), которые являются дверями в стене this.
     */
    std::vector<DoorPointWithSurroundingLines*> doorPoints;
};

}
