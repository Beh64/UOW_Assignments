
#include <iostream>     //  For cin, cout
#include <string>       //  For string type
#include <vector>       //  For vector (not used but good practice)
#include <algorithm>    //  For sorting algorithms

//  Include all shape classes
#include "ShapeTwoD.h"
#include "Square.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Cross.h"

using namespace std;


ShapeTwoD* shapes[100]; //  Array of pointers to shapes (can hold up to 100 shapes)

int shapeCount = 0;     //  Counter for how many shapes are currently stored

void displayHeader();
void displayMenu();
void inputSensorData();
void computeArea();
void printShapesReport();
void sortShapeData();


int main() {
    int choice; //  Stores user's menu choice
    
    //  Main program loop - runs until user chooses to quit
    while (true) {
        displayHeader();
        displayMenu();
        
        cout << "Please enter your choice : ";
        
        //  Input validation: Check if user entered a valid number
        if (!(cin >> choice)) {
            cin.clear();                    // Clear error state
            cin.ignore(10000, '\n');        // Remove bad input from buffer
            cout << endl;
            cout << "Invalid input! Please enter a number (1-5)." << endl;
            cout << endl;
            continue;  // Go back to start of loop (show menu again)
        }
        
        cout << endl;
        
        switch (choice) {
            case 1:
                inputSensorData();
                break;
            case 2:
                computeArea();
                break;
            case 3:
                printShapesReport();
                break;
            case 4:
                sortShapeData();
                break;
            case 5:
                cout << "Exiting program. Thank you!" << endl;
                //  Clean up memory - delete all dynamically allocated shapes
                for (int i = 0; i < shapeCount; i++) {
                    delete shapes[i];
                }
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        cout << endl;
    }
    
    return 0;
}


//  Displays student information banner at top of each screen
void displayHeader() {
    cout << "Student ID    : 9890385" << endl;
    cout << "Student Name  : Joel Beh Zhirong" << endl;
    cout << "-----------------------------------------------------" << endl;
}


//  Shows main menu options (1-5) to the user
void displayMenu() {
    cout << "Welcome to Assn2 program!" << endl;
    cout << endl;
    cout << "1)   Input sensor data" << endl;
    cout << "2)   Compute area (for all records)" << endl;
    cout << "3)   Print shapes report" << endl;
    cout << "4)   Sort shape data" << endl;
    cout << "5)   Quit" << endl;
    cout << endl;
}


// FUNCTION: inputSensorData

/*
    Gets shape data from user and stores it in the global array
    Uses polymorphism: Different shape types stored in same array
 */
void inputSensorData() {
    cout << "[ Input sensor data ]" << endl;
    
    string shapeName;
    string specialType;
    
    // Get shape name with validation
    cout << "Please enter name of shape : ";
    cin >> shapeName;
    cin.ignore(10000, '\n');  // Clear any remaining input
    
    // Validate shape name - must be Square, Rectangle, Circle, or Cross
    while (shapeName != "Square" && shapeName != "square" &&
           shapeName != "Rectangle" && shapeName != "rectangle" &&
           shapeName != "Circle" && shapeName != "circle" &&
           shapeName != "Cross" && shapeName != "cross") {
        cout << "Invalid shape name! Please enter: Square, Rectangle, Circle, or Cross." << endl;
        cout << "Please enter name of shape : ";
        cin >> shapeName;
        cin.ignore(10000, '\n');  // Clear buffer after each attempt
    }
    
    // Get special type with validation
    cout << "Please enter special type : ";
    cin >> specialType;
    cin.ignore(10000, '\n');  // Clear any remaining input (spaces, extra words)
    
    // Validate special type - must be WS, ws, NS, or ns
    while (specialType != "WS" && specialType != "ws" && 
           specialType != "NS" && specialType != "ns") {
        cout << "Invalid special type! Please enter 'WS' or 'NS'." << endl;
        cout << "Please enter special type : ";
        cin >> specialType;
        cin.ignore(10000, '\n');  // Clear buffer after each attempt
    }
    
    bool isWarpSpace = (specialType == "WS" || specialType == "ws");
    
    if (shapeName == "Square" || shapeName == "square") {
        Square* square = new Square(isWarpSpace);
        
        int x[4], y[4];
        for (int i = 0; i < 4; i++) {
            cout << "Please enter x-ordinate of pt." << (i+1) << " : ";
            while (!(cin >> x[i])) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input! Please enter a number." << endl;
                cout << "Please enter x-ordinate of pt." << (i+1) << " : ";
            }
            cout << "Please enter y-ordinate of pt." << (i+1) << " : ";
            while (!(cin >> y[i])) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input! Please enter a number." << endl;
                cout << "Please enter y-ordinate of pt." << (i+1) << " : ";
            }
        }
        
        square->setVertices(x[0], y[0], x[1], y[1], x[2], y[2], x[3], y[3]);
        shapes[shapeCount++] = square;
        
        cout << "\nRecord successfully stored. Going back to main menu ..." << endl;
        
    } else if (shapeName == "Rectangle" || shapeName == "rectangle") {
        Rectangle* rectangle = new Rectangle(isWarpSpace);
        
        int x[4], y[4];
        for (int i = 0; i < 4; i++) {
            cout << "Please enter x-ordinate of pt." << (i+1) << " : ";
            while (!(cin >> x[i])) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input! Please enter a number." << endl;
                cout << "Please enter x-ordinate of pt." << (i+1) << " : ";
            }
            cout << "Please enter y-ordinate of pt." << (i+1) << " : ";
            while (!(cin >> y[i])) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input! Please enter a number." << endl;
                cout << "Please enter y-ordinate of pt." << (i+1) << " : ";
            }
        }
        
        rectangle->setVertices(x[0], y[0], x[1], y[1], x[2], y[2], x[3], y[3]);
        shapes[shapeCount++] = rectangle;
        
        cout << "\nRecord successfully stored. Going back to main menu ..." << endl;
        
    } else if (shapeName == "Circle" || shapeName == "circle") {
        Circle* circle = new Circle(isWarpSpace);
        
        int cx, cy, radius;
        
        // Get center x with validation
        cout << "Please enter x-ordinate of center : ";
        while (!(cin >> cx)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a number." << endl;
            cout << "Please enter x-ordinate of center : ";
        }
        
        // Get center y with validation
        cout << "Please enter y-ordinate of center : ";
        while (!(cin >> cy)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a number." << endl;
            cout << "Please enter y-ordinate of center : ";
        }
        
        // Get radius with validation
        cout << "Please enter radius (units) : ";
        while (!(cin >> radius)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a number." << endl;
            cout << "Please enter radius (units) : ";
        }
        
        circle->setCenter(cx, cy);
        circle->setRadius(radius);
        shapes[shapeCount++] = circle;
        
        cout << "\nRecord successfully stored. Going back to main menu ..." << endl;
        
    } else if (shapeName == "Cross" || shapeName == "cross") {
        Cross* cross = new Cross(isWarpSpace);
        
        int vertices[12][2];
        for (int i = 0; i < 12; i++) {
            cout << "Please enter x-ordinate of pt." << (i+1) << " : ";
            while (!(cin >> vertices[i][0])) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input! Please enter a number." << endl;
                cout << "Please enter x-ordinate of pt." << (i+1) << " : ";
            }
            cout << "Please enter y-ordinate of pt." << (i+1) << " : ";
            while (!(cin >> vertices[i][1])) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input! Please enter a number." << endl;
                cout << "Please enter y-ordinate of pt." << (i+1) << " : ";
            }
        }
        
        cross->setVertices(vertices);
        shapes[shapeCount++] = cross;
        
        cout << "\nRecord successfully stored. Going back to main menu ..." << endl;
        
    } else {
        cout << "Unknown shape type!" << endl;
    }
}

