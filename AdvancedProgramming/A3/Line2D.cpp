#include "Line2D.h"
#include <iomanip>

Line2D::Line2D() : pt1(), pt2(), length(0.0) {}

Line2D::Line2D(Point2D pt1, Point2D pt2) : pt1(pt1), pt2(pt2) {
    setLength();
}

void Line2D::setLength() {
    int dx = pt2.getX() - pt1.getX();
    int dy = pt2.getY() - pt1.getY();
    length = sqrt(dx * dx + dy * dy);
}

Point2D Line2D::getPt1() const {
    return pt1;
}

Point2D Line2D::getPt2() const {
    return pt2;
}

double Line2D::getScalarValue() const {
    return length;
}

void Line2D::setPt1(Point2D pt1) {
    this->pt1 = pt1;
    setLength();
}

void Line2D::setPt2(Point2D pt2) {
    this->pt2 = pt2;
    setLength();
}

std::ostream& operator<<(std::ostream& os, const Line2D& line) {
    os << "( " << line.pt1 << " to " << line.pt2 << " ), length : " 
       << std::fixed << std::setprecision(3) << line.length;
    return os;
}

bool Line2D::operator==(const Line2D& other) const {
    return pt1 == other.pt1 && pt2 == other.pt2;
}

bool Line2D::operator<(const Line2D& other) const {
    if (!(pt1 == other.pt1)) return pt1 < other.pt1;
    return pt2 < other.pt2;
}
