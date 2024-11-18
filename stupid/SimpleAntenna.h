#ifndef SIMPLE_ANTENNA_H_
#define SIMPLE_ANTENNA_H_

#include"Antenna.h"

class SimpleAntenna : public Antenna {
    protected:
        int target_neighbours;
        double min_range;
        double max_range;
        double modifier;
    public:
        SimpleAntenna( int target_neighbours, double min_range, double max_range, double modifier );
        virtual double range( double current_range, int neighbours );
        virtual double initial_range();
};


#endif