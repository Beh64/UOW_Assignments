#include "ShapeTwoD.h"

//  Constructor implementation
ShapeTwoD::ShapeTwoD(string name, bool containsWarpSpace) {
    this->name = name;      //  Stores the name
    this->containsWarpSpace = containsWarpSpace;    //  Stores WS/NS status
    //  'this->' is used to distinguish parameter from member variable
}
//  Cleans up when object is deleted
ShapeTwoD::~ShapeTwoD() {
    // Virtual destructor
}
//  Returns the shape's name
string ShapeTwoD::getName() const {
    return name;
}
//  Getter for warp space status - returns true / false
bool ShapeTwoD::getContainsWarpSpace() const {
    return containsWarpSpace;
}
//  Allows the changing the shape's name
void ShapeTwoD::setName(string name) {
    this->name = name;
}
//  Setter for wrap space allowing changing WS/NS status
void ShapeTwoD::setContainsWarpSpace(bool containsWarpSpace) {
    this->containsWarpSpace = containsWarpSpace;
}