#include <iostream>
#include <fstream>
#include <vector>

#include "computations/WallsAndPointsProcessor.h"
#include "geometry/Wall.h"

/*
 * Сухомлинов Дмитрий, ММФ НГУ, 4 курс, 17121
 * 
 * Общая идея решения - фактически, входные данные представляют из себя витееватое задание графа.
 * Двери в стенах хранилища - это его вершины. Так же вершиной является точка, где находятся сокровища.
 * Внутри каждой комнаты все двери соединены между собой, образуя ребра графа. Исключение - комната с сокровищем, там достаточно соединить двери только с местоположением сокровища.
 * Нужно построить этот граф и используя поиск в графе найти кратчайий путь от вершины, которая является местоположением сокровища, до любой из вершин, находящихся на "внешних стенах" пирамиды.
 * Оптимальнее будет использовать поиск "в ширину", т.к. при таком поиске первый же найденный путь является кратчайшим, а при поиске "в глубину" необходимо сначала найти все пути, а потом сравнить их.
 * 
 */


int main(int argc, char* argv[]) {
    //Меню помощи
    if (argc == 1) {
        std::cout << "Treasure Hunt." << std::endl;
        std::cout << std::endl;
        std::cout << "[OPTION] - Path to the input file. The only argument." << std::endl;
        std::cout << "Example: \"treasure-hunt.exe C:\\inputs\\input.txt\"." << std::endl;
        return 0;
    }

    //Аргументов должно быть 2
    if (argc != 2) {
        std::cout << "Incorrect input." << std::endl;
        return 1 ;
    }

    //Я решил не делать проверку "на дурака", будем считать, что у нас всегда корректный формат входного файла.
    int wallsNumber = 0;
    std::ifstream fin(argv[1]);
    //Считываем количество стен
    fin >> wallsNumber;
    //Создаем обработчик, который будет заниматься всеми вычислениями
    TH::WallsAndPointsProcessor processor;
    for (int i = 0; i < wallsNumber; i++) {
        double x1 = 0;
        double y1 = 0;
        double x2 = 0;
        double y2 = 0;
        //Считываем координаты стены
        fin >> x1 >> y1 >> x2 >> y2;
        
        //Добавляем новую стену в обработчик
        processor.addWall(TH::Wall(TH::Point(x1, y1), TH::Point(x2, y2)));
    }
    //Находим предполагаемые местоположения всех дверей в стенах
    processor.findDoorLocations();

    double xTresure = 0;
    double yTreasure = 0;
    //Считываем координаты сокровища
    fin >> xTresure >> yTreasure;
    fin.close();
    //Рассчитываем всевозможные пути до сокровища через всевозможные двери, т.е. строим граф
    processor.calculateInternalStructureOfPyramid(TH::Point(xTresure, yTreasure));

    //Используя поиск в ширину находим кратчайший путь в графе от вершины, которая является координатами сокровища, до любое вершины, лежащей на внешних стенах.
    auto numberOfDoors = processor.findShortestWayByBfs();

    std::cout << "Number of doors = " << numberOfDoors << std::endl;

    return 0;
}