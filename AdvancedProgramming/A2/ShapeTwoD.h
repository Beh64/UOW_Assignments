//  Header guard that prevents this file from being included multiple times
#ifndef SHAPETWOD_H
#define SHAPETWOD_H

#include <string>   //  For string data type
#include <vector>   //  For dynamic array (vector)

using namespace std;

//  Abstract class - cannot create objects directly from this class
class ShapeTwoD {
protected:  //  Child classes can access these
    string name;                        //  Shape name
    bool containsWarpSpace;             //  True if warp space else false
    vector<pair<int, int>> vertices;    //  List of (x,y) coordinate pairs
    
//  Anyone can access this
public:
    // Constructor
    ShapeTwoD(string name, bool containsWarpSpace);
    
    // Virtual destructor - cleans up when object is deleted
    virtual ~ShapeTwoD();
    
    // Getters - Allow reading private / protected data
    string getName() const;             //  Returns the shape's name
    bool getContainsWarpSpace() const;  //  Returns true / false for WS/NS
    
    // Setters
    void setName(string name);          //  Changes the shape's name
    void setContainsWarpSpace(bool containsWarpSpace);  //  Changes WS/NS status
    
    // Virtual functions (must be overridden in derived classes)
    virtual double computeArea() = 0;
    //  Checks if a point (x,y) is completely inside the shape
    virtual bool isPointInShape(int x, int y) = 0;
    //  Checks if a point (x,y) is on the edge/perimeter of the shape
    virtual bool isPointOnShape(int x, int y) = 0;
    //  Returns a formatted string with all shape information
    virtual string toString() = 0;
};

#endif