//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: fitCircle.cpp
//
// MATLAB Coder version            : 3.4
// C/C++ source code generated on  : 12-Mar-2018 11:14:47
//

// Include Files
#include "fitCircle.h"
#include "fitCircle_emxutil.h"
#include "libmwbwpackctbb.h"
#include "libmwbwunpackctbb.h"
#include "libmwmorphop_packed.h"
#include "libmwippreconstruct.h"
#include "libmwimfilter.h"
#include "libmwippfilter.h"
#include "libmwgrayto8.h"
#include "libmwgetnumcores.h"
#include "libmwtbbhist.h"

// Variable Definitions
static const boolean_T bv0[9] = { true, false, false, false, true, false, false,
  false, true };

static const boolean_T bv1[9] = { false, false, true, false, true, false, true,
  false, false };

// Function Declarations
static void ExtractObject(const emxArray_boolean_T *bw, double rx, double ry,
  double radius, double ratio, emxArray_real_T *xCoords, emxArray_real_T
  *yCoords);
static void PointsOnCircle(double rx, double ry, double radius, emxArray_real_T *
  xCircle, emxArray_real_T *yCircle);
static void b_power(const emxArray_real_T *a, emxArray_real_T *y);
static void bwareaopen(const emxArray_boolean_T *varargin_1, double varargin_2,
  emxArray_boolean_T *BW2);
static void bwlabel(const emxArray_boolean_T *varargin_1, emxArray_real_T *L,
                    double *numComponents);
static void bwpack(const emxArray_boolean_T *varargin_1, emxArray_uint32_T *BWP);
static void bwunpack(const emxArray_uint32_T *varargin_1, double varargin_2,
                     emxArray_boolean_T *BW);
static double combineVectorElements(const emxArray_real_T *x);
static int div_s32_floor(int numerator, int denominator);
static void gradf(const emxArray_real_T *f, emxArray_real_T *y);
static void imGradBw(const emxArray_uint8_T *img, double radius,
                     emxArray_boolean_T *bw);
static void imbinarize(const emxArray_real_T *I, double varargin_1,
  emxArray_boolean_T *BW);
static void imdilate(const emxArray_boolean_T *A, emxArray_boolean_T *B);
static void imerode(const emxArray_boolean_T *A, emxArray_boolean_T *B);
static void imfill(emxArray_boolean_T *varargin_1);
static void imfilter(emxArray_real_T *varargin_1);
static void nullAssignment(emxArray_real_T *x, const emxArray_boolean_T *idx);
static double otsuGray(const emxArray_real_T *I);
static void padarray(const emxArray_uint8_T *varargin_1, emxArray_uint8_T *b);
static void power(const emxArray_real_T *a, emxArray_real_T *y);
static double rt_roundd(double u);
static double sum(const emxArray_boolean_T *x);

// Function Definitions

//
// Arguments    : const emxArray_boolean_T *bw
//                double rx
//                double ry
//                double radius
//                double ratio
//                emxArray_real_T *xCoords
//                emxArray_real_T *yCoords
// Return Type  : void
//
static void ExtractObject(const emxArray_boolean_T *bw, double rx, double ry,
  double radius, double ratio, emxArray_real_T *xCoords, emxArray_real_T
  *yCoords)
{
  emxArray_int32_T *ii;
  emxArray_int32_T *jj;
  emxArray_boolean_T *v;
  int nx;
  int idx;
  int b_jj;
  int b_ii;
  boolean_T exitg1;
  boolean_T guard1 = false;
  emxArray_real_T *b_xCoords;
  emxArray_real_T *r1;
  emxArray_real_T *r2;
  double a;
  emxInit_int32_T(&ii, 1);
  emxInit_int32_T(&jj, 1);
  emxInit_boolean_T1(&v, 1);
  nx = bw->size[0] * bw->size[1];
  idx = 0;
  b_jj = ii->size[0];
  ii->size[0] = nx;
  emxEnsureCapacity_int32_T(ii, b_jj);
  b_jj = jj->size[0];
  jj->size[0] = nx;
  emxEnsureCapacity_int32_T(jj, b_jj);
  if (nx == 0) {
    b_jj = ii->size[0];
    ii->size[0] = 0;
    emxEnsureCapacity_int32_T(ii, b_jj);
    b_jj = jj->size[0];
    jj->size[0] = 0;
    emxEnsureCapacity_int32_T(jj, b_jj);
  } else {
    b_ii = 1;
    b_jj = 1;
    exitg1 = false;
    while ((!exitg1) && (b_jj <= bw->size[1])) {
      guard1 = false;
      if (bw->data[(b_ii + bw->size[0] * (b_jj - 1)) - 1]) {
        idx++;
        ii->data[idx - 1] = b_ii;
        jj->data[idx - 1] = b_jj;
        if (idx >= nx) {
          exitg1 = true;
        } else {
          guard1 = true;
        }
      } else {
        guard1 = true;
      }

      if (guard1) {
        b_ii++;
        if (b_ii > bw->size[0]) {
          b_ii = 1;
          b_jj++;
        }
      }
    }

    if (nx == 1) {
      if (idx == 0) {
        b_jj = ii->size[0];
        ii->size[0] = 0;
        emxEnsureCapacity_int32_T(ii, b_jj);
        b_jj = jj->size[0];
        jj->size[0] = 0;
        emxEnsureCapacity_int32_T(jj, b_jj);
      }
    } else {
      b_jj = ii->size[0];
      if (1 > idx) {
        ii->size[0] = 0;
      } else {
        ii->size[0] = idx;
      }

      emxEnsureCapacity_int32_T(ii, b_jj);
      b_jj = jj->size[0];
      if (1 > idx) {
        jj->size[0] = 0;
      } else {
        jj->size[0] = idx;
      }

      emxEnsureCapacity_int32_T(jj, b_jj);
    }
  }

  b_jj = yCoords->size[0];
  yCoords->size[0] = ii->size[0];
  emxEnsureCapacity_real_T1(yCoords, b_jj);
  b_ii = ii->size[0];
  for (b_jj = 0; b_jj < b_ii; b_jj++) {
    yCoords->data[b_jj] = ii->data[b_jj];
  }

  emxFree_int32_T(&ii);
  b_jj = xCoords->size[0];
  xCoords->size[0] = jj->size[0];
  emxEnsureCapacity_real_T1(xCoords, b_jj);
  b_ii = jj->size[0];
  for (b_jj = 0; b_jj < b_ii; b_jj++) {
    xCoords->data[b_jj] = jj->data[b_jj];
  }

  emxFree_int32_T(&jj);
  emxInit_real_T(&b_xCoords, 1);
  b_jj = b_xCoords->size[0];
  b_xCoords->size[0] = xCoords->size[0];
  emxEnsureCapacity_real_T1(b_xCoords, b_jj);
  b_ii = xCoords->size[0];
  for (b_jj = 0; b_jj < b_ii; b_jj++) {
    b_xCoords->data[b_jj] = xCoords->data[b_jj] - rx;
  }

  emxInit_real_T(&r1, 1);
  b_power(b_xCoords, r1);
  b_jj = b_xCoords->size[0];
  b_xCoords->size[0] = yCoords->size[0];
  emxEnsureCapacity_real_T1(b_xCoords, b_jj);
  b_ii = yCoords->size[0];
  for (b_jj = 0; b_jj < b_ii; b_jj++) {
    b_xCoords->data[b_jj] = yCoords->data[b_jj] - ry;
  }

  emxInit_real_T(&r2, 1);
  b_power(b_xCoords, r2);
  a = ratio * radius;
  a *= a;
  b_jj = v->size[0];
  v->size[0] = r1->size[0];
  emxEnsureCapacity_boolean_T1(v, b_jj);
  b_ii = r1->size[0];
  emxFree_real_T(&b_xCoords);
  for (b_jj = 0; b_jj < b_ii; b_jj++) {
    v->data[b_jj] = ((r1->data[b_jj] + r2->data[b_jj]) - a < 0.0);
  }

  emxFree_real_T(&r2);
  emxFree_real_T(&r1);
  b_ii = v->size[0] - 1;
  idx = 0;
  for (b_jj = 0; b_jj <= b_ii; b_jj++) {
    if (v->data[b_jj]) {
      idx++;
    }
  }

  nx = 0;
  for (b_jj = 0; b_jj <= b_ii; b_jj++) {
    if (v->data[b_jj]) {
      xCoords->data[nx] = xCoords->data[b_jj];
      nx++;
    }
  }

  b_jj = xCoords->size[0];
  xCoords->size[0] = idx;
  emxEnsureCapacity_real_T1(xCoords, b_jj);
  b_ii = v->size[0] - 1;
  idx = 0;
  for (b_jj = 0; b_jj <= b_ii; b_jj++) {
    if (v->data[b_jj]) {
      idx++;
    }
  }

  nx = 0;
  for (b_jj = 0; b_jj <= b_ii; b_jj++) {
    if (v->data[b_jj]) {
      yCoords->data[nx] = yCoords->data[b_jj];
      nx++;
    }
  }

  emxFree_boolean_T(&v);
  b_jj = yCoords->size[0];
  yCoords->size[0] = idx;
  emxEnsureCapacity_real_T1(yCoords, b_jj);
}

