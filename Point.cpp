#include "Point.h"

using namespace std;

//static integer to declare id
static unsigned int idCount = 0;

//ctors
Point::Point(){};
Point::Point(double x, double y) {
    id = idCount++;
    setX(x);
    setY(y);
}
//print Point obj
void Point::print() {
    cout << "P" << id << ": (" << x << "|" << y << ")" << endl;
}
//print Point obj without id
void Point::printNoId() {
    cout << "P: (" << x << "|" << y << ")" << endl;
}
//id getter
unsigned int Point::getId() {
    return id;
}
//X coordinate setter/getter
void Point::setX(double x) {
    this->x = x;
}
double Point::getX() {
    return x;
}
//Y coordinate setter/getter
void Point::setY(double y) {
    this->y = y;
}
double Point::getY() {
    return y;
}
