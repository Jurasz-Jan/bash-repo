/*
Function.h
*/

#ifndef FUNCTION_H_
#define FUNCTION_H_

class Function {
protected:
  int dims;
  double *min_position;
  double *max_position;

public:
  Function( int dimensions );
  int dimensions();
  double *get_min_position();
  double *get_max_position();

  virtual double value(double *position) = 0;
};

#endif