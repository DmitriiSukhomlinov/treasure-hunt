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
    //Соотнесем каждую стену с каждой
    for (int i = 0; i < (int)walls.size(); i++) {
        //Найдем точки пересечения текущей стены со всеми остальными
        for (int j = 0; j < (int)walls.size(); j++) {
            if (i == j) {
                continue;
            }

            //Находим пересечение стены i со стеной j
            walls[i].findIntersectionPoint(&walls[j]);
        }

        //Необходимо отсортировать найденные точки пересечения "вдоль стены"
        walls[i].sortIntersectionPointsAlongWall();
        //Ищем все точки, соответствующие дверям, которые можно пробить
        walls[i].findDoorPoints();
    }
}

void WallsAndPointsProcessor::calculateInternalStructureOfPyramid(const Point& _treasurePoint) {
    //Сохраним значени координат сокровища, они нам еще понадобятся вне этой функции.
    //Т.к. сокровище не лежит ни на каких стенах, передаем в качестве указателей на них nullptr.
    treasurePoint = new DoorPointWithSurroundingLines(_treasurePoint, nullptr, nullptr, nullptr);

    //Пробежимся по всем стенам
    for (const Wall& wall : walls) {
        //У каждой стены пробежимся по дверным проемам, которые на ней расположены (могут располагаться)
        for (auto doorPoint : wall.getDoorPoints()) {
            //Рассмотрим конкретную дверь doorPoint. Это - наш основной объект дальше, все вычисления мы делаем для того, чтобы понять, к каким другим дверям можно попать из двери "doorPoint"
            //Она находится в стене, которая зажата между двумя другими стенами.
            //Мы возьмем одну из этих двух стен (например, "wall1", будем называть ее для простоты "стена слева", хотя она может быть с любой стороны) 
            //и найдем на стене слева дверь или двери (назовем их "соседние"), 
            //находящиеся с нашей doorPoint в одной комнате (комната одна, если стена, на которой находится doorPoint внешняя, две - если внутренняя).
            //Для того, чтобы найти "соседние" двери мы пробежимся по всем дверям стены слева и найдем две двери, ближайшие к рассматриваемой двери "doorPoint",
            //причем такие, что они должны находться с разных сторон от стены, на которой находится doorPoint (комнаты же с разных сторон).
            const auto& additionalDoorPoints = doorPoint->wall1->getDoorPoints();
            //Как уже было сказано - комнаты может быть две, соответственно, двери тоже две.
            //Поэтому, готовим два объекта, в которые "запомним" двери - один для комнаты с одной, другой для комнаты с другой стороны.
            //Первый объект будет хранить кратчайшее расстояние, второй - второе по величине.
            //DEFAULT_SHORTEST_DISTANCE - очень большое, любое реальное расстояние  между двумя дверями будет меньше
            std::pair<double, DoorPointWithSurroundingLines*> shortestDistance(DEFAULT_SHORTEST_DISTANCE, additionalDoorPoints.front());
            std::pair<double, DoorPointWithSurroundingLines*> secondShortestDistance(DEFAULT_SHORTEST_DISTANCE, additionalDoorPoints.front());

            //Пробегаемся по всем дверям со стены слева
            for (const auto& wall1DoorPoint : additionalDoorPoints) {
                //Вычисляем расстояние от рассматриваемой двери "doorPoint" до двери на стене слева
                double distance = doorPoint->doorPoint.distance(wall1DoorPoint->doorPoint);
                //Если расстояние меньше того, которое хранится в элементе с наикратчайшем расстоянием...
                if (distance < shortestDistance.first) {
                    //Если там не было еще ни одного элемента - запоминаем впервые.
                    if (shortestDistance.first == DEFAULT_SHORTEST_DISTANCE) {
                        shortestDistance = std::pair<double, DoorPointWithSurroundingLines*>(distance, wall1DoorPoint);
                    } else if (wall.arePointsOnTheSameSide(shortestDistance.second->doorPoint, wall1DoorPoint->doorPoint)) {
                        //Если в shortestDistance уже сохранена дверь, и мы, получается, нашли новую дверь с расстоянием до doorPoint, меньшим, чем сохранено,
                        //то нам не надо сразу подменять старую дверь другой, а сначала убедиться, что обе эти двери находятся с одной стороны от стены, на которой находится doorPoint
                        //потому что если нет, то это значит, что мы уже рассматриваем комнату с другой стороны.
                        shortestDistance = std::pair<double, DoorPointWithSurroundingLines*>(distance, wall1DoorPoint);
                    } else {
                        //Если мы попали сюда, значит, мы начали рассматривать двери с другой стороны. В этом случае, просто присваеваем ее во второй результат.
                        //Поскльку точки упорядочены, то дальше мы можем только отдаляться, значит, можно выйти из цикла.
                        secondShortestDistance = std::pair<double, DoorPointWithSurroundingLines*>(distance, wall1DoorPoint);
                        break;
                    }
                } else if (!wall.arePointsOnTheSameSide(shortestDistance.second->doorPoint, wall1DoorPoint->doorPoint)) {
                    //Если мы попали сюда, значит мы просмотрели все двери с одной стороны от стены и теперь начинаем просматривать двери с другой стороны
                    //В этом случае, сейчас мы находимся на ближайшей - просто присваеваем ее и выходим из цикла
                    secondShortestDistance = std::pair<double, DoorPointWithSurroundingLines*>(distance, wall1DoorPoint);
                    break;
                }
            }

            //Теперь у нас есть наша основная дверь doorPoint и дверь со стены слева. Дальшейшая идея в том, что мы будем "достраивать" комнату, 
            //каждый раз добавляя к ней стену слева (см. createRoom()).
            std::vector<DoorPointWithSurroundingLines*> firstRoom = findMeaningfullPointsInTheRoom(doorPoint, shortestDistance.second);
            //Если у нас не боковая стена, то у нас две комнаты...
            if (!wall.isEdgeLine()) {
                //...поэтому проделываем то же самое со второй
                std::vector<DoorPointWithSurroundingLines*> secondRoom = findMeaningfullPointsInTheRoom(doorPoint, secondShortestDistance.second);

                //Объединяем се найденные из обоих комнат двери
                firstRoom.insert(firstRoom.end(), secondRoom.begin(), secondRoom.end());
            }

            //Все найденные двери записываем в одно из полей doorPoint
            doorPoint->doorsThatCanBeReached = { firstRoom };
        }
    }
}

