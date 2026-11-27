#include "Line3D.h"
#include <iomanip>

Line3D::Line3D() : pt1(), pt2(), length(0.0) {}

Line3D::Line3D(Point3D pt1, Point3D pt2) : pt1(pt1), pt2(pt2) {
    setLength();
}

void Line3D::setLength() {
    int dx = pt2.getX() - pt1.getX();
    int dy = pt2.getY() - pt1.getY();
    int dz = pt2.getZ() - pt1.getZ();
    length = sqrt(dx * dx + dy * dy + dz * dz);
}

Point3D Line3D::getPt1() const {
    return pt1;
}

Point3D Line3D::getPt2() const {
    return pt2;
}

double Line3D::getScalarValue() const {
    return length;
}

void Line3D::setPt1(Point3D pt1) {
    this->pt1 = pt1;
    setLength();
}

void Line3D::setPt2(Point3D pt2) {
    this->pt2 = pt2;
    setLength();
}

std::ostream& operator<<(std::ostream& os, const Line3D& line) {
    os << "( " << line.pt1 << " to " << line.pt2 << " ), length : " 
       << std::fixed << std::setprecision(3) << line.length;
    return os;
}

bool Line3D::operator==(const Line3D& other) const {
    return pt1 == other.pt1 && pt2 == other.pt2;
}

bool Line3D::operator<(const Line3D& other) const {
    if (!(pt1 == other.pt1)) return pt1 < other.pt1;
    return pt2 < other.pt2;
}
