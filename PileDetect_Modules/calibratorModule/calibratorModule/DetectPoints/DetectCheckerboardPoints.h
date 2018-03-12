#pragma once
#include <algorithm>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "omp.h"
#include "emxTypes.h"

// Variable Declarations
extern omp_nest_lock_t emlrtNestLockGlobal;
//const double EnergyINF = 1.0E+10;

static double X[150];


// Function Declarations
void detectCheckerboardPointsOnImage(const emxArray_uint8_T *I, double sigma,
  double minCornerMetric, emxArray_real_T *points, double boardSize[2]);



void detectCheckerboardInitialize();
void CheckerboardPointsTerminate();
emxArray_uint8_T *emxCreateND_uint8_T(int numDimensions, int *size);

void emxEnsureCapacity(emxArray__common *emxArray, int oldNumel, int elementSize);
void emxFreeStruct_CheckerboardImage(CheckerboardImage *pStruct);
void emxFree_boolean_T(emxArray_boolean_T **pEmxArray);
void emxFree_int32_T(emxArray_int32_T **pEmxArray);
void emxFree_real32_T(emxArray_real32_T **pEmxArray);
void emxFree_real_T(emxArray_real_T **pEmxArray);
void emxFree_uint8_T(emxArray_uint8_T **pEmxArray);
void emxInitStruct_CheckerboardImage(CheckerboardImage *pStruct);
void emxInit_boolean_T(emxArray_boolean_T **pEmxArray, int numDimensions);

void emxInit_int32_T(emxArray_int32_T **pEmxArray, int numDimensions);
void emxInit_real32_T(emxArray_real32_T **pEmxArray, int numDimensions);
void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);
void emxInit_uint8_T(emxArray_uint8_T **pEmxArray, int numDimensions);


// Function Declarations
void CheckerboardImageExpandBoardUp(const CheckerboardImage *b_this,
	const emxArray_real_T *indices, emxArray_real_T *newBoard, emxArray_real_T
	*newBoardCoords);
double CheckerboardImageFindNeighbor(const CheckerboardImage *b_this,
	const emxArray_real32_T *pointVectors, const emxArray_real32_T *euclideanDists,
	const float v[2]);
void CheckerboardImageInitialize(CheckerboardImage *b_this, double
	seedIdx, const emxArray_real32_T *points, const float v1[2], const float v2[2]);

CheckerboardImage *c_CheckerboardImage_Checkerboar(CheckerboardImage *b_this);
float CheckerboardImageComputeNewE1(const CheckerboardImage *b_this,
	float oldEnergy);
void CheckerboardImageExpandBoard1(CheckerboardImage *b_this, double direction);

float CheckerboardImageComputeNewE2(const CheckerboardImage *b_this,
	const emxArray_real_T *idx, float oldEnergy);
void CheckerboardImageExpandBoard2(const CheckerboardImage *b_this,
	const emxArray_real_T *indices, emxArray_real_T *newBoard, emxArray_real_T
	*newBoardCoords);
void CheckerboardImageFindClosest(const CheckerboardImage *b_this,
	const emxArray_real_T *predictedPoints, emxArray_real_T *indices);
void CheckerboardImagePredictPoint(const CheckerboardImage *b_this,
	emxArray_real_T *newPoints);



void c_hypot(const emxArray_real_T *x, const emxArray_real_T *y,
	emxArray_real_T *r);
void b_imfilter(emxArray_real32_T *varargin_1);
void c_imfilter(emxArray_real32_T *varargin_1);
void d_imfilter(emxArray_real32_T *varargin_1);
void imfilter(emxArray_real32_T *varargin_1, const emxArray_real_T 	*varargin_2);


void c_xaxpy(int n, double a, const emxArray_real_T *x, int ix0,
	emxArray_real_T *y, int iy0);
void cat(const emxArray_real_T *varargin_1, const emxArray_real_T
	*varargin_2, emxArray_real_T *y);


void detectCheckerboardOnImage(const emxArray_real32_T *I, double sigma,
	double peakThreshold, emxArray_real_T *points, double boardSize[2]);

float CheckerboardImageComputeNewE3(const CheckerboardImage *b_this,float oldEnergy);

void subPixelLocation(const emxArray_real32_T *metric, emxArray_real_T 	*loc);
void subPixelLocationImpl_init();

