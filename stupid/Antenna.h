#ifndef ANTENNA_H_
#define ANTENNA_H_

class Antenna {
    public:
        virtual double range( double current_range, int neighbours ) = 0;
        virtual double initial_range() = 0;
};


#endif