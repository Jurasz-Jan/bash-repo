#include "SimpleAntenna.h"

SimpleAntenna::SimpleAntenna(int target_neighbours, double min_range, double max_range,
                             double modifier) {
  this->target_neighbours = target_neighbours;
  this->min_range = min_range;
  this->max_range = max_range;
  this->modifier = modifier;
}

double SimpleAntenna::range(double current_range, int neighbours) {
  if (neighbours > target_neighbours) {
    double new_range = current_range * modifier;
    if ( new_range < min_range )
       new_range = min_range;
    return new_range;
  }
  if (neighbours < target_neighbours) {
    double new_range = current_range / modifier;
    if ( new_range > max_range ) new_range = max_range;
    return new_range;
  }
  return current_range;
}

double SimpleAntenna::initial_range() {
  return max_range;
}

