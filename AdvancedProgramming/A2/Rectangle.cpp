#include "Rectangle.h"
#include <cmath>
#include <sstream>
#include <algorithm>

Rectangle::Rectangle(bool containsWarpSpace) : ShapeTwoD("Rectangle", containsWarpSpace) {
}

void Rectangle::setVertices(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3) {
    x[0] = x0; y[0] = y0;
    x[1] = x1; y[1] = y1;
    x[2] = x2; y[2] = y2;
    x[3] = x3; y[3] = y3;
    
    vertices.clear();
    vertices.push_back({x0, y0});
    vertices.push_back({x1, y1});
    vertices.push_back({x2, y2});
    vertices.push_back({x3, y3});
}

double Rectangle::computeArea() {
    // Using Shoelace formula
    double area = 0.0;
    for (int i = 0; i < 4; i++) {
        int j = (i + 1) % 4;
        area += x[i] * y[j];
        area -= x[j] * y[i];
    }
    return abs(area) / 2.0;
}

bool Rectangle::isPointOnShape(int px, int py) {
    // Check if point is on any of the 4 edges
    for (int i = 0; i < 4; i++) {
        int j = (i + 1) % 4;
        int x1 = x[i], y1 = y[i];
        int x2 = x[j], y2 = y[j];
        
        int minX = min(x1, x2), maxX = max(x1, x2);
        int minY = min(y1, y2), maxY = max(y1, y2);
        
        if (px >= minX && px <= maxX && py >= minY && py <= maxY) {
            int cross = (py - y1) * (x2 - x1) - (px - x1) * (y2 - y1);
            if (cross == 0) {
                return true;
            }
        }
    }
    return false;
}

bool Rectangle::isPointInShape(int px, int py) {
    // Ray casting algorithm - cast ray from point going right
    int count = 0;
    for (int i = 0; i < 4; i++) {
        int j = (i + 1) % 4;
        int x1 = x[i], y1 = y[i];
        int x2 = x[j], y2 = y[j];
        
        // Check if horizontal ray crosses this edge
        if ((y1 <= py && py < y2) || (y2 <= py && py < y1)) {
            double t = (double)(py - y1) / (y2 - y1);
            double xIntersect = x1 + t * (x2 - x1);
            if (xIntersect > px) {
                count++;
            }
        }
    }
    return (count % 2 == 1);
}

string Rectangle::toString() {
    stringstream ss;
    ss << "Name : " << name << "\n";
    ss << "Special Type : " << (containsWarpSpace ? "WS" : "NS") << "\n";
    ss << "Area : " << computeArea() << " units square\n";
    ss << "Vertices :\n";
    for (int i = 0; i < 4; i++) {
        ss << "Point [" << i << "] : (" << x[i] << ", " << y[i] << ")\n";
    }
    
    // Points on perimeter (excluding vertices)
    ss << "\nPoints on perimeter : ";
    vector<pair<int, int>> perimeterPoints;
    for (int i = 0; i < 4; i++) {
        int j = (i + 1) % 4;
        int x1 = x[i], y1 = y[i];
        int x2 = x[j], y2 = y[j];
        
        int dx = (x2 > x1) ? 1 : (x2 < x1 ? -1 : 0);
        int dy = (y2 > y1) ? 1 : (y2 < y1 ? -1 : 0);
        
        int cx = x1 + dx, cy = y1 + dy;
        while (cx != x2 || cy != y2) {
            perimeterPoints.push_back({cx, cy});
            cx += dx;
            cy += dy;
        }
    }
    
    if (perimeterPoints.empty()) {
        ss << "none!";
    } else {
        for (size_t i = 0; i < perimeterPoints.size(); i++) {
            if (i > 0) ss << ", ";
            ss << "(" << perimeterPoints[i].first << ", " << perimeterPoints[i].second << ")";
        }
    }
    
    // Points within shape
    ss << "\n\nPoints within shape : ";
    vector<pair<int, int>> innerPoints;
    
    int minX = x[0], maxX = x[0];
    int minY = y[0], maxY = y[0];
    for (int i = 1; i < 4; i++) {
        minX = min(minX, x[i]);
        maxX = max(maxX, x[i]);
        minY = min(minY, y[i]);
        maxY = max(maxY, y[i]);
    }
    
    for (int px = minX; px <= maxX; px++) {
        for (int py = minY; py <= maxY; py++) {
            if (isPointInShape(px, py) && !isPointOnShape(px, py)) {
                bool isVertex = false;
                for (int i = 0; i < 4; i++) {
                    if (px == x[i] && py == y[i]) {
                        isVertex = true;
                        break;
                    }
                }
                if (!isVertex) {
                    innerPoints.push_back({px, py});
                }
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