#pragma once

namespace TH {

/*
 * \class Point
 * \brief Точка в двумерном пространстве с координатами "x" и "y"
 */
class Point {
public:
    /*
     * Конструктор.
     * @param x координата на оси X.
     * @param y координата на оси Y.
     */
    Point(double x, double y);

    /**
     * Оператор сравнения, показывающий, равны две точки или нет.
     * @param other объект, с которым сравниваем наш объект this
     * @return true если равны, false иначе
     */
    bool operator==(const Point& other) const;

    /**
     * Вернуть значение "x"
     * @return значение на оси X
     */
    double getX() const;
    /**
     * Вернуть значение "y"
     * @return значение на оси Y
     */
    double getY() const;
    /**
     * Рассчитать расстояние между двумя точками на плоскости
     * @param other объект, расстояние до которого нужно найти
     * @return расстояние от точки this до точки other
     */
    double distance(const Point& other) const;

private:
    /**
     * Координата на оси Х
     */
    double x = -1.0;
    /**
     * Координата на оси Y
     */
    double y = -1.0;
};

}