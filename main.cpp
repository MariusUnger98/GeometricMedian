#include "Point.h"

#include <string.h>
#include <math.h>
#include <exception>
#include <fstream>
#include <sstream>
#include <iterator>

using namespace std;

//input path string and bools for print options
string input = "\0";
bool printP = false, printAv = false, printMin = false, useSearched = false;
double testDist = 1000, precision = 0.001;
//prints help page
void help() {
    cout << endl
         << "--- GeometricMedian.exe ---" << endl
         << "This program will read 2D points from an input file with given syntax" << endl
         << "and will afterwards print their geometric median." << endl
         << endl
         << "how to run:" << endl
         << "GeometricMedian.exe -i <input file> [-h|-p -a -m -s[<test distance> <precision>]]" << endl
         << endl
         << "arguments:" << endl
         << "-i: is used to pass the input file to the program."
         << "The next parameter after '-i' should be the input file path." << endl
         << "-h: prints the help page" << endl
         << "-p: prints all given points from the input file." << endl
         << "-a: also prints the mass average aside from the 2D-Median." << endl
         << "-m: prints the geometric medians minimum euclidean distance." << endl
         << "-s: calculates the geometric median by using a search algorithm." << endl
         << "    there can also be entered a <test distance> and <precision> for this algorithm." << endl
         << "    both can hold double values. first argument after '-s' is <test distance> and second is <precision>." << endl
         << "    it is not mandatory to enter values behind '-s'." << endl
         << "    default values: <test distance> = 1000.0, precision = 0.001" << endl
         << "not existing arguments will be ignored." << endl
         << endl
         << "rules:" << endl
         << "the input file where the points are located needs to be written in following syntax:" << endl
         << "<x> <y>" << endl
         << "the points are differentiated by line breaks." << endl
         << endl
         << "example:" << endl
         << "1 2.5" << endl
         << "4.5 2" << endl
         << "2.23 1.25" << endl
         << "this would create 3 point objects." << endl;
}

//function to print all Points from the given vector
void printAll(vector<Point> points) {
    for(unsigned int i=0; i<points.size(); i++)
        points[i].print();
}

//function to return the average x and y value of all points
Point getAvg(vector<Point> points) {
    double xSum, ySum;
    //count sum of x and y coordinates
    for(unsigned int i=0; i<points.size(); i++) {
        xSum += points[i].getX();
        ySum += points[i].getY();
    }
    return Point(xSum/points.size(), ySum/points.size());
}

//function to check if string contains valid double value
bool isDouble(const string& s) {
    try {
        std::stod(s);
    } catch(...) {
        return false;
    }
    return true;
}

//function to check passed arguments
bool checkArgs(int argc, char ** argv) {
    try {
        //throw error message if there were no arguments entered
        if(argc<=1)
            throw "ERR: There were no arguments entered.";

        for(int i= 1; i<argc; i++) {
            //get input
            if(strcmp(argv[i], "-i") == 0) {
                //check if there exists an argument behind -i
                if(i<argc-1) {
                    input = argv[i+1];
                    i++;
                } else
                    throw "ERR: No file path entered.";
            } else if(strcmp(argv[i], "-h") == 0) {
                //prints help page and returns false
                help();
                return false;
            } else if(strcmp(argv[i], "-p") == 0)
                //print points bool
                printP = true;
            else if(strcmp(argv[i], "-a") == 0)
                //print average bool
                printAv = true;
            else if(strcmp(argv[i], "-m") == 0)
                //print euclidean minimum distance
                printMin = true;
            else if(strcmp(argv[i], "-s") == 0) {
                //calculates geometric median by using a search algorithm
                useSearched = true;
                if(i<argc-1) {
                    //if arg behind -s is double -> set value for test distance
                    if(isDouble(argv[i+1])) {
                        i++;
                        testDist = stod(argv[i]);
                        if(i<argc-1) {
                            //if arg behind test distance is double -> set value for precision
                            if(isDouble(argv[i+1])) {
                                i++;
                                precision = stod(argv[i]);
                            }
                        }
                    }
                }
            }
        }
        //throw error message if there was no filepath entered
        if(strcmp(input.c_str(), "\0")==0)
            throw "ERR: No file path entered.";
        //return true if arguments have been entered correctly
        return true;
    } catch(const char* msg) {
        //show error message and return false
        cout << endl << msg << endl
             << "print help page with argument '-h' to learn more." << endl << endl;
        return false;
    } catch(...) {
        cout << endl << "ERR: You have entered invalid arguments." << endl
             << "print help page with argument '-h' to learn more." << endl << endl;
        return false;
    }
}

