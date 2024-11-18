/*
Swarm.h
*/

#ifndef SWARM_H_
#define SWARM_H_

#include"Antenna.h"
#include"Function.h"

class Swarm {
protected: 
    Function *function;
    Antenna *antenna;
    double **position;
    double **new_position;
    int dimensions;
    int robots;

public:
    Swarm( int robots, Antenna *antenna, Function *function );

    virtual void set_position( int dimension, int robot, double position ) = 0;
    virtual double get_position( int robot, int dimension ) = 0;
    virtual void allocate_memory() = 0;
    virtual void before_first_run() = 0;
    virtual void run( int steps ) = 0;
    virtual void before_get_position() = 0;
};


#endif