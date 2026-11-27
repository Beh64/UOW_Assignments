#ifndef CROSS_H
#define CROSS_H

#include "ShapeTwoD.h"

class Cross : public ShapeTwoD {
private:
    int x[12];
    int y[12];
    
public:
    Cross(bool containsWarpSpace);
    
    void setVertices(int vertices[][2]);
    
    double computeArea() override;
    bool isPointInShape(int x, int y) override;
    bool isPointOnShape(int x, int y) override;
    string toString() override;
};

#endif