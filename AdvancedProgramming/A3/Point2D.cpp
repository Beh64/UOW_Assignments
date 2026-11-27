#include "Point2D.h"
#include <iomanip>

Point2D::Point2D() : x(0), y(0), distFrOrigin(0.0) {}

Point2D::Point2D(int x, int y) : x(x), y(y) {
    setDistFrOrigin();
}

void Point2D::setDistFrOrigin() {
    distFrOrigin = sqrt(x * x + y * y);
}

int Point2D::getX() const {
    return x;
}

int Point2D::getY() const {
    return y;
}

double Point2D::getScalarValue() const {
    return distFrOrigin;
}

void Point2D::setX(int x) {
    this->x = x;
    setDistFrOrigin();
}

void Point2D::setY(int y) {
    this->y = y;
    setDistFrOrigin();
}

std::ostream& operator<<(std::ostream& os, const Point2D& pt) {
    os << "[" << pt.x << ", " << pt.y << "], distFrOrigin : " 
       << std::fixed << std::setprecision(3) << pt.distFrOrigin;
    return os;
}

bool Point2D::operator==(const Point2D& other) const {
    return x == other.x && y == other.y;
}

bool Point2D::operator<(const Point2D& other) const {
    if (x != other.x) return x < other.x;
    return y < other.y;
}