int WallsAndPointsProcessor::findShortestWayByBfs() {
    //Обычный поиск в ширину, не рекурсивный
    //Создаем очередь
    std::queue<DoorPointWithSurroundingLines*> q;
    //Помещаем в нее стартовую вершину
    q.push(treasurePoint);
    //Помечаем стартовую вершину как развернутую
    treasurePoint->used = true;

    //Сюда мы запишем конечную точку, когда найдем.
    DoorPointWithSurroundingLines* finishPoint = nullptr;
    //До тех пор, пока очередь не пуста
    while (!q.empty()) {
        //Извлекаем вершину из очереди
        auto current = q.front();
        q.pop();

        //Пробегаемся по всем ее соседним вершинам
        for (auto neighbor : current->doorsThatCanBeReached) {
            //Если вершина размечена, то она уже в очереди, поэтому дальнейшие действия только если вершина не размечена
            if (!neighbor->used) {
                //Добавляем вершину в очередь
                q.push(neighbor);
                //Помечаем ее как размеченную
                neighbor->used = true;
                //К расстоянию до вершины current добавляем 1 и записываем
                neighbor->distanceFromTreasure = current->distanceFromTreasure + 1;
                //Если вершина лежит на боковой стене, то мы нашли то, что искали
                if (neighbor->wallPointBelongs2->isEdgeLine()) {
                    //Запоминаем финальную вершину
                    finishPoint = neighbor;
                    //Очищаем очередь, сделав swap с пустой очерередью.
                    //Очередь пуста, следовательно исполнение while закончено.
                    std::queue<DoorPointWithSurroundingLines*> empty;
                    std::swap(q, empty);
                    //Выходим из цикла
                    break;
                }
            }
        }
    }

    assert(finishPoint != nullptr);
    
    return finishPoint->distanceFromTreasure;
}

