#include "DoorPointWithSurroundingLines.h"

#include "Wall.h"

namespace TH {

DoorPointWithSurroundingLines::DoorPointWithSurroundingLines(const Point& _doorPoint, Wall* _wallPointBelongs2, Wall* _wall1, Wall* _wall2)
    : doorPoint(_doorPoint), wallPointBelongs2(_wallPointBelongs2), wall1(_wall1), wall2(_wall2) {}

}