//
// Arguments    : double rx
//                double ry
//                double radius
//                emxArray_real_T *xCircle
//                emxArray_real_T *yCircle
// Return Type  : void
//
static void PointsOnCircle(double rx, double ry, double radius, emxArray_real_T *
  xCircle, emxArray_real_T *yCircle)
{
  int i14;
  int nx;
  emxArray_real_T *xList;
  double ndbl;
  double apnd;
  double absa;
  emxArray_real_T *yList;
  int k;
  int nm1d2;
  int i15;
  int i16;
  emxArray_int32_T *r3;
  emxArray_int32_T *r4;
  emxArray_real_T *b_yCircle;
  i14 = xCircle->size[0];
  xCircle->size[0] = (int)(4.0 * radius);
  emxEnsureCapacity_real_T1(xCircle, i14);
  nx = (int)(4.0 * radius);
  for (i14 = 0; i14 < nx; i14++) {
    xCircle->data[i14] = 0.0;
  }

  i14 = yCircle->size[0];
  yCircle->size[0] = (int)(4.0 * radius);
  emxEnsureCapacity_real_T1(yCircle, i14);
  nx = (int)(4.0 * radius);
  for (i14 = 0; i14 < nx; i14++) {
    yCircle->data[i14] = 0.0;
  }

  // 先求1/4圆
  // 左侧起始点
  emxInit_real_T1(&xList, 2);
  if (-1.0 < -radius) {
    i14 = xList->size[0] * xList->size[1];
    xList->size[0] = 1;
    xList->size[1] = 0;
    emxEnsureCapacity_real_T(xList, i14);
  } else if (floor(-radius) == -radius) {
    i14 = xList->size[0] * xList->size[1];
    xList->size[0] = 1;
    xList->size[1] = (int)floor(-1.0 - (-radius)) + 1;
    emxEnsureCapacity_real_T(xList, i14);
    nx = (int)floor(-1.0 - (-radius));
    for (i14 = 0; i14 <= nx; i14++) {
      xList->data[xList->size[0] * i14] = -radius + (double)i14;
    }
  } else {
    ndbl = floor((-1.0 - (-radius)) + 0.5);
    apnd = -radius + ndbl;
    absa = fabs(-radius);
    if (!(absa > 1.0)) {
      absa = 1.0;
    }

    if (fabs(apnd - -1.0) < 4.4408920985006262E-16 * absa) {
      ndbl++;
      apnd = -1.0;
    } else if (apnd - -1.0 > 0.0) {
      apnd = -radius + (ndbl - 1.0);
    } else {
      ndbl++;
    }

    if (ndbl >= 0.0) {
      nx = (int)ndbl;
    } else {
      nx = 0;
    }

    i14 = xList->size[0] * xList->size[1];
    xList->size[0] = 1;
    xList->size[1] = nx;
    emxEnsureCapacity_real_T(xList, i14);
    if (nx > 0) {
      xList->data[0] = -radius;
      if (nx > 1) {
        xList->data[nx - 1] = apnd;
        nm1d2 = (nx - 1) / 2;
        for (k = 1; k < nm1d2; k++) {
          xList->data[k] = -radius + (double)k;
          xList->data[(nx - k) - 1] = apnd - (double)k;
        }

        if (nm1d2 << 1 == nx - 1) {
          xList->data[nm1d2] = (-radius + apnd) / 2.0;
        } else {
          xList->data[nm1d2] = -radius + (double)nm1d2;
          xList->data[nm1d2 + 1] = apnd - (double)nm1d2;
        }
      }
    }
  }

  emxInit_real_T1(&yList, 2);
  ndbl = radius * radius;
  nx = xList->size[1];
  i14 = yList->size[0] * yList->size[1];
  yList->size[0] = 1;
  yList->size[1] = xList->size[1];
  emxEnsureCapacity_real_T(yList, i14);
  for (k = 0; k + 1 <= nx; k++) {
    yList->data[k] = xList->data[k] * xList->data[k];
  }

  i14 = yList->size[0] * yList->size[1];
  yList->size[0] = 1;
  emxEnsureCapacity_real_T(yList, i14);
  nx = yList->size[0];
  nm1d2 = yList->size[1];
  nx *= nm1d2;
  for (i14 = 0; i14 < nx; i14++) {
    yList->data[i14] = ndbl - yList->data[i14];
  }

  nx = yList->size[1];
  for (k = 0; k + 1 <= nx; k++) {
    yList->data[k] = sqrt(yList->data[k]);
  }

  nx = yList->size[1];
  for (k = 0; k + 1 <= nx; k++) {
    yList->data[k] = rt_roundd(yList->data[k]);
  }

  i14 = yList->size[0] * yList->size[1];
  yList->size[0] = 1;
  emxEnsureCapacity_real_T(yList, i14);
  nx = yList->size[0];
  nm1d2 = yList->size[1];
  nx *= nm1d2;
  for (i14 = 0; i14 < nx; i14++) {
    yList->data[i14] = -yList->data[i14];
  }

  nx = xList->size[1];
  for (i14 = 0; i14 < nx; i14++) {
    xCircle->data[i14] = xList->data[xList->size[0] * i14];
  }

  nx = yList->size[1];
  for (i14 = 0; i14 < nx; i14++) {
    yCircle->data[i14] = yList->data[yList->size[0] * i14];
  }

  xCircle->data[(int)(radius + 1.0) - 1] = 0.0;
  yCircle->data[(int)(radius + 1.0) - 1] = -radius;

  //  xList=-List;
  if (1 > xList->size[1]) {
    i14 = 1;
    nm1d2 = 1;
    k = 0;
  } else {
    i14 = xList->size[1];
    nm1d2 = -1;
    k = 1;
  }

  if (radius + 2.0 > 2.0 * radius + 1.0) {
    i15 = 0;
  } else {
    i15 = (int)(radius + 2.0) - 1;
  }

  nx = div_s32_floor(k - i14, nm1d2);
  for (k = 0; k <= nx; k++) {
    xCircle->data[i15 + k] = -xList->data[(i14 + nm1d2 * k) - 1];
  }

  if (1 > yList->size[1]) {
    i14 = 1;
    nm1d2 = 1;
    k = 0;
  } else {
    i14 = yList->size[1];
    nm1d2 = -1;
    k = 1;
  }

  if (radius + 2.0 > 2.0 * radius + 1.0) {
    i15 = 0;
  } else {
    i15 = (int)(radius + 2.0) - 1;
  }

  nx = div_s32_floor(k - i14, nm1d2);
  for (k = 0; k <= nx; k++) {
    yCircle->data[i15 + k] = yList->data[(i14 + nm1d2 * k) - 1];
  }

  emxFree_real_T(&yList);
  ndbl = 2.0 * radius;
  if (2.0 > ndbl) {
    i14 = 1;
    nm1d2 = 1;
    k = 0;
  } else {
    i14 = (int)ndbl;
    nm1d2 = -1;
    k = 2;
  }

  ndbl = 2.0 * radius + 2.0;
  if (ndbl > xCircle->size[0]) {
    i15 = 0;
  } else {
    i15 = (int)ndbl - 1;
  }

  i16 = xList->size[0] * xList->size[1];
  xList->size[0] = 1;
  xList->size[1] = div_s32_floor(k - i14, nm1d2) + 1;
  emxEnsureCapacity_real_T(xList, i16);
  nx = div_s32_floor(k - i14, nm1d2);
  for (k = 0; k <= nx; k++) {
    xList->data[xList->size[0] * k] = xCircle->data[(i14 + nm1d2 * k) - 1];
  }

  nx = xList->size[1];
  for (i14 = 0; i14 < nx; i14++) {
    xCircle->data[i15 + i14] = xList->data[xList->size[0] * i14];
  }

  emxFree_real_T(&xList);
  ndbl = 2.0 * radius;
  if (2.0 > ndbl) {
    i14 = 1;
    nm1d2 = 1;
    k = 0;
  } else {
    i14 = (int)ndbl;
    nm1d2 = -1;
    k = 2;
  }

  ndbl = 2.0 * radius + 2.0;
  if (ndbl > yCircle->size[0]) {
    i15 = 0;
    i16 = 0;
  } else {
    i15 = (int)ndbl - 1;
    i16 = yCircle->size[0];
  }

  emxInit_int32_T1(&r3, 2);
  nx = r3->size[0] * r3->size[1];
  r3->size[0] = 1;
  r3->size[1] = i16 - i15;
  emxEnsureCapacity_int32_T1(r3, nx);
  nx = i16 - i15;
  for (i16 = 0; i16 < nx; i16++) {
    r3->data[r3->size[0] * i16] = i15 + i16;
  }

  emxInit_int32_T(&r4, 1);
  i15 = r4->size[0];
  r4->size[0] = div_s32_floor(k - i14, nm1d2) + 1;
  emxEnsureCapacity_int32_T(r4, i15);
  nx = div_s32_floor(k - i14, nm1d2);
  for (k = 0; k <= nx; k++) {
    r4->data[k] = i14 + nm1d2 * k;
  }

  emxInit_real_T(&b_yCircle, 1);
  i14 = b_yCircle->size[0];
  b_yCircle->size[0] = r3->size[0] * r3->size[1];
  emxEnsureCapacity_real_T1(b_yCircle, i14);
  nx = r3->size[0] * r3->size[1];
  for (i14 = 0; i14 < nx; i14++) {
    b_yCircle->data[i14] = -yCircle->data[r4->data[i14] - 1];
  }

  emxFree_int32_T(&r4);
  nx = b_yCircle->size[0];
  for (i14 = 0; i14 < nx; i14++) {
    yCircle->data[r3->data[i14]] = b_yCircle->data[i14];
  }

  emxFree_real_T(&b_yCircle);
  emxFree_int32_T(&r3);
  i14 = xCircle->size[0];
  emxEnsureCapacity_real_T1(xCircle, i14);
  nx = xCircle->size[0];
  for (i14 = 0; i14 < nx; i14++) {
    xCircle->data[i14] += rx;
  }

  i14 = yCircle->size[0];
  emxEnsureCapacity_real_T1(yCircle, i14);
  nx = yCircle->size[0];
  for (i14 = 0; i14 < nx; i14++) {
    yCircle->data[i14] += ry;
  }
}

//
// Arguments    : const emxArray_real_T *a
//                emxArray_real_T *y
// Return Type  : void
//
static void b_power(const emxArray_real_T *a, emxArray_real_T *y)
{
  unsigned int a_idx_0;
  unsigned int b_a_idx_0;
  int k;
  a_idx_0 = (unsigned int)a->size[0];
  b_a_idx_0 = (unsigned int)a->size[0];
  k = y->size[0];
  y->size[0] = (int)b_a_idx_0;
  emxEnsureCapacity_real_T1(y, k);
  for (k = 0; k + 1 <= (int)a_idx_0; k++) {
    y->data[k] = a->data[k] * a->data[k];
  }
}

//
// Arguments    : const emxArray_boolean_T *varargin_1
//                double varargin_2
//                emxArray_boolean_T *BW2
// Return Type  : void
//
static void bwareaopen(const emxArray_boolean_T *varargin_1, double varargin_2,
  emxArray_boolean_T *BW2)
{
  emxArray_int32_T *regionLengths;
  emxArray_real_T *L;
  double numComponents;
  int numPixels;
  int j;
  int numForegroundPixels;
  emxArray_int32_T *beginIdx;
  int i;
  emxArray_int32_T *pixelIdxList;
  unsigned int uv1[2];
  emxInit_int32_T(&regionLengths, 1);
  emxInit_real_T1(&L, 2);
  bwlabel(varargin_1, L, &numComponents);
  numPixels = L->size[0] * L->size[1];
  j = regionLengths->size[0];
  regionLengths->size[0] = (int)numComponents;
  emxEnsureCapacity_int32_T(regionLengths, j);
  numForegroundPixels = (int)numComponents;
  for (j = 0; j < numForegroundPixels; j++) {
    regionLengths->data[j] = 0;
  }

  for (numForegroundPixels = 0; numForegroundPixels + 1 <= numPixels;
       numForegroundPixels++) {
    if ((int)L->data[numForegroundPixels] > 0) {
      regionLengths->data[(int)L->data[numForegroundPixels] - 1]++;
    }
  }

  emxInit_int32_T(&beginIdx, 1);
  numPixels = L->size[0] * L->size[1];
  numForegroundPixels = 0;
  j = beginIdx->size[0];
  beginIdx->size[0] = regionLengths->size[0];
  emxEnsureCapacity_int32_T(beginIdx, j);
  for (i = 0; i < regionLengths->size[0]; i++) {
    beginIdx->data[i] = numForegroundPixels;
    numForegroundPixels += regionLengths->data[i];
  }

  emxInit_int32_T(&pixelIdxList, 1);
  j = pixelIdxList->size[0];
  pixelIdxList->size[0] = numForegroundPixels;
  emxEnsureCapacity_int32_T(pixelIdxList, j);
  for (numForegroundPixels = 0; numForegroundPixels + 1 <= numPixels;
       numForegroundPixels++) {
    if ((int)L->data[numForegroundPixels] > 0) {
      beginIdx->data[(int)L->data[numForegroundPixels] - 1]++;
      pixelIdxList->data[beginIdx->data[(int)L->data[numForegroundPixels] - 1] -
        1] = numForegroundPixels + 1;
    }
  }

  emxFree_int32_T(&beginIdx);
  emxFree_real_T(&L);
  for (j = 0; j < 2; j++) {
    uv1[j] = (unsigned int)varargin_1->size[j];
  }

  j = BW2->size[0] * BW2->size[1];
  BW2->size[0] = (int)uv1[0];
  BW2->size[1] = (int)uv1[1];
  emxEnsureCapacity_boolean_T(BW2, j);
  numForegroundPixels = (int)uv1[0] * (int)uv1[1];
  for (j = 0; j < numForegroundPixels; j++) {
    BW2->data[j] = false;
  }

  numForegroundPixels = -1;
  for (i = 0; i < (int)numComponents; i++) {
    if (regionLengths->data[i] >= varargin_2) {
      for (j = 1; j <= regionLengths->data[i]; j++) {
        BW2->data[pixelIdxList->data[numForegroundPixels + j] - 1] = true;
      }
    }

    numForegroundPixels += regionLengths->data[i];
  }

  emxFree_int32_T(&pixelIdxList);
  emxFree_int32_T(&regionLengths);
}

