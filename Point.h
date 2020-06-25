#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include <iostream>
#include <vector>

class Point {
public:
    Point();
    Point(double x, double y);
    void print();
    void printNoId();
    unsigned int getId();
    void setX(double x);
    double getX();
    void setY(double y);
    double getY();
protected:
private:
    unsigned int id;
    double x;
    double y;
};

#endif // POINT_H_INCLUDED
