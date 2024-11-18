/*
 * Main.cpp
 */

#include "Antenna.h"
#include "Function.h"
#include "MathHelper.h"
#include "SequentialSwarm.h"
#include "SimpleAntenna.h"
#include "SimpleExpFunction.h"
#include "Swarm.h"
#include "consts.h"

#include <iostream>
#include <stdlib.h>

#define __MPI_ON

#ifdef MPI_ON
#include <mpi.h>
#endif

using namespace std;

double rnd() { return random() % 1000000 / 1000000.0; }

void initialize_swarm(Swarm *swarm, Function *function, int robots) {
  int dims = function->dimensions();
  double *size = new double[dims];

  for (int d = 0; d < dims; d++)
    size[d] = function->get_max_position()[d] - function->get_min_position()[d];

  srandom(SEED);

  for (int robot = 0; robot < robots; robot++) {
    for (int d = 0; d < dims; d++)
      swarm->set_position(d, robot,
                          function->get_min_position()[d] + size[d] * rnd());
  }

  delete[] size;
}

Function *initialize_function() {
  int DIMS = 3;
  SimpleExpFunction *f = new SimpleExpFunction(DIMS, 4);

  f->set_exp_params(0, 2.0, 0.25);
  f->set_exp_params(1, 2.0, 0.3);
  f->set_exp_params(2, 2.0, 0.4);
  f->set_exp_params(3, 2.0, 0.3);

  f->set_exp_location(0, 0, 1);
  f->set_exp_location(0, 1, 1);
  f->set_exp_location(0, 2, 0.5);

  f->set_exp_location(1, 0, 4);
  f->set_exp_location(1, 1, 4);
  f->set_exp_location(1, 2, 0.5);

  f->set_exp_location(2, 0, 1);
  f->set_exp_location(2, 1, 4);
  f->set_exp_location(2, 2, 0.5);

  f->set_exp_location(3, 0, 4);
  f->set_exp_location(3, 1, 1);
  f->set_exp_location(3, 2, 0.5);

  return f;
}

void show_report(int step, Swarm *swarm, int robots, int dims) {
  for (int r = 0; r < robots; r++) {
    cout << step << " " << r << " ";
    for (int d = 0; d < dims; d++) {
      cout << swarm->get_position(r, d) << " ";
    }
    cout << endl;
  }
}

int main(int argc, char **argv) {
  double start;
  int procs, rank = 0;

#ifdef MPI_ON
  MPI_Init(&argc, &argv);
  cout << "MPI version " << MPI_VERSION << endl;
  MPI_Comm_size(MPI_COMM_WORLD, &procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

  Function *function = initialize_function();
  double max_distance = MathHelper::distance(function->get_max_position(),
                                             function->get_min_position(),
                                             function->dimensions());

  Antenna *antenna = new SimpleAntenna(
      TARGET_NEIGHBOURS, max_distance / ANTENNA_MIN_RANGE_DIV,
      max_distance / ANTENNA_MAX_RANGE_DIV, ANTENNA_RANGE_MODIFIER);
  Swarm *swarm = new SequentialSwarm(ROBOTS, antenna, function);

  if (!rank) {
    initialize_swarm(swarm, function, ROBOTS);
  }

  swarm->before_first_run();

  int step = 0;
  do {
    swarm->run(REPORT_PERIOD);
    swarm->before_get_position();
    step += REPORT_PERIOD;
    if (!rank) {
      show_report(step, swarm, ROBOTS, function->dimensions());
    }
  } while (step < STEPS);

#ifdef MPI_ON
  MPI_Finalize();
#endif

  return 0;
}
