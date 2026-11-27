#include "Square.h"
#include <cmath>        //  For abs() function
#include <sstream>      //  For stringstream (string building)
#include <algorithm>    //  For min() and max() functions

//  Creates a new square object
Square::Square(bool containsWarpSpace)
 : ShapeTwoD("Square", containsWarpSpace) { //  Call parent constructor
    //  Parent constructor sets name to "Square" and stores WS/NS status
}
//  Set all 4 vertices (corners) of the square
void Square::setVertices(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3) {
   //   Store x-coordinates in array
    x[0] = x0; y[0] = y0;   //  First point
    x[1] = x1; y[1] = y1;   //  Second point
    x[2] = x2; y[2] = y2;   //  Third point
    x[3] = x3; y[3] = y3;   //  Fourth point
    
    //  Clear any old vertices from parent's vector
    vertices.clear();
    //  Add all 4 opints to parent's vertices vector
    vertices.push_back({x0, y0});   //  Push pair (x0, y0)
    vertices.push_back({x1, y1});   //  Push pair (x1, y1)
    vertices.push_back({x2, y2});   //  Push pair (x2, y2)
    vertices.push_back({x3, y3});   //  Push pair (x3, y3)
}
//  Calculate area using Shoelace formula
double Square::computeArea() {
    // Using Shoelace formula for polygon area
    double area = 0.0;
    //  Loop through all 4 vertices
    for (int i = 0; i < 4; i++) {
        int j = (i + 1) % 4;    //  Next vertex (wraps around: 0->1->2->3->0)
        //  Apply Shoelace formula
        area += x[i] * y[j];    //  Add x[i] * y[next]
        area -= x[j] * y[i];    //  Subtract x[next] * y[i]
    }
    return abs(area) / 2.0; //  Return absolute value divided by 2
}
//  Check if point (px, py) is on the square's perimeter
bool Square::isPointOnShape(int px, int py) {
    // Check if point is on any of the 4 edges
    for (int i = 0; i < 4; i++) {
        int j = (i + 1) % 4;    //  Get next vertex (creates edge from i to j)
        int x1 = x[i], y1 = y[i];   //  Start point of edge
        int x2 = x[j], y2 = y[j];   //  End point of edge
        
        // Check if point is on line segment
        int minX = min(x1, x2), maxX = max(x1, x2); //  X range
        int minY = min(y1, y2), maxY = max(y1, y2); //  Y range
        
        //  Check if point is within bounding box
        if (px >= minX && px <= maxX && py >= minY && py <= maxY) {
            //  Check collinearity using cross product
            //  Cross project = 0 means points are on same line
            int cross = (py - y1) * (x2 - x1) - (px - x1) * (y2 - y1);
            if (cross == 0) {
                return true;    //  Point is on this edge
            }
        }
    }
    return false;   //  Point is not on any edge
}
//  Check if point (px, py) is inside the square (not on edge)
//  Uses ray casting algorithm - cast ray from point going right
bool Square::isPointInShape(int px, int py) {
    
    int count = 0;
    //  Case a horizontal ray from point going right
    //  Count how may edges it crosses
    for (int i = 0; i < 4; i++) {
        int j = (i + 1) % 4;    //  Next vertex
        int x1 = x[i], y1 = y[i];   //  Edge start point
        int x2 = x[j], y2 = y[j];   //  Edge end point
        
        // Check if horizontal ray at py height crosses this edge
        //  Edge must span across py vertically
        if ((y1 <= py && py < y2) || (y2 <= py && py < y1)) {
            //  Calculate where ray intersects edge
            double t = (double)(py - y1) / (y2 - y1);   //  Proportion along edge
            double xIntersect = x1 + t * (x2 - x1);     //  X coordinate of intersection
            //  If intersection is to the right of point, count it
            if (xIntersect > px) {
                count++;
            }
        }
    }
    //  Odd count = inside, Even count = outside (Ray casting rule)
    return (count % 2 == 1);
}
//  Create formatted string with all square information
string Square::toString() {
    stringstream ss;    //  String stream for building output
    //  Add basic information
    ss << "Name : " << name << "\n";
    ss << "Special Type : " << (containsWarpSpace ? "WS" : "NS") << "\n";
    ss << "Area : " << computeArea() << " units square\n";
    //  Add all vertices
    ss << "Vertices :\n";
    for (int i = 0; i < 4; i++) {
        ss << "Point [" << i << "] : (" << x[i] << ", " << y[i] << ")\n";
    }
    
    // Find all points on perimeter (excluding vertices)
    ss << "\nPoints on perimeter : ";
    vector<pair<int, int>> perimeterPoints; //  Store perimeter points
    //  Check each edge
    for (int i = 0; i < 4; i++) {
        int j = (i + 1) % 4;    //  Next vertex
        int x1 = x[i], y1 = y[i];   //  Edge start
        int x2 = x[j], y2 = y[j];   //  Edge end
        //  Calculate step direction (1, -1, or 0)
        int dx = (x2 > x1) ? 1 : (x2 < x1 ? -1 : 0);    //  X direction
        int dy = (y2 > y1) ? 1 : (y2 < y1 ? -1 : 0);    //  Y direction
        //  Start from point after vertex
        int cx = x1 + dx, cy = y1 + dy;
        //  Walk along edge until reaching next vertex
        while (cx != x2 || cy != y2) {
            perimeterPoints.push_back({cx, cy});    //  Add this point
            cx += dx;   //  Move to next point
            cy += dy;
        }
    }
    //  Display perimeter points
    if (perimeterPoints.empty()) {
        ss << "none!";  //  No points between vertices
    } else {
        for (size_t i = 0; i < perimeterPoints.size(); i++) {
            if (i > 0) ss << ", ";  //  Add comma separator
            ss << "(" << perimeterPoints[i].first << ", " << perimeterPoints[i].second << ")";
        }
    }
    
    // Find all interior points (inside but not on edge or vertex)
    ss << "\n\nPoints within shape : ";
    vector<pair<int, int>> innerPoints; //  Store interior points
    //  Find bounding box of square
    int minX = x[0], maxX = x[0];
    int minY = y[0], maxY = y[0];
    for (int i = 1; i < 4; i++) {
        minX = min(minX, x[i]); //  Find minimum X
        maxX = max(maxX, x[i]); //  Find maximum X
        minY = min(minY, y[i]); //  Find minimum Y
        maxY = max(maxY, y[i]); //  Find maximum Y
    }
    //  Check every point in bounding box
    for (int px = minX; px <= maxX; px++) {
        for (int py = minY; py <= maxY; py++) {
            //  Point must be inside and not on edge and not a vertex
            if (isPointInShape(px, py) && !isPointOnShape(px, py)) {
                //  Check if it's a vertex
                bool isVertex = false;
                for (int i = 0; i < 4; i++) {
                    if (px == x[i] && py == y[i]) {
                        isVertex = true;    //  This is a corner point
                        break;
                    }
                }
                //  Add if not a vertex
                if (!isVertex) {
                    innerPoints.push_back({px, py});
                }
            }
        }
    }
    //  Display interior points
    if (innerPoints.empty()) {
        ss << "none!";  //  No interior points
    } else {
        for (size_t i = 0; i < innerPoints.size(); i++) {
            if (i > 0) ss << ", ";  //  Add comma separator
            ss << "(" << innerPoints[i].first << ", " << innerPoints[i].second << ")";
        }
    }
    
    return ss.str();    //  Convert stringstream to string and return
}