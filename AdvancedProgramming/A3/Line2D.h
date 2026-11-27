#ifndef LINE2D_H
#define LINE2D_H

#include "Point2D.h"

class Line2D {
private:
    Point2D pt1;
    Point2D pt2;
    double length;
    
    void setLength();
    
public:
    // Constructors
    Line2D();
    Line2D(Point2D pt1, Point2D pt2);
    
    // Getters
    Point2D getPt1() const;
    Point2D getPt2() const;
    double getScalarValue() const;
    
    // Setters
    void setPt1(Point2D pt1);
    void setPt2(Point2D pt2);
    
    // Operator overloading
    friend std::ostream& operator<<(std::ostream& os, const Line2D& line);
    
    // Comparison operators for sorting
    bool operator==(const Line2D& other) const;
    bool operator<(const Line2D& other) const;
};

#endif