void toPoints(const CheckerboardImage *b_this, emxArray_real_T *points,	double boardSize[2]);
void imgFindPeaks(const emxArray_real32_T *metric, double quality,
	emxArray_real32_T *loc);
void fspecial(double varargin_2, double varargin_3, emxArray_real_T *h);
CheckerboardImage *growCheckerboard(const emxArray_real32_T *points,
	const emxArray_real32_T *scores, const emxArray_real32_T *Ix2, const
	emxArray_real32_T *Iy2, const emxArray_real32_T *Ixy, double theta,
	CheckerboardImage *iobj_0, CheckerboardImage *iobj_1, CheckerboardImage
	*iobj_2);
CheckerboardImage *orient(CheckerboardImage *board, const
	emxArray_real32_T *I);
CheckerboardImage *rot90_checkerboard(CheckerboardImage *board);



float mean(const emxArray_real32_T *x);
void merge(emxArray_int32_T *idx, emxArray_real32_T *x, int offset, int
	np, int nq, emxArray_int32_T *iwork, emxArray_real32_T *xwork);
void merge_block(emxArray_int32_T *idx, emxArray_real32_T *x, int offset,
	int n, int preSortLevel, emxArray_int32_T *iwork, emxArray_real32_T *xwork);
double norm(const emxArray_real_T *x);

void padImage(const emxArray_real32_T *a_tmp, const double pad[2],
	emxArray_real32_T *a);
void padarray(const emxArray_real32_T *varargin_1, const double
	varargin_2[2], emxArray_real32_T *b);
void poly2RectMask(double b_X[4], double Y[4], double height, double
	width, emxArray_boolean_T *mask);
void power(const emxArray_real32_T *a, emxArray_real32_T *y);
void rdivide(const emxArray_real_T *x, const emxArray_real_T *y,
	emxArray_real_T *z);
void repmat(const double varargin_1[2], emxArray_real32_T *b);
void rot90(const emxArray_real_T *A, emxArray_real_T *B);
double rt_hypotd(double u0, double u1);
float rt_hypotf(float u0, float u1);
double rt_roundd(double u);
float rt_roundf(float u);
void sort(emxArray_real32_T *x, emxArray_int32_T *idx);
void sortIdx(emxArray_real32_T *x, emxArray_int32_T *idx);
void squeeze(const emxArray_real_T *a, emxArray_real_T *b);
void xaxpy(int n, double a, int ix0, emxArray_real_T *y, int iy0);
double xdotc(int n, const emxArray_real_T *x, int ix0, const
	emxArray_real_T *y, int iy0);
double xnrm2(int n, const emxArray_real_T *x, int ix0);
void xrot(int n, emxArray_real_T *x, int ix0, int iy0, double c, double s);
void xrotg(double *a, double *b, double *c, double *s);
void xscal(int n, double a, emxArray_real_T *x, int ix0);
void xswap(int n, emxArray_real_T *x, int ix0, int iy0);

void diag(const emxArray_real_T *v, emxArray_real_T *d);
int div_s32_floor(int numerator, int denominator);

boolean_T all(const emxArray_boolean_T *x);
boolean_T any(const emxArray_boolean_T *x);
void b_abs(const emxArray_real32_T *x, emxArray_real32_T *y);
void b_hypot(const emxArray_real32_T *x, const emxArray_real32_T *y,
	emxArray_real32_T *r);

void b_padarray(const emxArray_real32_T *varargin_1, const double
	varargin_2[2], emxArray_real32_T *b);
void b_rot90(const emxArray_real_T *A, emxArray_real_T *B);
void b_sort(emxArray_real32_T *x, int dim, emxArray_int32_T *idx);
void b_squeeze(const emxArray_real_T *a, emxArray_real_T *b);
void svd(const emxArray_real_T *A, emxArray_real_T *U, emxArray_real_T *
	S, emxArray_real_T *V);
void b_xaxpy(int n, double a, const emxArray_real_T *x, int ix0,
	emxArray_real_T *y, int iy0);
double b_xnrm2(int n, const emxArray_real_T *x, int ix0);
void bwlookup(const emxArray_boolean_T *bwin, emxArray_boolean_T *B);
void bwmorphApplyOnce(emxArray_boolean_T *bw);