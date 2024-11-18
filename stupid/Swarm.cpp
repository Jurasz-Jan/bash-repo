#include"Swarm.h"

Swarm::Swarm( int robots, Antenna *antenna, Function *function ) {
    this->robots = robots;
    this->antenna = antenna;
    this->function = function;
    this->dimensions = function->dimensions();
}
