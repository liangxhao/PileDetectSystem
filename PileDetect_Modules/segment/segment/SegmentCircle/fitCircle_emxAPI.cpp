//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: fitCircle_emxAPI.cpp
//
// MATLAB Coder version            : 3.4
// C/C++ source code generated on  : 12-Mar-2018 11:14:47
//

// Include Files
#include "fitCircle.h"
#include "fitCircle_emxAPI.h"
#include "fitCircle_emxutil.h"

// Function Definitions

//
// Arguments    : int numDimensions
//                int *size
// Return Type  : emxArray_uint8_T *
//
emxArray_uint8_T *emxCreateND_uint8_T(int numDimensions, int *size)
{
  emxArray_uint8_T *emx;
  int numEl;
  int i;
  emxInit_uint8_T1(&emx, numDimensions);
  numEl = 1;
  for (i = 0; i < numDimensions; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = (unsigned char *)calloc((unsigned int)numEl, sizeof(unsigned char));
  emx->numDimensions = numDimensions;
  emx->allocatedSize = numEl;
  return emx;
}

//
// Arguments    : unsigned char *data
//                int numDimensions
//                int *size
// Return Type  : emxArray_uint8_T *
//
emxArray_uint8_T *emxCreateWrapperND_uint8_T(unsigned char *data, int
  numDimensions, int *size)
{
  emxArray_uint8_T *emx;
  int numEl;
  int i;
  emxInit_uint8_T1(&emx, numDimensions);
  numEl = 1;
  for (i = 0; i < numDimensions; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = data;
  emx->numDimensions = numDimensions;
  emx->allocatedSize = numEl;
  emx->canFreeData = false;
  return emx;
}

//
// Arguments    : unsigned char *data
//                int rows
//                int cols
// Return Type  : emxArray_uint8_T *
//
emxArray_uint8_T *emxCreateWrapper_uint8_T(unsigned char *data, int rows, int
  cols)
{
  emxArray_uint8_T *emx;
  int size[2];
  int numEl;
  int i;
  size[0] = rows;
  size[1] = cols;
  emxInit_uint8_T1(&emx, 2);
  numEl = 1;
  for (i = 0; i < 2; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = data;
  emx->numDimensions = 2;
  emx->allocatedSize = numEl;
  emx->canFreeData = false;
  return emx;
}

//
// Arguments    : int rows
//                int cols
// Return Type  : emxArray_uint8_T *
//
emxArray_uint8_T *emxCreate_uint8_T(int rows, int cols)
{
  emxArray_uint8_T *emx;
  int size[2];
  int numEl;
  int i;
  size[0] = rows;
  size[1] = cols;
  emxInit_uint8_T1(&emx, 2);
  numEl = 1;
  for (i = 0; i < 2; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = (unsigned char *)calloc((unsigned int)numEl, sizeof(unsigned char));
  emx->numDimensions = 2;
  emx->allocatedSize = numEl;
  return emx;
}

//
// Arguments    : emxArray_uint8_T *emxArray
// Return Type  : void
//
void emxDestroyArray_uint8_T(emxArray_uint8_T *emxArray)
{
  emxFree_uint8_T(&emxArray);
}

//
// Arguments    : emxArray_uint8_T **pEmxArray
//                int numDimensions
// Return Type  : void
//
void emxInitArray_uint8_T(emxArray_uint8_T **pEmxArray, int numDimensions)
{
  emxInit_uint8_T1(pEmxArray, numDimensions);
}

//
// File trailer for fitCircle_emxAPI.cpp
//
// [EOF]
//