//
// Arguments    : const emxArray_boolean_T *varargin_1
//                emxArray_real_T *L
//                double *numComponents
// Return Type  : void
//
static void bwlabel(const emxArray_boolean_T *varargin_1, emxArray_real_T *L,
                    double *numComponents)
{
  int numRuns;
  int lastRunOnPreviousColumn;
  int firstRunOnThisColumn;
  emxArray_int32_T *startRow;
  emxArray_int32_T *endRow;
  emxArray_int32_T *startCol;
  double offset;
  emxArray_int32_T *labelForEachRun;
  int nextLabel;
  int k;
  double b_k;
  int runCounter;
  unsigned int unnamed_idx_0;
  unsigned int unnamed_idx_1;
  int firstRunOnPreviousColumn;
  emxArray_int32_T *labelsRenumbered;
  int p;
  int root_k;
  int root_p;
  numRuns = 0;
  lastRunOnPreviousColumn = varargin_1->size[0];
  if ((varargin_1->size[0] != 0) && (varargin_1->size[1] != 0)) {
    for (firstRunOnThisColumn = 0; firstRunOnThisColumn < varargin_1->size[1];
         firstRunOnThisColumn++) {
      offset = ((1.0 + (double)firstRunOnThisColumn) - 1.0) * (double)
        lastRunOnPreviousColumn;
      if (varargin_1->data[(int)(1.0 + offset) - 1]) {
        numRuns++;
      }

      nextLabel = (int)(((double)lastRunOnPreviousColumn + offset) + (1.0 - (2.0
        + offset)));
      for (k = 0; k < nextLabel; k++) {
        b_k = (2.0 + offset) + (double)k;
        if (varargin_1->data[(int)b_k - 1] && (!varargin_1->data[(int)b_k - 2]))
        {
          numRuns++;
        }
      }
    }
  }

  emxInit_int32_T(&startRow, 1);
  emxInit_int32_T(&endRow, 1);
  emxInit_int32_T(&startCol, 1);
  emxInit_int32_T(&labelForEachRun, 1);
  if (numRuns == 0) {
    nextLabel = startRow->size[0];
    startRow->size[0] = 0;
    emxEnsureCapacity_int32_T(startRow, nextLabel);
    nextLabel = endRow->size[0];
    endRow->size[0] = 0;
    emxEnsureCapacity_int32_T(endRow, nextLabel);
    nextLabel = startCol->size[0];
    startCol->size[0] = 0;
    emxEnsureCapacity_int32_T(startCol, nextLabel);
    nextLabel = labelForEachRun->size[0];
    labelForEachRun->size[0] = 0;
    emxEnsureCapacity_int32_T(labelForEachRun, nextLabel);
    unnamed_idx_0 = (unsigned int)varargin_1->size[0];
    unnamed_idx_1 = (unsigned int)varargin_1->size[1];
    nextLabel = L->size[0] * L->size[1];
    L->size[0] = (int)unnamed_idx_0;
    L->size[1] = (int)unnamed_idx_1;
    emxEnsureCapacity_real_T(L, nextLabel);
    runCounter = (int)unnamed_idx_0 * (int)unnamed_idx_1;
    for (nextLabel = 0; nextLabel < runCounter; nextLabel++) {
      L->data[nextLabel] = 0.0;
    }

    offset = 0.0;
  } else {
    nextLabel = startRow->size[0];
    startRow->size[0] = numRuns;
    emxEnsureCapacity_int32_T(startRow, nextLabel);
    nextLabel = endRow->size[0];
    endRow->size[0] = numRuns;
    emxEnsureCapacity_int32_T(endRow, nextLabel);
    nextLabel = startCol->size[0];
    startCol->size[0] = numRuns;
    emxEnsureCapacity_int32_T(startCol, nextLabel);
    lastRunOnPreviousColumn = varargin_1->size[0];
    runCounter = 0;
    for (firstRunOnThisColumn = 0; firstRunOnThisColumn < varargin_1->size[1];
         firstRunOnThisColumn++) {
      nextLabel = (int)(((1.0 + (double)firstRunOnThisColumn) - 1.0) * (double)
                        lastRunOnPreviousColumn) - 1;
      firstRunOnPreviousColumn = 1;
      while (firstRunOnPreviousColumn <= lastRunOnPreviousColumn) {
        while ((firstRunOnPreviousColumn <= lastRunOnPreviousColumn) &&
               (!varargin_1->data[firstRunOnPreviousColumn + nextLabel])) {
          firstRunOnPreviousColumn++;
        }

        if ((firstRunOnPreviousColumn <= lastRunOnPreviousColumn) &&
            varargin_1->data[firstRunOnPreviousColumn + nextLabel]) {
          startCol->data[runCounter] = firstRunOnThisColumn + 1;
          startRow->data[runCounter] = firstRunOnPreviousColumn;
          while ((firstRunOnPreviousColumn <= lastRunOnPreviousColumn) &&
                 varargin_1->data[firstRunOnPreviousColumn + nextLabel]) {
            firstRunOnPreviousColumn++;
          }

          endRow->data[runCounter] = firstRunOnPreviousColumn - 1;
          runCounter++;
        }
      }
    }

    nextLabel = labelForEachRun->size[0];
    labelForEachRun->size[0] = numRuns;
    emxEnsureCapacity_int32_T(labelForEachRun, nextLabel);
    for (nextLabel = 0; nextLabel < numRuns; nextLabel++) {
      labelForEachRun->data[nextLabel] = 0;
    }

    k = 0;
    runCounter = 1;
    nextLabel = 1;
    firstRunOnPreviousColumn = -1;
    lastRunOnPreviousColumn = -1;
    firstRunOnThisColumn = 0;
    while (k + 1 <= numRuns) {
      if (startCol->data[k] == runCounter + 1) {
        firstRunOnPreviousColumn = firstRunOnThisColumn + 1;
        firstRunOnThisColumn = k;
        lastRunOnPreviousColumn = k;
        runCounter = startCol->data[k];
      } else {
        if (startCol->data[k] > runCounter + 1) {
          firstRunOnPreviousColumn = -1;
          lastRunOnPreviousColumn = -1;
          firstRunOnThisColumn = k;
          runCounter = startCol->data[k];
        }
      }

      if (firstRunOnPreviousColumn >= 0) {
        for (p = firstRunOnPreviousColumn - 1; p + 1 <= lastRunOnPreviousColumn;
             p++) {
          if ((endRow->data[k] >= startRow->data[p] - 1) && (startRow->data[k] <=
               endRow->data[p] + 1)) {
            if (labelForEachRun->data[k] == 0) {
              labelForEachRun->data[k] = labelForEachRun->data[p];
              nextLabel++;
            } else {
              if (labelForEachRun->data[k] != labelForEachRun->data[p]) {
                for (root_k = k; root_k + 1 != labelForEachRun->data[root_k];
                     root_k = labelForEachRun->data[root_k] - 1) {
                  labelForEachRun->data[root_k] = labelForEachRun->
                    data[labelForEachRun->data[root_k] - 1];
                }

                for (root_p = p; root_p + 1 != labelForEachRun->data[root_p];
                     root_p = labelForEachRun->data[root_p] - 1) {
                  labelForEachRun->data[root_p] = labelForEachRun->
                    data[labelForEachRun->data[root_p] - 1];
                }

                if (root_k + 1 != root_p + 1) {
                  if (root_p + 1 < root_k + 1) {
                    labelForEachRun->data[root_k] = root_p + 1;
                    labelForEachRun->data[k] = root_p + 1;
                  } else {
                    labelForEachRun->data[root_p] = root_k + 1;
                    labelForEachRun->data[p] = root_k + 1;
                  }
                }
              }
            }
          }
        }
      }

      if (labelForEachRun->data[k] == 0) {
        labelForEachRun->data[k] = nextLabel;
        nextLabel++;
      }

      k++;
    }

    emxInit_int32_T(&labelsRenumbered, 1);
    nextLabel = labelsRenumbered->size[0];
    labelsRenumbered->size[0] = labelForEachRun->size[0];
    emxEnsureCapacity_int32_T(labelsRenumbered, nextLabel);
    offset = 0.0;
    unnamed_idx_0 = (unsigned int)varargin_1->size[0];
    unnamed_idx_1 = (unsigned int)varargin_1->size[1];
    nextLabel = L->size[0] * L->size[1];
    L->size[0] = (int)unnamed_idx_0;
    L->size[1] = (int)unnamed_idx_1;
    emxEnsureCapacity_real_T(L, nextLabel);
    runCounter = (int)unnamed_idx_0 * (int)unnamed_idx_1;
    for (nextLabel = 0; nextLabel < runCounter; nextLabel++) {
      L->data[nextLabel] = 0.0;
    }

    for (k = 0; k + 1 <= numRuns; k++) {
      if (labelForEachRun->data[k] == k + 1) {
        offset++;
        labelsRenumbered->data[k] = (int)offset;
      }

      labelsRenumbered->data[k] = labelsRenumbered->data[labelForEachRun->data[k]
        - 1];
      runCounter = (startCol->data[k] - 1) * varargin_1->size[0];
      for (nextLabel = startRow->data[k]; nextLabel <= endRow->data[k];
           nextLabel++) {
        L->data[(nextLabel + runCounter) - 1] = labelsRenumbered->data[k];
      }
    }

    emxFree_int32_T(&labelsRenumbered);
  }

  emxFree_int32_T(&labelForEachRun);
  emxFree_int32_T(&startCol);
  emxFree_int32_T(&endRow);
  emxFree_int32_T(&startRow);
  *numComponents = offset;
}

//
// Arguments    : const emxArray_boolean_T *varargin_1
//                emxArray_uint32_T *BWP
// Return Type  : void
//
static void bwpack(const emxArray_boolean_T *varargin_1, emxArray_uint32_T *BWP)
{
  int x;
  int i6;
  double b_varargin_1[2];
  double b_BWP[2];
  x = (int)ceil((double)varargin_1->size[0] / 32.0);
  i6 = BWP->size[0] * BWP->size[1];
  BWP->size[0] = x;
  BWP->size[1] = varargin_1->size[1];
  emxEnsureCapacity_uint32_T(BWP, i6);
  for (i6 = 0; i6 < 2; i6++) {
    b_varargin_1[i6] = varargin_1->size[i6];
  }

  for (i6 = 0; i6 < 2; i6++) {
    b_BWP[i6] = BWP->size[i6];
  }

  bwPackingtbb(&varargin_1->data[0], b_varargin_1, &BWP->data[0], b_BWP);
}

//
// Arguments    : const emxArray_uint32_T *varargin_1
//                double varargin_2
//                emxArray_boolean_T *BW
// Return Type  : void
//
static void bwunpack(const emxArray_uint32_T *varargin_1, double varargin_2,
                     emxArray_boolean_T *BW)
{
  int i7;
  double b_varargin_1[2];
  double b_BW[2];
  i7 = BW->size[0] * BW->size[1];
  BW->size[0] = (int)varargin_2;
  BW->size[1] = varargin_1->size[1];
  emxEnsureCapacity_boolean_T(BW, i7);
  for (i7 = 0; i7 < 2; i7++) {
    b_varargin_1[i7] = varargin_1->size[i7];
  }

  for (i7 = 0; i7 < 2; i7++) {
    b_BW[i7] = BW->size[i7];
  }

  bwUnpackingtbb(&varargin_1->data[0], b_varargin_1, &BW->data[0], b_BW);
}

//
// Arguments    : const emxArray_real_T *x
// Return Type  : double
//
static double combineVectorElements(const emxArray_real_T *x)
{
  double y;
  int k;
  if (x->size[0] == 0) {
    y = 0.0;
  } else {
    y = x->data[0];
    for (k = 2; k <= x->size[0]; k++) {
      y += x->data[k - 1];
    }
  }

  return y;
}

