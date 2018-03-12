//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: fitCircle_emxAPI.h
//
// MATLAB Coder version            : 3.4
// C/C++ source code generated on  : 12-Mar-2018 11:14:47
//
#ifndef FITCIRCLE_EMXAPI_H
#define FITCIRCLE_EMXAPI_H

// Include Files
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "fitCircle_types.h"

// Function Declarations
extern emxArray_uint8_T *emxCreateND_uint8_T(int numDimensions, int *size);
extern emxArray_uint8_T *emxCreateWrapperND_uint8_T(unsigned char *data, int
  numDimensions, int *size);
extern emxArray_uint8_T *emxCreateWrapper_uint8_T(unsigned char *data, int rows,
  int cols);
extern emxArray_uint8_T *emxCreate_uint8_T(int rows, int cols);
extern void emxDestroyArray_uint8_T(emxArray_uint8_T *emxArray);
extern void emxInitArray_uint8_T(emxArray_uint8_T **pEmxArray, int numDimensions);

#endif

//
// File trailer for fitCircle_emxAPI.h
//
// [EOF]
//
