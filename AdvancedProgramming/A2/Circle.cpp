#include "Circle.h"
#include <cmath>
#include <sstream>

Circle::Circle(bool containsWarpSpace) : ShapeTwoD("Circle", containsWarpSpace) {
    centerX = 0;
    centerY = 0;
    radius = 0;
}

void Circle::setCenter(int x, int y) {
    centerX = x;
    centerY = y;
}

void Circle::setRadius(int r) {
    radius = r;
}

double Circle::computeArea() {
    return 3.14159265359 * radius * radius;
}

bool Circle::isPointOnShape(int px, int py) {
    int dx = px - centerX;
    int dy = py - centerY;
    int distSquared = dx * dx + dy * dy;
    int radiusSquared = radius * radius;
    
    return (distSquared == radiusSquared);
}

bool Circle::isPointInShape(int px, int py) {
    int dx = px - centerX;
    int dy = py - centerY;
    int distSquared = dx * dx + dy * dy;
    int radiusSquared = radius * radius;
    
    return (distSquared < radiusSquared);
}

string Circle::toString() {
    stringstream ss;
    ss << "Name : " << name << "\n";
    ss << "Special Type : " << (containsWarpSpace ? "WS" : "NS") << "\n";
    ss << "Area : " << computeArea() << " units square\n";
    ss << "Vertices :\n";
    ss << "Point [0] : (" << centerX << ", " << centerY << ")\n";
    
    // Points on perimeter (4 cardinal points: N, S, E, W)
    ss << "\nPoints on perimeter : ";
    vector<pair<int, int>> perimeterPoints;
    
    // North, South, East, West points
    perimeterPoints.push_back({centerX, centerY + radius});
    perimeterPoints.push_back({centerX, centerY - radius});
    perimeterPoints.push_back({centerX + radius, centerY});
    perimeterPoints.push_back({centerX - radius, centerY});
    
    for (size_t i = 0; i < perimeterPoints.size(); i++) {
        if (i > 0) ss << ", ";
        ss << "(" << perimeterPoints[i].first << ", " << perimeterPoints[i].second << ")";
    }
    
    // Points within shape (excluding perimeter, center included if inside)
    ss << "\n\nPoints within shape : ";
    vector<pair<int, int>> innerPoints;
    
    for (int px = centerX - radius; px <= centerX + radius; px++) {
        for (int py = centerY - radius; py <= centerY + radius; py++) {
            if (isPointInShape(px, py) && !isPointOnShape(px, py)) {
                innerPoints.push_back({px, py});
            }
        }
    }
    
    if (innerPoints.empty()) {
        ss << "none!";
    } else {
        for (size_t i = 0; i < innerPoints.size(); i++) {
            if (i > 0) ss << ", ";
            ss << "(" << innerPoints[i].first << ", " << innerPoints[i].second << ")";
        }
    }
    
    return ss.str();
}