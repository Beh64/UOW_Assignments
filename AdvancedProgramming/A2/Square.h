#ifndef SQUARE_H
#define SQUARE_H

#include "ShapeTwoD.h"  //  Include parent class

//  Square class inherits from shapeTwoD
class Square : public ShapeTwoD {
private:   
    int x[4];   //  Array to store 4 x-coordinates (one for each corner)
    int y[4];   //  Array to store 4 y-coordinates (one for each corner)
    
public:
//  Takes warp space status (true/false) as a parameter
    Square(bool containsWarpSpace);
    
    //  Method to set all 4 vertices (corners) of the square
    //  Takes 8 parameters: x0, y0 for point 1, x1, y1 for point 2 etc
    void setVertices(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);
    
    //  Override parent's pure virtual functions with square-specific implementations
    double computeArea() override;              //  Calculate area using shoelace formula
    bool isPointInShape(int x, int y) override; //  Check if point is inside square
    bool isPointOnShape(int x, int y) override; //  Check if point is on edge
    string toString() override;                 //  Format all square data as string
};

#endif