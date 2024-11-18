#include "SequentialSwarm.h"
#include "MathHelper.h"
#include "Swarm.h"
#include "consts.h"
#include <iostream>

using namespace std;

SequentialSwarm::SequentialSwarm(int robots, Antenna *antenna,
                                 Function *function)
    : Swarm(robots, antenna, function) {
  step = 0;
  allocate_memory();
  initialize_antennas();
}

void SequentialSwarm::run(int steps) {
  for (int step = 0; step < steps; step++)
    single_step();
}

void SequentialSwarm::single_step() {
  step++;
  evaluate_function();
  find_neighbours_and_remember_best();
  move();
  fit_antenna_range();
}

void SequentialSwarm::evaluate_function() {
  for (int robot = 0; robot < robots; robot++) {
    function_value[robot] = function->value(position[robot]);
  }
}

void SequentialSwarm::find_neighbours_and_remember_best() {
  for (int robot = 0; robot < robots; robot++) {
    best_id = robot;
    nearest_neighbours[robot] = 0;

    best_function_value = function_value[robot];
    my_antenna_range_sq = antenna_range_sq[robot];
    my_position = position[robot];

    for (int other_robot = 0; other_robot < robot; other_robot++) {
      compare_with_other_robot(robot, other_robot);
    }
    for (int other_robot = robot + 1; other_robot < robots; other_robot++) {
      compare_with_other_robot(robot, other_robot);
    }

    neighbour_id[robot] = best_id;
  }
}

void SequentialSwarm::compare_with_other_robot(int robot, int other_robot) {
  if (MathHelper::distanceSQ(my_position, position[other_robot], dimensions) <
      my_antenna_range_sq) {
    // inny robot jest w zasięgu anteny
    nearest_neighbours[robot]++;

    if (best_function_value < function_value[other_robot]) {
      // w dodatku ma lepszą wartość funkcji
      best_function_value = function_value[other_robot];
      best_id = other_robot;
    }
  }
}

void show(double *pos, double *pos2, double *pos3, int size) {
  for (int i = 0; i < size; i++) {
    cout << "r1 " << pos[i] << " vs r2 " << pos2[i] << " vs r3 " << pos3[i]
         << endl;
  }
}

void SequentialSwarm::move() {
  for (int robot = 0; robot < robots; robot++) {
    MathHelper::move(position[robot], position[neighbour_id[robot]],
                     new_position[robot], dimensions, STEP_SIZE / sqrt(step));
  }
  for (int robot = 0; robot < robots; robot++)
    for (int d = 0; d < dimensions; d++)
      position[robot][d] = new_position[robot][d];
}

void SequentialSwarm::allocate_memory() {
  position = new double *[robots];
  new_position = new double *[robots];
  for (int i = 0; i < robots; i++) {
    position[i] = new double[dimensions];
    new_position[i] = new double[dimensions];
  }

  neighbour_id = new int[robots];
  nearest_neighbours = new int[robots];
  function_value = new double[robots];
  antenna_range_sq = new double[robots];
}

void SequentialSwarm::fit_antenna_range() {
  double range;
  for (int robot = 0; robot < robots; robot++) {
    range = antenna->range(sqrt(antenna_range_sq[robot]),
                           nearest_neighbours[robot]);
    antenna_range_sq[robot] = range * range;
  }
}

void SequentialSwarm::initialize_antennas() {
  double vSQ = antenna->initial_range();
  vSQ *= vSQ;
  for (int r = 0; r < robots; r++) {
    antenna_range_sq[r] = vSQ;
  }
}

void SequentialSwarm::set_position(int dimension, int robot, double position) {
  this->position[robot][dimension] = position;
}

double SequentialSwarm::get_position(int robot, int dimension) {
  return position[robot][dimension];
}
