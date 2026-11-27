#ifndef POINT3D_H
#define POINT3D_H

#include "Point2D.h"

class Point3D : public Point2D {
private:
    int z;
    double distFrOrigin;
    
    void setDistFrOrigin();
    
public:
    // Constructors
    Point3D();
    Point3D(int x, int y, int z);
    
    // Getters
    int getZ() const;
    double getScalarValue() const;
    
    // Setters
    void setZ(int z);
    
    // Operator overloading
    friend std::ostream& operator<<(std::ostream& os, const Point3D& pt);
    
    // Comparison operators for sorting
    bool operator==(const Point3D& other) const;
    bool operator<(const Point3D& other) const;
};

#endif
