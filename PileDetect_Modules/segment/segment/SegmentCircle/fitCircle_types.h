//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: fitCircle_types.h
//
// MATLAB Coder version            : 3.4
// C/C++ source code generated on  : 12-Mar-2018 11:14:47
//
#ifndef FITCIRCLE_TYPES_H
#define FITCIRCLE_TYPES_H

// Include Files
#include "rtwtypes.h"

// Type Definitions
#ifndef struct_emxArray_boolean_T
#define struct_emxArray_boolean_T

struct emxArray_boolean_T
{
  boolean_T *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray_boolean_T

#ifndef struct_emxArray_int32_T
#define struct_emxArray_int32_T

struct emxArray_int32_T
{
  int *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray_int32_T

#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T

struct emxArray_real_T
{
  double *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray_real_T

#ifndef struct_emxArray_uint32_T
#define struct_emxArray_uint32_T

struct emxArray_uint32_T
{
  unsigned int *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray_uint32_T

#ifndef struct_emxArray_uint8_T
#define struct_emxArray_uint8_T

struct emxArray_uint8_T
{
  unsigned char *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray_uint8_T
#endif

//
// File trailer for fitCircle_types.h
//
// [EOF]
//
