#ifndef DETECTCHECKERBOARDPOINTSONIMAGE_TYPES_H
#define DETECTCHECKERBOARDPOINTSONIMAGE_TYPES_H

#include "rtwtypes.h"


template<typename T>
struct arrayBase
{
	T *data;
	int *size;
	int allocatedSize;
	int numDimensions;
	boolean_T canFreeData;
};

typedef arrayBase<float> emxArray_real32_T;
typedef arrayBase<double> emxArray_real_T;
typedef arrayBase<unsigned char> emxArray_uint8_T;
typedef arrayBase<void> emxArray__common;
typedef arrayBase<int> emxArray_int32_T;
typedef arrayBase<boolean_T> emxArray_boolean_T;



typedef struct {
	boolean_T isValid;
	float Energy;
	emxArray_real_T *BoardCoords;
	emxArray_real_T *BoardIdx;
	emxArray_real32_T *Points;
	boolean_T IsDirectionBad[4];
	double LastExpandDirection;
	float PreviousEnergy;
} CheckerboardImage;

#endif