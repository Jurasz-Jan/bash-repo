/*
SimpleExpFunction.h
*/

#ifndef SIMPLE_EXP_FUNCTION_H_
#define SIMPLE_EXP_FUNCTION_H_

#include"Function.h"

class SimpleExpFunction : public Function {
private:
  int exponents;
  double *ampl;
  double *decay;
  double **exp_location;
  double singleExp( double *position, int idx );
public:
  SimpleExpFunction( int dims, int exponents );
  int dimensions();
  double *get_min_position();
  double *get_max_position();
  void set_exp_location( int exp, int dim, double value );
  void set_exp_params( int exp, double ampl, double decay );

  virtual double value(double *position);
};

#endif