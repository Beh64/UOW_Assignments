#ifndef POINT2D_H
#define POINT2D_H

#include <iostream>
#include <cmath>

class Point2D {
private:
    int x;
    int y;
    double distFrOrigin;
    
    void setDistFrOrigin();
    
public:
    // Constructors
    Point2D();
    Point2D(int x, int y);
    
    // Getters
    int getX() const;
    int getY() const;
    double getScalarValue() const;
    
    // Setters
    void setX(int x);
    void setY(int y);
    
    // Operator overloading
    friend std::ostream& operator<<(std::ostream& os, const Point2D& pt);
    
    // Comparison operators for sorting
    bool operator==(const Point2D& other) const;
    bool operator<(const Point2D& other) const;
};

#endif
