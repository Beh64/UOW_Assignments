#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "ShapeTwoD.h"

class Rectangle : public ShapeTwoD {
private:
    int x[4];
    int y[4];
    
public:
    Rectangle(bool containsWarpSpace);
    
    void setVertices(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);
    
    double computeArea() override;
    bool isPointInShape(int x, int y) override;
    bool isPointOnShape(int x, int y) override;
    string toString() override;
};

#endif