//
// Arguments    : int numerator
//                int denominator
// Return Type  : int
//
static int div_s32_floor(int numerator, int denominator)
{
  int quotient;
  unsigned int absNumerator;
  unsigned int absDenominator;
  boolean_T quotientNeedsNegation;
  unsigned int tempAbsQuotient;
  if (denominator == 0) {
    if (numerator >= 0) {
      quotient = MAX_int32_T;
    } else {
      quotient = MIN_int32_T;
    }
  } else {
    if (numerator < 0) {
      absNumerator = ~(unsigned int)numerator + 1U;
    } else {
      absNumerator = (unsigned int)numerator;
    }

    if (denominator < 0) {
      absDenominator = ~(unsigned int)denominator + 1U;
    } else {
      absDenominator = (unsigned int)denominator;
    }

    quotientNeedsNegation = ((numerator < 0) != (denominator < 0));
    tempAbsQuotient = absNumerator / absDenominator;
    if (quotientNeedsNegation) {
      absNumerator %= absDenominator;
      if (absNumerator > 0U) {
        tempAbsQuotient++;
      }

      quotient = -(int)tempAbsQuotient;
    } else {
      quotient = (int)tempAbsQuotient;
    }
  }

  return quotient;
}

//
// Arguments    : const emxArray_real_T *f
//                emxArray_real_T *y
// Return Type  : void
//
static void gradf(const emxArray_real_T *f, emxArray_real_T *y)
{
  int vlen;
  int i;
  int i2;
  int i1;
  int k;
  vlen = f->size[0] - 2;
  i = y->size[0] * y->size[1];
  y->size[0] = f->size[0];
  y->size[1] = f->size[1];
  emxEnsureCapacity_real_T(y, i);
  if (f->size[0] < 2) {
    i = y->size[0] * y->size[1];
    y->size[0] = f->size[0];
    y->size[1] = f->size[1];
    emxEnsureCapacity_real_T(y, i);
    i2 = f->size[0] * f->size[1];
    for (i = 0; i < i2; i++) {
      y->data[i] = 0.0;
    }
  } else {
    i2 = -1;
    for (i = 1; i <= f->size[1]; i++) {
      i1 = i2;
      i2 = (i2 + vlen) + 2;
      i1++;
      y->data[i1] = f->data[i1 + 1] - f->data[i1];
      for (k = 1; k <= vlen; k++) {
        y->data[i1 + k] = (f->data[(i1 + k) + 1] - f->data[(i1 + k) - 1]) / 2.0;
      }

      y->data[i2] = f->data[i2] - f->data[i2 - 1];
    }
  }
}

//
// Arguments    : const emxArray_uint8_T *img
//                double radius
//                emxArray_boolean_T *bw
// Return Type  : void
//
static void imGradBw(const emxArray_uint8_T *img, double radius,
                     emxArray_boolean_T *bw)
{
  emxArray_real_T *b_img;
  int vstride;
  int i1;
  boolean_T p;
  emxArray_real_T *fx;
  emxArray_real_T *fy;
  int i2;
  int j;
  emxArray_real_T *gradgraph;
  int k;
  emxArray_boolean_T *b_bw;
  double level;
  double b_level;
  emxInit_real_T1(&b_img, 2);
  vstride = b_img->size[0] * b_img->size[1];
  b_img->size[0] = img->size[0];
  b_img->size[1] = img->size[1];
  emxEnsureCapacity_real_T(b_img, vstride);
  i1 = img->size[0] * img->size[1];
  for (vstride = 0; vstride < i1; vstride++) {
    b_img->data[vstride] = img->data[vstride];
  }

  imfilter(b_img);
  p = (b_img->size[1] == 1);
  emxInit_real_T1(&fx, 2);
  emxInit_real_T1(&fy, 2);
  if (p) {
    gradf(b_img, fx);
    vstride = fy->size[0] * fy->size[1];
    fy->size[0] = b_img->size[0];
    fy->size[1] = b_img->size[1];
    emxEnsureCapacity_real_T(fy, vstride);
    i1 = b_img->size[0] * b_img->size[1];
    for (vstride = 0; vstride < i1; vstride++) {
      fy->data[vstride] = 0.0;
    }
  } else {
    vstride = fx->size[0] * fx->size[1];
    fx->size[0] = b_img->size[0];
    fx->size[1] = b_img->size[1];
    emxEnsureCapacity_real_T(fx, vstride);
    if (b_img->size[1] < 2) {
      vstride = fx->size[0] * fx->size[1];
      fx->size[0] = b_img->size[0];
      fx->size[1] = b_img->size[1];
      emxEnsureCapacity_real_T(fx, vstride);
      i1 = b_img->size[0] * b_img->size[1];
      for (vstride = 0; vstride < i1; vstride++) {
        fx->data[vstride] = 0.0;
      }
    } else {
      vstride = b_img->size[0];
      i1 = -1;
      i2 = (b_img->size[1] - 1) * b_img->size[0] - 1;
      for (j = 1; j <= vstride; j++) {
        i1++;
        i2++;
        fx->data[i1] = b_img->data[i1 + vstride] - b_img->data[i1];
        for (k = 1; k <= b_img->size[1] - 2; k++) {
          fx->data[i1 + k * vstride] = (b_img->data[i1 + (k + 1) * vstride] -
            b_img->data[i1 + (k - 1) * vstride]) / 2.0;
        }

        fx->data[i2] = b_img->data[i2] - b_img->data[i2 - vstride];
      }
    }

    gradf(b_img, fy);
  }

  emxInit_real_T1(&gradgraph, 2);
  power(fx, gradgraph);
  power(fy, b_img);
  vstride = gradgraph->size[0] * gradgraph->size[1];
  emxEnsureCapacity_real_T(gradgraph, vstride);
  vstride = gradgraph->size[0];
  i1 = gradgraph->size[1];
  i1 *= vstride;
  emxFree_real_T(&fy);
  emxFree_real_T(&fx);
  for (vstride = 0; vstride < i1; vstride++) {
    gradgraph->data[vstride] += b_img->data[vstride];
  }

  emxFree_real_T(&b_img);
  i1 = gradgraph->size[0] * gradgraph->size[1];
  for (k = 0; k + 1 <= i1; k++) {
    gradgraph->data[k] = sqrt(gradgraph->data[k]);
  }

  emxInit_boolean_T(&b_bw, 2);
  level = otsuGray(gradgraph);
  imbinarize(gradgraph, level, bw);
  imerode(bw, b_bw);
  imdilate(b_bw, bw);
  level = 3.1415926535897931 * radius * radius / 3.0;
  emxFree_real_T(&gradgraph);
  imfill(bw);

  //  bw=imfill(bw,4);
  vstride = b_bw->size[0] * b_bw->size[1];
  b_bw->size[0] = bw->size[0];
  b_bw->size[1] = bw->size[1];
  emxEnsureCapacity_boolean_T(b_bw, vstride);
  i1 = bw->size[0] * bw->size[1];
  for (vstride = 0; vstride < i1; vstride++) {
    b_bw->data[vstride] = bw->data[vstride];
  }

  if (level < 0.0) {
    b_level = ceil(level);
  } else {
    b_level = floor(level);
  }

  bwareaopen(b_bw, b_level, bw);
  emxFree_boolean_T(&b_bw);
}

//
// Arguments    : const emxArray_real_T *I
//                double varargin_1
//                emxArray_boolean_T *BW
// Return Type  : void
//
static void imbinarize(const emxArray_real_T *I, double varargin_1,
  emxArray_boolean_T *BW)
{
  int i1;
  int loop_ub;
  i1 = BW->size[0] * BW->size[1];
  BW->size[0] = I->size[0];
  BW->size[1] = I->size[1];
  emxEnsureCapacity_boolean_T(BW, i1);
  loop_ub = I->size[0] * I->size[1];
  for (i1 = 0; i1 < loop_ub; i1++) {
    BW->data[i1] = (I->data[i1] > varargin_1);
  }
}

