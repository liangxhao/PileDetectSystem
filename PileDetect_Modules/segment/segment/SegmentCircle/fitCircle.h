//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: fitCircle.h
//
// MATLAB Coder version            : 3.4
// C/C++ source code generated on  : 12-Mar-2018 11:14:47
//
#ifndef FITCIRCLE_H
#define FITCIRCLE_H

// Include Files
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "fitCircle_types.h"

// Function Declarations
extern void fitCircle(const emxArray_uint8_T *img, double rx, double ry, double
                      radius, double ratio, double *objX, double *objY, double
                      *objRadius, double *flag);
extern void fitCircle_initialize();
extern void fitCircle_terminate();

#endif

//
// File trailer for fitCircle.h
//
// [EOF]
//
