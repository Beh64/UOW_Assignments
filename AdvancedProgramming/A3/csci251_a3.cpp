/*

CSCI251 Assignment 3 - Data Processing Application

*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
#include "Point2D.h"
#include "Point3D.h"
#include "Line2D.h"
#include "Line3D.h"
#include "MyTemplates.h"

using namespace std;

// Enum for filtering criteria
enum FilterCriteria {
    POINT2D_RECORDS,
    POINT3D_RECORDS,
    LINE2D_RECORDS,
    LINE3D_RECORDS
};

// Enum for sorting criteria
enum SortCriteria {
    X_ORDINATE,
    Y_ORDINATE,
    Z_ORDINATE,
    DIST_FR_ORIGIN,
    PT1_XY,
    PT2_XY,
    LENGTH
};

// Enum for sorting order
enum SortOrder {
    ASCENDING,
    DESCENDING
};

// Data storage containers
vector<Point2D> point2DList;
vector<Point3D> point3DList;
vector<Line2D> line2DList;
vector<Line3D> line3DList;

// Current criteria settings
FilterCriteria currentFilter = POINT2D_RECORDS;
SortCriteria currentSortCriteria = X_ORDINATE;
SortOrder currentSortOrder = ASCENDING;

// Functions
void displayMainMenu();
void readData();
void specifyFilteringCriteria();
void specifySortingCriteria();
void specifySortingOrder();
void viewData();
void storeData();
void clearScreen();
string filterToString(FilterCriteria fc);
string sortCriteriaToString(SortCriteria sc);
string sortOrderToString(SortOrder so);

// Comparator functions for Point2D
bool comparePoint2DByX(const Point2D& a, const Point2D& b) {
    if (a.getX() != b.getX()) return a.getX() < b.getX();
    return a.getY() < b.getY();
}

bool comparePoint2DByY(const Point2D& a, const Point2D& b) {
    if (a.getY() != b.getY()) return a.getY() < b.getY();
    return a.getX() < b.getX();
}

bool comparePoint2DByDist(const Point2D& a, const Point2D& b) {
    return a.getScalarValue() < b.getScalarValue();
}

// Comparator functions for Point3D
bool comparePoint3DByX(const Point3D& a, const Point3D& b) {
    if (a.getX() != b.getX()) return a.getX() < b.getX();
    if (a.getY() != b.getY()) return a.getY() < b.getY();
    return a.getZ() < b.getZ();
}

bool comparePoint3DByY(const Point3D& a, const Point3D& b) {
    if (a.getY() != b.getY()) return a.getY() < b.getY();
    if (a.getX() != b.getX()) return a.getX() < b.getX();
    return a.getZ() < b.getZ();
}

bool comparePoint3DByZ(const Point3D& a, const Point3D& b) {
    if (a.getZ() != b.getZ()) return a.getZ() < b.getZ();
    if (a.getX() != b.getX()) return a.getX() < b.getX();
    return a.getY() < b.getY();
}

bool comparePoint3DByDist(const Point3D& a, const Point3D& b) {
    return a.getScalarValue() < b.getScalarValue();
}

// Comparator functions for Line2D
bool compareLine2DByPt1XY(const Line2D& a, const Line2D& b) {
    Point2D apt1 = a.getPt1();
    Point2D bpt1 = b.getPt1();
    if (apt1.getX() != bpt1.getX()) return apt1.getX() < bpt1.getX();
    if (apt1.getY() != bpt1.getY()) return apt1.getY() < bpt1.getY();
    Point2D apt2 = a.getPt2();
    Point2D bpt2 = b.getPt2();
    if (apt2.getX() != bpt2.getX()) return apt2.getX() < bpt2.getX();
    return apt2.getY() < bpt2.getY();
}

bool compareLine2DByPt2XY(const Line2D& a, const Line2D& b) {
    Point2D apt2 = a.getPt2();
    Point2D bpt2 = b.getPt2();
    if (apt2.getX() != bpt2.getX()) return apt2.getX() < bpt2.getX();
    if (apt2.getY() != bpt2.getY()) return apt2.getY() < bpt2.getY();
    Point2D apt1 = a.getPt1();
    Point2D bpt1 = b.getPt1();
    if (apt1.getX() != bpt1.getX()) return apt1.getX() < bpt1.getX();
    return apt1.getY() < bpt1.getY();
}

bool compareLine2DByLength(const Line2D& a, const Line2D& b) {
    return a.getScalarValue() < b.getScalarValue();
}

// Comparator functions for Line3D
bool compareLine3DByPt1XY(const Line3D& a, const Line3D& b) {
    Point3D apt1 = a.getPt1();
    Point3D bpt1 = b.getPt1();
    if (apt1.getX() != bpt1.getX()) return apt1.getX() < bpt1.getX();
    if (apt1.getY() != bpt1.getY()) return apt1.getY() < bpt1.getY();
    Point3D apt2 = a.getPt2();
    Point3D bpt2 = b.getPt2();
    if (apt2.getX() != bpt2.getX()) return apt2.getX() < bpt2.getX();
    return apt2.getY() < bpt2.getY();
}

bool compareLine3DByPt2XY(const Line3D& a, const Line3D& b) {
    Point3D apt2 = a.getPt2();
    Point3D bpt2 = b.getPt2();
    if (apt2.getX() != bpt2.getX()) return apt2.getX() < bpt2.getX();
    if (apt2.getY() != bpt2.getY()) return apt2.getY() < bpt2.getY();
    Point3D apt1 = a.getPt1();
    Point3D bpt1 = b.getPt1();
    if (apt1.getX() != bpt1.getX()) return apt1.getX() < bpt1.getX();
    return apt1.getY() < bpt1.getY();
}

bool compareLine3DByLength(const Line3D& a, const Line3D& b) {
    return a.getScalarValue() < b.getScalarValue();
}

int main() {
    cout.setf(ios_base::fixed, ios_base::floatfield);
    cout.precision(3);
    
    int choice;
    bool running = true;
    
    while (running) {
        displayMainMenu();
        cout << "\nPlease enter your choice : ";
        
        // Check if input is valid integer
        if (!(cin >> choice)) {
            cin.clear(); // Clear error flags
            cin.ignore(10000, '\n'); // Discard invalid input
            cout << "\n[Error] Invalid input! Please enter a number between 0-6.\n";
            cout << "\nPress Enter to continue...";
            cin.get();
            continue;
        }
        
        // Validate choice is in range
        if (choice < 0 || choice > 6) {
            cout << "\n[Error] Invalid choice! Please enter a number between 0-6.\n";
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
            continue;
        }
        
        switch (choice) {
            case 1:
                readData();
                break;
            case 2:
                specifyFilteringCriteria();
                break;
            case 3:
                specifySortingCriteria();
                break;
            case 4:
                specifySortingOrder();
                break;
            case 5:
                viewData();
                break;
            case 6:
                storeData();
                break;
            case 0:
                cout << "\nThank you for using the program!\n";
                running = false;
                break;
        }
        
        if (running && choice != 5) {
            cout << "\nGoing back to main menu ...\n";
            cin.ignore();
            cin.get();
        }
    }
    
    return 0;
}

void displayMainMenu() {
    cout << "\n";
    cout << "Student ID   : 9890385\n";
    cout << "Student Name : Joel Beh Zhirong\n";
    cout << "----------------------------------------------\n";
    cout << "Welcome to Assn3 program!\n\n";
    cout << "1)	Read in data\n";
    cout << "2)	Specify filtering criteria (current : " << filterToString(currentFilter) << ")\n";
    cout << "3)	Specify sorting criteria (current : " << sortCriteriaToString(currentSortCriteria) << ")\n";
    cout << "4)	Specify sorting order (current : " << sortOrderToString(currentSortOrder) << ")\n";
    cout << "5)	View data\n";
    cout << "6)	Store data\n";
    cout << "0)	Exit\n";
}

void readData() {
    string filename;
    cout << "\nPlease enter filename : ";
    cin >> filename;
    
    ifstream infile(filename);
    if (!infile) {
        cout << "Error opening file: " << filename << endl;
        return;
    }
    
    // Clear existing data
    point2DList.clear();
    point3DList.clear();
    line2DList.clear();
    line3DList.clear();
    
    string line;
    int recordCount = 0;
    
    while (getline(infile, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string className;
        char comma;
        
        // Read class name
        getline(ss, className, ',');
        ss >> ws; // Skip whitespace
        
        if (className == "Point2D") {
            char lbracket, rbracket;
            int x, y;
            ss >> lbracket >> x >> comma >> y >> rbracket;
            point2DList.push_back(Point2D(x, y));
            recordCount++;
        }
        else if (className == "Point3D") {
            char lbracket, rbracket;
            int x, y, z;
            ss >> lbracket >> x >> comma >> y >> comma >> z >> rbracket;
            point3DList.push_back(Point3D(x, y, z));
            recordCount++;
        }
        else if (className == "Line2D") {
            char lbracket1, rbracket1, lbracket2, rbracket2;
            int x1, y1, x2, y2;
            ss >> lbracket1 >> x1 >> comma >> y1 >> rbracket1 >> comma;
            ss >> lbracket2 >> x2 >> comma >> y2 >> rbracket2;
            Point2D pt1(x1, y1);
            Point2D pt2(x2, y2);
            line2DList.push_back(Line2D(pt1, pt2));
            recordCount++;
        }
        else if (className == "Line3D") {
            char lbracket1, rbracket1, lbracket2, rbracket2;
            int x1, y1, z1, x2, y2, z2;
            ss >> lbracket1 >> x1 >> comma >> y1 >> comma >> z1 >> rbracket1 >> comma;
            ss >> lbracket2 >> x2 >> comma >> y2 >> comma >> z2 >> rbracket2;
            Point3D pt1(x1, y1, z1);
            Point3D pt2(x2, y2, z2);
            line3DList.push_back(Line3D(pt1, pt2));
            recordCount++;
        }
    }
    
    infile.close();
    
    // Remove duplicates
    sort(point2DList.begin(), point2DList.end());
    point2DList.erase(unique(point2DList.begin(), point2DList.end()), point2DList.end());
    
    sort(point3DList.begin(), point3DList.end());
    point3DList.erase(unique(point3DList.begin(), point3DList.end()), point3DList.end());
    
    sort(line2DList.begin(), line2DList.end());
    line2DList.erase(unique(line2DList.begin(), line2DList.end()), line2DList.end());
    
    sort(line3DList.begin(), line3DList.end());
    line3DList.erase(unique(line3DList.begin(), line3DList.end()), line3DList.end());
    
    cout << "\n" << recordCount << " records read in successfully!\n";
}

void specifyFilteringCriteria() {
    cout << "\n[ Specifying filtering criteria (current : " << filterToString(currentFilter) << ") ]\n\n";
    cout << "a)	Point2D records\n";
    cout << "b)	Point3D records\n";
    cout << "c)	Line2D records\n";
    cout << "d)	Line3D records\n\n";
    
    char choice;
    cout << "Please enter your criteria (a - d) : ";
    cin >> choice;
    
    switch (choice) {
        case 'a':
            currentFilter = POINT2D_RECORDS;
            currentSortCriteria = X_ORDINATE;
            cout << "Filter criteria successfully set to 'Point2D'!\n";
            break;
        case 'b':
            currentFilter = POINT3D_RECORDS;
            currentSortCriteria = X_ORDINATE;
            cout << "Filter criteria successfully set to 'Point3D'!\n";
            break;
        case 'c':
            currentFilter = LINE2D_RECORDS;
            currentSortCriteria = PT1_XY;
            cout << "Filter criteria successfully set to 'Line2D'!\n";
            break;
        case 'd':
            currentFilter = LINE3D_RECORDS;
            currentSortCriteria = PT1_XY;
            cout << "Filter criteria successfully set to 'Line3D'!\n";
            break;
        default:
            cout << "Invalid choice!\n";
    }
}

void specifySortingCriteria() {
    cout << "\n[ Specifying sorting criteria (current : " << sortCriteriaToString(currentSortCriteria) << ") ]\n\n";
    
    if (currentFilter == POINT2D_RECORDS) {
        cout << "a)	X ordinate value\n";
        cout << "b)	Y ordinate value\n";
        cout << "c)	Dist. Fr Origin value\n\n";
        
        char choice;
        cout << "Please enter your criteria (a - c) : ";
        cin >> choice;
        
        switch (choice) {
            case 'a':
                currentSortCriteria = X_ORDINATE;
                cout << "Sorting criteria successfully set to 'X ordinate'!\n";
                break;
            case 'b':
                currentSortCriteria = Y_ORDINATE;
                cout << "Sorting criteria successfully set to 'Y ordinate'!\n";
                break;
            case 'c':
                currentSortCriteria = DIST_FR_ORIGIN;
                cout << "Sorting criteria successfully set to 'Dist. Fr Origin'!\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    }
    else if (currentFilter == POINT3D_RECORDS) {
        cout << "a)	X ordinate value\n";
        cout << "b)	Y ordinate value\n";
        cout << "c)	Z ordinate value\n";
        cout << "d)	Dist. Fr Origin value\n\n";
        
        char choice;
        cout << "Please enter your criteria (a - d) : ";
        cin >> choice;
        
        switch (choice) {
            case 'a':
                currentSortCriteria = X_ORDINATE;
                cout << "Sorting criteria successfully set to 'X ordinate'!\n";
                break;
            case 'b':
                currentSortCriteria = Y_ORDINATE;
                cout << "Sorting criteria successfully set to 'Y ordinate'!\n";
                break;
            case 'c':
                currentSortCriteria = Z_ORDINATE;
                cout << "Sorting criteria successfully set to 'Z ordinate'!\n";
                break;
            case 'd':
                currentSortCriteria = DIST_FR_ORIGIN;
                cout << "Sorting criteria successfully set to 'Dist. Fr Origin'!\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    }
    else if (currentFilter == LINE2D_RECORDS || currentFilter == LINE3D_RECORDS) {
        cout << "a)	Pt. 1's (x, y) values\n";
        cout << "b)	Pt. 2's (x, y) values\n";
        cout << "c)	Length value\n\n";
        
        char choice;
        cout << "Please enter your criteria (a - c) : ";
        cin >> choice;
        
        switch (choice) {
            case 'a':
                currentSortCriteria = PT1_XY;
                cout << "Sorting criteria successfully set to 'Pt. 1'!\n";
                break;
            case 'b':
                currentSortCriteria = PT2_XY;
                cout << "Sorting criteria successfully set to 'Pt. 2'!\n";
                break;
            case 'c':
                currentSortCriteria = LENGTH;
                cout << "Sorting criteria successfully set to 'Length'!\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    }
}

void specifySortingOrder() {
    cout << "\n[ Specifying sorting order (current : " << sortOrderToString(currentSortOrder) << ") ]\n\n";
    cout << "a)	ASC (Ascending order)\n";
    cout << "b)	DSC (Descending order)\n\n";
    
    char choice;
    cout << "Please enter your criteria (a - b) : ";
    cin >> choice;
    
    switch (choice) {
        case 'a':
            currentSortOrder = ASCENDING;
            cout << "Sorting order successfully set to 'ASC'!\n";
            break;
        case 'b':
            currentSortOrder = DESCENDING;
            cout << "Sorting order successfully set to 'DSC'!\n";
            break;
        default:
            cout << "Invalid choice!\n";
    }
}

void viewData() {
    cout << "\n[ View data ... ]\n";
    cout << "filtering criteria : " << filterToString(currentFilter) << "\n";
    cout << "sorting criteria : " << sortCriteriaToString(currentSortCriteria) << "\n";
    cout << "sorting order : " << sortOrderToString(currentSortOrder) << "\n\n";
    
    if (currentFilter == POINT2D_RECORDS) {
        vector<Point2D> tempList = point2DList;
        
        // Sort based on criteria
        if (currentSortCriteria == X_ORDINATE) {
            sort(tempList.begin(), tempList.end(), comparePoint2DByX);
        } else if (currentSortCriteria == Y_ORDINATE) {
            sort(tempList.begin(), tempList.end(), comparePoint2DByY);
        } else if (currentSortCriteria == DIST_FR_ORIGIN) {
            sort(tempList.begin(), tempList.end(), comparePoint2DByDist);
        }
        
        // Reverse if descending
        if (currentSortOrder == DESCENDING) {
            reverse(tempList.begin(), tempList.end());
        }
        
        // Display header
        cout << "Point2D\n";
        cout << "   X    Y   Dist. Fr Origin\n";
        cout << "-----------------------------\n";
        
        // Display data
        for (const auto& pt : tempList) {
            cout << setw(4) << right << pt.getX() << ", "
                 << setw(4) << right << pt.getY() << "   "
                 << setw(7) << right << fixed << setprecision(3) << pt.getScalarValue() << "\n";
        }
    }
    else if (currentFilter == POINT3D_RECORDS) {
        vector<Point3D> tempList = point3DList;
        
        // Sort based on criteria
        if (currentSortCriteria == X_ORDINATE) {
            sort(tempList.begin(), tempList.end(), comparePoint3DByX);
        } else if (currentSortCriteria == Y_ORDINATE) {
            sort(tempList.begin(), tempList.end(), comparePoint3DByY);
        } else if (currentSortCriteria == Z_ORDINATE) {
            sort(tempList.begin(), tempList.end(), comparePoint3DByZ);
        } else if (currentSortCriteria == DIST_FR_ORIGIN) {
            sort(tempList.begin(), tempList.end(), comparePoint3DByDist);
        }
        
        // Reverse if descending
        if (currentSortOrder == DESCENDING) {
            reverse(tempList.begin(), tempList.end());
        }
        
        // Display header
        cout << "Point3D\n";
        cout << "   X    Y    Z   Dist. Fr Origin\n";
        cout << "------------------------------------\n";
        
        // Display data
        for (const auto& pt : tempList) {
            cout << setw(4) << right << pt.getX() << ", "
                 << setw(4) << right << pt.getY() << ", "
                 << setw(4) << right << pt.getZ() << "   "
                 << setw(7) << right << fixed << setprecision(3) << pt.getScalarValue() << "\n";
        }
    }
    else if (currentFilter == LINE2D_RECORDS) {
        vector<Line2D> tempList = line2DList;
        
        // Sort based on criteria
        if (currentSortCriteria == PT1_XY) {
            sort(tempList.begin(), tempList.end(), compareLine2DByPt1XY);
        } else if (currentSortCriteria == PT2_XY) {
            sort(tempList.begin(), tempList.end(), compareLine2DByPt2XY);
        } else if (currentSortCriteria == LENGTH) {
            sort(tempList.begin(), tempList.end(), compareLine2DByLength);
        }
        
        // Reverse if descending
        if (currentSortOrder == DESCENDING) {
            reverse(tempList.begin(), tempList.end());
        }
        
        // Display header
        cout << "Line2D\n";
        cout << "  P1-X  P1-Y   P2-X  P2-Y      Length\n";
        cout << "----------------------------------------\n";
        
        // Display data
        for (const auto& line : tempList) {
            Point2D pt1 = line.getPt1();
            Point2D pt2 = line.getPt2();
            cout << setw(5) << right << pt1.getX() << ", "
                 << setw(5) << right << pt1.getY() << "   "
                 << setw(5) << right << pt2.getX() << ", "
                 << setw(5) << right << pt2.getY() << "   "
                 << setw(8) << right << fixed << setprecision(3) << line.getScalarValue() << "\n";
        }
    }
    else if (currentFilter == LINE3D_RECORDS) {
        vector<Line3D> tempList = line3DList;
        
        // Sort based on criteria
        if (currentSortCriteria == PT1_XY) {
            sort(tempList.begin(), tempList.end(), compareLine3DByPt1XY);
        } else if (currentSortCriteria == PT2_XY) {
            sort(tempList.begin(), tempList.end(), compareLine3DByPt2XY);
        } else if (currentSortCriteria == LENGTH) {
            sort(tempList.begin(), tempList.end(), compareLine3DByLength);
        }
        
        // Reverse if descending
        if (currentSortOrder == DESCENDING) {
            reverse(tempList.begin(), tempList.end());
        }
        
        // Display header
        cout << "Line3D\n";
        cout << "  P1-X  P1-Y  P1-Z   P2-X  P2-Y  P2-Z      Length\n";
        cout << "---------------------------------------------------\n";
        
        // Display data
        for (const auto& line : tempList) {
            Point3D pt1 = line.getPt1();
            Point3D pt2 = line.getPt2();
            cout << setw(5) << right << pt1.getX() << ", "
                 << setw(5) << right << pt1.getY() << ", "
                 << setw(5) << right << pt1.getZ() << "   "
                 << setw(5) << right << pt2.getX() << ", "
                 << setw(5) << right << pt2.getY() << ", "
                 << setw(5) << right << pt2.getZ() << "   "
                 << setw(8) << right << fixed << setprecision(3) << line.getScalarValue() << "\n";
        }
    }
    
    cout << "\nPress any key to go back to main menu ...\n";
    cin.ignore();
    cin.get();
}

void storeData() {
    string filename;
    cout << "\nPlease enter filename : ";
    cin >> filename;
    
    ofstream outfile(filename);
    if (!outfile) {
        cout << "Error creating file: " << filename << endl;
        return;
    }
    
    outfile.setf(ios_base::fixed, ios_base::floatfield);
    outfile.precision(3);
    
    int recordCount = 0;
    
    if (currentFilter == POINT2D_RECORDS) {
        vector<Point2D> tempList = point2DList;
        
        // Sort based on criteria
        if (currentSortCriteria == X_ORDINATE) {
            sort(tempList.begin(), tempList.end(), comparePoint2DByX);
        } else if (currentSortCriteria == Y_ORDINATE) {
            sort(tempList.begin(), tempList.end(), comparePoint2DByY);
        } else if (currentSortCriteria == DIST_FR_ORIGIN) {
            sort(tempList.begin(), tempList.end(), comparePoint2DByDist);
        }
        
        // Reverse if descending
        if (currentSortOrder == DESCENDING) {
            reverse(tempList.begin(), tempList.end());
        }
        
        // Write header
        outfile << "Point2D\n";
        outfile << "   X    Y   Dist. Fr Origin\n";
        outfile << "-----------------------------\n";
        
        // Write data
        for (const auto& pt : tempList) {
            outfile << setw(4) << right << pt.getX() << ", "
                    << setw(4) << right << pt.getY() << "   "
                    << setw(7) << right << pt.getScalarValue() << "\n";
            recordCount++;
        }
    }
    else if (currentFilter == POINT3D_RECORDS) {
        vector<Point3D> tempList = point3DList;
        
        // Sort based on criteria
        if (currentSortCriteria == X_ORDINATE) {
            sort(tempList.begin(), tempList.end(), comparePoint3DByX);
        } else if (currentSortCriteria == Y_ORDINATE) {
            sort(tempList.begin(), tempList.end(), comparePoint3DByY);
        } else if (currentSortCriteria == Z_ORDINATE) {
            sort(tempList.begin(), tempList.end(), comparePoint3DByZ);
        } else if (currentSortCriteria == DIST_FR_ORIGIN) {
            sort(tempList.begin(), tempList.end(), comparePoint3DByDist);
        }
        
        // Reverse if descending
        if (currentSortOrder == DESCENDING) {
            reverse(tempList.begin(), tempList.end());
        }
        
        // Write header
        outfile << "Point3D\n";
        outfile << "   X    Y    Z   Dist. Fr Origin\n";
        outfile << "------------------------------------\n";
        
        // Write data
        for (const auto& pt : tempList) {
            outfile << setw(4) << right << pt.getX() << ", "
                    << setw(4) << right << pt.getY() << ", "
                    << setw(4) << right << pt.getZ() << "   "
                    << setw(7) << right << pt.getScalarValue() << "\n";
            recordCount++;
        }
    }
    else if (currentFilter == LINE2D_RECORDS) {
        vector<Line2D> tempList = line2DList;
        
        // Sort based on criteria
        if (currentSortCriteria == PT1_XY) {
            sort(tempList.begin(), tempList.end(), compareLine2DByPt1XY);
        } else if (currentSortCriteria == PT2_XY) {
            sort(tempList.begin(), tempList.end(), compareLine2DByPt2XY);
        } else if (currentSortCriteria == LENGTH) {
            sort(tempList.begin(), tempList.end(), compareLine2DByLength);
        }
        
        // Reverse if descending
        if (currentSortOrder == DESCENDING) {
            reverse(tempList.begin(), tempList.end());
        }
        
        // Write header
        outfile << "Line2D\n";
        outfile << "  P1-X  P1-Y   P2-X  P2-Y      Length\n";
        outfile << "----------------------------------------\n";
        
        // Write data
        for (const auto& line : tempList) {
            Point2D pt1 = line.getPt1();
            Point2D pt2 = line.getPt2();
            outfile << setw(5) << right << pt1.getX() << ", "
                    << setw(5) << right << pt1.getY() << "   "
                    << setw(5) << right << pt2.getX() << ", "
                    << setw(5) << right << pt2.getY() << "   "
                    << setw(8) << right << line.getScalarValue() << "\n";
            recordCount++;
        }
    }
    else if (currentFilter == LINE3D_RECORDS) {
        vector<Line3D> tempList = line3DList;
        
        // Sort based on criteria
        if (currentSortCriteria == PT1_XY) {
            sort(tempList.begin(), tempList.end(), compareLine3DByPt1XY);
        } else if (currentSortCriteria == PT2_XY) {
            sort(tempList.begin(), tempList.end(), compareLine3DByPt2XY);
        } else if (currentSortCriteria == LENGTH) {
            sort(tempList.begin(), tempList.end(), compareLine3DByLength);
        }
        
        // Reverse if descending
        if (currentSortOrder == DESCENDING) {
            reverse(tempList.begin(), tempList.end());
        }
        
        // Write header
        outfile << "Line3D\n";
        outfile << "  P1-X  P1-Y  P1-Z   P2-X  P2-Y  P2-Z      Length\n";
        outfile << "---------------------------------------------------\n";
        
        // Write data
        for (const auto& line : tempList) {
            Point3D pt1 = line.getPt1();
            Point3D pt2 = line.getPt2();
            outfile << setw(5) << right << pt1.getX() << ", "
                    << setw(5) << right << pt1.getY() << ", "
                    << setw(5) << right << pt1.getZ() << "   "
                    << setw(5) << right << pt2.getX() << ", "
                    << setw(5) << right << pt2.getY() << ", "
                    << setw(5) << right << pt2.getZ() << "   "
                    << setw(8) << right << line.getScalarValue() << "\n";
            recordCount++;
        }
    }
    
    outfile.close();
    cout << "\n" << recordCount << " records output successfully!\n";
}

string filterToString(FilterCriteria fc) {
    switch (fc) {
        case POINT2D_RECORDS: return "Point2D";
        case POINT3D_RECORDS: return "Point3D";
        case LINE2D_RECORDS: return "Line2D";
        case LINE3D_RECORDS: return "Line3D";
        default: return "Unknown";
    }
}

string sortCriteriaToString(SortCriteria sc) {
    switch (sc) {
        case X_ORDINATE: return "x-ordinate";
        case Y_ORDINATE: return "y-ordinate";
        case Z_ORDINATE: return "z-ordinate";
        case DIST_FR_ORIGIN: return "Dist. Fr Origin";
        case PT1_XY: return "Pt. 1";
        case PT2_XY: return "Pt. 2";
        case LENGTH: return "Length";
        default: return "Unknown";
    }
}

string sortOrderToString(SortOrder so) {
    switch (so) {
        case ASCENDING: return "ASC";
        case DESCENDING: return "DSC";
        default: return "Unknown";
    }
}
