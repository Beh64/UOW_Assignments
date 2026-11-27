#ifndef LINE3D_H
#define LINE3D_H

#include "Point3D.h"

class Line3D {
private:
    Point3D pt1;
    Point3D pt2;
    double length;
    
    void setLength();
    
public:
    // Constructors
    Line3D();
    Line3D(Point3D pt1, Point3D pt2);
    
    // Getters
    Point3D getPt1() const;
    Point3D getPt2() const;
    double getScalarValue() const;
    
    // Setters
    void setPt1(Point3D pt1);
    void setPt2(Point3D pt2);
    
    // Operator overloading
    friend std::ostream& operator<<(std::ostream& os, const Line3D& line);
    
    // Comparison operators for sorting
    bool operator==(const Line3D& other) const;
    bool operator<(const Line3D& other) const;
};

#endif