void computeArea() {
    
    if (shapeCount == 0) {
        cout << "No shapes available!" << endl;
        return;
    }
    
    //  Loop through all shapes and compute areas
    for (int i = 0; i < shapeCount; i++) {
        //  Safety check: Ensure pointer is valid
        if (shapes[i] == nullptr) {
            cout << "WARNING: Shape " << i << " is null, skipping..." << endl;
            continue;
        }
        shapes[i]->computeArea();   //  Polymorphic call
    }
    
    cout << "Computation completed! ( " << shapeCount << " records were updated )" << endl;
}

void printShapesReport() {
    
    if (shapeCount == 0) {
        cout << "No shapes available!" << endl;
        return;
    }
    
    cout << "Total no. of records available = " << shapeCount << endl;
    cout << endl;
    
    for (int i = 0; i < shapeCount; i++) {
        cout << "Shape [" << i << "]" << endl;
        
        //  Safety check: Ensure pointer is valid before accessing
        if (shapes[i] == nullptr) {
            cout << "ERROR: Shape " << i << " is null (not properly initialized)!" << endl;
            cout << "Please delete this shape and re-add it." << endl;
            cout << endl;
            continue;  //  Skip to next shape
        }
        
        //  Print the shape details
        cout << shapes[i]->toString() << endl;
        cout << endl;
    }
}