//
// Arguments    : const emxArray_boolean_T *A
//                emxArray_boolean_T *B
// Return Type  : void
//
static void imdilate(const emxArray_boolean_T *A, emxArray_boolean_T *B)
{
  emxArray_boolean_T *Apadpre;
  int i8;
  double sizeB[2];
  int j;
  int i;
  emxArray_boolean_T *Apad;
  int i9;
  emxArray_uint32_T *Apadpack;
  emxArray_uint32_T *b_B;
  boolean_T nhood[3];
  double varargin_1[2];
  int i10;
  int i11;
  int loop_ub;
  emxInit_boolean_T(&Apadpre, 2);
  if ((A->size[0] == 0) || (A->size[1] == 0)) {
    for (i8 = 0; i8 < 2; i8++) {
      sizeB[i8] = (double)A->size[i8] + 4.0;
    }

    i8 = Apadpre->size[0] * Apadpre->size[1];
    Apadpre->size[0] = (int)(unsigned int)sizeB[0];
    Apadpre->size[1] = (int)(unsigned int)sizeB[1];
    emxEnsureCapacity_boolean_T(Apadpre, i8);
    j = (int)(unsigned int)sizeB[0] * (int)(unsigned int)sizeB[1];
    for (i8 = 0; i8 < j; i8++) {
      Apadpre->data[i8] = false;
    }
  } else {
    for (i8 = 0; i8 < 2; i8++) {
      sizeB[i8] = (double)A->size[i8] + 4.0;
    }

    i8 = Apadpre->size[0] * Apadpre->size[1];
    Apadpre->size[0] = (int)sizeB[0];
    Apadpre->size[1] = (int)sizeB[1];
    emxEnsureCapacity_boolean_T(Apadpre, i8);
    for (j = 0; j < 4; j++) {
      i8 = Apadpre->size[0];
      for (i = 0; i < i8; i++) {
        Apadpre->data[i + Apadpre->size[0] * j] = false;
      }
    }

    i8 = Apadpre->size[1];
    for (j = 5; j <= i8; j++) {
      for (i = 0; i < 4; i++) {
        Apadpre->data[i + Apadpre->size[0] * (j - 1)] = false;
      }
    }

    for (j = 0; j < A->size[1]; j++) {
      for (i = 0; i < A->size[0]; i++) {
        Apadpre->data[(i + Apadpre->size[0] * (j + 4)) + 4] = A->data[i +
          A->size[0] * j];
      }
    }
  }

  for (i8 = 0; i8 < 2; i8++) {
    sizeB[i8] = (double)Apadpre->size[i8] + 4.0;
  }

  emxInit_boolean_T(&Apad, 2);
  i8 = Apad->size[0] * Apad->size[1];
  Apad->size[0] = (int)sizeB[0];
  Apad->size[1] = (int)sizeB[1];
  emxEnsureCapacity_boolean_T(Apad, i8);
  i8 = Apad->size[1];
  for (j = Apadpre->size[1]; j + 1 <= i8; j++) {
    i9 = Apad->size[0];
    for (i = 0; i < i9; i++) {
      Apad->data[i + Apad->size[0] * j] = false;
    }
  }

  for (j = 0; j < Apadpre->size[1]; j++) {
    i8 = Apad->size[0];
    for (i = Apadpre->size[0]; i + 1 <= i8; i++) {
      Apad->data[i + Apad->size[0] * j] = false;
    }
  }

  for (j = 0; j < Apadpre->size[1]; j++) {
    for (i = 0; i < Apadpre->size[0]; i++) {
      Apad->data[i + Apad->size[0] * j] = Apadpre->data[i + Apadpre->size[0] * j];
    }
  }

  emxInit_uint32_T(&Apadpack, 2);
  emxInit_uint32_T(&b_B, 2);
  bwpack(Apad, Apadpack);
  i8 = b_B->size[0] * b_B->size[1];
  b_B->size[0] = Apadpack->size[0];
  b_B->size[1] = Apadpack->size[1];
  emxEnsureCapacity_uint32_T(b_B, i8);
  for (i8 = 0; i8 < 2; i8++) {
    sizeB[i8] = Apadpack->size[i8];
  }

  for (i = 0; i < 3; i++) {
    nhood[i] = true;
  }

  for (i8 = 0; i8 < 2; i8++) {
    varargin_1[i8] = 3.0 + -2.0 * (double)i8;
  }

  dilate_packed_uint32(&Apadpack->data[0], sizeB, 2.0, nhood, varargin_1, 2.0,
                       &b_B->data[0]);
  i8 = Apadpack->size[0] * Apadpack->size[1];
  Apadpack->size[0] = b_B->size[0];
  Apadpack->size[1] = b_B->size[1];
  emxEnsureCapacity_uint32_T(Apadpack, i8);
  j = b_B->size[0] * b_B->size[1];
  for (i8 = 0; i8 < j; i8++) {
    Apadpack->data[i8] = b_B->data[i8];
  }

  for (i8 = 0; i8 < 2; i8++) {
    sizeB[i8] = b_B->size[i8];
  }

  for (i8 = 0; i8 < 2; i8++) {
    varargin_1[i8] = 3.0;
  }

  dilate_packed_uint32(&Apadpack->data[0], sizeB, 2.0, bv0, varargin_1, 2.0,
                       &b_B->data[0]);
  i8 = Apadpack->size[0] * Apadpack->size[1];
  Apadpack->size[0] = b_B->size[0];
  Apadpack->size[1] = b_B->size[1];
  emxEnsureCapacity_uint32_T(Apadpack, i8);
  j = b_B->size[0] * b_B->size[1];
  for (i8 = 0; i8 < j; i8++) {
    Apadpack->data[i8] = b_B->data[i8];
  }

  for (i8 = 0; i8 < 2; i8++) {
    sizeB[i8] = b_B->size[i8];
  }

  for (i = 0; i < 3; i++) {
    nhood[i] = true;
  }

  for (i8 = 0; i8 < 2; i8++) {
    varargin_1[i8] = 1.0 + 2.0 * (double)i8;
  }

  dilate_packed_uint32(&Apadpack->data[0], sizeB, 2.0, nhood, varargin_1, 2.0,
                       &b_B->data[0]);
  i8 = Apadpack->size[0] * Apadpack->size[1];
  Apadpack->size[0] = b_B->size[0];
  Apadpack->size[1] = b_B->size[1];
  emxEnsureCapacity_uint32_T(Apadpack, i8);
  j = b_B->size[0] * b_B->size[1];
  for (i8 = 0; i8 < j; i8++) {
    Apadpack->data[i8] = b_B->data[i8];
  }

  for (i8 = 0; i8 < 2; i8++) {
    sizeB[i8] = b_B->size[i8];
  }

  for (i8 = 0; i8 < 2; i8++) {
    varargin_1[i8] = 3.0;
  }

  dilate_packed_uint32(&Apadpack->data[0], sizeB, 2.0, bv1, varargin_1, 2.0,
                       &b_B->data[0]);
  i8 = Apadpack->size[0] * Apadpack->size[1];
  Apadpack->size[0] = b_B->size[0];
  Apadpack->size[1] = b_B->size[1];
  emxEnsureCapacity_uint32_T(Apadpack, i8);
  j = b_B->size[0] * b_B->size[1];
  for (i8 = 0; i8 < j; i8++) {
    Apadpack->data[i8] = b_B->data[i8];
  }

  for (i8 = 0; i8 < 2; i8++) {
    sizeB[i8] = b_B->size[i8];
  }

  for (i = 0; i < 3; i++) {
    nhood[i] = true;
  }

  for (i8 = 0; i8 < 2; i8++) {
    varargin_1[i8] = 1.0 + 2.0 * (double)i8;
  }

  dilate_packed_uint32(&Apadpack->data[0], sizeB, 2.0, nhood, varargin_1, 2.0,
                       &b_B->data[0]);
  i8 = Apadpack->size[0] * Apadpack->size[1];
  Apadpack->size[0] = b_B->size[0];
  Apadpack->size[1] = b_B->size[1];
  emxEnsureCapacity_uint32_T(Apadpack, i8);
  j = b_B->size[0] * b_B->size[1];
  for (i8 = 0; i8 < j; i8++) {
    Apadpack->data[i8] = b_B->data[i8];
  }

  for (i8 = 0; i8 < 2; i8++) {
    sizeB[i8] = b_B->size[i8];
  }

  for (i = 0; i < 3; i++) {
    nhood[i] = true;
  }

  for (i8 = 0; i8 < 2; i8++) {
    varargin_1[i8] = 3.0 + -2.0 * (double)i8;
  }

  dilate_packed_uint32(&Apadpack->data[0], sizeB, 2.0, nhood, varargin_1, 2.0,
                       &b_B->data[0]);
  bwunpack(b_B, (double)Apad->size[0], Apadpre);
  emxFree_uint32_T(&b_B);
  emxFree_uint32_T(&Apadpack);
  emxFree_boolean_T(&Apad);
  for (i8 = 0; i8 < 2; i8++) {
    sizeB[i8] = (((5.0 + (double)Apadpre->size[i8]) - 4.0) - 4.0) - 1.0;
  }

  if (5 > (int)sizeB[0]) {
    i8 = 0;
    i9 = 0;
  } else {
    i8 = 4;
    i9 = (int)sizeB[0];
  }

  if (5 > (int)sizeB[1]) {
    i = 0;
    i10 = 0;
  } else {
    i = 4;
    i10 = (int)sizeB[1];
  }

  i11 = B->size[0] * B->size[1];
  B->size[0] = i9 - i8;
  B->size[1] = i10 - i;
  emxEnsureCapacity_boolean_T(B, i11);
  j = i10 - i;
  for (i10 = 0; i10 < j; i10++) {
    loop_ub = i9 - i8;
    for (i11 = 0; i11 < loop_ub; i11++) {
      B->data[i11 + B->size[0] * i10] = Apadpre->data[(i8 + i11) + Apadpre->
        size[0] * (i + i10)];
    }
  }

  emxFree_boolean_T(&Apadpre);
}

//
// Arguments    : const emxArray_boolean_T *A
//                emxArray_boolean_T *B
// Return Type  : void
//
static void imerode(const emxArray_boolean_T *A, emxArray_boolean_T *B)
{
  emxArray_boolean_T *Apadpre;
  int i2;
  double sizeB[2];
  int j;
  int i;
  emxArray_boolean_T *Apad;
  int i3;
  emxArray_uint32_T *Apadpack;
  emxArray_uint32_T *b_B;
  boolean_T nhood[3];
  double varargin_1[2];
  int i4;
  int i5;
  int loop_ub;
  emxInit_boolean_T(&Apadpre, 2);
  if ((A->size[0] == 0) || (A->size[1] == 0)) {
    for (i2 = 0; i2 < 2; i2++) {
      sizeB[i2] = (double)A->size[i2] + 4.0;
    }

    i2 = Apadpre->size[0] * Apadpre->size[1];
    Apadpre->size[0] = (int)(unsigned int)sizeB[0];
    Apadpre->size[1] = (int)(unsigned int)sizeB[1];
    emxEnsureCapacity_boolean_T(Apadpre, i2);
    j = (int)(unsigned int)sizeB[0] * (int)(unsigned int)sizeB[1];
    for (i2 = 0; i2 < j; i2++) {
      Apadpre->data[i2] = true;
    }
  } else {
    for (i2 = 0; i2 < 2; i2++) {
      sizeB[i2] = (double)A->size[i2] + 4.0;
    }

    i2 = Apadpre->size[0] * Apadpre->size[1];
    Apadpre->size[0] = (int)sizeB[0];
    Apadpre->size[1] = (int)sizeB[1];
    emxEnsureCapacity_boolean_T(Apadpre, i2);
    for (j = 0; j < 4; j++) {
      i2 = Apadpre->size[0];
      for (i = 0; i < i2; i++) {
        Apadpre->data[i + Apadpre->size[0] * j] = true;
      }
    }

    i2 = Apadpre->size[1];
    for (j = 5; j <= i2; j++) {
      for (i = 0; i < 4; i++) {
        Apadpre->data[i + Apadpre->size[0] * (j - 1)] = true;
      }
    }

    for (j = 0; j < A->size[1]; j++) {
      for (i = 0; i < A->size[0]; i++) {
        Apadpre->data[(i + Apadpre->size[0] * (j + 4)) + 4] = A->data[i +
          A->size[0] * j];
      }
    }
  }

  for (i2 = 0; i2 < 2; i2++) {
    sizeB[i2] = (double)Apadpre->size[i2] + 4.0;
  }

  emxInit_boolean_T(&Apad, 2);
  i2 = Apad->size[0] * Apad->size[1];
  Apad->size[0] = (int)sizeB[0];
  Apad->size[1] = (int)sizeB[1];
  emxEnsureCapacity_boolean_T(Apad, i2);
  i2 = Apad->size[1];
  for (j = Apadpre->size[1]; j + 1 <= i2; j++) {
    i3 = Apad->size[0];
    for (i = 0; i < i3; i++) {
      Apad->data[i + Apad->size[0] * j] = true;
    }
  }

  for (j = 0; j < Apadpre->size[1]; j++) {
    i2 = Apad->size[0];
    for (i = Apadpre->size[0]; i + 1 <= i2; i++) {
      Apad->data[i + Apad->size[0] * j] = true;
    }
  }

  for (j = 0; j < Apadpre->size[1]; j++) {
    for (i = 0; i < Apadpre->size[0]; i++) {
      Apad->data[i + Apad->size[0] * j] = Apadpre->data[i + Apadpre->size[0] * j];
    }
  }

  emxInit_uint32_T(&Apadpack, 2);
  emxInit_uint32_T(&b_B, 2);
  bwpack(Apad, Apadpack);
  i2 = b_B->size[0] * b_B->size[1];
  b_B->size[0] = Apadpack->size[0];
  b_B->size[1] = Apadpack->size[1];
  emxEnsureCapacity_uint32_T(b_B, i2);
  for (i2 = 0; i2 < 2; i2++) {
    sizeB[i2] = Apadpack->size[i2];
  }

  for (i = 0; i < 3; i++) {
    nhood[i] = true;
  }

  for (i2 = 0; i2 < 2; i2++) {
    varargin_1[i2] = 3.0 + -2.0 * (double)i2;
  }

  erode_packed_uint32(&Apadpack->data[0], sizeB, 2.0, nhood, varargin_1, 2.0,
                      (double)Apad->size[0], &b_B->data[0]);
  i2 = Apadpack->size[0] * Apadpack->size[1];
  Apadpack->size[0] = b_B->size[0];
  Apadpack->size[1] = b_B->size[1];
  emxEnsureCapacity_uint32_T(Apadpack, i2);
  j = b_B->size[0] * b_B->size[1];
  for (i2 = 0; i2 < j; i2++) {
    Apadpack->data[i2] = b_B->data[i2];
  }

  for (i2 = 0; i2 < 2; i2++) {
    sizeB[i2] = b_B->size[i2];
  }

  for (i2 = 0; i2 < 2; i2++) {
    varargin_1[i2] = 3.0;
  }

  erode_packed_uint32(&Apadpack->data[0], sizeB, 2.0, bv0, varargin_1, 2.0,
                      (double)Apad->size[0], &b_B->data[0]);
  i2 = Apadpack->size[0] * Apadpack->size[1];
  Apadpack->size[0] = b_B->size[0];
  Apadpack->size[1] = b_B->size[1];
  emxEnsureCapacity_uint32_T(Apadpack, i2);
  j = b_B->size[0] * b_B->size[1];
  for (i2 = 0; i2 < j; i2++) {
    Apadpack->data[i2] = b_B->data[i2];
  }

  for (i2 = 0; i2 < 2; i2++) {
    sizeB[i2] = b_B->size[i2];
  }

  for (i = 0; i < 3; i++) {
    nhood[i] = true;
  }

  for (i2 = 0; i2 < 2; i2++) {
    varargin_1[i2] = 1.0 + 2.0 * (double)i2;
  }

  erode_packed_uint32(&Apadpack->data[0], sizeB, 2.0, nhood, varargin_1, 2.0,
                      (double)Apad->size[0], &b_B->data[0]);
  i2 = Apadpack->size[0] * Apadpack->size[1];
  Apadpack->size[0] = b_B->size[0];
  Apadpack->size[1] = b_B->size[1];
  emxEnsureCapacity_uint32_T(Apadpack, i2);
  j = b_B->size[0] * b_B->size[1];
  for (i2 = 0; i2 < j; i2++) {
    Apadpack->data[i2] = b_B->data[i2];
  }

  for (i2 = 0; i2 < 2; i2++) {
    sizeB[i2] = b_B->size[i2];
  }

  for (i2 = 0; i2 < 2; i2++) {
    varargin_1[i2] = 3.0;
  }

  erode_packed_uint32(&Apadpack->data[0], sizeB, 2.0, bv1, varargin_1, 2.0,
                      (double)Apad->size[0], &b_B->data[0]);
  i2 = Apadpack->size[0] * Apadpack->size[1];
  Apadpack->size[0] = b_B->size[0];
  Apadpack->size[1] = b_B->size[1];
  emxEnsureCapacity_uint32_T(Apadpack, i2);
  j = b_B->size[0] * b_B->size[1];
  for (i2 = 0; i2 < j; i2++) {
    Apadpack->data[i2] = b_B->data[i2];
  }

  for (i2 = 0; i2 < 2; i2++) {
    sizeB[i2] = b_B->size[i2];
  }

  for (i = 0; i < 3; i++) {
    nhood[i] = true;
  }

  for (i2 = 0; i2 < 2; i2++) {
    varargin_1[i2] = 1.0 + 2.0 * (double)i2;
  }

  erode_packed_uint32(&Apadpack->data[0], sizeB, 2.0, nhood, varargin_1, 2.0,
                      (double)Apad->size[0], &b_B->data[0]);
  i2 = Apadpack->size[0] * Apadpack->size[1];
  Apadpack->size[0] = b_B->size[0];
  Apadpack->size[1] = b_B->size[1];
  emxEnsureCapacity_uint32_T(Apadpack, i2);
  j = b_B->size[0] * b_B->size[1];
  for (i2 = 0; i2 < j; i2++) {
    Apadpack->data[i2] = b_B->data[i2];
  }

  for (i2 = 0; i2 < 2; i2++) {
    sizeB[i2] = b_B->size[i2];
  }

  for (i = 0; i < 3; i++) {
    nhood[i] = true;
  }

  for (i2 = 0; i2 < 2; i2++) {
    varargin_1[i2] = 3.0 + -2.0 * (double)i2;
  }

  erode_packed_uint32(&Apadpack->data[0], sizeB, 2.0, nhood, varargin_1, 2.0,
                      (double)Apad->size[0], &b_B->data[0]);
  bwunpack(b_B, (double)Apad->size[0], Apadpre);
  emxFree_uint32_T(&b_B);
  emxFree_uint32_T(&Apadpack);
  emxFree_boolean_T(&Apad);
  for (i2 = 0; i2 < 2; i2++) {
    sizeB[i2] = (((5.0 + (double)Apadpre->size[i2]) - 4.0) - 4.0) - 1.0;
  }

  if (5 > (int)sizeB[0]) {
    i2 = 0;
    i3 = 0;
  } else {
    i2 = 4;
    i3 = (int)sizeB[0];
  }

  if (5 > (int)sizeB[1]) {
    i = 0;
    i4 = 0;
  } else {
    i = 4;
    i4 = (int)sizeB[1];
  }

  i5 = B->size[0] * B->size[1];
  B->size[0] = i3 - i2;
  B->size[1] = i4 - i;
  emxEnsureCapacity_boolean_T(B, i5);
  j = i4 - i;
  for (i4 = 0; i4 < j; i4++) {
    loop_ub = i3 - i2;
    for (i5 = 0; i5 < loop_ub; i5++) {
      B->data[i5 + B->size[0] * i4] = Apadpre->data[(i2 + i5) + Apadpre->size[0]
        * (i + i4)];
    }
  }

  emxFree_boolean_T(&Apadpre);
}

