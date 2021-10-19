/* 
Daniel Reuveni
*/
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

// ------------ DO NOT CHANGE -----------
class Point{
public:
	float x,y;
	Point(float x,float y):x(x),y(y){}
};

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------

// implement
float distance(const Point& a, const Point& b);
Point getCenterOfCircle(float a1, float a2, float a3, float a4);
Circle from2Points(const Point& a, const Point& b);
Circle from3Points(const Point& a, const Point& b, const Point& c);
Circle recursiveAlgorithm(vector<Point>& P, vector<Point> R, size_t size);
Circle algorithm(Point** points, size_t size);

Circle findMinCircle(Point** points,size_t size){
	return algorithm(points, size);
}

// you may add helper functions here

Circle findMinCircleLittle(Point* points, size_t size) {
    if (size > 3)
        return Circle(Point(0,0),0);
    if (size == 0){
        return Circle(Point(0,0),0);
    }
    if (size == 1){
        return Circle(points[0] ,0);
    }
    if (size == 2){
        return from2Points(points[0], points[1]);
    }
    if (size == 3){
        return from3Points(points[0], points[1], points[2]);
    }

    return Circle(Point(0,0),0); 
}

bool pointInCircle(const Point& a, const Circle& c){
    if (c.radius >= distance(c.center, a))
        return true;
    return false;
}

float distance(const Point& a, const Point& b){
    float d = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
    return d;
}

Circle from2Points(const Point& a, const Point& b){
    return Circle(Point((a.x +b.x) / 2, (a.y + b.y) / 2), distance(a, b) / 2);
}

Circle from3Points(const Point& a, const Point& b, const Point& c){
    Point p = getCenterOfCircle(b.x - a.x, b.y - a.y, c.x - a.x, c.y - a.y);
    p.x = a.x + p.x;
    p.y = a.y + p.y;
    float r = distance(a, p);
    return Circle(Point(p.x ,p.y), r);
}

Point getCenterOfCircle(float a1, float a2, float a3, float a4) {
    float b, c, d, x, y;
    b = pow(a1, 2) + pow (a2, 2);
    c = pow(a3, 2) + pow (a4, 2);
    d = a1*a4 - a2*a3;

    x = (b * a4 - c * a2) / 2*d;
    y = (c * a1 - b * a3) / 2*d;
    return Point(x, y);

}

Circle algorithm(Point** points, size_t size) 
{ 
    vector<Point> P(*points, *points + size); 
    vector<Point> R;
    return recursiveAlgorithm(P, R, P.size()); 
} 

Circle recursiveAlgorithm(vector<Point>& P, vector<Point> R, size_t size) 
{ 
    if (R.size() == 3 || size == 0 ) { 
        return findMinCircleLittle(R.data(), size); 
    } 
    int index = rand() % size; 
    Point p = P[index]; 
    swap(P[index], P[size - 1]);  
    Circle c = recursiveAlgorithm(P, R, size - 1);  
    if (pointInCircle(p, c)) { 
        return c; 
    } 
    R.push_back(p); 
    return recursiveAlgorithm(P, R, size - 1); 
}

#endif /* MINCIRCLE_H_ */