std::vector<DoorPointWithSurroundingLines*> WallsAndPointsProcessor::findMeaningfullPointsInTheRoom(DoorPointWithSurroundingLines* first, DoorPointWithSurroundingLines* second) {
    //Добавляем объекты, содержащие уже найденные двери и стены, которым они принадлежат
    std::vector<DoorPointWithSurroundingLines*> result = { first, second };
    //Вызываем функцию, которая находит все оставшиеся объекты с координатами дверей и стен, как бы замыкая комнату.
    //После того, как fillRoomWithWalls() отработает, у нас последний элемент вектора result будет иметь поле "wall1", 
    //указывающее на ту же стену, что и у первого элемента result указывает поле "wallPointBelongs2"
    fillRoomWithWalls(result);

    //Проверим, лежит ли сокровище в найденной комнате
    bool roomContainsTreasure = true;
    for (int i = 0; i < (int)result.size(); i++) {
        //Возьмем стену с индексом i и лежащую слева от нее. 
        DoorPointWithSurroundingLines* nextDoor = nullptr;
        if (i < (int)result.size() - 1) {
            nextDoor = result[i + 1];
        } else {
            nextDoor = result.front();
        }
        
        //"Сокровище" и "дверь со стены слева от стены с индексом i" должны находиться с одной стороны от стены с индексом "i"
        if (!result[i]->wallPointBelongs2->arePointsOnTheSameSide(treasurePoint->doorPoint, nextDoor->doorPoint)) {
            //Если это условие не выполняется для каких-то стен, то сокровища в комнате нет.
            roomContainsTreasure = false;
            break;
        }
    }

    //Удалим самый первый элемент, т.к. мы ищем связи дверей с ним - будет не к месте рассматривать его связь с самим собой.
    result.erase(result.begin());

    //Если сокровище лежит в комнате, то в этой комнате нас интересует как пробраться только к нему.
    //Удаляем все двери и добавляем единственную точку с сокровищами.
    if (roomContainsTreasure) {
        result.clear();
        result.push_back(treasurePoint);
        //В саму точку с сокровищем добаляем инфу о там, что из нее можно достигнуть дверь first
        treasurePoint->doorsThatCanBeReached.push_back(first);
    }

    return result;
}

void WallsAndPointsProcessor::fillRoomWithWalls(std::vector<DoorPointWithSurroundingLines*>& room) const {
    //Выберем последнюю и предпоследнюю добавленную дверь
    const auto& lastDoorPoint = room.back();
    const auto& doorPointBeforeLast = room.at(room.size() - 2);
    Wall* nexLine2Check = nullptr;
    //Если последняя дверь была "слева" от предпоследней, то мы будем рассматривать тоже левую от последней
    if (doorPointBeforeLast->wallPointBelongs2 == lastDoorPoint->wall1) {
        nexLine2Check = lastDoorPoint->wall2;
    } else if (doorPointBeforeLast->wallPointBelongs2 == lastDoorPoint->wall2) {
        //Если справа - то правую
        nexLine2Check = lastDoorPoint->wall1;
    } else {
        assert(false);
    }
    
    //Рассмотрим все двери на стене, которую мы рассматриваем. Найдем из них ближайшую к той двери, что сейчас в room на последнем месте
    const auto& doorPoints = nexLine2Check->getDoorPoints();
    std::pair<double, DoorPointWithSurroundingLines*> shortestDistance(DEFAULT_SHORTEST_DISTANCE, doorPoints.front());
    //Пройдемся по всем дверям
    for (const auto& middlePoint : doorPoints) {
        //Теперь нам важно, чтобы новая дверь была с той же стороны от последней стены, что и дверь на самой первой стене.
        //Если это не так - эта дверь находится с нами не в одной комнате и нам не подходит
        bool onTheSameSide = lastDoorPoint->wallPointBelongs2->arePointsOnTheSameSide(room.front()->doorPoint, middlePoint->doorPoint);
        if (!onTheSameSide) {
            continue;
        }

        //Сравниваем расстояние до предыдущей двери, оно должно быть наименьшим
        double distance = middlePoint->doorPoint.distance(lastDoorPoint->doorPoint);
        if (distance < shortestDistance.first) {
            shortestDistance = std::pair<double, DoorPointWithSurroundingLines*>(distance, middlePoint);
        }
    }

    //Добавляем дверь в вектор
    room.push_back(shortestDistance.second);
    //Если у новонайденной стены одна из соседних стен это та - что у нас в списке первая, значит, мы замкнули комнату и можем выходить...
    if (shortestDistance.second->wall1 == room.front()->wallPointBelongs2 
     || shortestDistance.second->wall2 == room.front()->wallPointBelongs2) {
        return;
    }
    //...если же нет - продолжаем двигаться дальше "по стеночке"
    fillRoomWithWalls(room);
}

}