//
// Arguments    : emxArray_boolean_T *varargin_1
// Return Type  : void
//
static void imfill(emxArray_boolean_T *varargin_1)
{
  emxArray_uint8_T *mask;
  int i19;
  int loop_ub;
  emxArray_uint8_T *maskPad;
  double b_varargin_1[2];
  emxArray_uint8_T *markerPad;
  emxArray_uint8_T *r5;
  int b_loop_ub;
  int k;
  emxArray_int32_T *idx;
  emxArray_int32_T *r6;
  emxArray_int32_T *r7;
  emxArray_boolean_T *b_idx;
  double idx1;
  double idx2;
  if (!((varargin_1->size[0] == 0) || (varargin_1->size[1] == 0))) {
    emxInit_uint8_T1(&mask, 2);
    i19 = mask->size[0] * mask->size[1];
    mask->size[0] = varargin_1->size[0];
    mask->size[1] = varargin_1->size[1];
    emxEnsureCapacity_uint8_T1(mask, i19);
    loop_ub = varargin_1->size[0] * varargin_1->size[1];
    for (i19 = 0; i19 < loop_ub; i19++) {
      mask->data[i19] = varargin_1->data[i19];
    }

    for (i19 = 0; i19 < 2; i19++) {
      b_varargin_1[i19] = (double)varargin_1->size[i19] + 2.0;
    }

    emxInit_uint8_T1(&maskPad, 2);
    i19 = maskPad->size[0] * maskPad->size[1];
    maskPad->size[0] = (int)b_varargin_1[0];
    maskPad->size[1] = (int)b_varargin_1[1];
    emxEnsureCapacity_uint8_T1(maskPad, i19);
    for (i19 = 0; i19 < 2; i19++) {
      b_varargin_1[i19] = (double)varargin_1->size[i19] + 2.0;
    }

    emxInit_uint8_T1(&markerPad, 2);
    emxInit_uint8_T1(&r5, 2);
    i19 = markerPad->size[0] * markerPad->size[1];
    markerPad->size[0] = (int)b_varargin_1[0];
    markerPad->size[1] = (int)b_varargin_1[1];
    emxEnsureCapacity_uint8_T1(markerPad, i19);
    padarray(mask, r5);
    loop_ub = r5->size[1];
    for (i19 = 0; i19 < loop_ub; i19++) {
      b_loop_ub = r5->size[0];
      for (k = 0; k < b_loop_ub; k++) {
        maskPad->data[k + maskPad->size[0] * i19] = r5->data[k + r5->size[0] *
          i19];
      }
    }

    i19 = r5->size[0] * r5->size[1];
    r5->size[0] = mask->size[0];
    r5->size[1] = mask->size[1];
    emxEnsureCapacity_uint8_T1(r5, i19);
    loop_ub = mask->size[0] * mask->size[1];
    for (i19 = 0; i19 < loop_ub; i19++) {
      r5->data[i19] = MAX_uint8_T;
    }

    padarray(r5, mask);
    loop_ub = mask->size[1];
    emxFree_uint8_T(&r5);
    for (i19 = 0; i19 < loop_ub; i19++) {
      b_loop_ub = mask->size[0];
      for (k = 0; k < b_loop_ub; k++) {
        markerPad->data[k + markerPad->size[0] * i19] = mask->data[k +
          mask->size[0] * i19];
      }
    }

    emxFree_uint8_T(&mask);
    for (i19 = 0; i19 < 2; i19++) {
      b_varargin_1[i19] = markerPad->size[i19];
    }

    b_loop_ub = (int)(unsigned int)b_varargin_1[0] - 2;
    if ((unsigned int)b_varargin_1[1] > (unsigned int)b_varargin_1[0]) {
      b_loop_ub = (int)(unsigned int)b_varargin_1[1] - 2;
    }

    emxInit_int32_T1(&idx, 2);
    i19 = idx->size[0] * idx->size[1];
    idx->size[0] = b_loop_ub;
    idx->size[1] = 2;
    emxEnsureCapacity_int32_T1(idx, i19);
    loop_ub = b_loop_ub << 1;
    for (i19 = 0; i19 < loop_ub; i19++) {
      idx->data[i19] = 0;
    }

    emxInit_int32_T(&r6, 1);
    emxInit_int32_T1(&r7, 2);
    for (k = 0; k < 2; k++) {
      if (1 > markerPad->size[k] - 2) {
        loop_ub = 0;
      } else {
        loop_ub = markerPad->size[k] - 2;
      }

      i19 = r6->size[0];
      r6->size[0] = loop_ub;
      emxEnsureCapacity_int32_T(r6, i19);
      for (i19 = 0; i19 < loop_ub; i19++) {
        r6->data[i19] = i19;
      }

      if (markerPad->size[k] - 1 < 2) {
        i19 = r7->size[0] * r7->size[1];
        r7->size[0] = 1;
        r7->size[1] = 0;
        emxEnsureCapacity_int32_T1(r7, i19);
      } else {
        loop_ub = markerPad->size[k] - 3;
        i19 = r7->size[0] * r7->size[1];
        r7->size[0] = 1;
        r7->size[1] = loop_ub + 1;
        emxEnsureCapacity_int32_T1(r7, i19);
        for (i19 = 0; i19 <= loop_ub; i19++) {
          r7->data[r7->size[0] * i19] = 2 + i19;
        }
      }

      b_loop_ub = r6->size[0];
      for (i19 = 0; i19 < b_loop_ub; i19++) {
        idx->data[r6->data[i19] + idx->size[0] * k] = r7->data[i19];
      }
    }

    emxFree_int32_T(&r7);
    emxFree_int32_T(&r6);
    i19 = maskPad->size[0] * maskPad->size[1];
    emxEnsureCapacity_uint8_T1(maskPad, i19);
    b_loop_ub = maskPad->size[0];
    k = maskPad->size[1];
    loop_ub = b_loop_ub * k;
    for (i19 = 0; i19 < loop_ub; i19++) {
      maskPad->data[i19] = (unsigned char)(255U - maskPad->data[i19]);
    }

    i19 = markerPad->size[0] * markerPad->size[1];
    emxEnsureCapacity_uint8_T1(markerPad, i19);
    b_loop_ub = markerPad->size[0];
    k = markerPad->size[1];
    loop_ub = b_loop_ub * k;
    for (i19 = 0; i19 < loop_ub; i19++) {
      markerPad->data[i19] = (unsigned char)(255U - markerPad->data[i19]);
    }

    for (i19 = 0; i19 < 2; i19++) {
      b_varargin_1[i19] = markerPad->size[i19];
    }

    ippreconstruct_uint8(&markerPad->data[0], &maskPad->data[0], b_varargin_1,
                         1.0);
    i19 = markerPad->size[0] * markerPad->size[1];
    emxEnsureCapacity_uint8_T1(markerPad, i19);
    b_loop_ub = markerPad->size[0];
    k = markerPad->size[1];
    loop_ub = b_loop_ub * k;
    emxFree_uint8_T(&maskPad);
    for (i19 = 0; i19 < loop_ub; i19++) {
      markerPad->data[i19] = (unsigned char)(255U - markerPad->data[i19]);
    }

    emxInit_boolean_T1(&b_idx, 1);
    loop_ub = idx->size[0];
    i19 = b_idx->size[0];
    b_idx->size[0] = loop_ub;
    emxEnsureCapacity_boolean_T1(b_idx, i19);
    for (i19 = 0; i19 < loop_ub; i19++) {
      b_idx->data[i19] = (idx->data[i19] != 0);
    }

    idx1 = sum(b_idx);
    loop_ub = idx->size[0];
    i19 = b_idx->size[0];
    b_idx->size[0] = loop_ub;
    emxEnsureCapacity_boolean_T1(b_idx, i19);
    for (i19 = 0; i19 < loop_ub; i19++) {
      b_idx->data[i19] = (idx->data[i19 + idx->size[0]] != 0);
    }

    idx2 = sum(b_idx);
    b_varargin_1[0] = markerPad->size[0] - 2;
    b_varargin_1[1] = markerPad->size[1] - 2;
    i19 = varargin_1->size[0] * varargin_1->size[1];
    varargin_1->size[0] = (int)b_varargin_1[0];
    varargin_1->size[1] = (int)b_varargin_1[1];
    emxEnsureCapacity_boolean_T(varargin_1, i19);
    b_loop_ub = 0;
    emxFree_boolean_T(&b_idx);
    while (b_loop_ub <= (int)idx1 - 1) {
      for (k = 0; k < (int)idx2; k++) {
        varargin_1->data[b_loop_ub + varargin_1->size[0] * k] = (markerPad->
          data[(idx->data[b_loop_ub] + markerPad->size[0] * (idx->data[k +
          idx->size[0]] - 1)) - 1] != 0);
      }

      b_loop_ub++;
    }

    emxFree_int32_T(&idx);
    emxFree_uint8_T(&markerPad);
    i19 = varargin_1->size[0] * varargin_1->size[1];
    emxEnsureCapacity_boolean_T(varargin_1, i19);
  }
}

