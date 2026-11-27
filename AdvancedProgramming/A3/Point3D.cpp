#include "Point3D.h"
#include <iomanip>

Point3D::Point3D() : Point2D(), z(0), distFrOrigin(0.0) {}

Point3D::Point3D(int x, int y, int z) : Point2D(x, y), z(z) {
    setDistFrOrigin();
}

void Point3D::setDistFrOrigin() {
    int x = getX();
    int y = getY();
    distFrOrigin = sqrt(x * x + y * y + z * z);
}

int Point3D::getZ() const {
    return z;
}

double Point3D::getScalarValue() const {
    return distFrOrigin;
}

void Point3D::setZ(int z) {
    this->z = z;
    setDistFrOrigin();
}

std::ostream& operator<<(std::ostream& os, const Point3D& pt) {
    os << "[" << pt.getX() << ", " << pt.getY() << ", " << pt.z << "], distFrOrigin : " 
       << std::fixed << std::setprecision(3) << pt.distFrOrigin;
    return os;
}

bool Point3D::operator==(const Point3D& other) const {
    return getX() == other.getX() && getY() == other.getY() && z == other.z;
}

bool Point3D::operator<(const Point3D& other) const {
    if (getX() != other.getX()) return getX() < other.getX();
    if (getY() != other.getY()) return getY() < other.getY();
    return z < other.z;
}
