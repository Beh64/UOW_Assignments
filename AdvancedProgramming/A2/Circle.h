#ifndef CIRCLE_H
#define CIRCLE_H

#include "ShapeTwoD.h"

class Circle : public ShapeTwoD {
private:
    int centerX;
    int centerY;
    int radius;
    
public:
    Circle(bool containsWarpSpace);
    
    void setCenter(int x, int y);
    void setRadius(int r);
    
    double computeArea() override;
    bool isPointInShape(int x, int y) override;
    bool isPointOnShape(int x, int y) override;
    string toString() override;
};

#endif