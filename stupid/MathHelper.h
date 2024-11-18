#include<math.h>

#ifndef MATH_HELPER_H_
#define MATH_HELPER_H_

class MathHelper {
    public:
    static double distanceSQ( double *p1, double *p2, int dimensions );
    static double distance( double *p1, double *p2, int dimensions );
    static void move( double *p1, double *p2, double *p3, int dimensions, double ratio );
};

#endif