void sortShapeData() {

    if (shapeCount == 0) {
        cout << "No shapes available!" << endl;
        return;
    }
    
    cout << "a)   Sort by area (ascending)" << endl;
    cout << "b)   Sort by area (descending)" << endl;
    cout << "c)   Sort by special type and area" << endl;
    cout << endl;
    
    char sortChoice;
    cout << "Please select sort option (q to go main menu) : ";
    cin >> sortChoice;
    
    //  Clear any remaining characters in input buffer
    //  This prevents leftover input from affecting next menu choice
    cin.ignore(10000, '\n');
    
    cout << endl;
    
    if (sortChoice == 'q' || sortChoice == 'Q') {
        return;
    }
    
    if (sortChoice == 'a' || sortChoice == 'A') {
        // Sort by area ascending
        for (int i = 0; i < shapeCount - 1; i++) {
            for (int j = i + 1; j < shapeCount; j++) {
                if (shapes[i]->computeArea() > shapes[j]->computeArea()) {
                    ShapeTwoD* temp = shapes[i];
                    shapes[i] = shapes[j];
                    shapes[j] = temp;
                }
            }
        }
        cout << "Sort by area (smallest to largest) ..." << endl;
        
    } else if (sortChoice == 'b' || sortChoice == 'B') {
        // Sort by area descending
        for (int i = 0; i < shapeCount - 1; i++) {
            for (int j = i + 1; j < shapeCount; j++) {
                if (shapes[i]->computeArea() < shapes[j]->computeArea()) {
                    ShapeTwoD* temp = shapes[i];
                    shapes[i] = shapes[j];
                    shapes[j] = temp;
                }
            }
        }
        cout << "Sort by area (largest to smallest) ..." << endl;
        
    } else if (sortChoice == 'c' || sortChoice == 'C') {
        // Sort by special type and area
        // WS shapes first (descending by area), then NS shapes (descending by area)
        for (int i = 0; i < shapeCount - 1; i++) {
            for (int j = i + 1; j < shapeCount; j++) {
                bool swap = false;
                
                bool iWS = shapes[i]->getContainsWarpSpace();
                bool jWS = shapes[j]->getContainsWarpSpace();
                
                if (!iWS && jWS) {
                    // j is WS, i is NS - swap
                    swap = true;
                } else if (iWS == jWS) {
                    // Same type, sort by area descending
                    if (shapes[i]->computeArea() < shapes[j]->computeArea()) {
                        swap = true;
                    }
                }
                
                if (swap) {
                    ShapeTwoD* temp = shapes[i];
                    shapes[i] = shapes[j];
                    shapes[j] = temp;
                }
            }
        }
        cout << "Sort by special type and area ..." << endl;
        
    } else {
        cout << "Invalid sort option!" << endl;
        return;
    }
    
    cout << endl;
    
    // Print sorted shapes
    for (int i = 0; i < shapeCount; i++) {
        cout << "Shape [" << i << "]" << endl;
        cout << shapes[i]->toString() << endl;
        cout << endl;
    }
}