//
// Arguments    : emxArray_real_T *varargin_1
// Return Type  : void
//
static void imfilter(emxArray_real_T *varargin_1)
{
  double finalSize[2];
  double padSize[2];
  emxArray_real_T *a;
  int i17;
  double sizeB[2];
  int j;
  int i;
  boolean_T tooBig;
  int i18;
  double nonZeroKernel[25];
  boolean_T conn[25];
  double b_varargin_1[2];
  finalSize[0] = varargin_1->size[0];
  padSize[0] = 2.0;
  finalSize[1] = varargin_1->size[1];
  padSize[1] = 2.0;
  if (!((varargin_1->size[0] == 0) || (varargin_1->size[1] == 0))) {
    emxInit_real_T1(&a, 2);
    if ((varargin_1->size[0] == 0) || (varargin_1->size[1] == 0)) {
      for (i17 = 0; i17 < 2; i17++) {
        sizeB[i17] = (double)varargin_1->size[i17] + 4.0;
      }

      i17 = a->size[0] * a->size[1];
      a->size[0] = (int)sizeB[0];
      a->size[1] = (int)sizeB[1];
      emxEnsureCapacity_real_T(a, i17);
      j = (int)sizeB[0] * (int)sizeB[1];
      for (i17 = 0; i17 < j; i17++) {
        a->data[i17] = 0.0;
      }
    } else {
      for (i17 = 0; i17 < 2; i17++) {
        sizeB[i17] = (double)varargin_1->size[i17] + 4.0;
      }

      i17 = a->size[0] * a->size[1];
      a->size[0] = (int)sizeB[0];
      a->size[1] = (int)sizeB[1];
      emxEnsureCapacity_real_T(a, i17);
      for (j = 0; j < 2; j++) {
        i17 = a->size[0];
        for (i = 0; i < i17; i++) {
          a->data[i + a->size[0] * j] = 0.0;
        }
      }

      i17 = a->size[1];
      for (j = varargin_1->size[1] + 2; j + 1 <= i17; j++) {
        i18 = a->size[0];
        for (i = 0; i < i18; i++) {
          a->data[i + a->size[0] * j] = 0.0;
        }
      }

      for (j = 0; j < varargin_1->size[1]; j++) {
        for (i = 0; i < 2; i++) {
          a->data[i + a->size[0] * (j + 2)] = 0.0;
        }
      }

      for (j = 0; j < varargin_1->size[1]; j++) {
        i17 = a->size[0];
        for (i = varargin_1->size[0] + 2; i + 1 <= i17; i++) {
          a->data[i + a->size[0] * (j + 2)] = 0.0;
        }
      }

      for (j = 0; j < varargin_1->size[1]; j++) {
        for (i = 0; i < varargin_1->size[0]; i++) {
          a->data[(i + a->size[0] * (j + 2)) + 2] = varargin_1->data[i +
            varargin_1->size[0] * j];
        }
      }
    }

    tooBig = true;
    for (i = 0; i < 2; i++) {
      if (tooBig && (finalSize[i] > 65500.0)) {
        tooBig = true;
      } else {
        tooBig = false;
      }
    }

    i17 = varargin_1->size[0] * varargin_1->size[1];
    varargin_1->size[0] = (int)finalSize[0];
    varargin_1->size[1] = (int)finalSize[1];
    emxEnsureCapacity_real_T(varargin_1, i17);
    if (!tooBig) {
      for (i17 = 0; i17 < 2; i17++) {
        padSize[i17] = a->size[i17];
      }

      for (i = 0; i < 25; i++) {
        nonZeroKernel[i] = 0.04;
      }

      for (i17 = 0; i17 < 2; i17++) {
        sizeB[i17] = 5.0;
      }

      ippfilter_real64(&a->data[0], &varargin_1->data[0], finalSize, 2.0,
                       padSize, nonZeroKernel, sizeB, false);
    } else {
      for (i17 = 0; i17 < 2; i17++) {
        sizeB[i17] = a->size[i17];
      }

      for (i = 0; i < 25; i++) {
        nonZeroKernel[i] = 0.04;
        conn[i] = true;
      }

      for (i17 = 0; i17 < 2; i17++) {
        b_varargin_1[i17] = 5.0;
      }

      imfilter_real64(&a->data[0], &varargin_1->data[0], 2.0, finalSize, 2.0,
                      sizeB, nonZeroKernel, 25.0, conn, 2.0, b_varargin_1,
                      padSize, 2.0, true, false);
    }

    emxFree_real_T(&a);
  }
}

//
// Arguments    : emxArray_real_T *x
//                const emxArray_boolean_T *idx
// Return Type  : void
//
static void nullAssignment(emxArray_real_T *x, const emxArray_boolean_T *idx)
{
  int nxin;
  int k0;
  int k;
  int nxout;
  emxArray_real_T *b_x;
  nxin = x->size[0];
  k0 = 0;
  for (k = 1; k <= idx->size[0]; k++) {
    k0 += idx->data[k - 1];
  }

  nxout = x->size[0] - k0;
  k0 = -1;
  for (k = 1; k <= nxin; k++) {
    if ((k > idx->size[0]) || (!idx->data[k - 1])) {
      k0++;
      x->data[k0] = x->data[k - 1];
    }
  }

  if (1 > nxout) {
    k0 = 0;
  } else {
    k0 = nxout;
  }

  emxInit_real_T(&b_x, 1);
  nxout = b_x->size[0];
  b_x->size[0] = k0;
  emxEnsureCapacity_real_T1(b_x, nxout);
  for (nxout = 0; nxout < k0; nxout++) {
    b_x->data[nxout] = x->data[nxout];
  }

  nxout = x->size[0];
  x->size[0] = b_x->size[0];
  emxEnsureCapacity_real_T1(x, nxout);
  k0 = b_x->size[0];
  for (nxout = 0; nxout < k0; nxout++) {
    x->data[nxout] = b_x->data[nxout];
  }

  emxFree_real_T(&b_x);
}

//
// Arguments    : const emxArray_real_T *I
// Return Type  : double
//
static double otsuGray(const emxArray_real_T *I)
{
  emxArray_real_T *img;
  int i;
  int loop_ub;
  emxArray_uint8_T *b_I;
  double numCores;
  double counts[256];
  boolean_T useParallel;
  double localBins2[256];
  double localBins3[256];
  double localBins1[256];
  boolean_T rngFlag;
  double maxval;
  double p;
  emxInit_real_T(&img, 1);
  i = img->size[0];
  img->size[0] = I->size[0] * I->size[1];
  emxEnsureCapacity_real_T1(img, i);
  loop_ub = I->size[0] * I->size[1];
  for (i = 0; i < loop_ub; i++) {
    img->data[i] = I->data[i];
  }

  emxInit_uint8_T(&b_I, 1);
  i = b_I->size[0];
  b_I->size[0] = I->size[0] * I->size[1];
  emxEnsureCapacity_uint8_T(b_I, i);
  grayto8_real64(&img->data[0], &b_I->data[0], (double)(I->size[0] * I->size[1]));
  emxFree_real_T(&img);
  if (b_I->size[0] == 0) {
    memset(&counts[0], 0, sizeof(double) << 8);
  } else {
    numCores = 1.0;
    getnumcores(&numCores);
    if ((b_I->size[0] > 500000) && (numCores > 1.0)) {
      useParallel = true;
    } else {
      useParallel = false;
    }

    if (useParallel) {
      useParallel = false;
      rngFlag = false;
      tbbhist_uint8(&b_I->data[0], (double)b_I->size[0], (double)b_I->size[0],
                    (double)b_I->size[0] / (double)b_I->size[0], counts, 256.0,
                    256.0, &rngFlag, &useParallel);
    } else {
      memset(&counts[0], 0, sizeof(double) << 8);
      memset(&localBins1[0], 0, sizeof(double) << 8);
      memset(&localBins2[0], 0, sizeof(double) << 8);
      memset(&localBins3[0], 0, sizeof(double) << 8);
      for (i = 0; i + 4 <= b_I->size[0]; i += 4) {
        localBins1[b_I->data[i]]++;
        localBins2[b_I->data[i + 1]]++;
        localBins3[b_I->data[i + 2]]++;
        counts[b_I->data[i + 3]]++;
      }

      while (i + 1 <= b_I->size[0]) {
        counts[b_I->data[i]]++;
        i++;
      }

      for (i = 0; i < 256; i++) {
        counts[i] = ((counts[i] + localBins1[i]) + localBins2[i]) + localBins3[i];
      }
    }
  }

  emxFree_uint8_T(&b_I);
  numCores = 0.0;
  for (i = 0; i < 256; i++) {
    numCores += counts[i];
  }

  localBins2[0] = counts[0] / numCores;
  localBins3[0] = counts[0] / numCores;
  for (i = 0; i < 255; i++) {
    p = counts[i + 1] / numCores;
    localBins2[i + 1] = localBins2[i] + p;
    localBins3[i + 1] = localBins3[i] + p * (2.0 + (double)i);
  }

  maxval = -1.7976931348623157E+308;
  for (i = 0; i < 256; i++) {
    numCores = localBins3[255] * localBins2[i] - localBins3[i];
    numCores = numCores * numCores / (localBins2[i] * (1.0 - localBins2[i]));
    if (!(maxval > numCores)) {
      maxval = numCores;
    }

    localBins1[i] = numCores;
  }

  numCores = 0.0;
  p = 0.0;
  for (i = 0; i < 256; i++) {
    numCores += (double)((1 + i) * (localBins1[i] == maxval));
    p += (double)(localBins1[i] == maxval);
  }

  numCores /= p;
  return (numCores - 1.0) / 255.0;
}

//
// Arguments    : const emxArray_uint8_T *varargin_1
//                emxArray_uint8_T *b
// Return Type  : void
//
static void padarray(const emxArray_uint8_T *varargin_1, emxArray_uint8_T *b)
{
  int i12;
  unsigned int sizeB[2];
  int i;
  int j;
  int i13;
  if ((varargin_1->size[0] == 0) || (varargin_1->size[1] == 0)) {
    for (i12 = 0; i12 < 2; i12++) {
      sizeB[i12] = varargin_1->size[i12] + 2U;
    }

    i12 = b->size[0] * b->size[1];
    b->size[0] = (int)sizeB[0];
    b->size[1] = (int)sizeB[1];
    emxEnsureCapacity_uint8_T1(b, i12);
    i = (int)sizeB[0] * (int)sizeB[1];
    for (i12 = 0; i12 < i; i12++) {
      b->data[i12] = 0;
    }
  } else {
    for (i12 = 0; i12 < 2; i12++) {
      sizeB[i12] = varargin_1->size[i12] + 2U;
    }

    i12 = b->size[0] * b->size[1];
    b->size[0] = (int)sizeB[0];
    b->size[1] = (int)sizeB[1];
    emxEnsureCapacity_uint8_T1(b, i12);
    i12 = b->size[0];
    for (i = 0; i < i12; i++) {
      b->data[i] = 0;
    }

    i12 = b->size[1];
    for (j = varargin_1->size[1] + 1; j + 1 <= i12; j++) {
      i13 = b->size[0];
      for (i = 0; i < i13; i++) {
        b->data[i + b->size[0] * j] = 0;
      }
    }

    for (j = 0; j < varargin_1->size[1]; j++) {
      b->data[b->size[0] * (j + 1)] = 0;
    }

    for (j = 0; j < varargin_1->size[1]; j++) {
      i12 = b->size[0];
      for (i = varargin_1->size[0] + 1; i + 1 <= i12; i++) {
        b->data[i + b->size[0] * (j + 1)] = 0;
      }
    }

    for (j = 0; j < varargin_1->size[1]; j++) {
      for (i = 0; i < varargin_1->size[0]; i++) {
        b->data[(i + b->size[0] * (j + 1)) + 1] = varargin_1->data[i +
          varargin_1->size[0] * j];
      }
    }
  }
}

