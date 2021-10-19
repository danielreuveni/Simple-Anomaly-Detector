/*
Daniel Reuveni
 */

#include <math.h>
#include "anomaly_detection_util.h"



float avg(float* x, int size){
    float sum = 0;
    for(int i = 0; i < size; i++)
        sum += x[i];
	return sum / size;
}

    // returns the variance of X and Y
float var(float* x, int size){  
    float sum = 0, u = 0;
    for(int i = 0; i < size; i++) {
        sum += pow(x[i], 2);
    }
    u = avg(x, size);
    return sum / size - pow(u, 2);
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
    float avg_x = 0, avg_y, sum = 0;
    avg_x = avg(x, size);
    avg_y = avg(y, size);
    for(int i = 0; i < size; i++) {
        sum += (x[i] - avg_x)*(y[i] - avg_y);
    }
    return sum / size;
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
	return cov(x, y, size) / (sqrt(var(x, size)) * sqrt(var(y, size)));
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){
    float a = 0, b = 0;
    float x[size], y[size];
    for(int i = 0; i < size; i++) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    a = cov(x, y, size) / var(x, size);
    b = avg(y, size) - a * avg(x, size);

	return Line(a,b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
    Line l = linear_reg(points, size);
	return dev(p, l);
}

// returns the deviation between point p and the line
float dev(Point p,Line l){
    float x=p.y-l.f(p.x);
	if(x<0)
		x*=-1;
	return x;
}




