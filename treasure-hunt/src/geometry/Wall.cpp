#include "Wall.h"

#include <algorithm>

namespace TH {

const Wall Wall::TOP(Point(0, 100), Point(100, 100));
const Wall Wall::RIGHT(Point(100, 0), Point(100, 100));
const Wall Wall::BOTTOM(Point(0, 0), Point(100, 0));
const Wall Wall::LEFT(Point(0, 0), Point(0, 100));

Wall::Wall(const Point& _point1, const Point& _point2) 
    : point1(_point1), point2(_point2) {

    //По двум точкам находим коэффициенты уравнения прямой
    auto x1 = point1.getX();
    auto y1 = point1.getY();
    auto x2 = point2.getX();
    auto y2 = point2.getY();

    a = y1 - y2;
    b = x2 - x1;
    c = x1 * y2 - x2 * y1;
}

Wall::~Wall() {
    for (auto m : doorPoints) {
        delete m;
    }
}

Wall::Wall(const Wall& other) 
    : Wall(other.point1, other.point2) {
    this->intersectionPointsAndWallsTheyIntersectsOn = other.intersectionPointsAndWallsTheyIntersectsOn;
    //Копируем данные объекта, выделяем память
    for (auto m : other.doorPoints) {
        this->doorPoints.push_back(new DoorPointWithSurroundingLines(*m));
    }
}

Wall::Wall(Wall&& other) noexcept 
    : Wall(other.point1, other.point2) {
    this->intersectionPointsAndWallsTheyIntersectsOn = other.intersectionPointsAndWallsTheyIntersectsOn;
    //Перемещаем данные временного объекта...
    for (auto m : other.doorPoints) {
        this->doorPoints.push_back(m);
    }
    //... и очищаем исходный временный объект
    other.doorPoints.clear();
}

Wall& Wall::operator=(const Wall& other) {
    if (*this == other) {
        //Проверка на самоприсваивание
        return *this;
    }

    this->point1 = other.point1;
    this->point2 = other.point2;
    this->a = other.a;
    this->b = other.b;
    this->c = other.c;
    this->intersectionPointsAndWallsTheyIntersectsOn = other.intersectionPointsAndWallsTheyIntersectsOn;
    //Копируем данные объекта, выделяем память
    for (auto m : other.doorPoints) {
        this->doorPoints.push_back(new DoorPointWithSurroundingLines(*m));
    }

    return *this;
}

Wall& Wall::operator=(Wall&& other) noexcept {
    if (*this == other) {
        //Проверка на самоприсваивание
        return *this;
    }

    this->point1 = other.point1;
    this->point2 = other.point2;
    this->a = other.a;
    this->b = other.b;
    this->c = other.c;
    this->intersectionPointsAndWallsTheyIntersectsOn = other.intersectionPointsAndWallsTheyIntersectsOn;
    //Перемещаем данные временного объекта...
    for (auto m : other.doorPoints) {
        this->doorPoints.push_back(m);
    }
    //... и очищаем исходный временный объект
    other.doorPoints.clear();

    return *this;
}

bool Wall::operator==(const Wall& other) const {
    //Для равенства достаточно считать, что изначальные точки были равны.
    return this->point1 == other.point1 && this->point2 == other.point2;
}

void Wall::findIntersectionPoint(Wall* other) {
    //Фактически, нужно решить систему из двух линейных уравнений такого вида:
    // (a1 b1)  x = c1
    // (a2 b2)  y = c2
    //Считаем определитель матрицы
    const auto determinant = this->a * other->b - this->b * other->a;
    if (determinant == 0) {
        //Матрица вырождена, линии параллельны
        return;
    }

    //Находим точки пересечения
    const double xIntersectionPoint = (this->b * other->c - other->b * this->c) / determinant;
    const double yIntersectionPoint = (other->a * this->c - this->a * other->c) / determinant;

    if (xIntersectionPoint < 0 || xIntersectionPoint > 100
        || yIntersectionPoint < 0 || yIntersectionPoint > 100) {
        //Точка пересечения находится вне квадрата, в котором мы ищем решения
        return;
    }

    auto point = Point(xIntersectionPoint, yIntersectionPoint);

    //Добавляем в результат точку и стену, при пересечении с которой эта точка обраЗуется
    std::pair<Point, Wall*> p(point, other);
    intersectionPointsAndWallsTheyIntersectsOn.push_back(p);
}

void Wall::sortIntersectionPointsAlongWall() {
    //Соортируем используя функцию стандартной библиотеки
    //Завявленная вычислительная сложность - O(N*logN)
    std::sort(intersectionPointsAndWallsTheyIntersectsOn.begin(), intersectionPointsAndWallsTheyIntersectsOn.end(), 
        [&](const std::pair<Point, Wall*>& first, const std::pair<Point, Wall*>& second) -> bool {
        //Сравниваем расстояния до одной из точек, лежащих на внешей стене. Путь это будет point1.
        auto distanceFromPoint1ToFirst = point1.distance(first.first);
        auto distanceFromPoint1ToSecond = point1.distance(second.first);
        return distanceFromPoint1ToFirst < distanceFromPoint1ToSecond;
    });
}

void Wall::findDoorPoints() {
    //Пробегаемся по всем точкам пересечения этой стены с другими стенами (идем только до size()-1, т.к. в самом цикле есть i+1)
    for (int i = 0; i < (int)intersectionPointsAndWallsTheyIntersectsOn.size() - 1; i++) {
        //Берем точки пересечения с индексами i и i+1.
        //Т.к. они были отсортированы, гарантированно это углы одной комнаты.
        const auto& crossoverPointI = intersectionPointsAndWallsTheyIntersectsOn[i].first;
        const auto& crossoverPointIPlus1 = intersectionPointsAndWallsTheyIntersectsOn[i + 1].first;
        //Находим координаты точки посередине между ними.
        double xMiddlePoint = (crossoverPointI.getX() + crossoverPointIPlus1.getX()) / 2;
        double yMiddlePoint = (crossoverPointI.getY() + crossoverPointIPlus1.getY()) / 2;

        Point doorPoint(xMiddlePoint, yMiddlePoint);
        //Помимо точки - координаты двери - нам впослествии понадобится знать:
        //1. На какой стене лежит эта дверь. В нашем случае, мы все двери ищем на this, поэтому сохраняем указатель на нее.
        //2. Стены слева и справа - т.е. между какими двумя стенами находится наша дверь. Указатели на них так же сохраняем.
        auto dpwsl = new DoorPointWithSurroundingLines(doorPoint, this, intersectionPointsAndWallsTheyIntersectsOn[i].second, intersectionPointsAndWallsTheyIntersectsOn[i + 1].second);
        doorPoints.push_back(dpwsl);
    }
}

const std::vector<DoorPointWithSurroundingLines*>& Wall::getDoorPoints() const {
    return doorPoints;
}

bool Wall::isEdgeLine() const {
    return *this == Wall::TOP || 
           *this == Wall::RIGHT ||
           *this == Wall::BOTTOM ||
           *this == Wall::LEFT;
}

bool Wall::arePointsOnTheSameSide(const Point& p1, const Point& p2) const {
    //Точки лежат с одной стороны от прямой, если при подставлении их в уравнение прямой у результатов будут одинаковые знаки.
    auto v1 = a * p1.getX() + b * p1.getY() + c;
    auto v2 = a * p2.getX() + b * p2.getY() + c;
    return (v1 > 0 && v2 > 0) || (v1 < 0 && v2 < 0);
}

}