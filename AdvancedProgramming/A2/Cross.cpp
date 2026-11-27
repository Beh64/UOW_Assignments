#include "Cross.h"
#include <cmath>
#include <sstream>
#include <algorithm>
#include <iostream>  

using namespace std;  

Cross::Cross(bool containsWarpSpace) : ShapeTwoD("Cross", containsWarpSpace) {
}

void Cross::setVertices(int v[][2]) {
    for (int i = 0; i < 12; i++) {
        x[i] = v[i][0];
        y[i] = v[i][1];
        vertices.push_back({v[i][0], v[i][1]});
    }
}

double Cross::computeArea() {
    // Using Shoelace formula for polygon area
    double area = 0.0;
    for (int i = 0; i < 12; i++) {
        int j = (i + 1) % 12;
        area += x[i] * y[j];
        area -= x[j] * y[i];
    }
    return abs(area) / 2.0;
}

bool Cross::isPointOnShape(int px, int py) {
    // Check if point is on any of the 12 edges
    for (int i = 0; i < 12; i++) {
        int j = (i + 1) % 12;
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

bool Cross::isPointInShape(int px, int py) {
    // Ray casting algorithm - cast ray from point going right
    int count = 0;
    for (int i = 0; i < 12; i++) {
        int j = (i + 1) % 12;
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

string Cross::toString() {
    stringstream ss;
    ss << "Name : " << name << "\n";
    ss << "Special Type : " << (containsWarpSpace ? "WS" : "NS") << "\n";
    ss << "Area : " << computeArea() << " units square\n";
    ss << "Vertices :\n";
    for (int i = 0; i < 12; i++) {
        ss << "Point [" << i << "] : (" << x[i] << ", " << y[i] << ")\n";
    }
    
    // Points on perimeter (excluding vertices)

ss << "\nPoints on perimeter : ";
vector<pair<int, int>> perimeterPoints;

for (int i = 0; i < 12; i++) {
    int j = (i + 1) % 12;
    int x1 = x[i], y1 = y[i];
    int x2 = x[j], y2 = y[j];
    
    // Skip duplicate vertices
    if (x1 == x2 && y1 == y2) {
        continue;
    }
    
    // Use Bresenham's line algorithm for all cases
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    int cx = x1;
    int cy = y1;
    
    while (true) {
        // Move to next point
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            cx += sx;
        }
        if (e2 < dx) {
            err += dx;
            cy += sy;
        }
        
        // Stop when we reach the end point
        if (cx == x2 && cy == y2) {
            break;
        }
        
        // Add the point (excludes both start and end vertices)
        perimeterPoints.push_back({cx, cy});
        
        // Safety check
        if (perimeterPoints.size() > 10000) {
            cout << "ERROR: Too many perimeter points on edge " << i << endl;
            break;
        }
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
    
    // Find bounding box
    int minX = x[0], maxX = x[0];
    int minY = y[0], maxY = y[0];
    for (int i = 1; i < 12; i++) {
        minX = min(minX, x[i]);
        maxX = max(maxX, x[i]);
        minY = min(minY, y[i]);
        maxY = max(maxY, y[i]);
    }
    
    // Check all points in bounding box
    for (int px = minX; px <= maxX; px++) {
        for (int py = minY; py <= maxY; py++) {
            if (isPointInShape(px, py) && !isPointOnShape(px, py)) {
                // Check if it's a vertex
                bool isVertex = false;
                for (int i = 0; i < 12; i++) {
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