//function to get the sum of euclidean distances to one point
double getEuclidSum(Point cur, vector<Point> points) {
    double sum = 0;
    double distX, distY;
    for (unsigned int i=0; i<points.size(); i++) {
        //get absolute x and y distance from current point to another
        distX = fabs(points[i].getX() - cur.getX());
        distY = fabs(points[i].getY() - cur.getY());
        //add euclidean distance to euclidean distance sum
        sum += sqrt((distX * distX) + (distY * distY));
    }
    return sum;
}

//function to get geometrical median
Point getMedian(vector<Point> points) {
    //initiate starting point and starting min dist from (0|0)
    Point current(0, 0);
    double minDist = getEuclidSum(current, points);
    double tempDist;

    //searching for Point obj with least euclidean distance to all points
    for(unsigned int i=0; i<points.size(); i++) {
        //get euclidean distance of temporary point from all points
        tempDist = getEuclidSum(points[i], points);
        /*
        if the temporary points euclidean distance is smaller than the current
        points euclidean distance the current points values and minimum distance
        will be changed to the temporary points corresponding values
        */
        if(tempDist < minDist) {
            minDist = tempDist;
            current.setX(points[i].getX());
            current.setY(points[i].getY());
        }
    }

    if(printMin)
        cout << endl << "minimum sum of euclidean distances:" << endl << minDist << endl;

    return current;
}

//function to get geometric median by using a search algorithm
Point getMedianSearched(vector<Point> points) {
    //initiate directions in -x, +y, +x, -y coordinate directions to find neighbours
    vector<Point> directions = {
        { -1.0, 0.0 },
        { 0.0, 1.0 },
        { 1.0, 0.0 },
        { 0.0, -1.0 }
    };
    //initiate current point as mass center
    Point current = getAvg(points);
    //starting point for minimum distance is euclidean distance sum of mass center
    double minDist = getEuclidSum(current, points);
    double tempDist;
    //variable to check if neighbour with smaller minimum distance was in scope
    bool check;

    //exit while loop if test distance shrank to precision value
    while(testDist > precision) {
        check = false;
        for(int i=0; i<4; i++) {
            Point neighbour;
            //set x and y coordinate to scope where neighbours should be find in
            neighbour.setX(current.getX() + testDist * directions[i].getX());
            neighbour.setY(current.getY() + testDist * directions[i].getY());
            //get sum of neighbors euclidean distance
            tempDist = getEuclidSum(neighbour, points);

            //switch geometric median to new point with smallest euclidean distance sum
            if(tempDist < minDist) {
                minDist = tempDist;
                current.setX(neighbour.getX());
                current.setY(neighbour.getY());
                //neighbour has been found
                check = true;
                break;
            }
        }
        //if no neighbour with smaller min distance has been found reduce test distance
        if(!check)
            testDist /= 2;
    }
    if(printMin)
        cout << endl << "minimum sum of euclidean distances:" << endl << minDist << endl;

    return current;
}

//main
int main(int argc, char ** argv) {

    ifstream in;
    string line;
    vector<Point> points;

    //terminate program if arguments have been entered incorrectly
    if(!checkArgs(argc, argv))
        return 0;

    try {
        //open input stream
        in.open(input.c_str());
    } catch(int e) {
        cout << "ERR: The file '" << input << "' could not be opened or found!" << endl;
    }
    //read each line from file
    while(getline(in, line)) {
        //read line into string stream and put strings into vector divided by spaces
        istringstream iss(line);
        vector<string> lineSub ((istream_iterator<string>(iss)), istream_iterator<string>());
        //ignore empty lines
        if(lineSub.size() < 1)
            continue;

        try {
            //if the user entered a wrong amount of coordinates in input file
            if(lineSub.size() != 2)
                throw "Less or more than 2 coordinates have been found for at least one point in your input file.";
            /*
            convert strings to doubles to get x and y coordinates and
            pass them to Point constructor to initiate a Point obj.
            after that push the new Point obj to the vector which will contain all Point objects
            */
            if(isDouble(lineSub[0]) && isDouble(lineSub[1]))
                points.push_back(Point(stod(lineSub[0]), stod(lineSub[1])));
            else
                throw "There has been entered coordinates which could not be converted to double values.";
        } catch(const char* msg) {
            cout << endl << "ERR: program terminated" << endl;
            cout << msg << endl;
            return 0;
        }
    }
    //prints all Point objects
    if(printP) {
        cout << endl;
        printAll(points);
    }
    //print mass center of points by calling getAvg function and printing returned Point obj
    if(printAv) {
        cout << endl << "point mass center:" << endl;
        getAvg(points).printNoId();
    }

    //creates geometric median point and prints it
    Point medRes;
    if(useSearched)
        medRes = getMedianSearched(points);
    else
        medRes = getMedian(points);
    cout << endl << "geometric median:" << endl;
    medRes.printNoId();
    return 0;
}
