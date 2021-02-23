#include "PointBetweenLines.h"

#include "Line.h"

namespace TH {

PointBetweenLines::PointBetweenLines(const Point& _point, Line* _linePointBelongs2, Line* _line1, Line* _line2)
    : point(_point), linePointBelongs2(_linePointBelongs2), line1(_line1), line2(_line2) {}

}