//
// Arguments    : const emxArray_real_T *a
//                emxArray_real_T *y
// Return Type  : void
//
static void power(const emxArray_real_T *a, emxArray_real_T *y)
{
  int nx;
  emxArray_real_T *ztemp;
  unsigned int uv0[2];
  int k;
  for (nx = 0; nx < 2; nx++) {
    uv0[nx] = (unsigned int)a->size[nx];
  }

  emxInit_real_T1(&ztemp, 2);
  nx = ztemp->size[0] * ztemp->size[1];
  ztemp->size[0] = (int)uv0[0];
  ztemp->size[1] = (int)uv0[1];
  emxEnsureCapacity_real_T(ztemp, nx);
  nx = y->size[0] * y->size[1];
  y->size[0] = (int)uv0[0];
  y->size[1] = (int)uv0[1];
  emxEnsureCapacity_real_T(y, nx);
  nx = ztemp->size[0] * ztemp->size[1];
  k = 0;
  emxFree_real_T(&ztemp);
  while (k + 1 <= nx) {
    y->data[k] = a->data[k] * a->data[k];
    k++;
  }
}

//
// Arguments    : double u
// Return Type  : double
//
static double rt_roundd(double u)
{
  double y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

//
// Arguments    : const emxArray_boolean_T *x
// Return Type  : double
//
static double sum(const emxArray_boolean_T *x)
{
  double y;
  int k;
  if (x->size[0] == 0) {
    y = 0.0;
  } else {
    y = x->data[0];
    for (k = 2; k <= x->size[0]; k++) {
      y += (double)x->data[k - 1];
    }
  }

  return y;
}

//
// 提取圆形目标并拟合
// Arguments    : const emxArray_uint8_T *img
//                double rx
//                double ry
//                double radius
//                double ratio
//                double *objX
//                double *objY
//                double *objRadius
//                double *flag
// Return Type  : void
//
void fitCircle(const emxArray_uint8_T *img, double rx, double ry, double radius,
               double ratio, double *objX, double *objY, double *objRadius,
               double *flag)
{
  emxArray_boolean_T *bw;
  emxArray_real_T *xCoords;
  emxArray_real_T *yCoords;
  int cols;
  int rows;
  emxArray_real_T *range;
  emxArray_real_T *numPixel;
  emxArray_real_T *xRing;
  emxArray_boolean_T *tmp;
  emxArray_real_T *yCircle;
  emxArray_real_T *yCircleIn;
  emxArray_int32_T *r0;
  emxArray_real_T *b_img;
  emxArray_boolean_T *b_bw;
  boolean_T guard1 = false;
  int i0;
  int loop_ub;
  unsigned int img_idx_0;
  double level;
  double b_level;
  double x;
  int i;
  emxInit_boolean_T(&bw, 2);
  emxInit_real_T(&xCoords, 1);
  emxInit_real_T(&yCoords, 1);

  // flag为0表示拟合失败，返回默认值
  *objX = rx;
  *objY = ry;
  *objRadius = radius;
  *flag = 0.0;
  cols = img->size[1];
  rows = img->size[0];
  imGradBw(img, radius, bw);

  // 目标坐标
  ExtractObject(bw, rx, ry, radius, ratio, xCoords, yCoords);
  emxInit_real_T1(&range, 2);
  emxInit_real_T(&numPixel, 1);
  emxInit_real_T(&xRing, 1);
  emxInit_boolean_T1(&tmp, 1);
  emxInit_real_T(&yCircle, 1);
  emxInit_real_T(&yCircleIn, 1);
  emxInit_int32_T(&r0, 1);
  emxInit_real_T1(&b_img, 2);
  emxInit_boolean_T(&b_bw, 2);
  guard1 = false;
  if (xCoords->size[0] == 0) {
    i0 = b_img->size[0] * b_img->size[1];
    b_img->size[0] = img->size[0];
    b_img->size[1] = img->size[1];
    emxEnsureCapacity_real_T(b_img, i0);
    loop_ub = img->size[0] * img->size[1];
    for (i0 = 0; i0 < loop_ub; i0++) {
      b_img->data[i0] = (double)img->data[i0] / 255.0;
    }

    imfilter(b_img);
    level = otsuGray(b_img);
    imbinarize(b_img, level, bw);
    imerode(bw, b_bw);
    imdilate(b_bw, bw);
    level = 3.1415926535897931 * radius * radius / 3.0;
    imfill(bw);
    i0 = b_bw->size[0] * b_bw->size[1];
    b_bw->size[0] = bw->size[0];
    b_bw->size[1] = bw->size[1];
    emxEnsureCapacity_boolean_T(b_bw, i0);
    loop_ub = bw->size[0] * bw->size[1];
    for (i0 = 0; i0 < loop_ub; i0++) {
      b_bw->data[i0] = bw->data[i0];
    }

    if (level < 0.0) {
      b_level = ceil(level);
    } else {
      b_level = floor(level);
    }

    bwareaopen(b_bw, b_level, bw);
    ExtractObject(bw, rx, ry, radius, ratio, xCoords, yCoords);
    if (xCoords->size[0] == 0) {
    } else {
      guard1 = true;
    }
  } else {
    guard1 = true;
  }

  if (guard1) {
    // 目标中心
    *objX = rt_roundd(combineVectorElements(xCoords) / (double)xCoords->size[0]);
    *objY = rt_roundd(combineVectorElements(yCoords) / (double)yCoords->size[0]);

    // 目标二值图
    img_idx_0 = (unsigned int)img->size[0];
    i0 = b_img->size[0] * b_img->size[1];
    b_img->size[0] = img->size[0];
    b_img->size[1] = img->size[1];
    emxEnsureCapacity_real_T(b_img, i0);
    loop_ub = img->size[0] * img->size[1];
    for (i0 = 0; i0 < loop_ub; i0++) {
      b_img->data[i0] = 0.0;
    }

    i0 = r0->size[0];
    r0->size[0] = yCoords->size[0];
    emxEnsureCapacity_int32_T(r0, i0);
    loop_ub = yCoords->size[0];
    for (i0 = 0; i0 < loop_ub; i0++) {
      r0->data[i0] = (int)yCoords->data[i0] + (int)img_idx_0 * ((int)
        xCoords->data[i0] - 1);
    }

    loop_ub = r0->size[0];
    for (i0 = 0; i0 < loop_ub; i0++) {
      b_img->data[r0->data[i0] - 1] = 1.0;
    }

    level = floor(radius * 0.5);
    x = floor(radius * 1.5);
    if (x < level) {
      i0 = range->size[0] * range->size[1];
      range->size[0] = 1;
      range->size[1] = 0;
      emxEnsureCapacity_real_T(range, i0);
    } else {
      i0 = range->size[0] * range->size[1];
      range->size[0] = 1;
      range->size[1] = (int)(x - level) + 1;
      emxEnsureCapacity_real_T(range, i0);
      loop_ub = (int)(x - level);
      for (i0 = 0; i0 <= loop_ub; i0++) {
        range->data[range->size[0] * i0] = level + (double)i0;
      }
    }

    i0 = numPixel->size[0];
    numPixel->size[0] = range->size[1];
    emxEnsureCapacity_real_T1(numPixel, i0);
    for (i = 0; i < range->size[1]; i++) {
      // 计算圆环的坐标
      PointsOnCircle(*objX, *objY, range->data[i], xCoords, yCircle);
      PointsOnCircle(*objX, *objY, range->data[i] - 1.0, yCoords, yCircleIn);
      i0 = xRing->size[0];
      xRing->size[0] = xCoords->size[0] + yCoords->size[0];
      emxEnsureCapacity_real_T1(xRing, i0);
      loop_ub = xCoords->size[0];
      for (i0 = 0; i0 < loop_ub; i0++) {
        xRing->data[i0] = xCoords->data[i0];
      }

      loop_ub = yCoords->size[0];
      for (i0 = 0; i0 < loop_ub; i0++) {
        xRing->data[i0 + xCoords->size[0]] = yCoords->data[i0];
      }

      i0 = xCoords->size[0];
      xCoords->size[0] = yCircle->size[0] + yCircleIn->size[0];
      emxEnsureCapacity_real_T1(xCoords, i0);
      loop_ub = yCircle->size[0];
      for (i0 = 0; i0 < loop_ub; i0++) {
        xCoords->data[i0] = yCircle->data[i0];
      }

      loop_ub = yCircleIn->size[0];
      for (i0 = 0; i0 < loop_ub; i0++) {
        xCoords->data[i0 + yCircle->size[0]] = yCircleIn->data[i0];
      }

      // 去掉可能超过图像边界的部分
      i0 = tmp->size[0];
      tmp->size[0] = xRing->size[0];
      emxEnsureCapacity_boolean_T1(tmp, i0);
      loop_ub = xRing->size[0];
      for (i0 = 0; i0 < loop_ub; i0++) {
        tmp->data[i0] = ((xRing->data[i0] < 1.0) || (xRing->data[i0] > cols) ||
                         (xCoords->data[i0] < 1.0) || (xCoords->data[i0] > rows));
      }

      nullAssignment(xRing, tmp);
      nullAssignment(xCoords, tmp);
      i0 = yCoords->size[0];
      yCoords->size[0] = xCoords->size[0];
      emxEnsureCapacity_real_T1(yCoords, i0);
      loop_ub = xCoords->size[0];
      for (i0 = 0; i0 < loop_ub; i0++) {
        yCoords->data[i0] = (signed char)b_img->data[((int)xCoords->data[i0] +
          rows * ((int)xRing->data[i0] - 1)) - 1];
      }

      numPixel->data[i] = combineVectorElements(yCoords);
    }

    level = numPixel->data[0];
    cols = 0;
    if (numPixel->size[0] > 1) {
      for (rows = 1; rows + 1 <= numPixel->size[0]; rows++) {
        if (numPixel->data[rows] > level) {
          level = numPixel->data[rows];
          cols = rows;
        }
      }
    }

    if (!(level == 0.0)) {
      *objRadius = range->data[cols];
      *flag = 1.0;
    }
  }

  emxFree_boolean_T(&b_bw);
  emxFree_real_T(&b_img);
  emxFree_int32_T(&r0);
  emxFree_real_T(&yCircleIn);
  emxFree_real_T(&yCircle);
  emxFree_boolean_T(&tmp);
  emxFree_real_T(&xRing);
  emxFree_real_T(&numPixel);
  emxFree_real_T(&range);
  emxFree_real_T(&yCoords);
  emxFree_real_T(&xCoords);
  emxFree_boolean_T(&bw);
}

//
// Arguments    : void
// Return Type  : void
//
void fitCircle_initialize()
{
}

//
// Arguments    : void
// Return Type  : void
//
void fitCircle_terminate()
{
  // (no terminate code required)
}

//
// File trailer for fitCircle.cpp
//
// [EOF]
//
