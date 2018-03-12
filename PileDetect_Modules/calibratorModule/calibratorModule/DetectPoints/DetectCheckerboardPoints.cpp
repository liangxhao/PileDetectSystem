#include "stdafx.h"
#include "DetectCheckerboardPoints.h"
#include "libmwbwlookup_tbb.h"
#include "libmwimfilter.h"
#include "libmwimregionalmax.h"
#include "libmwippfilter.h"

omp_nest_lock_t emlrtNestLockGlobal;



//
// Arguments    : const CheckerboardImage *b_this
//                const emxArray_real_T *indices
//                emxArray_real_T *newBoard
//                emxArray_real_T *newBoardCoords
// Return Type  : void
//
void CheckerboardImageExpandBoardUp(const CheckerboardImage *b_this,
	const emxArray_real_T *indices, emxArray_real_T *newBoard, emxArray_real_T
	*newBoardCoords)
{
	int i25;
	int loop_ub;
	int i26;
	int unnamed_idx_2;
	int i27;
	emxArray_int32_T *r20;
	emxArray_int32_T *r21;
	emxArray_real_T *r22;
	int b_loop_ub;
	int i28;

	// 'CheckerboardImage:382' newBoard = zeros(size(this.BoardIdx, 1)+1, size(this.BoardIdx, 2)); 
	i25 = newBoard->size[0] * newBoard->size[1];
	newBoard->size[0] = b_this->BoardIdx->size[0] + 1;
	newBoard->size[1] = b_this->BoardIdx->size[1];
	emxEnsureCapacity((emxArray__common *)newBoard, i25, (int)sizeof(double));
	loop_ub = (b_this->BoardIdx->size[0] + 1) * b_this->BoardIdx->size[1];
	for (i25 = 0; i25 < loop_ub; i25++) {
		newBoard->data[i25] = 0.0;
	}

	// 'CheckerboardImage:383' newBoard(1, :) = indices;
	loop_ub = indices->size[1];
	for (i25 = 0; i25 < loop_ub; i25++) {
		newBoard->data[newBoard->size[0] * i25] = indices->data[indices->size[0] *
			i25];
	}

	// 'CheckerboardImage:384' newBoard(2:end, :) = this.BoardIdx;
	if (2 > newBoard->size[0]) {
		i25 = 0;
	}
	else {
		i25 = 1;
	}

	loop_ub = b_this->BoardIdx->size[1];
	for (i26 = 0; i26 < loop_ub; i26++) {
		unnamed_idx_2 = b_this->BoardIdx->size[0];
		for (i27 = 0; i27 < unnamed_idx_2; i27++) {
			newBoard->data[(i25 + i27) + newBoard->size[0] * i26] = b_this->
				BoardIdx->data[i27 + b_this->BoardIdx->size[0] * i26];
		}
	}

	// 'CheckerboardImage:386' newBoardCoords = zeros(size(this.BoardCoords, 1)+1, ... 
	// 'CheckerboardImage:387'                 size(this.BoardCoords, 2), size(this.BoardCoords, 3)); 
	i25 = newBoardCoords->size[0] * newBoardCoords->size[1] * newBoardCoords->
		size[2];
	newBoardCoords->size[0] = b_this->BoardCoords->size[0] + 1;
	newBoardCoords->size[1] = b_this->BoardCoords->size[1];
	newBoardCoords->size[2] = b_this->BoardCoords->size[2];
	emxEnsureCapacity((emxArray__common *)newBoardCoords, i25, (int)sizeof(double));
	loop_ub = (b_this->BoardCoords->size[0] + 1) * b_this->BoardCoords->size[1] *
		b_this->BoardCoords->size[2];
	for (i25 = 0; i25 < loop_ub; i25++) {
		newBoardCoords->data[i25] = 0.0;
	}

	emxInit_int32_T(&r20, 1);

	// 'CheckerboardImage:388' newBoardCoords(1, :, :) = this.Points(indices, :);
	loop_ub = newBoardCoords->size[1];
	i25 = r20->size[0];
	r20->size[0] = loop_ub;
	emxEnsureCapacity((emxArray__common *)r20, i25, (int)sizeof(int));
	for (i25 = 0; i25 < loop_ub; i25++) {
		r20->data[i25] = i25;
	}

	emxInit_int32_T(&r21, 1);
	loop_ub = newBoardCoords->size[2];
	i25 = r21->size[0];
	r21->size[0] = loop_ub;
	emxEnsureCapacity((emxArray__common *)r21, i25, (int)sizeof(int));
	for (i25 = 0; i25 < loop_ub; i25++) {
		r21->data[i25] = i25;
	}

	emxInit_real_T(&r22, 2);
	i25 = r22->size[0] * r22->size[1];
	r22->size[0] = indices->size[1];
	r22->size[1] = 2;
	emxEnsureCapacity((emxArray__common *)r22, i25, (int)sizeof(double));
	for (i25 = 0; i25 < 2; i25++) {
		loop_ub = indices->size[1];
		for (i26 = 0; i26 < loop_ub; i26++) {
			r22->data[i26 + r22->size[0] * i25] = b_this->Points->data[((int)
				indices->data[indices->size[0] * i26] + b_this->Points->size[0] * i25) -
				1];
		}
	}

	loop_ub = r20->size[0];
	unnamed_idx_2 = r21->size[0];
	for (i25 = 0; i25 < unnamed_idx_2; i25++) {
		for (i26 = 0; i26 < loop_ub; i26++) {
			newBoardCoords->data[newBoardCoords->size[0] * r20->data[i26] +
				newBoardCoords->size[0] * newBoardCoords->size[1] * r21->data[i25]] =
				r22->data[i26 + loop_ub * i25];
		}
	}

	emxFree_real_T(&r22);
	emxFree_int32_T(&r21);
	emxFree_int32_T(&r20);

	// 'CheckerboardImage:389' newBoardCoords(2:end, :, :) = this.BoardCoords;
	if (2 > newBoardCoords->size[0]) {
		i25 = 0;
	}
	else {
		i25 = 1;
	}

	loop_ub = b_this->BoardCoords->size[2];
	for (i26 = 0; i26 < loop_ub; i26++) {
		unnamed_idx_2 = b_this->BoardCoords->size[1];
		for (i27 = 0; i27 < unnamed_idx_2; i27++) {
			b_loop_ub = b_this->BoardCoords->size[0];
			for (i28 = 0; i28 < b_loop_ub; i28++) {
				newBoardCoords->data[((i25 + i28) + newBoardCoords->size[0] * i27) +
					newBoardCoords->size[0] * newBoardCoords->size[1] * i26] =
					b_this->BoardCoords->data[(i28 + b_this->BoardCoords->size[0] * i27) +
					b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * i26];
			}
		}
	}
}

//
// Arguments    : const CheckerboardImage *b_this
//                const emxArray_real32_T *pointVectors
//                const emxArray_real32_T *euclideanDists
//                const float v[2]
// Return Type  : double
//
double CheckerboardImageFindNeighbor(const CheckerboardImage *b_this,
	const emxArray_real32_T *pointVectors, const emxArray_real32_T *euclideanDists,
	const float v[2])
{
	double neighborIdx;
	emxArray_real32_T *angleCosines;
	unsigned int pointVectors_idx_0;
	int ic;
	int m;
	int cr;
	float mtmp;
	int br;
	int ar;
	emxArray_real32_T *dists;
	int ia;
	emxArray_boolean_T *r6;
	emxArray_int32_T *r7;
	emxArray_int32_T *r8;
	emxInit_real32_T(&angleCosines, 1);

	// 'CheckerboardImage:206' angleCosines = pointVectors * v' ./ (euclideanDists * hypot(v(1), v(2))); 
	pointVectors_idx_0 = (unsigned int)pointVectors->size[0];
	ic = angleCosines->size[0];
	angleCosines->size[0] = (int)pointVectors_idx_0;
	emxEnsureCapacity((emxArray__common *)angleCosines, ic, (int)sizeof(float));
	m = pointVectors->size[0];
	cr = angleCosines->size[0];
	ic = angleCosines->size[0];
	angleCosines->size[0] = cr;
	emxEnsureCapacity((emxArray__common *)angleCosines, ic, (int)sizeof(float));
	for (ic = 0; ic < cr; ic++) {
		angleCosines->data[ic] = 0.0F;
	}

	if (pointVectors->size[0] == 0) {
	}
	else {
		cr = 0;
		while ((m > 0) && (cr <= 0)) {
			for (ic = 1; ic <= m; ic++) {
				angleCosines->data[ic - 1] = 0.0F;
			}

			cr = m;
		}

		br = 0;
		cr = 0;
		while ((m > 0) && (cr <= 0)) {
			ar = 0;
			for (cr = br; cr + 1 <= br + 2; cr++) {
				if (v[cr] != 0.0F) {
					ia = ar;
					for (ic = 0; ic + 1 <= m; ic++) {
						ia++;
						angleCosines->data[ic] += v[cr] * pointVectors->data[ia - 1];
					}
				}

				ar += m;
			}

			br += 2;
			cr = m;
		}
	}

	mtmp = rt_hypotf(v[0], v[1]);
	ic = angleCosines->size[0];
	emxEnsureCapacity((emxArray__common *)angleCosines, ic, (int)sizeof(float));
	cr = angleCosines->size[0];
	for (ic = 0; ic < cr; ic++) {
		angleCosines->data[ic] /= euclideanDists->data[ic] * mtmp;
	}

	emxInit_real32_T(&dists, 1);

	// 'CheckerboardImage:210' dists = euclideanDists + 1.5 * euclideanDists .* (1 - angleCosines); 
	ic = dists->size[0];
	dists->size[0] = euclideanDists->size[0];
	emxEnsureCapacity((emxArray__common *)dists, ic, (int)sizeof(float));
	cr = euclideanDists->size[0];
	for (ic = 0; ic < cr; ic++) {
		dists->data[ic] = euclideanDists->data[ic] + 1.5F * euclideanDists->data[ic]
			* (1.0F - angleCosines->data[ic]);
	}

	emxInit_boolean_T(&r6, 2);

	// 'CheckerboardImage:213' dists(this.BoardIdx(this.BoardIdx > 0)) = 10e10;
	ic = r6->size[0] * r6->size[1];
	r6->size[0] = b_this->BoardIdx->size[0];
	r6->size[1] = b_this->BoardIdx->size[1];
	emxEnsureCapacity((emxArray__common *)r6, ic, (int)sizeof(boolean_T));
	cr = b_this->BoardIdx->size[0] * b_this->BoardIdx->size[1];
	for (ic = 0; ic < cr; ic++) {
		r6->data[ic] = (b_this->BoardIdx->data[ic] > 0.0);
	}

	emxInit_int32_T(&r7, 1);
	ia = r6->size[0] * r6->size[1] - 1;
	ar = 0;
	for (cr = 0; cr <= ia; cr++) {
		if (r6->data[cr]) {
			ar++;
		}
	}

	ic = r7->size[0];
	r7->size[0] = ar;
	emxEnsureCapacity((emxArray__common *)r7, ic, (int)sizeof(int));
	ar = 0;
	for (cr = 0; cr <= ia; cr++) {
		if (r6->data[cr]) {
			r7->data[ar] = cr + 1;
			ar++;
		}
	}

	emxFree_boolean_T(&r6);
	emxInit_int32_T(&r8, 1);
	ic = r8->size[0];
	r8->size[0] = r7->size[0];
	emxEnsureCapacity((emxArray__common *)r8, ic, (int)sizeof(int));
	cr = r7->size[0];
	for (ic = 0; ic < cr; ic++) {
		r8->data[ic] = (int)b_this->BoardIdx->data[r7->data[ic] - 1];
	}

	emxFree_int32_T(&r7);
	cr = r8->size[0];
	for (ic = 0; ic < cr; ic++) {
		dists->data[r8->data[ic] - 1] = 1.0E+11F;
	}

	emxFree_int32_T(&r8);

	// 'CheckerboardImage:216' dists(angleCosines < 0) = 10e10;
	ia = angleCosines->size[0];
	for (cr = 0; cr < ia; cr++) {
		if (angleCosines->data[cr] < 0.0F) {
			dists->data[cr] = 1.0E+11F;
		}
	}

	emxFree_real32_T(&angleCosines);

	// 'CheckerboardImage:219' [dirDist, neighborIdx] = min(dists);
	mtmp = dists->data[0];
	cr = 0;
	if (dists->size[0] > 1) {
		for (ar = 1; ar + 1 <= dists->size[0]; ar++) {
			if (dists->data[ar] < mtmp) {
				mtmp = dists->data[ar];
				cr = ar;
			}
		}
	}

	emxFree_real32_T(&dists);
	neighborIdx = cr + 1;

	// 'CheckerboardImage:220' if dirDist==10e10
	if (mtmp == 1.0E+11) {
		// 'CheckerboardImage:221' neighborIdx = -1;
		neighborIdx = -1.0;
	}

	return neighborIdx;
}

//
// Constructor. Creates a 4x4 checkerboard with the seed point
//  in the center. points is an Mx2 matrix of x,y coordinates of
//  all possible checkerboard corners. seedIdx is the index of
//  the seed point. The coordinates of the seed point are
//  points(seedIdx, :). e1 and e2 are 2-element vectors
//  specifying the edge orientations at the seed point.
// Arguments    : CheckerboardImage *b_this
//                double seedIdx
//                const emxArray_real32_T *points
//                const float v1[2]
//                const float v2[2]
// Return Type  : void
//
void CheckerboardImageInitialize(CheckerboardImage *b_this, double
	seedIdx, const emxArray_real32_T *points, const float v1[2], const float v2[2])
{
	int i15;
	int csz_idx_0;
	float center[2];
	emxArray_int32_T *r5;
	double b_center[2];
	emxArray_real32_T *a;
	emxArray_real32_T *pointVectors;
	int na1;
	emxArray_real32_T *av;
	emxArray_real32_T *b_pointVectors;
	int ak;
	int bk;
	int nc1;
	int ck;
	emxArray_real32_T *cv;
	emxArray_real32_T *c_pointVectors;
	int i16;
	float b_v1[2];
	emxArray_boolean_T *c_this;
	float r[2];
	float l[2];
	float d[2];
	float u[2];
	float b_u;
	float b_l;
	emxArray_boolean_T *d_this;
	emxArray_boolean_T *e_this;
	CheckerboardImage *f_this;
	float e;
	float col1[6];
	float row3[6];
	float col2[6];
	float b_col2;
	float b_r[3];
	float mtmp;
	float boardSize;

	// 'up'
	// 'CheckerboardImage:34' this.BoardIdx = 0;
	i15 = b_this->BoardIdx->size[0] * b_this->BoardIdx->size[1];
	b_this->BoardIdx->size[0] = 1;
	b_this->BoardIdx->size[1] = 1;
	emxEnsureCapacity((emxArray__common *)b_this->BoardIdx, i15, (int)sizeof
	(double));
	b_this->BoardIdx->data[0] = 0.0;

	// 'CheckerboardImage:35' this.BoardIdx = zeros(3, 3);
	i15 = b_this->BoardIdx->size[0] * b_this->BoardIdx->size[1];
	b_this->BoardIdx->size[0] = 3;
	b_this->BoardIdx->size[1] = 3;
	emxEnsureCapacity((emxArray__common *)b_this->BoardIdx, i15, (int)sizeof
	(double));
	for (i15 = 0; i15 < 9; i15++) {
		b_this->BoardIdx->data[i15] = 0.0;
	}

	// 'CheckerboardImage:36' this.IsDirectionBad = false(1, 4);
	for (i15 = 0; i15 < 4; i15++) {
		b_this->IsDirectionBad[i15] = false;
	}

	// 'CheckerboardImage:37' this.BoardCoords = 0;
	i15 = b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] *
		b_this->BoardCoords->size[2];
	b_this->BoardCoords->size[0] = 1;
	b_this->BoardCoords->size[1] = 1;
	b_this->BoardCoords->size[2] = 1;
	emxEnsureCapacity((emxArray__common *)b_this->BoardCoords, i15, (int)sizeof
	(double));
	b_this->BoardCoords->data[0] = 0.0;

	// 'CheckerboardImage:38' this.BoardCoords = zeros(3, 3, 2);
	i15 = b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] *
		b_this->BoardCoords->size[2];
	b_this->BoardCoords->size[0] = 3;
	b_this->BoardCoords->size[1] = 3;
	b_this->BoardCoords->size[2] = 2;
	emxEnsureCapacity((emxArray__common *)b_this->BoardCoords, i15, (int)sizeof
	(double));
	for (i15 = 0; i15 < 18; i15++) {
		b_this->BoardCoords->data[i15] = 0.0;
	}

	// 'CheckerboardImage:39' this.Points = points;
	i15 = b_this->Points->size[0] * b_this->Points->size[1];
	b_this->Points->size[0] = points->size[0];
	b_this->Points->size[1] = 2;
	emxEnsureCapacity((emxArray__common *)b_this->Points, i15, (int)sizeof(float));
	csz_idx_0 = points->size[0] * points->size[1];
	for (i15 = 0; i15 < csz_idx_0; i15++) {
		b_this->Points->data[i15] = points->data[i15];
	}

	// 'CheckerboardImage:40' center = this.Points(seedIdx, :);
	for (i15 = 0; i15 < 2; i15++) {
		center[i15] = b_this->Points->data[((int)seedIdx + b_this->Points->size[0] *
			i15) - 1];
	}

	emxInit_int32_T(&r5, 1);

	// 'CheckerboardImage:41' this.BoardIdx(2, 2) = seedIdx;
	b_this->BoardIdx->data[1 + b_this->BoardIdx->size[0]] = seedIdx;

	// 'CheckerboardImage:42' this.BoardCoords(2, 2, :) = center;
	csz_idx_0 = b_this->BoardCoords->size[2];
	i15 = r5->size[0];
	r5->size[0] = csz_idx_0;
	emxEnsureCapacity((emxArray__common *)r5, i15, (int)sizeof(int));
	for (i15 = 0; i15 < csz_idx_0; i15++) {
		r5->data[i15] = i15;
	}

	for (i15 = 0; i15 < 2; i15++) {
		b_center[i15] = center[i15];
	}

	csz_idx_0 = r5->size[0];
	for (i15 = 0; i15 < csz_idx_0; i15++) {
		b_this->BoardCoords->data[(b_this->BoardCoords->size[0] +
			b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * r5->data[i15])
			+ 1] = b_center[i15];
	}

	emxInit_real32_T(&a, 2);

	// 'CheckerboardImage:43' this.LastExpandDirection = 1;
	b_this->LastExpandDirection = 1.0;

	// 'up';
	// 'CheckerboardImage:44' this.PreviousEnergy = single(10e10);
	b_this->PreviousEnergy = 1.0E+11F;

	// 'CheckerboardImage:45' this.isValid = false;
	b_this->isValid = false;

	//  compute distances from all the points to the center
	// 'CheckerboardImage:48' pointVectors = bsxfun(@minus, this.Points, center);
	i15 = a->size[0] * a->size[1];
	a->size[0] = b_this->Points->size[0];
	a->size[1] = 2;
	emxEnsureCapacity((emxArray__common *)a, i15, (int)sizeof(float));
	csz_idx_0 = b_this->Points->size[0] * b_this->Points->size[1];
	for (i15 = 0; i15 < csz_idx_0; i15++) {
		a->data[i15] = b_this->Points->data[i15];
	}

	emxInit_real32_T(&pointVectors, 2);
	na1 = a->size[0];
	csz_idx_0 = a->size[0];
	i15 = pointVectors->size[0] * pointVectors->size[1];
	pointVectors->size[0] = csz_idx_0;
	pointVectors->size[1] = 2;
	emxEnsureCapacity((emxArray__common *)pointVectors, i15, (int)sizeof(float));
	emxInit_real32_T(&av, 1);
	if (pointVectors->size[0] == 0) {
	}
	else {
		csz_idx_0 = a->size[0];
		i15 = av->size[0];
		av->size[0] = csz_idx_0;
		emxEnsureCapacity((emxArray__common *)av, i15, (int)sizeof(float));
		ak = -1;
		bk = 0;
		nc1 = pointVectors->size[0];
		i15 = (pointVectors->size[0] << 1) - pointVectors->size[0];
		ck = 0;
		emxInit_real32_T(&cv, 1);
		while (ck <= i15) {
			for (csz_idx_0 = 1; csz_idx_0 <= na1; csz_idx_0++) {
				av->data[csz_idx_0 - 1] = a->data[ak + csz_idx_0];
			}

			i16 = cv->size[0];
			cv->size[0] = av->size[0];
			emxEnsureCapacity((emxArray__common *)cv, i16, (int)sizeof(float));
			csz_idx_0 = av->size[0];
			for (i16 = 0; i16 < csz_idx_0; i16++) {
				cv->data[i16] = av->data[i16] - center[bk];
			}

			for (csz_idx_0 = 1; csz_idx_0 <= nc1; csz_idx_0++) {
				pointVectors->data[(ck + csz_idx_0) - 1] = cv->data[csz_idx_0 - 1];
			}

			ak += na1;
			bk++;
			ck += nc1;
		}

		emxFree_real32_T(&cv);
	}

	emxFree_real32_T(&a);
	emxInit_real32_T(&b_pointVectors, 1);

	// 'CheckerboardImage:49' euclideanDists = hypot(pointVectors(:, 1), pointVectors(:, 2)); 
	csz_idx_0 = pointVectors->size[0];
	i15 = b_pointVectors->size[0];
	b_pointVectors->size[0] = csz_idx_0;
	emxEnsureCapacity((emxArray__common *)b_pointVectors, i15, (int)sizeof(float));
	for (i15 = 0; i15 < csz_idx_0; i15++) {
		b_pointVectors->data[i15] = pointVectors->data[i15];
	}

	emxInit_real32_T(&c_pointVectors, 1);
	csz_idx_0 = pointVectors->size[0];
	i15 = c_pointVectors->size[0];
	c_pointVectors->size[0] = csz_idx_0;
	emxEnsureCapacity((emxArray__common *)c_pointVectors, i15, (int)sizeof(float));
	for (i15 = 0; i15 < csz_idx_0; i15++) {
		c_pointVectors->data[i15] = pointVectors->data[i15 + pointVectors->size[0]];
	}

	b_hypot(b_pointVectors, c_pointVectors, av);

	//  find vertical and horizontal neighbors
	// 'CheckerboardImage:52' [this.BoardIdx(2, 3)] = findNeighbor(this, pointVectors, euclideanDists, v1); 
	b_this->BoardIdx->data[1 + (b_this->BoardIdx->size[0] << 1)] =
		CheckerboardImageFindNeighbor(b_this, pointVectors, av, v1);

	// 'CheckerboardImage:53' [this.BoardIdx(2, 1)] = findNeighbor(this, pointVectors, euclideanDists, -v1); 
	emxFree_real32_T(&c_pointVectors);
	emxFree_real32_T(&b_pointVectors);
	for (i15 = 0; i15 < 2; i15++) {
		b_v1[i15] = -v1[i15];
	}

	b_this->BoardIdx->data[1] = CheckerboardImageFindNeighbor(b_this,
		pointVectors, av, b_v1);

	// 'CheckerboardImage:54' [this.BoardIdx(3, 2)] = findNeighbor(this, pointVectors, euclideanDists, v2); 
	b_this->BoardIdx->data[2 + b_this->BoardIdx->size[0]] =
		CheckerboardImageFindNeighbor(b_this, pointVectors, av, v2);

	// 'CheckerboardImage:55' [this.BoardIdx(1, 2)] = findNeighbor(this, pointVectors, euclideanDists, -v2); 
	for (i15 = 0; i15 < 2; i15++) {
		b_v1[i15] = -v2[i15];
	}

	emxInit_boolean_T(&c_this, 1);
	b_this->BoardIdx->data[b_this->BoardIdx->size[0]] =
		CheckerboardImageFindNeighbor(b_this, pointVectors, av, b_v1);

	// 'CheckerboardImage:57' if any(this.BoardIdx(:) < 0)
	i15 = c_this->size[0];
	c_this->size[0] = b_this->BoardIdx->size[0] * b_this->BoardIdx->size[1];
	emxEnsureCapacity((emxArray__common *)c_this, i15, (int)sizeof(boolean_T));
	csz_idx_0 = b_this->BoardIdx->size[0] * b_this->BoardIdx->size[1];
	for (i15 = 0; i15 < csz_idx_0; i15++) {
		c_this->data[i15] = (b_this->BoardIdx->data[i15] < 0.0);
	}

	if (any(c_this)) {
		// 'CheckerboardImage:58' this.isValid = false;
		b_this->isValid = false;
	}
	else {
		// 'CheckerboardImage:62' r = this.Points(this.BoardIdx(2, 3), :);
		csz_idx_0 = (int)b_this->BoardIdx->data[1 + (b_this->BoardIdx->size[0] << 1)];
		for (i15 = 0; i15 < 2; i15++) {
			r[i15] = b_this->Points->data[(csz_idx_0 + b_this->Points->size[0] * i15)
				- 1];
		}

		// 'CheckerboardImage:63' this.BoardCoords(2, 3, :) = r;
		csz_idx_0 = b_this->BoardCoords->size[2];
		i15 = r5->size[0];
		r5->size[0] = csz_idx_0;
		emxEnsureCapacity((emxArray__common *)r5, i15, (int)sizeof(int));
		for (i15 = 0; i15 < csz_idx_0; i15++) {
			r5->data[i15] = i15;
		}

		for (i15 = 0; i15 < 2; i15++) {
			b_center[i15] = r[i15];
		}

		csz_idx_0 = r5->size[0];
		for (i15 = 0; i15 < csz_idx_0; i15++) {
			b_this->BoardCoords->data[((b_this->BoardCoords->size[0] << 1) +
				b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * r5->
				data[i15]) + 1] = b_center[i15];
		}

		// 'CheckerboardImage:64' l = this.Points(this.BoardIdx(2, 1), :);
		csz_idx_0 = (int)b_this->BoardIdx->data[1];
		for (i15 = 0; i15 < 2; i15++) {
			l[i15] = b_this->Points->data[(csz_idx_0 + b_this->Points->size[0] * i15)
				- 1];
		}

		// 'CheckerboardImage:65' this.BoardCoords(2, 1, :) = l;
		csz_idx_0 = b_this->BoardCoords->size[2];
		i15 = r5->size[0];
		r5->size[0] = csz_idx_0;
		emxEnsureCapacity((emxArray__common *)r5, i15, (int)sizeof(int));
		for (i15 = 0; i15 < csz_idx_0; i15++) {
			r5->data[i15] = i15;
		}

		for (i15 = 0; i15 < 2; i15++) {
			b_center[i15] = l[i15];
		}

		csz_idx_0 = r5->size[0];
		for (i15 = 0; i15 < csz_idx_0; i15++) {
			b_this->BoardCoords->data[1 + b_this->BoardCoords->size[0] *
				b_this->BoardCoords->size[1] * r5->data[i15]] = b_center[i15];
		}

		// 'CheckerboardImage:66' d = this.Points(this.BoardIdx(3, 2), :);
		csz_idx_0 = (int)b_this->BoardIdx->data[2 + b_this->BoardIdx->size[0]];
		for (i15 = 0; i15 < 2; i15++) {
			d[i15] = b_this->Points->data[(csz_idx_0 + b_this->Points->size[0] * i15)
				- 1];
		}

		// 'CheckerboardImage:67' this.BoardCoords(3, 2, :) = d;
		csz_idx_0 = b_this->BoardCoords->size[2];
		i15 = r5->size[0];
		r5->size[0] = csz_idx_0;
		emxEnsureCapacity((emxArray__common *)r5, i15, (int)sizeof(int));
		for (i15 = 0; i15 < csz_idx_0; i15++) {
			r5->data[i15] = i15;
		}

		for (i15 = 0; i15 < 2; i15++) {
			b_center[i15] = d[i15];
		}

		csz_idx_0 = r5->size[0];
		for (i15 = 0; i15 < csz_idx_0; i15++) {
			b_this->BoardCoords->data[(b_this->BoardCoords->size[0] +
				b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * r5->
				data[i15]) + 2] = b_center[i15];
		}

		// 'CheckerboardImage:68' u = this.Points(this.BoardIdx(1,2), :);
		csz_idx_0 = (int)b_this->BoardIdx->data[b_this->BoardIdx->size[0]];
		for (i15 = 0; i15 < 2; i15++) {
			u[i15] = b_this->Points->data[(csz_idx_0 + b_this->Points->size[0] * i15)
				- 1];
		}

		// 'CheckerboardImage:69' this.BoardCoords(1, 2, :) = u;
		csz_idx_0 = b_this->BoardCoords->size[2];
		i15 = r5->size[0];
		r5->size[0] = csz_idx_0;
		emxEnsureCapacity((emxArray__common *)r5, i15, (int)sizeof(int));
		for (i15 = 0; i15 < csz_idx_0; i15++) {
			r5->data[i15] = i15;
		}

		for (i15 = 0; i15 < 2; i15++) {
			b_center[i15] = u[i15];
		}

		csz_idx_0 = r5->size[0];
		for (i15 = 0; i15 < csz_idx_0; i15++) {
			b_this->BoardCoords->data[b_this->BoardCoords->size[0] +
				b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * r5->
				data[i15]] = b_center[i15];
		}

		//  find diagonal neighbors
		// 'CheckerboardImage:72' up    = u - center;
		// 'CheckerboardImage:73' down  = d - center;
		// 'CheckerboardImage:74' right = r - center;
		// 'CheckerboardImage:75' left  = l - center;
		// 'CheckerboardImage:77' [this.BoardIdx(1, 1)] = findNeighbor(this, pointVectors, euclideanDists, up + left); 
		for (i15 = 0; i15 < 2; i15++) {
			b_u = u[i15] - center[i15];
			b_l = l[i15] - center[i15];
			b_v1[i15] = b_u + b_l;
			u[i15] = b_u;
			d[i15] -= center[i15];
			r[i15] -= center[i15];
			l[i15] = b_l;
		}

		b_this->BoardIdx->data[0] = CheckerboardImageFindNeighbor(b_this,
			pointVectors, av, b_v1);

		// 'CheckerboardImage:78' [this.BoardIdx(3, 1)] = findNeighbor(this, pointVectors, euclideanDists, down + left); 
		for (i15 = 0; i15 < 2; i15++) {
			b_v1[i15] = d[i15] + l[i15];
		}

		b_this->BoardIdx->data[2] = CheckerboardImageFindNeighbor(b_this,
			pointVectors, av, b_v1);

		// 'CheckerboardImage:79' [this.BoardIdx(3, 3)] = findNeighbor(this, pointVectors, euclideanDists, down + right); 
		for (i15 = 0; i15 < 2; i15++) {
			b_v1[i15] = d[i15] + r[i15];
		}

		b_this->BoardIdx->data[2 + (b_this->BoardIdx->size[0] << 1)] =
			CheckerboardImageFindNeighbor(b_this, pointVectors, av, b_v1);

		// 'CheckerboardImage:80' [this.BoardIdx(1, 3)] = findNeighbor(this, pointVectors, euclideanDists, up + right); 
		for (i15 = 0; i15 < 2; i15++) {
			b_v1[i15] = u[i15] + r[i15];
		}

		emxInit_boolean_T(&d_this, 1);
		b_this->BoardIdx->data[b_this->BoardIdx->size[0] << 1] =
			CheckerboardImageFindNeighbor(b_this, pointVectors, av, b_v1);

		// 'CheckerboardImage:81' this.isValid = all(this.BoardIdx(:) > 0);
		i15 = d_this->size[0];
		d_this->size[0] = b_this->BoardIdx->size[0] * b_this->BoardIdx->size[1];
		emxEnsureCapacity((emxArray__common *)d_this, i15, (int)sizeof(boolean_T));
		csz_idx_0 = b_this->BoardIdx->size[0] * b_this->BoardIdx->size[1];
		for (i15 = 0; i15 < csz_idx_0; i15++) {
			d_this->data[i15] = (b_this->BoardIdx->data[i15] > 0.0);
		}

		b_this->isValid = all(d_this);

		// 'CheckerboardImage:82' if ~this.isValid
		emxFree_boolean_T(&d_this);
		if (!b_this->isValid) {
		}
		else {
			// 'CheckerboardImage:86' this.BoardCoords(1, 1, :) = this.Points(this.BoardIdx(1, 1), :); 
			csz_idx_0 = b_this->BoardCoords->size[2];
			i15 = r5->size[0];
			r5->size[0] = csz_idx_0;
			emxEnsureCapacity((emxArray__common *)r5, i15, (int)sizeof(int));
			for (i15 = 0; i15 < csz_idx_0; i15++) {
				r5->data[i15] = i15;
			}

			csz_idx_0 = (int)b_this->BoardIdx->data[0] - 1;
			for (i15 = 0; i15 < 2; i15++) {
				b_center[i15] = b_this->Points->data[csz_idx_0 + b_this->Points->size[0]
					* i15];
			}

			csz_idx_0 = r5->size[0];
			for (i15 = 0; i15 < csz_idx_0; i15++) {
				b_this->BoardCoords->data[b_this->BoardCoords->size[0] *
					b_this->BoardCoords->size[1] * r5->data[i15]] = b_center[i15];
			}

			// 'CheckerboardImage:87' this.BoardCoords(3, 1, :) = this.Points(this.BoardIdx(3, 1), :); 
			csz_idx_0 = b_this->BoardCoords->size[2];
			i15 = r5->size[0];
			r5->size[0] = csz_idx_0;
			emxEnsureCapacity((emxArray__common *)r5, i15, (int)sizeof(int));
			for (i15 = 0; i15 < csz_idx_0; i15++) {
				r5->data[i15] = i15;
			}

			csz_idx_0 = (int)b_this->BoardIdx->data[2] - 1;
			for (i15 = 0; i15 < 2; i15++) {
				b_center[i15] = b_this->Points->data[csz_idx_0 + b_this->Points->size[0]
					* i15];
			}

			csz_idx_0 = r5->size[0];
			for (i15 = 0; i15 < csz_idx_0; i15++) {
				b_this->BoardCoords->data[2 + b_this->BoardCoords->size[0] *
					b_this->BoardCoords->size[1] * r5->data[i15]] = b_center[i15];
			}

			// 'CheckerboardImage:88' this.BoardCoords(3, 3, :) = this.Points(this.BoardIdx(3, 3), :); 
			csz_idx_0 = b_this->BoardCoords->size[2];
			i15 = r5->size[0];
			r5->size[0] = csz_idx_0;
			emxEnsureCapacity((emxArray__common *)r5, i15, (int)sizeof(int));
			for (i15 = 0; i15 < csz_idx_0; i15++) {
				r5->data[i15] = i15;
			}

			csz_idx_0 = (int)b_this->BoardIdx->data[2 + (b_this->BoardIdx->size[0] <<
				1)] - 1;
			for (i15 = 0; i15 < 2; i15++) {
				b_center[i15] = b_this->Points->data[csz_idx_0 + b_this->Points->size[0]
					* i15];
			}

			csz_idx_0 = r5->size[0];
			for (i15 = 0; i15 < csz_idx_0; i15++) {
				b_this->BoardCoords->data[((b_this->BoardCoords->size[0] << 1) +
					b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * r5->
					data[i15]) + 2] = b_center[i15];
			}

			// 'CheckerboardImage:89' this.BoardCoords(1, 3, :) = this.Points(this.BoardIdx(1, 3), :); 
			csz_idx_0 = b_this->BoardCoords->size[2];
			i15 = r5->size[0];
			r5->size[0] = csz_idx_0;
			emxEnsureCapacity((emxArray__common *)r5, i15, (int)sizeof(int));
			for (i15 = 0; i15 < csz_idx_0; i15++) {
				r5->data[i15] = i15;
			}

			csz_idx_0 = (int)b_this->BoardIdx->data[b_this->BoardIdx->size[0] << 1] -
				1;
			for (i15 = 0; i15 < 2; i15++) {
				b_center[i15] = b_this->Points->data[csz_idx_0 + b_this->Points->size[0]
					* i15];
			}

			csz_idx_0 = r5->size[0];
			for (i15 = 0; i15 < csz_idx_0; i15++) {
				b_this->BoardCoords->data[(b_this->BoardCoords->size[0] << 1) +
					b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * r5->
					data[i15]] = b_center[i15];
			}

			emxInit_boolean_T(&e_this, 1);

			// 'CheckerboardImage:91' this.Energy = computeInitialEnergy(this);
			f_this = b_this;

			// ------------------------------------------------------------------
			// 'CheckerboardImage:101' if any(this.BoardIdx(:) < 0)
			i15 = e_this->size[0];
			e_this->size[0] = f_this->BoardIdx->size[0] * f_this->BoardIdx->size[1];
			emxEnsureCapacity((emxArray__common *)e_this, i15, (int)sizeof(boolean_T));
			csz_idx_0 = f_this->BoardIdx->size[0] * f_this->BoardIdx->size[1];
			for (i15 = 0; i15 < csz_idx_0; i15++) {
				e_this->data[i15] = (f_this->BoardIdx->data[i15] < 0.0);
			}

			if (any(e_this)) {
				// 'CheckerboardImage:102' e = single(10e10);
				e = 1.0E+11F;
			}
			else {
				// 'CheckerboardImage:106' e = single(0);
				//  compute energy over rows
				// 'CheckerboardImage:109' row1 = this.getPoints(1, 1:3);
				// 'CheckerboardImage:263' p = single(this.Points(this.BoardIdx(i, j), :)); 
				// 'CheckerboardImage:110' row2 = this.getPoints(2, 1:3);
				// 'CheckerboardImage:263' p = single(this.Points(this.BoardIdx(i, j), :)); 
				// 'CheckerboardImage:111' row3 = this.getPoints(3, 1:3);
				// 'CheckerboardImage:263' p = single(this.Points(this.BoardIdx(i, j), :)); 
				for (i15 = 0; i15 < 2; i15++) {
					for (i16 = 0; i16 < 3; i16++) {
						col1[i16 + 3 * i15] = f_this->Points->data[((int)f_this->
							BoardIdx->data[f_this->BoardIdx->size[0] * i16] + f_this->
							Points->size[0] * i15) - 1];
						col2[i16 + 3 * i15] = f_this->Points->data[((int)f_this->
							BoardIdx->data[1 + f_this->BoardIdx->size[0] * i16] +
							f_this->Points->size[0] * i15) - 1];
						row3[i16 + 3 * i15] = f_this->Points->data[((int)f_this->
							BoardIdx->data[2 + f_this->BoardIdx->size[0] * i16] +
							f_this->Points->size[0] * i15) - 1];
					}
				}

				// 'CheckerboardImage:113' num = row1 + row3 - 2 * row2;
				// 'CheckerboardImage:114' denom = row1 - row3;
				for (i15 = 0; i15 < 6; i15++) {
					b_col2 = (col1[i15] + row3[i15]) - 2.0F * col2[i15];
					col1[i15] -= row3[i15];
					col2[i15] = b_col2;
				}

				// 'CheckerboardImage:115' e = max(e, max(hypot(num(:, 1), num(:, 2)) ./ hypot(denom(:, 1), denom(:, 2)))); 
				for (csz_idx_0 = 0; csz_idx_0 < 3; csz_idx_0++) {
					b_r[csz_idx_0] = rt_hypotf(col2[csz_idx_0], col2[3 + csz_idx_0]) /
						rt_hypotf(col1[csz_idx_0], col1[3 + csz_idx_0]);
				}

				mtmp = b_r[0];
				for (csz_idx_0 = 0; csz_idx_0 < 2; csz_idx_0++) {
					if (b_r[csz_idx_0 + 1] > mtmp) {
						mtmp = b_r[csz_idx_0 + 1];
					}

					for (i15 = 0; i15 < 3; i15++) {
						col1[i15 + 3 * csz_idx_0] = f_this->Points->data[((int)
							f_this->BoardIdx->data[i15] + f_this->Points->size[0] * csz_idx_0)
							- 1];
						col2[i15 + 3 * csz_idx_0] = f_this->Points->data[((int)
							f_this->BoardIdx->data[i15 + f_this->BoardIdx->size[0]] +
							f_this->Points->size[0] * csz_idx_0) - 1];
						row3[i15 + 3 * csz_idx_0] = f_this->Points->data[((int)
							f_this->BoardIdx->data[i15 + (f_this->BoardIdx->size[0] << 1)] +
							f_this->Points->size[0] * csz_idx_0) - 1];
					}
				}

				if (0.0F >= mtmp) {
					e = 0.0F;
				}
				else {
					e = mtmp;
				}

				//  compute energy over columns
				// 'CheckerboardImage:118' col1 = this.getPoints(1:3, 1);
				// 'CheckerboardImage:263' p = single(this.Points(this.BoardIdx(i, j), :)); 
				// 'CheckerboardImage:119' col2 = this.getPoints(1:3, 2);
				// 'CheckerboardImage:263' p = single(this.Points(this.BoardIdx(i, j), :)); 
				// 'CheckerboardImage:120' col3 = this.getPoints(1:3, 3);
				// 'CheckerboardImage:263' p = single(this.Points(this.BoardIdx(i, j), :)); 
				// 'CheckerboardImage:122' num = col1 + col3 - 2 * col2;
				// 'CheckerboardImage:123' denom = col1 - col3;
				for (i15 = 0; i15 < 6; i15++) {
					b_col2 = (col1[i15] + row3[i15]) - 2.0F * col2[i15];
					col1[i15] -= row3[i15];
					col2[i15] = b_col2;
				}

				// 'CheckerboardImage:124' e = max(e, max(hypot(num(:, 1), num(:, 2)) ./ hypot(denom(:, 1), denom(:, 2)))); 
				for (csz_idx_0 = 0; csz_idx_0 < 3; csz_idx_0++) {
					b_r[csz_idx_0] = rt_hypotf(col2[csz_idx_0], col2[3 + csz_idx_0]) /
						rt_hypotf(col1[csz_idx_0], col1[3 + csz_idx_0]);
				}

				mtmp = b_r[0];
				for (csz_idx_0 = 0; csz_idx_0 < 2; csz_idx_0++) {
					if (b_r[csz_idx_0 + 1] > mtmp) {
						mtmp = b_r[csz_idx_0 + 1];
					}
				}

				if (e >= mtmp) {
				}
				else {
					e = mtmp;
				}

				// 'CheckerboardImage:126' boardSize = single(numel(this.BoardIdx));
				boardSize = (float)(f_this->BoardIdx->size[0] * f_this->BoardIdx->size[1]);

				// 'CheckerboardImage:127' e = boardSize * e - boardSize;
				e = boardSize * e - boardSize;
			}

			emxFree_boolean_T(&e_this);
			b_this->Energy = e;

			//  a perfect initial board should have the energy of -9.
			// 'CheckerboardImage:93' maxEnergy = -7;
			// 'CheckerboardImage:94' this.isValid = this.Energy < maxEnergy;
			b_this->isValid = (b_this->Energy < -7.0F);
		}
	}

	emxFree_boolean_T(&c_this);
	emxFree_real32_T(&av);
	emxFree_int32_T(&r5);
	emxFree_real32_T(&pointVectors);
}

//
// Arguments    : const emxArray_boolean_T *x
// Return Type  : boolean_T
//
boolean_T all(const emxArray_boolean_T *x)
{
	boolean_T y;
	int ix;
	boolean_T exitg1;
	y = true;
	ix = 1;
	exitg1 = false;
	while ((!exitg1) && (ix <= x->size[0])) {
		if (!x->data[ix - 1]) {
			y = false;
			exitg1 = true;
		}
		else {
			ix++;
		}
	}

	return y;
}

//
// Arguments    : const emxArray_boolean_T *x
// Return Type  : boolean_T
//
boolean_T any(const emxArray_boolean_T *x)
{
	boolean_T y;
	int ix;
	boolean_T exitg1;
	boolean_T b0;
	y = false;
	ix = 1;
	exitg1 = false;
	while ((!exitg1) && (ix <= x->size[0])) {
		b0 = !x->data[ix - 1];
		if (!b0) {
			y = true;
			exitg1 = true;
		}
		else {
			ix++;
		}
	}

	return y;
}

//
// Arguments    : const emxArray_real32_T *x
//                emxArray_real32_T *y
// Return Type  : void
//
void b_abs(const emxArray_real32_T *x, emxArray_real32_T *y)
{
	unsigned int uv1[2];
	int n;
	int k;
	for (n = 0; n < 2; n++) {
		uv1[n] = (unsigned int)x->size[n];
	}

	n = y->size[0] * y->size[1];
	y->size[0] = (int)uv1[0];
	y->size[1] = (int)uv1[1];
	emxEnsureCapacity((emxArray__common *)y, n, (int)sizeof(float));
	n = x->size[0] * x->size[1];
	for (k = 0; k + 1 <= n; k++) {
		y->data[k] = (float)fabs((double)x->data[k]);
	}
}

//
// Arguments    : const emxArray_real32_T *x
//                const emxArray_real32_T *y
//                emxArray_real32_T *r
// Return Type  : void
//
void b_hypot(const emxArray_real32_T *x, const emxArray_real32_T *y,
	emxArray_real32_T *r)
{
	int k;
	int i17;
	if (x->size[0] <= y->size[0]) {
		k = x->size[0];
	}
	else {
		k = y->size[0];
	}

	i17 = r->size[0];
	r->size[0] = k;
	emxEnsureCapacity((emxArray__common *)r, i17, (int)sizeof(float));
	for (k = 0; k + 1 <= x->size[0]; k++) {
		r->data[k] = rt_hypotf(x->data[k], y->data[k]);
	}
}

//
// Arguments    : emxArray_real32_T *varargin_1
// Return Type  : void
//
void b_imfilter(emxArray_real32_T *varargin_1)
{
	double pad[2];
	emxArray_real32_T *A;
	int j;
	int iv2[2];
	int k;
	int firstRowA;
	int aidx;
	boolean_T b2;
	int cidx;
	int a;
	int b_a;
	int ma;
	int na;
	int lastColB;
	int lastRowB;
	int lastColA;
	int iC;
	int c;
	int iB;
	int i;
	int b_i;
	int a_length;
	pad[0] = 1.0;
	pad[1] = 0.0;
	if ((varargin_1->size[0] == 0) || (varargin_1->size[1] == 0)) {
	}
	else {
		emxInit_real32_T(&A, 2);
		padImage(varargin_1, pad, A);
		for (j = 0; j < 2; j++) {
			pad[j] = ((double)A->size[j] - (3.0 + -2.0 * (double)j)) + 1.0;
		}

		for (k = 0; k < 2; k++) {
			if (0 >= (int)pad[k]) {
				iv2[k] = 0;
			}
			else {
				iv2[k] = (int)pad[k];
			}
		}

		j = varargin_1->size[0] * varargin_1->size[1];
		varargin_1->size[0] = iv2[0];
		varargin_1->size[1] = iv2[1];
		emxEnsureCapacity((emxArray__common *)varargin_1, j, (int)sizeof(float));
		firstRowA = iv2[1];
		for (j = 0; j < firstRowA; j++) {
			aidx = iv2[0];
			for (cidx = 0; cidx < aidx; cidx++) {
				varargin_1->data[cidx + varargin_1->size[0] * j] = 0.0F;
			}
		}

		if ((A->size[1] == 0) || ((iv2[0] == 0) || (iv2[1] == 0))) {
			b2 = true;
		}
		else {
			b2 = false;
		}

		if (!b2) {
			a = A->size[1];
			b_a = A->size[0];
			ma = A->size[0];
			na = A->size[1];
			if (1 <= A->size[1] - 1) {
				lastColB = 1;
			}
			else {
				lastColB = A->size[1];
			}

			if (3 <= A->size[0] - 1) {
				lastRowB = 3;
			}
			else {
				lastRowB = A->size[0];
			}

			j = 0;
			while (j <= lastColB - 1) {
				if (na - 1 < a - 1) {
					lastColA = na;
				}
				else {
					lastColA = a;
				}

				for (k = 0; k < lastColA; k++) {
					if (k >= 0) {
						firstRowA = k;
					}
					else {
						firstRowA = 0;
					}

					iC = firstRowA * (A->size[0] - 2);
					c = k * ma;
					iB = 1;
					for (i = 0; i < lastRowB; i++) {
						if (i < 2) {
							firstRowA = 2 - i;
						}
						else {
							firstRowA = 0;
						}

						if (i + ma <= b_a - 1) {
							b_i = ma;
						}
						else {
							b_i = b_a - i;
						}

						a_length = b_i - firstRowA;
						aidx = c + firstRowA;
						cidx = iC;
						for (j = 1; j <= a_length; j++) {
							varargin_1->data[cidx] += (-1.0F + ((float)iB - 1.0F)) * A->
								data[aidx];
							aidx++;
							cidx++;
						}

						iB++;
						if (i >= 2) {
							iC++;
						}
					}
				}

				j = 1;
			}
		}

		emxFree_real32_T(&A);
	}
}

//
// Arguments    : const emxArray_real32_T *varargin_1
//                const double varargin_2[2]
//                emxArray_real32_T *b
// Return Type  : void
//
void b_padarray(const emxArray_real32_T *varargin_1, const double
	varargin_2[2], emxArray_real32_T *b)
{
	double sizeB[2];
	int i6;
	double b_sizeB[2];
	int j;
	int i7;
	int i;
	if ((varargin_1->size[0] == 0) || (varargin_1->size[1] == 0)) {
		for (i6 = 0; i6 < 2; i6++) {
			sizeB[i6] = (double)varargin_1->size[i6] + varargin_2[i6];
		}

		b_sizeB[0] = sizeB[0];
		b_sizeB[1] = sizeB[1];
		repmat(b_sizeB, b);
	}
	else {
		for (i6 = 0; i6 < 2; i6++) {
			sizeB[i6] = (double)varargin_1->size[i6] + varargin_2[i6];
		}

		i6 = b->size[0] * b->size[1];
		b->size[0] = (int)sizeB[0];
		b->size[1] = (int)sizeB[1];
		emxEnsureCapacity((emxArray__common *)b, i6, (int)sizeof(float));
		i6 = b->size[1];
		for (j = varargin_1->size[1]; j + 1 <= i6; j++) {
			i7 = b->size[0];
			for (i = 0; i < i7; i++) {
				b->data[i + b->size[0] * j] = 0.0F;
			}
		}

		for (j = 0; j < varargin_1->size[1]; j++) {
			i6 = b->size[0];
			for (i = varargin_1->size[0]; i + 1 <= i6; i++) {
				b->data[i + b->size[0] * j] = 0.0F;
			}
		}

		for (j = 0; j < varargin_1->size[1]; j++) {
			for (i = 0; i < varargin_1->size[0]; i++) {
				b->data[i + b->size[0] * j] = varargin_1->data[i + varargin_1->size[0] *
					j];
			}
		}
	}
}

//
// Arguments    : const emxArray_real_T *A
//                emxArray_real_T *B
// Return Type  : void
//
void b_rot90(const emxArray_real_T *A, emxArray_real_T *B)
{
	int m;
	int n;
	int j;
	int i;
	int A_idx_0;
	int B_idx_0;
	m = A->size[0];
	n = A->size[1];
	for (j = 0; j < 2; j++) {
		i = B->size[0] * B->size[1];
		B->size[j] = A->size[j];
		emxEnsureCapacity((emxArray__common *)B, i, (int)sizeof(double));
	}

	for (j = 1; j <= n; j++) {
		for (i = 1; i <= m; i++) {
			A_idx_0 = A->size[0];
			B_idx_0 = B->size[0];
			B->data[(i + B_idx_0 * (j - 1)) - 1] = A->data[(m - i) + A_idx_0 * (n - j)];
		}
	}
}

//
// Arguments    : emxArray_real32_T *x
//                int dim
//                emxArray_int32_T *idx
// Return Type  : void
//
void b_sort(emxArray_real32_T *x, int dim, emxArray_int32_T *idx)
{
	int i58;
	emxArray_real32_T *vwork;
	int vstride;
	int x_idx_0;
	int j;
	emxArray_int32_T *iidx;
	if (dim <= 1) {
		i58 = x->size[0];
	}
	else {
		i58 = 1;
	}

	emxInit_real32_T(&vwork, 1);
	vstride = vwork->size[0];
	vwork->size[0] = i58;
	emxEnsureCapacity((emxArray__common *)vwork, vstride, (int)sizeof(float));
	x_idx_0 = x->size[0];
	vstride = idx->size[0];
	idx->size[0] = x_idx_0;
	emxEnsureCapacity((emxArray__common *)idx, vstride, (int)sizeof(int));
	vstride = 1;
	x_idx_0 = 1;
	while (x_idx_0 <= dim - 1) {
		vstride *= x->size[0];
		x_idx_0 = 2;
	}

	j = 0;
	emxInit_int32_T(&iidx, 1);
	while (j + 1 <= vstride) {
		for (x_idx_0 = 0; x_idx_0 + 1 <= i58; x_idx_0++) {
			vwork->data[x_idx_0] = x->data[j + x_idx_0 * vstride];
		}

		sortIdx(vwork, iidx);
		for (x_idx_0 = 0; x_idx_0 + 1 <= i58; x_idx_0++) {
			x->data[j + x_idx_0 * vstride] = vwork->data[x_idx_0];
			idx->data[j + x_idx_0 * vstride] = iidx->data[x_idx_0];
		}

		j++;
	}

	emxFree_int32_T(&iidx);
	emxFree_real32_T(&vwork);
}

//
// Arguments    : const emxArray_real_T *a
//                emxArray_real_T *b
// Return Type  : void
//
void b_squeeze(const emxArray_real_T *a, emxArray_real_T *b)
{
	int k;
	int sqsz[3];
	int i35;
	k = 3;
	while ((k > 2) && (a->size[2] == 1)) {
		k = 2;
	}

	if (k <= 2) {
		sqsz[0] = a->size[0];
		i35 = b->size[0] * b->size[1];
		b->size[0] = sqsz[0];
		b->size[1] = 1;
		emxEnsureCapacity((emxArray__common *)b, i35, (int)sizeof(double));
		i35 = a->size[0] * a->size[2];
		for (k = 0; k + 1 <= i35; k++) {
			b->data[k] = a->data[k];
		}
	}
	else {
		for (i35 = 0; i35 < 3; i35++) {
			sqsz[i35] = 1;
		}

		k = 0;
		if (a->size[0] != 1) {
			sqsz[0] = a->size[0];
			k = 1;
		}

		if (a->size[2] != 1) {
			sqsz[k] = a->size[2];
		}

		i35 = b->size[0] * b->size[1];
		b->size[0] = sqsz[0];
		b->size[1] = sqsz[1];
		emxEnsureCapacity((emxArray__common *)b, i35, (int)sizeof(double));
		i35 = a->size[0] * a->size[2];
		for (k = 0; k + 1 <= i35; k++) {
			b->data[k] = a->data[k];
		}
	}
}

//
// Arguments    : const emxArray_real_T *A
//                emxArray_real_T *U
//                emxArray_real_T *S
//                emxArray_real_T *V
// Return Type  : void
//
void svd(const emxArray_real_T *A, emxArray_real_T *U, emxArray_real_T *
	S, emxArray_real_T *V)
{
	emxArray_real_T *b_A;
	int m;
	int ns;
	int n;
	int p;
	int minnp;
	emxArray_real_T *s;
	emxArray_real_T *e;
	emxArray_real_T *work;
	int qs;
	int nrt;
	int nct;
	int q;
	int iter;
	int nmq;
	unsigned int uv0[2];
	boolean_T apply_transform;
	double ztest0;
	int jj;
	double ztest;
	double snorm;
	boolean_T exitg2;
	boolean_T exitg1;
	double f;
	double varargin_1[5];
	double mtmp;
	double sqds;
	double b;
	emxInit_real_T(&b_A, 2);
	m = b_A->size[0] * b_A->size[1];
	b_A->size[0] = A->size[0];
	b_A->size[1] = A->size[1];
	emxEnsureCapacity((emxArray__common *)b_A, m, (int)sizeof(double));
	ns = A->size[0] * A->size[1];
	for (m = 0; m < ns; m++) {
		b_A->data[m] = A->data[m];
	}

	n = A->size[0];
	p = A->size[1];
	if (A->size[0] + 1 <= A->size[1]) {
		ns = A->size[0] + 1;
	}
	else {
		ns = A->size[1];
	}

	if (A->size[0] <= A->size[1]) {
		minnp = A->size[0];
	}
	else {
		minnp = A->size[1];
	}

	emxInit_real_T(&s, 1);
	m = s->size[0];
	s->size[0] = ns;
	emxEnsureCapacity((emxArray__common *)s, m, (int)sizeof(double));
	for (m = 0; m < ns; m++) {
		s->data[m] = 0.0;
	}

	emxInit_real_T(&e, 1);
	ns = A->size[1];
	m = e->size[0];
	e->size[0] = ns;
	emxEnsureCapacity((emxArray__common *)e, m, (int)sizeof(double));
	for (m = 0; m < ns; m++) {
		e->data[m] = 0.0;
	}

	emxInit_real_T(&work, 1);
	ns = A->size[0];
	m = work->size[0];
	work->size[0] = ns;
	emxEnsureCapacity((emxArray__common *)work, m, (int)sizeof(double));
	for (m = 0; m < ns; m++) {
		work->data[m] = 0.0;
	}

	ns = A->size[0];
	qs = A->size[0];
	m = U->size[0] * U->size[1];
	U->size[0] = ns;
	U->size[1] = qs;
	emxEnsureCapacity((emxArray__common *)U, m, (int)sizeof(double));
	ns *= qs;
	for (m = 0; m < ns; m++) {
		U->data[m] = 0.0;
	}

	ns = A->size[1];
	qs = A->size[1];
	m = V->size[0] * V->size[1];
	V->size[0] = ns;
	V->size[1] = qs;
	emxEnsureCapacity((emxArray__common *)V, m, (int)sizeof(double));
	ns *= qs;
	for (m = 0; m < ns; m++) {
		V->data[m] = 0.0;
	}

	if ((A->size[0] == 0) || (A->size[1] == 0)) {
		m = A->size[0];
		for (ns = 0; ns + 1 <= m; ns++) {
			U->data[ns + U->size[0] * ns] = 1.0;
		}

		m = A->size[1];
		for (ns = 0; ns + 1 <= m; ns++) {
			V->data[ns + V->size[0] * ns] = 1.0;
		}
	}
	else {
		if (A->size[1] > 2) {
			ns = A->size[1] - 2;
		}
		else {
			ns = 0;
		}

		if (ns <= A->size[0]) {
			nrt = ns;
		}
		else {
			nrt = A->size[0];
		}

		if (A->size[0] > 1) {
			ns = A->size[0] - 1;
		}
		else {
			ns = 0;
		}

		if (ns <= A->size[1]) {
			nct = ns;
		}
		else {
			nct = A->size[1];
		}

		if (nct >= nrt) {
			m = nct;
		}
		else {
			m = nrt;
		}

		for (q = 0; q + 1 <= m; q++) {
			iter = q + n * q;
			nmq = n - q;
			apply_transform = false;
			if (q + 1 <= nct) {
				ztest0 = xnrm2(nmq, b_A, iter + 1);
				if (ztest0 > 0.0) {
					apply_transform = true;
					if (b_A->data[iter] < 0.0) {
						ztest0 = -ztest0;
					}

					s->data[q] = ztest0;
					if (fabs(s->data[q]) >= 1.0020841800044864E-292) {
						xscal(nmq, 1.0 / s->data[q], b_A, iter + 1);
					}
					else {
						ns = iter + nmq;
						for (qs = iter; qs + 1 <= ns; qs++) {
							b_A->data[qs] /= s->data[q];
						}
					}

					b_A->data[iter]++;
					s->data[q] = -s->data[q];
				}
				else {
					s->data[q] = 0.0;
				}
			}

			for (jj = q + 1; jj + 1 <= p; jj++) {
				ns = q + n * jj;
				if (apply_transform) {
					ztest0 = xdotc(nmq, b_A, iter + 1, b_A, ns + 1);
					ztest0 = -(ztest0 / b_A->data[q + b_A->size[0] * q]);
					xaxpy(nmq, ztest0, iter + 1, b_A, ns + 1);
				}

				e->data[jj] = b_A->data[ns];
			}

			if (q + 1 <= nct) {
				for (ns = q; ns + 1 <= n; ns++) {
					U->data[ns + U->size[0] * q] = b_A->data[ns + b_A->size[0] * q];
				}
			}

			if (q + 1 <= nrt) {
				iter = p - q;
				ztest0 = b_xnrm2(iter - 1, e, q + 2);
				if (ztest0 == 0.0) {
					e->data[q] = 0.0;
				}
				else {
					if (e->data[q + 1] < 0.0) {
						ztest0 = -ztest0;
					}

					e->data[q] = ztest0;
					ztest0 = e->data[q];
					if (fabs(ztest0) >= 1.0020841800044864E-292) {
						ztest0 = 1.0 / ztest0;
						ns = q + iter;
						for (qs = q + 1; qs + 1 <= ns; qs++) {
							e->data[qs] *= ztest0;
						}
					}
					else {
						ns = q + iter;
						for (qs = q + 1; qs + 1 <= ns; qs++) {
							e->data[qs] /= ztest0;
						}
					}

					e->data[q + 1]++;
					e->data[q] = -e->data[q];
					if (q + 2 <= n) {
						for (ns = q + 1; ns + 1 <= n; ns++) {
							work->data[ns] = 0.0;
						}

						for (jj = q + 1; jj + 1 <= p; jj++) {
							b_xaxpy(nmq - 1, e->data[jj], b_A, (q + n * jj) + 2, work, q + 2);
						}

						for (jj = q + 1; jj + 1 <= p; jj++) {
							c_xaxpy(nmq - 1, -e->data[jj] / e->data[q + 1], work, q + 2, b_A,
								(q + n * jj) + 2);
						}
					}
				}

				for (ns = q + 1; ns + 1 <= p; ns++) {
					V->data[ns + V->size[0] * q] = e->data[ns];
				}
			}
		}

		if (A->size[1] <= A->size[0] + 1) {
			m = A->size[1];
		}
		else {
			m = A->size[0] + 1;
		}

		if (nct < A->size[1]) {
			s->data[nct] = b_A->data[nct + b_A->size[0] * nct];
		}

		if (A->size[0] < m) {
			s->data[m - 1] = 0.0;
		}

		if (nrt + 1 < m) {
			e->data[nrt] = b_A->data[nrt + b_A->size[0] * (m - 1)];
		}

		e->data[m - 1] = 0.0;
		if (nct + 1 <= A->size[0]) {
			for (jj = nct; jj + 1 <= n; jj++) {
				for (ns = 1; ns <= n; ns++) {
					U->data[(ns + U->size[0] * jj) - 1] = 0.0;
				}

				U->data[jj + U->size[0] * jj] = 1.0;
			}
		}

		for (q = nct - 1; q + 1 > 0; q--) {
			nmq = n - q;
			iter = q + n * q;
			if (s->data[q] != 0.0) {
				for (jj = q + 1; jj + 1 <= n; jj++) {
					ns = (q + n * jj) + 1;
					ztest0 = xdotc(nmq, U, iter + 1, U, ns);
					ztest0 = -(ztest0 / U->data[iter]);
					xaxpy(nmq, ztest0, iter + 1, U, ns);
				}

				for (ns = q; ns + 1 <= n; ns++) {
					U->data[ns + U->size[0] * q] = -U->data[ns + U->size[0] * q];
				}

				U->data[iter]++;
				for (ns = 1; ns <= q; ns++) {
					U->data[(ns + U->size[0] * q) - 1] = 0.0;
				}
			}
			else {
				for (ns = 1; ns <= n; ns++) {
					U->data[(ns + U->size[0] * q) - 1] = 0.0;
				}

				U->data[iter] = 1.0;
			}
		}

		for (q = A->size[1] - 1; q + 1 > 0; q--) {
			if ((q + 1 <= nrt) && (e->data[q] != 0.0)) {
				iter = (p - q) - 1;
				ns = (q + p * q) + 2;
				for (jj = q + 1; jj + 1 <= p; jj++) {
					qs = (q + p * jj) + 2;
					ztest0 = xdotc(iter, V, ns, V, qs);
					ztest0 = -(ztest0 / V->data[ns - 1]);
					xaxpy(iter, ztest0, ns, V, qs);
				}
			}

			for (ns = 1; ns <= p; ns++) {
				V->data[(ns + V->size[0] * q) - 1] = 0.0;
			}

			V->data[q + V->size[0] * q] = 1.0;
		}

		for (q = 0; q + 1 <= m; q++) {
			if (s->data[q] != 0.0) {
				ztest = fabs(s->data[q]);
				ztest0 = s->data[q] / ztest;
				s->data[q] = ztest;
				if (q + 1 < m) {
					e->data[q] /= ztest0;
				}

				if (q + 1 <= n) {
					xscal(n, ztest0, U, 1 + n * q);
				}
			}

			if ((q + 1 < m) && (e->data[q] != 0.0)) {
				ztest = fabs(e->data[q]);
				ztest0 = ztest / e->data[q];
				e->data[q] = ztest;
				s->data[q + 1] *= ztest0;
				xscal(p, ztest0, V, 1 + p * (q + 1));
			}
		}

		nct = m;
		iter = 0;
		snorm = 0.0;
		for (ns = 0; ns + 1 <= m; ns++) {
			ztest0 = fabs(s->data[ns]);
			ztest = fabs(e->data[ns]);
			if (ztest0 >= ztest) {
			}
			else {
				ztest0 = ztest;
			}

			if (snorm >= ztest0) {
			}
			else {
				snorm = ztest0;
			}
		}

		while ((m > 0) && (!(iter >= 75))) {
			q = m - 1;
			exitg2 = false;
			while (!(exitg2 || (q == 0))) {
				ztest0 = fabs(e->data[q - 1]);
				if ((ztest0 <= 2.2204460492503131E-16 * (fabs(s->data[q - 1]) + fabs
				(s->data[q]))) || (ztest0 <= 1.0020841800044864E-292) || ((iter >
					20) && (ztest0 <= 2.2204460492503131E-16 * snorm))) {
					e->data[q - 1] = 0.0;
					exitg2 = true;
				}
				else {
					q--;
				}
			}

			if (q == m - 1) {
				ns = 4;
			}
			else {
				qs = m;
				ns = m;
				exitg1 = false;
				while ((!exitg1) && (ns >= q)) {
					qs = ns;
					if (ns == q) {
						exitg1 = true;
					}
					else {
						ztest0 = 0.0;
						if (ns < m) {
							ztest0 = fabs(e->data[ns - 1]);
						}

						if (ns > q + 1) {
							ztest0 += fabs(e->data[ns - 2]);
						}

						ztest = fabs(s->data[ns - 1]);
						if ((ztest <= 2.2204460492503131E-16 * ztest0) || (ztest <=
							1.0020841800044864E-292)) {
							s->data[ns - 1] = 0.0;
							exitg1 = true;
						}
						else {
							ns--;
						}
					}
				}

				if (qs == q) {
					ns = 3;
				}
				else if (qs == m) {
					ns = 1;
				}
				else {
					ns = 2;
					q = qs;
				}
			}

			switch (ns) {
			case 1:
				f = e->data[m - 2];
				e->data[m - 2] = 0.0;
				for (qs = m - 3; qs + 2 >= q + 1; qs--) {
					xrotg(&s->data[qs + 1], &f, &ztest0, &ztest);
					if (qs + 2 > q + 1) {
						f = -ztest * e->data[qs];
						e->data[qs] *= ztest0;
					}

					xrot(p, V, 1 + p * (qs + 1), 1 + p * (m - 1), ztest0, ztest);
				}
				break;

			case 2:
				f = e->data[q - 1];
				e->data[q - 1] = 0.0;
				for (qs = q; qs + 1 <= m; qs++) {
					xrotg(&s->data[qs], &f, &ztest0, &ztest);
					f = -ztest * e->data[qs];
					e->data[qs] *= ztest0;
					xrot(n, U, 1 + n * qs, 1 + n * (q - 1), ztest0, ztest);
				}
				break;

			case 3:
				varargin_1[0] = fabs(s->data[m - 1]);
				varargin_1[1] = fabs(s->data[m - 2]);
				varargin_1[2] = fabs(e->data[m - 2]);
				varargin_1[3] = fabs(s->data[q]);
				varargin_1[4] = fabs(e->data[q]);
				mtmp = varargin_1[0];
				for (ns = 0; ns < 4; ns++) {
					if (varargin_1[ns + 1] > mtmp) {
						mtmp = varargin_1[ns + 1];
					}
				}

				f = s->data[m - 1] / mtmp;
				ztest0 = s->data[m - 2] / mtmp;
				ztest = e->data[m - 2] / mtmp;
				sqds = s->data[q] / mtmp;
				b = ((ztest0 + f) * (ztest0 - f) + ztest * ztest) / 2.0;
				ztest0 = f * ztest;
				ztest0 *= ztest0;
				if ((b != 0.0) || (ztest0 != 0.0)) {
					ztest = sqrt(b * b + ztest0);
					if (b < 0.0) {
						ztest = -ztest;
					}

					ztest = ztest0 / (b + ztest);
				}
				else {
					ztest = 0.0;
				}

				f = (sqds + f) * (sqds - f) + ztest;
				b = sqds * (e->data[q] / mtmp);
				for (qs = q + 1; qs < m; qs++) {
					xrotg(&f, &b, &ztest0, &ztest);
					if (qs > q + 1) {
						e->data[qs - 2] = f;
					}

					f = ztest0 * s->data[qs - 1] + ztest * e->data[qs - 1];
					e->data[qs - 1] = ztest0 * e->data[qs - 1] - ztest * s->data[qs - 1];
					b = ztest * s->data[qs];
					s->data[qs] *= ztest0;
					xrot(p, V, 1 + p * (qs - 1), 1 + p * qs, ztest0, ztest);
					s->data[qs - 1] = f;
					xrotg(&s->data[qs - 1], &b, &ztest0, &ztest);
					f = ztest0 * e->data[qs - 1] + ztest * s->data[qs];
					s->data[qs] = -ztest * e->data[qs - 1] + ztest0 * s->data[qs];
					b = ztest * e->data[qs];
					e->data[qs] *= ztest0;
					if (qs < n) {
						xrot(n, U, 1 + n * (qs - 1), 1 + n * qs, ztest0, ztest);
					}
				}

				e->data[m - 2] = f;
				iter++;
				break;

			default:
				if (s->data[q] < 0.0) {
					s->data[q] = -s->data[q];
					xscal(p, -1.0, V, 1 + p * q);
				}

				ns = q + 1;
				while ((q + 1 < nct) && (s->data[q] < s->data[ns])) {
					ztest = s->data[q];
					s->data[q] = s->data[ns];
					s->data[ns] = ztest;
					if (q + 1 < p) {
						xswap(p, V, 1 + p * q, 1 + p * (q + 1));
					}

					if (q + 1 < n) {
						xswap(n, U, 1 + n * q, 1 + n * (q + 1));
					}

					q = ns;
					ns++;
				}

				iter = 0;
				m--;
				break;
			}
		}
	}

	emxFree_real_T(&work);
	emxFree_real_T(&b_A);
	m = e->size[0];
	e->size[0] = minnp;
	emxEnsureCapacity((emxArray__common *)e, m, (int)sizeof(double));
	for (qs = 0; qs + 1 <= minnp; qs++) {
		e->data[qs] = s->data[qs];
	}

	emxFree_real_T(&s);
	for (m = 0; m < 2; m++) {
		uv0[m] = (unsigned int)A->size[m];
	}

	m = S->size[0] * S->size[1];
	S->size[0] = (int)uv0[0];
	S->size[1] = (int)uv0[1];
	emxEnsureCapacity((emxArray__common *)S, m, (int)sizeof(double));
	ns = (int)uv0[0] * (int)uv0[1];
	for (m = 0; m < ns; m++) {
		S->data[m] = 0.0;
	}

	for (qs = 0; qs < e->size[0]; qs++) {
		S->data[qs + S->size[0] * qs] = e->data[qs];
	}

	emxFree_real_T(&e);
}

//
// Arguments    : int n
//                double a
//                const emxArray_real_T *x
//                int ix0
//                emxArray_real_T *y
//                int iy0
// Return Type  : void
//
void b_xaxpy(int n, double a, const emxArray_real_T *x, int ix0,
	emxArray_real_T *y, int iy0)
{
	int ix;
	int iy;
	int k;
	if ((n < 1) || (a == 0.0)) {
	}
	else {
		ix = ix0 - 1;
		iy = iy0 - 1;
		for (k = 0; k < n; k++) {
			y->data[iy] += a * x->data[ix];
			ix++;
			iy++;
		}
	}
}

//
// Arguments    : int n
//                const emxArray_real_T *x
//                int ix0
// Return Type  : double
//
double b_xnrm2(int n, const emxArray_real_T *x, int ix0)
{
	double y;
	double scale;
	int kend;
	int k;
	double absxk;
	double t;
	y = 0.0;
	if (n < 1) {
	}
	else if (n == 1) {
		y = fabs(x->data[ix0 - 1]);
	}
	else {
		scale = 2.2250738585072014E-308;
		kend = (ix0 + n) - 1;
		for (k = ix0; k <= kend; k++) {
			absxk = fabs(x->data[k - 1]);
			if (absxk > scale) {
				t = scale / absxk;
				y = 1.0 + y * t * t;
				scale = absxk;
			}
			else {
				t = absxk / scale;
				y += t * t;
			}
		}

		y = scale * sqrt(y);
	}

	return y;
}

//
// Arguments    : const emxArray_boolean_T *bwin
//                emxArray_boolean_T *B
// Return Type  : void
//
void bwlookup(const emxArray_boolean_T *bwin, emxArray_boolean_T *B)
{
	unsigned int uv3[2];
	int i9;
	double b_bwin[2];
	const boolean_T lut[512] = { false, false, false, false, false, false,
	false, false, false, false, false, false, false, false, false, false, false,
	true, true, true, true, false, true, true, true, true, true, true, false,
	false, true, true, false, false, false, false, false, false, false, false,
	false, false, false, false, false, false, false, false, true, false, true,
	true, true, false, true, true, false, false, true, true, false, false, true,
	true, false, false, false, false, false, false, false, false, false, false,
	false, false, false, false, false, false, true, false, false, false, false,
	false, false, false, true, true, true, true, false, false, true, true, false,
	false, false, false, false, false, false, false, false, false, false, false,
	false, false, false, false, false, false, false, false, false, false, false,
	false, false, false, true, true, false, false, true, true, false, false,
	false, false, false, false, false, false, false, false, false, false, false,
	false, false, false, true, false, false, false, false, false, false, false,
	true, true, true, true, false, false, true, true, false, false, false, false,
	false, false, false, false, false, false, false, false, false, false, false,
	false, true, false, true, true, true, false, true, true, true, true, false,
	false, true, true, false, false, false, false, false, false, false, false,
	false, false, false, false, false, false, false, false, false, false, true,
	false, false, false, false, false, false, false, true, true, true, true,
	false, false, true, true, false, false, false, false, false, false, false,
	false, false, false, false, false, false, false, false, false, true, false,
	true, true, true, false, true, true, true, true, false, false, true, true,
	false, false, false, false, false, false, false, false, false, false, false,
	false, false, false, false, false, false, false, true, false, false, false,
	false, false, false, false, false, false, false, false, false, false, false,
	false, false, false, false, false, false, false, false, false, false, false,
	false, false, false, false, false, false, true, false, true, true, true,
	false, true, true, false, false, true, true, false, false, true, true, false,
	false, false, false, false, false, false, false, false, false, false, false,
	false, false, false, false, false, false, false, false, false, false, false,
	false, false, false, false, false, false, false, false, false, false, false,
	false, false, false, false, false, false, false, false, false, false, false,
	false, false, false, false, false, false, false, false, false, false, false,
	false, false, true, true, false, false, true, true, false, false, false,
	false, false, false, false, false, false, false, false, false, false, false,
	false, false, true, false, false, false, false, false, false, false, true,
	true, true, true, false, false, true, true, false, false, false, false,
	false, false, false, false, false, false, false, false, false, false, false,
	false, true, false, true, true, true, false, true, true, true, true, false,
	false, true, true, false, false, false, false, false, false, false, false,
	false, false, false, false, false, false, false, false, false, false, true,
	false, false, false, false, false, false, false, true, true, true, true,
	false, false, true, true, false, false, false, false, false, false, false,
	false, false, false, false, false, false, false, false, false, true, false,
	true, true, true, false, true, true, true, true, false, false, true, true,
	false, false };

	for (i9 = 0; i9 < 2; i9++) {
		uv3[i9] = (unsigned int)bwin->size[i9];
	}

	i9 = B->size[0] * B->size[1];
	B->size[0] = (int)uv3[0];
	B->size[1] = (int)uv3[1];
	emxEnsureCapacity((emxArray__common *)B, i9, (int)sizeof(boolean_T));
	if ((bwin->size[0] == 0) || (bwin->size[1] == 0)) {
	}
	else {
		for (i9 = 0; i9 < 2; i9++) {
			b_bwin[i9] = bwin->size[i9];
		}

		bwlookup_tbb_boolean(&bwin->data[0], b_bwin, 2.0, lut, 512.0, &B->data[0]);
	}
}

//
// Arguments    : emxArray_boolean_T *bw
// Return Type  : void
//
void bwmorphApplyOnce(emxArray_boolean_T *bw)
{
	emxArray_boolean_T *m;
	int i47;
	int loop_ub;
	int i48;
	int i49;
	int i50;
	int i51;
	int i52;
	int b_loop_ub;
	int i53;
	int i54;
	int i55;
	int i56;
	int i57;
	emxInit_boolean_T(&m, 2);

	// 'bwmorphShrink:17' table = images.internal.lutshrink();
	//  First subiteration
	// 'bwmorphShrink:20' m   = bwlookup(bw, table);
	bwlookup(bw, m);

	// 'bwmorphShrink:21' sub = bw & ~m;
	i47 = m->size[0] * m->size[1];
	m->size[0] = bw->size[0];
	m->size[1] = bw->size[1];
	emxEnsureCapacity((emxArray__common *)m, i47, (int)sizeof(boolean_T));
	loop_ub = bw->size[0] * bw->size[1];
	for (i47 = 0; i47 < loop_ub; i47++) {
		m->data[i47] = (bw->data[i47] && (!m->data[i47]));
	}

	// 'bwmorphShrink:22' bw(1:2:end,1:2:end) = sub(1:2:end,1:2:end);
	if (1 > m->size[0]) {
		i47 = 1;
		i48 = -1;
	}
	else {
		i47 = 2;
		i48 = m->size[0] - 1;
	}

	if (1 > m->size[1]) {
		i49 = 1;
		i50 = -1;
	}
	else {
		i49 = 2;
		i50 = m->size[1] - 1;
	}

	if (1 > bw->size[0]) {
		i51 = 1;
	}
	else {
		i51 = 2;
	}

	if (1 > bw->size[1]) {
		i52 = 1;
	}
	else {
		i52 = 2;
	}

	loop_ub = div_s32_floor(i50, i49);
	for (i50 = 0; i50 <= loop_ub; i50++) {
		b_loop_ub = div_s32_floor(i48, i47);
		for (i53 = 0; i53 <= b_loop_ub; i53++) {
			bw->data[i51 * i53 + bw->size[0] * (i52 * i50)] = m->data[i47 * i53 +
				m->size[0] * (i49 * i50)];
		}
	}

	//  Second subiteration
	// 'bwmorphShrink:25' m   = bwlookup(bw, table);
	bwlookup(bw, m);

	// 'bwmorphShrink:26' sub = bw & ~m;
	i47 = m->size[0] * m->size[1];
	m->size[0] = bw->size[0];
	m->size[1] = bw->size[1];
	emxEnsureCapacity((emxArray__common *)m, i47, (int)sizeof(boolean_T));
	loop_ub = bw->size[0] * bw->size[1];
	for (i47 = 0; i47 < loop_ub; i47++) {
		m->data[i47] = (bw->data[i47] && (!m->data[i47]));
	}

	// 'bwmorphShrink:27' bw(2:2:end,2:2:end) = sub(2:2:end,2:2:end);
	if (2 > m->size[0]) {
		i47 = 0;
		i48 = 1;
		i49 = 0;
	}
	else {
		i47 = 1;
		i48 = 2;
		i49 = m->size[0];
	}

	if (2 > m->size[1]) {
		i50 = 0;
		i51 = 1;
		i52 = 0;
	}
	else {
		i50 = 1;
		i51 = 2;
		i52 = m->size[1];
	}

	if (2 > bw->size[0]) {
		i53 = 0;
		i54 = 1;
	}
	else {
		i53 = 1;
		i54 = 2;
	}

	if (2 > bw->size[1]) {
		i55 = 0;
		i56 = 1;
	}
	else {
		i55 = 1;
		i56 = 2;
	}

	loop_ub = div_s32_floor((i52 - i50) - 1, i51);
	for (i52 = 0; i52 <= loop_ub; i52++) {
		b_loop_ub = div_s32_floor((i49 - i47) - 1, i48);
		for (i57 = 0; i57 <= b_loop_ub; i57++) {
			bw->data[(i53 + i54 * i57) + bw->size[0] * (i55 + i56 * i52)] = m->data
				[(i47 + i48 * i57) + m->size[0] * (i50 + i51 * i52)];
		}
	}

	//  Third subiteration
	// 'bwmorphShrink:30' m   = bwlookup(bw, table);
	bwlookup(bw, m);

	// 'bwmorphShrink:31' sub = bw & ~m;
	i47 = m->size[0] * m->size[1];
	m->size[0] = bw->size[0];
	m->size[1] = bw->size[1];
	emxEnsureCapacity((emxArray__common *)m, i47, (int)sizeof(boolean_T));
	loop_ub = bw->size[0] * bw->size[1];
	for (i47 = 0; i47 < loop_ub; i47++) {
		m->data[i47] = (bw->data[i47] && (!m->data[i47]));
	}

	// 'bwmorphShrink:32' bw(1:2:end,2:2:end) = sub(1:2:end,2:2:end);
	if (1 > m->size[0]) {
		i47 = 1;
		i48 = -1;
	}
	else {
		i47 = 2;
		i48 = m->size[0] - 1;
	}

	if (2 > m->size[1]) {
		i49 = 0;
		i50 = 1;
		i51 = 0;
	}
	else {
		i49 = 1;
		i50 = 2;
		i51 = m->size[1];
	}

	if (1 > bw->size[0]) {
		i52 = 1;
	}
	else {
		i52 = 2;
	}

	if (2 > bw->size[1]) {
		i53 = 0;
		i54 = 1;
	}
	else {
		i53 = 1;
		i54 = 2;
	}

	loop_ub = div_s32_floor((i51 - i49) - 1, i50);
	for (i51 = 0; i51 <= loop_ub; i51++) {
		b_loop_ub = div_s32_floor(i48, i47);
		for (i55 = 0; i55 <= b_loop_ub; i55++) {
			bw->data[i52 * i55 + bw->size[0] * (i53 + i54 * i51)] = m->data[i47 * i55
				+ m->size[0] * (i49 + i50 * i51)];
		}
	}

	//  Fourth subiteration
	// 'bwmorphShrink:35' m   = bwlookup(bw, table);
	bwlookup(bw, m);

	// 'bwmorphShrink:36' sub = bw & ~m;
	i47 = m->size[0] * m->size[1];
	m->size[0] = bw->size[0];
	m->size[1] = bw->size[1];
	emxEnsureCapacity((emxArray__common *)m, i47, (int)sizeof(boolean_T));
	loop_ub = bw->size[0] * bw->size[1];
	for (i47 = 0; i47 < loop_ub; i47++) {
		m->data[i47] = (bw->data[i47] && (!m->data[i47]));
	}

	// 'bwmorphShrink:37' bw(2:2:end,1:2:end) = sub(2:2:end,1:2:end);
	if (2 > m->size[0]) {
		i47 = 0;
		i48 = 1;
		i49 = 0;
	}
	else {
		i47 = 1;
		i48 = 2;
		i49 = m->size[0];
	}

	if (1 > m->size[1]) {
		i50 = 1;
		i51 = -1;
	}
	else {
		i50 = 2;
		i51 = m->size[1] - 1;
	}

	if (2 > bw->size[0]) {
		i52 = 0;
		i53 = 1;
	}
	else {
		i52 = 1;
		i53 = 2;
	}

	if (1 > bw->size[1]) {
		i54 = 1;
	}
	else {
		i54 = 2;
	}

	loop_ub = div_s32_floor(i51, i50);
	for (i51 = 0; i51 <= loop_ub; i51++) {
		b_loop_ub = div_s32_floor((i49 - i47) - 1, i48);
		for (i55 = 0; i55 <= b_loop_ub; i55++) {
			bw->data[(i52 + i53 * i55) + bw->size[0] * (i54 * i51)] = m->data[(i47 +
				i48 * i55) + m->size[0] * (i50 * i51)];
		}
	}

	emxFree_boolean_T(&m);
}

//
// Arguments    : CheckerboardImage *b_this
// Return Type  : CheckerboardImage *
//
CheckerboardImage *c_CheckerboardImage_Checkerboar(CheckerboardImage
	*b_this)
{
	CheckerboardImage *c_this;
	int i14;
	c_this = b_this;
	c_this->isValid = false;
	c_this->Energy = 1.0E+11F;

	// 'CheckerboardImage:20' this.BoardIdx = 0;
	i14 = c_this->BoardIdx->size[0] * c_this->BoardIdx->size[1];
	c_this->BoardIdx->size[0] = 1;
	c_this->BoardIdx->size[1] = 1;
	emxEnsureCapacity((emxArray__common *)c_this->BoardIdx, i14, (int)sizeof
	(double));
	c_this->BoardIdx->data[0] = 0.0;

	// 'CheckerboardImage:21' this.BoardIdx = zeros(3, 3);
	i14 = c_this->BoardIdx->size[0] * c_this->BoardIdx->size[1];
	c_this->BoardIdx->size[0] = 3;
	c_this->BoardIdx->size[1] = 3;
	emxEnsureCapacity((emxArray__common *)c_this->BoardIdx, i14, (int)sizeof
	(double));
	for (i14 = 0; i14 < 9; i14++) {
		c_this->BoardIdx->data[i14] = 0.0;
	}

	// 'CheckerboardImage:22' this.BoardCoords = 0;
	i14 = c_this->BoardCoords->size[0] * c_this->BoardCoords->size[1] *
		c_this->BoardCoords->size[2];
	c_this->BoardCoords->size[0] = 1;
	c_this->BoardCoords->size[1] = 1;
	c_this->BoardCoords->size[2] = 1;
	emxEnsureCapacity((emxArray__common *)c_this->BoardCoords, i14, (int)sizeof
	(double));
	c_this->BoardCoords->data[0] = 0.0;

	// 'CheckerboardImage:23' this.BoardCoords = zeros(3, 3, 2);
	i14 = c_this->BoardCoords->size[0] * c_this->BoardCoords->size[1] *
		c_this->BoardCoords->size[2];
	c_this->BoardCoords->size[0] = 3;
	c_this->BoardCoords->size[1] = 3;
	c_this->BoardCoords->size[2] = 2;
	emxEnsureCapacity((emxArray__common *)c_this->BoardCoords, i14, (int)sizeof
	(double));
	for (i14 = 0; i14 < 18; i14++) {
		c_this->BoardCoords->data[i14] = 0.0;
	}

	return c_this;
}

//
// Arguments    : const CheckerboardImage *b_this
//                float oldEnergy
// Return Type  : float
//
float CheckerboardImageComputeNewE1(const CheckerboardImage *b_this,
	float oldEnergy)
{
	float newEnergy;
	emxArray_real_T *r23;
	int loop_ub;
	int ix;
	int i29;
	emxArray_real_T *r24;
	int num;
	emxArray_real_T *b;
	emxArray_real_T *r25;
	emxArray_real_T *b_num;
	emxArray_real_T *r26;
	emxArray_real_T *denom;
	emxArray_real_T *c_num;
	emxArray_real_T *d_num;
	emxArray_real_T *r27;
	emxArray_real_T *b_denom;
	emxArray_real_T *c_denom;
	emxArray_real_T *varargin_2;
	emxArray_real_T *r28;
	double mtmp;
	double d1;
	int i;
	emxArray_real_T *e_num;
	emxArray_real_T *d_denom;
	emxArray_real_T *r29;
	int f_num[1];
	emxArray_real_T g_num;
	int e_denom[1];
	double y;
	emxInit_real_T(&r23, 3);

	// 'CheckerboardImage:430' num = squeeze(this.BoardCoords(idx(1),:,:) + this.BoardCoords(idx(3),:,:) ... 
	// 'CheckerboardImage:431'                 - 2*this.BoardCoords(idx(2),:,:));
	loop_ub = b_this->BoardCoords->size[1];
	ix = b_this->BoardCoords->size[2];
	i29 = r23->size[0] * r23->size[1] * r23->size[2];
	r23->size[0] = 1;
	r23->size[1] = loop_ub;
	r23->size[2] = ix;
	emxEnsureCapacity((emxArray__common *)r23, i29, (int)sizeof(double));
	for (i29 = 0; i29 < ix; i29++) {
		for (num = 0; num < loop_ub; num++) {
			r23->data[r23->size[0] * num + r23->size[0] * r23->size[1] * i29] =
				b_this->BoardCoords->data[b_this->BoardCoords->size[0] * num +
				b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * i29];
		}
	}

	emxInit_real_T(&r24, 3);
	loop_ub = b_this->BoardCoords->size[1];
	ix = b_this->BoardCoords->size[2];
	i29 = r24->size[0] * r24->size[1] * r24->size[2];
	r24->size[0] = 1;
	r24->size[1] = loop_ub;
	r24->size[2] = ix;
	emxEnsureCapacity((emxArray__common *)r24, i29, (int)sizeof(double));
	for (i29 = 0; i29 < ix; i29++) {
		for (num = 0; num < loop_ub; num++) {
			r24->data[r24->size[0] * num + r24->size[0] * r24->size[1] * i29] =
				b_this->BoardCoords->data[(b_this->BoardCoords->size[0] * num +
					b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * i29) + 2];
		}
	}

	emxInit_real_T(&b, 3);
	loop_ub = b_this->BoardCoords->size[1];
	ix = b_this->BoardCoords->size[2];
	i29 = b->size[0] * b->size[1] * b->size[2];
	b->size[0] = 1;
	b->size[1] = loop_ub;
	b->size[2] = ix;
	emxEnsureCapacity((emxArray__common *)b, i29, (int)sizeof(double));
	for (i29 = 0; i29 < ix; i29++) {
		for (num = 0; num < loop_ub; num++) {
			b->data[b->size[0] * num + b->size[0] * b->size[1] * i29] =
				b_this->BoardCoords->data[(b_this->BoardCoords->size[0] * num +
					b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * i29) + 1];
		}
	}

	emxInit_real_T(&r25, 3);
	i29 = r25->size[0] * r25->size[1] * r25->size[2];
	r25->size[0] = 1;
	r25->size[1] = r23->size[1];
	r25->size[2] = r23->size[2];
	emxEnsureCapacity((emxArray__common *)r25, i29, (int)sizeof(double));
	loop_ub = r23->size[0] * r23->size[1] * r23->size[2];
	for (i29 = 0; i29 < loop_ub; i29++) {
		r25->data[i29] = (r23->data[i29] + r24->data[i29]) - 2.0 * b->data[i29];
	}

	emxFree_real_T(&b);
	emxInit_real_T(&b_num, 2);
	squeeze(r25, b_num);

	// 'CheckerboardImage:432' denom = squeeze(this.BoardCoords(idx(1),:,:) - this.BoardCoords(idx(3),:,:)); 
	loop_ub = b_this->BoardCoords->size[1];
	ix = b_this->BoardCoords->size[2];
	i29 = r23->size[0] * r23->size[1] * r23->size[2];
	r23->size[0] = 1;
	r23->size[1] = loop_ub;
	r23->size[2] = ix;
	emxEnsureCapacity((emxArray__common *)r23, i29, (int)sizeof(double));
	emxFree_real_T(&r25);
	for (i29 = 0; i29 < ix; i29++) {
		for (num = 0; num < loop_ub; num++) {
			r23->data[r23->size[0] * num + r23->size[0] * r23->size[1] * i29] =
				b_this->BoardCoords->data[b_this->BoardCoords->size[0] * num +
				b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * i29];
		}
	}

	loop_ub = b_this->BoardCoords->size[1];
	ix = b_this->BoardCoords->size[2];
	i29 = r24->size[0] * r24->size[1] * r24->size[2];
	r24->size[0] = 1;
	r24->size[1] = loop_ub;
	r24->size[2] = ix;
	emxEnsureCapacity((emxArray__common *)r24, i29, (int)sizeof(double));
	for (i29 = 0; i29 < ix; i29++) {
		for (num = 0; num < loop_ub; num++) {
			r24->data[r24->size[0] * num + r24->size[0] * r24->size[1] * i29] =
				b_this->BoardCoords->data[(b_this->BoardCoords->size[0] * num +
					b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * i29) + 2];
		}
	}

	emxInit_real_T(&r26, 3);
	i29 = r26->size[0] * r26->size[1] * r26->size[2];
	r26->size[0] = 1;
	r26->size[1] = r23->size[1];
	r26->size[2] = r23->size[2];
	emxEnsureCapacity((emxArray__common *)r26, i29, (int)sizeof(double));
	loop_ub = r23->size[0] * r23->size[1] * r23->size[2];
	for (i29 = 0; i29 < loop_ub; i29++) {
		r26->data[i29] = r23->data[i29] - r24->data[i29];
	}

	emxFree_real_T(&r24);
	emxFree_real_T(&r23);
	emxInit_real_T(&denom, 2);
	emxInit_real_T(&c_num, 1);
	squeeze(r26, denom);

	// 'CheckerboardImage:433' newEnergy = max(oldEnergy, ...
	// 'CheckerboardImage:434'                 max(hypot(num(:, 1), num(:,2)) ./ hypot(denom(:, 1), denom(:, 2)))); 
	loop_ub = b_num->size[0];
	i29 = c_num->size[0];
	c_num->size[0] = loop_ub;
	emxEnsureCapacity((emxArray__common *)c_num, i29, (int)sizeof(double));
	emxFree_real_T(&r26);
	for (i29 = 0; i29 < loop_ub; i29++) {
		c_num->data[i29] = b_num->data[i29];
	}

	emxInit_real_T(&d_num, 1);
	loop_ub = b_num->size[0];
	i29 = d_num->size[0];
	d_num->size[0] = loop_ub;
	emxEnsureCapacity((emxArray__common *)d_num, i29, (int)sizeof(double));
	for (i29 = 0; i29 < loop_ub; i29++) {
		d_num->data[i29] = b_num->data[i29 + b_num->size[0]];
	}

	emxFree_real_T(&b_num);
	emxInit_real_T(&r27, 1);
	emxInit_real_T(&b_denom, 1);
	c_hypot(c_num, d_num, r27);
	loop_ub = denom->size[0];
	i29 = b_denom->size[0];
	b_denom->size[0] = loop_ub;
	emxEnsureCapacity((emxArray__common *)b_denom, i29, (int)sizeof(double));
	emxFree_real_T(&d_num);
	emxFree_real_T(&c_num);
	for (i29 = 0; i29 < loop_ub; i29++) {
		b_denom->data[i29] = denom->data[i29];
	}

	emxInit_real_T(&c_denom, 1);
	loop_ub = denom->size[0];
	i29 = c_denom->size[0];
	c_denom->size[0] = loop_ub;
	emxEnsureCapacity((emxArray__common *)c_denom, i29, (int)sizeof(double));
	for (i29 = 0; i29 < loop_ub; i29++) {
		c_denom->data[i29] = denom->data[i29 + denom->size[0]];
	}

	emxFree_real_T(&denom);
	emxInit_real_T(&varargin_2, 1);
	emxInit_real_T(&r28, 1);
	c_hypot(b_denom, c_denom, r28);
	rdivide(r27, r28, varargin_2);
	mtmp = varargin_2->data[0];
	emxFree_real_T(&c_denom);
	emxFree_real_T(&b_denom);
	emxFree_real_T(&r28);
	emxFree_real_T(&r27);
	if (varargin_2->size[0] > 1) {
		for (ix = 1; ix + 1 <= varargin_2->size[0]; ix++) {
			if (varargin_2->data[ix] > mtmp) {
				mtmp = varargin_2->data[ix];
			}
		}
	}

	emxFree_real_T(&varargin_2);
	if (oldEnergy > mtmp) {
		newEnergy = oldEnergy;
	}
	else {
		newEnergy = (float)mtmp;
	}

	// 'CheckerboardImage:436' for i = 1:size(this.BoardCoords, 2)-2
	d1 = (double)b_this->BoardCoords->size[1] - 2.0;
	i = 0;
	emxInit_real_T(&e_num, 3);
	emxInit_real_T(&d_denom, 3);
	emxInit_real_T(&r29, 3);
	while (i <= (int)d1 - 1) {
		// 'CheckerboardImage:437' num = this.BoardCoords(idx(1), i, :) + this.BoardCoords(idx(1), i+2, :)... 
		// 'CheckerboardImage:438'                     - 2*this.BoardCoords(idx(1), i+1, :); 
		loop_ub = b_this->BoardCoords->size[2];
		i29 = e_num->size[0] * e_num->size[1] * e_num->size[2];
		e_num->size[0] = 1;
		e_num->size[1] = 1;
		e_num->size[2] = loop_ub;
		emxEnsureCapacity((emxArray__common *)e_num, i29, (int)sizeof(double));
		for (i29 = 0; i29 < loop_ub; i29++) {
			e_num->data[e_num->size[0] * e_num->size[1] * i29] = b_this->
				BoardCoords->data[b_this->BoardCoords->size[0] * i + b_this->
				BoardCoords->size[0] * b_this->BoardCoords->size[1] * i29];
		}

		loop_ub = b_this->BoardCoords->size[2];
		i29 = r29->size[0] * r29->size[1] * r29->size[2];
		r29->size[0] = 1;
		r29->size[1] = 1;
		r29->size[2] = loop_ub;
		emxEnsureCapacity((emxArray__common *)r29, i29, (int)sizeof(double));
		for (i29 = 0; i29 < loop_ub; i29++) {
			r29->data[r29->size[0] * r29->size[1] * i29] = b_this->BoardCoords->
				data[b_this->BoardCoords->size[0] * ((int)((1.0 + (double)i) + 2.0) - 1)
				+ b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * i29];
		}

		loop_ub = b_this->BoardCoords->size[2];
		i29 = d_denom->size[0] * d_denom->size[1] * d_denom->size[2];
		d_denom->size[0] = 1;
		d_denom->size[1] = 1;
		d_denom->size[2] = loop_ub;
		emxEnsureCapacity((emxArray__common *)d_denom, i29, (int)sizeof(double));
		for (i29 = 0; i29 < loop_ub; i29++) {
			d_denom->data[d_denom->size[0] * d_denom->size[1] * i29] =
				b_this->BoardCoords->data[b_this->BoardCoords->size[0] * ((int)((1.0 +
				(double)i) + 1.0) - 1) + b_this->BoardCoords->size[0] *
				b_this->BoardCoords->size[1] * i29];
		}

		i29 = e_num->size[0] * e_num->size[1] * e_num->size[2];
		e_num->size[0] = 1;
		e_num->size[1] = 1;
		emxEnsureCapacity((emxArray__common *)e_num, i29, (int)sizeof(double));
		ix = e_num->size[0];
		num = e_num->size[1];
		loop_ub = e_num->size[2];
		loop_ub *= ix * num;
		for (i29 = 0; i29 < loop_ub; i29++) {
			e_num->data[i29] = (e_num->data[i29] + r29->data[i29]) - 2.0 *
				d_denom->data[i29];
		}

		// 'CheckerboardImage:439' denom = this.BoardCoords(idx(1), i, :) - this.BoardCoords(idx(1),i+2,:); 
		loop_ub = b_this->BoardCoords->size[2];
		i29 = d_denom->size[0] * d_denom->size[1] * d_denom->size[2];
		d_denom->size[0] = 1;
		d_denom->size[1] = 1;
		d_denom->size[2] = loop_ub;
		emxEnsureCapacity((emxArray__common *)d_denom, i29, (int)sizeof(double));
		for (i29 = 0; i29 < loop_ub; i29++) {
			d_denom->data[d_denom->size[0] * d_denom->size[1] * i29] =
				b_this->BoardCoords->data[b_this->BoardCoords->size[0] * i +
				b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * i29];
		}

		loop_ub = b_this->BoardCoords->size[2];
		i29 = r29->size[0] * r29->size[1] * r29->size[2];
		r29->size[0] = 1;
		r29->size[1] = 1;
		r29->size[2] = loop_ub;
		emxEnsureCapacity((emxArray__common *)r29, i29, (int)sizeof(double));
		for (i29 = 0; i29 < loop_ub; i29++) {
			r29->data[r29->size[0] * r29->size[1] * i29] = b_this->BoardCoords->
				data[b_this->BoardCoords->size[0] * ((int)((1.0 + (double)i) + 2.0) - 1)
				+ b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * i29];
		}

		i29 = d_denom->size[0] * d_denom->size[1] * d_denom->size[2];
		d_denom->size[0] = 1;
		d_denom->size[1] = 1;
		emxEnsureCapacity((emxArray__common *)d_denom, i29, (int)sizeof(double));
		ix = d_denom->size[0];
		num = d_denom->size[1];
		loop_ub = d_denom->size[2];
		loop_ub *= ix * num;
		for (i29 = 0; i29 < loop_ub; i29++) {
			d_denom->data[i29] -= r29->data[i29];
		}

		// 'CheckerboardImage:440' newEnergy = max(newEnergy, norm(num(:)) ./ norm(denom(:))); 
		f_num[0] = e_num->size[2];
		g_num = *e_num;
		g_num.size = (int *)&f_num;
		g_num.numDimensions = 1;
		mtmp = norm(&g_num);
		e_denom[0] = d_denom->size[2];
		g_num = *d_denom;
		g_num.size = (int *)&e_denom;
		g_num.numDimensions = 1;
		y = norm(&g_num);
		mtmp /= y;
		if (newEnergy > mtmp) {
		}
		else {
			newEnergy = (float)mtmp;
		}

		i++;
	}

	emxFree_real_T(&r29);
	emxFree_real_T(&d_denom);
	emxFree_real_T(&e_num);

	// 'CheckerboardImage:442' newEnergy = newEnergy * numel(this.BoardIdx) - numel(this.BoardIdx); 
	ix = b_this->BoardIdx->size[0] * b_this->BoardIdx->size[1];
	return newEnergy * (float)ix - (float)(b_this->BoardIdx->size[0] *
		b_this->BoardIdx->size[1]);
}

//
// Arguments    : CheckerboardImage *b_this
//                double direction
// Return Type  : void
//
void CheckerboardImageExpandBoard1(CheckerboardImage *b_this, double
	direction)
{
	float A;
	int nm1d2;
	float oldEnergy;
	emxArray_real_T *predictedPoints;
	emxArray_real_T *newIndices;
	emxArray_real_T *r9;
	emxArray_real_T *idx;
	emxArray_real_T *p2;
	emxArray_int32_T *r10;
	emxArray_int32_T *r11;
	emxArray_real_T *r12;
	emxArray_real_T *c_this;
	float newEnergy;
	int numRows;
	int numCols;
	CheckerboardImage *d_this;
	int anew;
	int n;
	double ndbl;
	double apnd;
	int i18;
	double cdiff;
	unsigned int absa;
	unsigned int absb;
	emxArray_real_T *e_this;
	int i19;
	int k;
	emxArray_real_T *b_p2;
	int i20;
	emxArray_real_T *r13;
	emxArray_real_T *r14;
	emxArray_real_T *b;
	emxArray_real_T *r15;
	emxArray_real_T *r16;
	emxArray_real_T *b_predictedPoints;
	emxArray_real_T *c_predictedPoints;
	emxArray_real_T *r17;
	emxArray_real_T *c_p2;
	emxArray_real_T *d_p2;
	emxArray_real_T *varargin_2;
	emxArray_real_T *r18;
	int i;
	emxArray_real_T *num;
	emxArray_real_T *denom;
	emxArray_real_T *r19;
	int b_num[1];
	emxArray_real_T c_num;
	int b_denom[1];

	// 'CheckerboardImage:311' oldEnergy = (this.Energy + numel(this.BoardIdx)) / numel(this.BoardIdx); 
	A = b_this->Energy + (float)(b_this->BoardIdx->size[0] * b_this->
		BoardIdx->size[1]);
	nm1d2 = b_this->BoardIdx->size[0] * b_this->BoardIdx->size[1];
	oldEnergy = A / (float)nm1d2;

	// 'CheckerboardImage:312' switch direction
	emxInit_real_T(&predictedPoints, 2);
	emxInit_real_T(&newIndices, 2);
	emxInit_real_T(&r9, 3);
	emxInit_real_T(&idx, 2);
	emxInit_real_T(&p2, 2);
	emxInit_int32_T(&r10, 1);
	emxInit_int32_T(&r11, 1);
	emxInit_real_T(&r12, 2);
	switch ((int)direction) {
	case 1:
		// 'CheckerboardImage:313' case 1 %'up'
		// 'CheckerboardImage:314' idx = 1:3;
		// 'CheckerboardImage:315' predictedPoints = predictPointsVertical(this, idx); 
		CheckerboardImagePredictPoint(b_this, predictedPoints);

		// 'CheckerboardImage:316' newIndices = findClosestIndices(this, predictedPoints); 
		CheckerboardImageFindClosest(b_this, predictedPoints, newIndices);

		// 'CheckerboardImage:317' [this.BoardIdx, this.BoardCoords] = expandBoardUp(this, newIndices); 
		CheckerboardImageExpandBoardUp(b_this, newIndices, predictedPoints, r9);
		i18 = b_this->BoardIdx->size[0] * b_this->BoardIdx->size[1];
		b_this->BoardIdx->size[0] = predictedPoints->size[0];
		b_this->BoardIdx->size[1] = predictedPoints->size[1];
		emxEnsureCapacity((emxArray__common *)b_this->BoardIdx, i18, (int)sizeof
		(double));
		n = predictedPoints->size[0] * predictedPoints->size[1];
		for (i18 = 0; i18 < n; i18++) {
			b_this->BoardIdx->data[i18] = predictedPoints->data[i18];
		}

		i18 = b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] *
			b_this->BoardCoords->size[2];
		b_this->BoardCoords->size[0] = r9->size[0];
		b_this->BoardCoords->size[1] = r9->size[1];
		b_this->BoardCoords->size[2] = r9->size[2];
		emxEnsureCapacity((emxArray__common *)b_this->BoardCoords, i18, (int)sizeof
		(double));
		n = r9->size[0] * r9->size[1] * r9->size[2];
		for (i18 = 0; i18 < n; i18++) {
			b_this->BoardCoords->data[i18] = r9->data[i18];
		}

		// 'CheckerboardImage:318' newEnergy = computeNewEnergyVertical(this, idx, oldEnergy); 
		newEnergy = CheckerboardImageComputeNewE1(b_this, oldEnergy);
		break;

	case 2:
		// 'CheckerboardImage:320' case 2 %'down'
		// 'CheckerboardImage:321' numRows = size(this.BoardCoords, 1);
		numRows = b_this->BoardCoords->size[0];

		// 'CheckerboardImage:322' idx = numRows:-1:numRows-2;
		if (numRows < (double)numRows - 2.0) {
			n = 0;
			anew = numRows;
			apnd = (double)numRows - 2.0;
		}
		else {
			anew = numRows;
			ndbl = floor((((double)numRows - 2.0) - (double)numRows) / -1.0 + 0.5);
			apnd = (double)numRows + -ndbl;
			cdiff = ((double)numRows - 2.0) - apnd;
			absa = (unsigned int)fabs((double)numRows);
			absb = (unsigned int)fabs((double)numRows - 2.0);
			if (absa >= absb) {
				absb = absa;
			}

			if (fabs(cdiff) < 4.4408920985006262E-16 * (double)absb) {
				ndbl++;
				apnd = (double)numRows - 2.0;
			}
			else if (cdiff > 0.0) {
				apnd = (double)numRows + -(ndbl - 1.0);
			}
			else {
				ndbl++;
			}

			if (ndbl >= 0.0) {
				n = (int)ndbl;
			}
			else {
				n = 0;
			}
		}

		i18 = idx->size[0] * idx->size[1];
		idx->size[0] = 1;
		idx->size[1] = n;
		emxEnsureCapacity((emxArray__common *)idx, i18, (int)sizeof(double));
		if (n > 0) {
			idx->data[0] = anew;
			if (n > 1) {
				idx->data[n - 1] = apnd;
				nm1d2 = (n - 1) / 2;
				for (k = 1; k < nm1d2; k++) {
					idx->data[k] = (double)anew + -(double)k;
					idx->data[(n - k) - 1] = apnd - (-(double)k);
				}

				if (nm1d2 << 1 == n - 1) {
					idx->data[nm1d2] = ((double)anew + apnd) / 2.0;
				}
				else {
					idx->data[nm1d2] = (double)anew + -(double)nm1d2;
					idx->data[nm1d2 + 1] = apnd - (-(double)nm1d2);
				}
			}
		}

		emxInit_real_T(&c_this, 3);

		// 'CheckerboardImage:323' predictedPoints = predictPointsVertical(this, idx); 
		d_this = b_this;

		// 'CheckerboardImage:354' p1 = squeeze(this.BoardCoords(idx(2), :, :));
		n = d_this->BoardCoords->size[1];
		anew = d_this->BoardCoords->size[2];
		k = (int)idx->data[1];
		i18 = c_this->size[0] * c_this->size[1] * c_this->size[2];
		c_this->size[0] = 1;
		c_this->size[1] = n;
		c_this->size[2] = anew;
		emxEnsureCapacity((emxArray__common *)c_this, i18, (int)sizeof(double));
		for (i18 = 0; i18 < anew; i18++) {
			for (i19 = 0; i19 < n; i19++) {
				c_this->data[c_this->size[0] * i19 + c_this->size[0] * c_this->size[1] *
					i18] = d_this->BoardCoords->data[((k + d_this->BoardCoords->size[0] *
						i19) + d_this->BoardCoords->size[0] * d_this->BoardCoords->size[1] *
						i18) - 1];
			}
		}

		emxInit_real_T(&e_this, 3);
		squeeze(c_this, predictedPoints);

		// 'CheckerboardImage:355' p2 = squeeze(this.BoardCoords(idx(1), :, :));
		n = d_this->BoardCoords->size[1];
		anew = d_this->BoardCoords->size[2];
		k = (int)idx->data[0];
		i18 = e_this->size[0] * e_this->size[1] * e_this->size[2];
		e_this->size[0] = 1;
		e_this->size[1] = n;
		e_this->size[2] = anew;
		emxEnsureCapacity((emxArray__common *)e_this, i18, (int)sizeof(double));
		emxFree_real_T(&c_this);
		for (i18 = 0; i18 < anew; i18++) {
			for (i19 = 0; i19 < n; i19++) {
				e_this->data[e_this->size[0] * i19 + e_this->size[0] * e_this->size[1] *
					i18] = d_this->BoardCoords->data[((k + d_this->BoardCoords->size[0] *
						i19) + d_this->BoardCoords->size[0] * d_this->BoardCoords->size[1] *
						i18) - 1];
			}
		}

		emxInit_real_T(&b_p2, 2);
		squeeze(e_this, p2);

		// 'CheckerboardImage:356' newPoints = p2 + p2 - p1;
		// 'CheckerboardImage:324' newIndices = findClosestIndices(this, predictedPoints); 
		i18 = b_p2->size[0] * b_p2->size[1];
		b_p2->size[0] = p2->size[0];
		b_p2->size[1] = p2->size[1];
		emxEnsureCapacity((emxArray__common *)b_p2, i18, (int)sizeof(double));
		n = p2->size[0] * p2->size[1];
		emxFree_real_T(&e_this);
		for (i18 = 0; i18 < n; i18++) {
			b_p2->data[i18] = (p2->data[i18] + p2->data[i18]) - predictedPoints->
				data[i18];
		}

		CheckerboardImageFindClosest(b_this, b_p2, newIndices);

		// 'CheckerboardImage:325' [this.BoardIdx, this.BoardCoords] = expandBoardDown(this, newIndices); 
		CheckerboardImageExpandBoard2(b_this, newIndices, predictedPoints, r9);
		i18 = b_this->BoardIdx->size[0] * b_this->BoardIdx->size[1];
		b_this->BoardIdx->size[0] = predictedPoints->size[0];
		b_this->BoardIdx->size[1] = predictedPoints->size[1];
		emxEnsureCapacity((emxArray__common *)b_this->BoardIdx, i18, (int)sizeof
		(double));
		n = predictedPoints->size[0] * predictedPoints->size[1];
		emxFree_real_T(&b_p2);
		for (i18 = 0; i18 < n; i18++) {
			b_this->BoardIdx->data[i18] = predictedPoints->data[i18];
		}

		i18 = b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] *
			b_this->BoardCoords->size[2];
		b_this->BoardCoords->size[0] = r9->size[0];
		b_this->BoardCoords->size[1] = r9->size[1];
		b_this->BoardCoords->size[2] = r9->size[2];
		emxEnsureCapacity((emxArray__common *)b_this->BoardCoords, i18, (int)sizeof
		(double));
		n = r9->size[0] * r9->size[1] * r9->size[2];
		for (i18 = 0; i18 < n; i18++) {
			b_this->BoardCoords->data[i18] = r9->data[i18];
		}

		// 'CheckerboardImage:326' idx = idx + 1;
		i18 = idx->size[0] * idx->size[1];
		idx->size[0] = 1;
		emxEnsureCapacity((emxArray__common *)idx, i18, (int)sizeof(double));
		k = idx->size[0];
		nm1d2 = idx->size[1];
		n = k * nm1d2;
		for (i18 = 0; i18 < n; i18++) {
			idx->data[i18]++;
		}

		// 'CheckerboardImage:327' newEnergy = computeNewEnergyVertical(this, idx, oldEnergy); 
		newEnergy = CheckerboardImageComputeNewE2(b_this, idx, oldEnergy);
		break;

	case 3:
		emxInit_real_T(&c_this, 3);

		// 'CheckerboardImage:329' case 3 %'left'
		// 'CheckerboardImage:330' idx = 1:3;
		// 'CheckerboardImage:331' predictedPoints = predictPointsHorizontal(this, idx); 
		d_this = b_this;

		// 'CheckerboardImage:361' p1 = squeeze(this.BoardCoords(:, idx(2), :));
		n = d_this->BoardCoords->size[0];
		anew = d_this->BoardCoords->size[2];
		i18 = c_this->size[0] * c_this->size[1] * c_this->size[2];
		c_this->size[0] = n;
		c_this->size[1] = 1;
		c_this->size[2] = anew;
		emxEnsureCapacity((emxArray__common *)c_this, i18, (int)sizeof(double));
		for (i18 = 0; i18 < anew; i18++) {
			for (i19 = 0; i19 < n; i19++) {
				c_this->data[i19 + c_this->size[0] * c_this->size[1] * i18] =
					d_this->BoardCoords->data[(i19 + d_this->BoardCoords->size[0]) +
					d_this->BoardCoords->size[0] * d_this->BoardCoords->size[1] * i18];
			}
		}

		emxInit_real_T(&e_this, 3);
		b_squeeze(c_this, predictedPoints);

		// 'CheckerboardImage:362' p2 = squeeze(this.BoardCoords(:, idx(1), :));
		n = d_this->BoardCoords->size[0];
		anew = d_this->BoardCoords->size[2];
		i18 = e_this->size[0] * e_this->size[1] * e_this->size[2];
		e_this->size[0] = n;
		e_this->size[1] = 1;
		e_this->size[2] = anew;
		emxEnsureCapacity((emxArray__common *)e_this, i18, (int)sizeof(double));
		emxFree_real_T(&c_this);
		for (i18 = 0; i18 < anew; i18++) {
			for (i19 = 0; i19 < n; i19++) {
				e_this->data[i19 + e_this->size[0] * e_this->size[1] * i18] =
					d_this->BoardCoords->data[i19 + d_this->BoardCoords->size[0] *
					d_this->BoardCoords->size[1] * i18];
			}
		}

		emxInit_real_T(&b_p2, 2);
		b_squeeze(e_this, p2);

		// 'CheckerboardImage:363' newPoints = p2 + p2 - p1;
		// 'CheckerboardImage:332' newIndices = findClosestIndices(this, predictedPoints); 
		i18 = b_p2->size[0] * b_p2->size[1];
		b_p2->size[0] = p2->size[0];
		b_p2->size[1] = p2->size[1];
		emxEnsureCapacity((emxArray__common *)b_p2, i18, (int)sizeof(double));
		n = p2->size[0] * p2->size[1];
		emxFree_real_T(&e_this);
		for (i18 = 0; i18 < n; i18++) {
			b_p2->data[i18] = (p2->data[i18] + p2->data[i18]) - predictedPoints->
				data[i18];
		}

		CheckerboardImageFindClosest(b_this, b_p2, newIndices);

		// 'CheckerboardImage:333' [this.BoardIdx, this.BoardCoords] = expandBoardLeft(this, newIndices); 
		d_this = b_this;

		// 'CheckerboardImage:406' newBoard = zeros(size(this.BoardIdx, 1), 1 + size(this.BoardIdx, 2)); 
		i18 = predictedPoints->size[0] * predictedPoints->size[1];
		predictedPoints->size[0] = d_this->BoardIdx->size[0];
		predictedPoints->size[1] = d_this->BoardIdx->size[1] + 1;
		emxEnsureCapacity((emxArray__common *)predictedPoints, i18, (int)sizeof
		(double));
		n = d_this->BoardIdx->size[0] * (d_this->BoardIdx->size[1] + 1);
		emxFree_real_T(&b_p2);
		for (i18 = 0; i18 < n; i18++) {
			predictedPoints->data[i18] = 0.0;
		}

		// 'CheckerboardImage:407' newBoard(:, 1) = indices;
		n = predictedPoints->size[0];
		i18 = r10->size[0];
		r10->size[0] = n;
		emxEnsureCapacity((emxArray__common *)r10, i18, (int)sizeof(int));
		for (i18 = 0; i18 < n; i18++) {
			r10->data[i18] = i18;
		}

		k = r10->size[0];
		for (i18 = 0; i18 < k; i18++) {
			predictedPoints->data[r10->data[i18]] = newIndices->data[i18];
		}

		// 'CheckerboardImage:408' newBoard(:, 2:end) = this.BoardIdx;
		if (2 > predictedPoints->size[1]) {
			i18 = 0;
		}
		else {
			i18 = 1;
		}

		n = d_this->BoardIdx->size[1];
		for (i19 = 0; i19 < n; i19++) {
			anew = d_this->BoardIdx->size[0];
			for (i20 = 0; i20 < anew; i20++) {
				predictedPoints->data[i20 + predictedPoints->size[0] * (i18 + i19)] =
					d_this->BoardIdx->data[i20 + d_this->BoardIdx->size[0] * i19];
			}
		}

		// 'CheckerboardImage:410' newBoardCoords = zeros(size(this.BoardCoords, 1), ... 
		// 'CheckerboardImage:411'                 size(this.BoardCoords, 2) + 1, size(this.BoardCoords, 3)); 
		i18 = r9->size[0] * r9->size[1] * r9->size[2];
		r9->size[0] = d_this->BoardCoords->size[0];
		r9->size[1] = d_this->BoardCoords->size[1] + 1;
		r9->size[2] = d_this->BoardCoords->size[2];
		emxEnsureCapacity((emxArray__common *)r9, i18, (int)sizeof(double));
		n = d_this->BoardCoords->size[0] * (d_this->BoardCoords->size[1] + 1) *
			d_this->BoardCoords->size[2];
		for (i18 = 0; i18 < n; i18++) {
			r9->data[i18] = 0.0;
		}

		// 'CheckerboardImage:412' newBoardCoords(:, 1, :) = this.Points(indices, :); 
		n = r9->size[0];
		i18 = r10->size[0];
		r10->size[0] = n;
		emxEnsureCapacity((emxArray__common *)r10, i18, (int)sizeof(int));
		for (i18 = 0; i18 < n; i18++) {
			r10->data[i18] = i18;
		}

		n = r9->size[2];
		i18 = r11->size[0];
		r11->size[0] = n;
		emxEnsureCapacity((emxArray__common *)r11, i18, (int)sizeof(int));
		for (i18 = 0; i18 < n; i18++) {
			r11->data[i18] = i18;
		}

		i18 = r12->size[0] * r12->size[1];
		r12->size[0] = newIndices->size[1];
		r12->size[1] = 2;
		emxEnsureCapacity((emxArray__common *)r12, i18, (int)sizeof(double));
		for (i18 = 0; i18 < 2; i18++) {
			n = newIndices->size[1];
			for (i19 = 0; i19 < n; i19++) {
				r12->data[i19 + r12->size[0] * i18] = d_this->Points->data[((int)
					newIndices->data[newIndices->size[0] * i19] + d_this->Points->size[0] *
					i18) - 1];
			}
		}

		k = r10->size[0];
		nm1d2 = r11->size[0];
		for (i18 = 0; i18 < nm1d2; i18++) {
			for (i19 = 0; i19 < k; i19++) {
				r9->data[r10->data[i19] + r9->size[0] * r9->size[1] * r11->data[i18]] =
					r12->data[i19 + k * i18];
			}
		}

		// 'CheckerboardImage:413' newBoardCoords(:, 2:end, :) = this.BoardCoords;
		if (2 > r9->size[1]) {
			i18 = 0;
		}
		else {
			i18 = 1;
		}

		n = d_this->BoardCoords->size[2];
		for (i19 = 0; i19 < n; i19++) {
			anew = d_this->BoardCoords->size[1];
			for (i20 = 0; i20 < anew; i20++) {
				nm1d2 = d_this->BoardCoords->size[0];
				for (k = 0; k < nm1d2; k++) {
					r9->data[(k + r9->size[0] * (i18 + i20)) + r9->size[0] * r9->size[1] *
						i19] = d_this->BoardCoords->data[(k + d_this->BoardCoords->size[0] *
							i20) + d_this->BoardCoords->size[0] * d_this->BoardCoords->size[1] *
						i19];
				}
			}
		}

		i18 = b_this->BoardIdx->size[0] * b_this->BoardIdx->size[1];
		b_this->BoardIdx->size[0] = predictedPoints->size[0];
		b_this->BoardIdx->size[1] = predictedPoints->size[1];
		emxEnsureCapacity((emxArray__common *)b_this->BoardIdx, i18, (int)sizeof
		(double));
		n = predictedPoints->size[0] * predictedPoints->size[1];
		for (i18 = 0; i18 < n; i18++) {
			b_this->BoardIdx->data[i18] = predictedPoints->data[i18];
		}

		i18 = b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] *
			b_this->BoardCoords->size[2];
		b_this->BoardCoords->size[0] = r9->size[0];
		b_this->BoardCoords->size[1] = r9->size[1];
		b_this->BoardCoords->size[2] = r9->size[2];
		emxEnsureCapacity((emxArray__common *)b_this->BoardCoords, i18, (int)sizeof
		(double));
		n = r9->size[0] * r9->size[1] * r9->size[2];
		for (i18 = 0; i18 < n; i18++) {
			b_this->BoardCoords->data[i18] = r9->data[i18];
		}

		// 'CheckerboardImage:334' newEnergy = computeNewEnergyHorizontal(this, idx, oldEnergy); 
		newEnergy = CheckerboardImageComputeNewE3(b_this, oldEnergy);
		break;

	case 4:
		// 'CheckerboardImage:336' case 4 %'right'
		// 'CheckerboardImage:337' numCols = size(this.BoardCoords, 2);
		numCols = b_this->BoardCoords->size[1];

		// 'CheckerboardImage:338' idx = numCols:-1:numCols-2;
		if (numCols < (double)numCols - 2.0) {
			n = 0;
			anew = numCols;
			apnd = (double)numCols - 2.0;
		}
		else {
			anew = numCols;
			ndbl = floor((((double)numCols - 2.0) - (double)numCols) / -1.0 + 0.5);
			apnd = (double)numCols + -ndbl;
			cdiff = ((double)numCols - 2.0) - apnd;
			absa = (unsigned int)fabs((double)numCols);
			absb = (unsigned int)fabs((double)numCols - 2.0);
			if (absa >= absb) {
				absb = absa;
			}

			if (fabs(cdiff) < 4.4408920985006262E-16 * (double)absb) {
				ndbl++;
				apnd = (double)numCols - 2.0;
			}
			else if (cdiff > 0.0) {
				apnd = (double)numCols + -(ndbl - 1.0);
			}
			else {
				ndbl++;
			}

			if (ndbl >= 0.0) {
				n = (int)ndbl;
			}
			else {
				n = 0;
			}
		}

		i18 = idx->size[0] * idx->size[1];
		idx->size[0] = 1;
		idx->size[1] = n;
		emxEnsureCapacity((emxArray__common *)idx, i18, (int)sizeof(double));
		if (n > 0) {
			idx->data[0] = anew;
			if (n > 1) {
				idx->data[n - 1] = apnd;
				nm1d2 = (n - 1) / 2;
				for (k = 1; k < nm1d2; k++) {
					idx->data[k] = (double)anew + -(double)k;
					idx->data[(n - k) - 1] = apnd - (-(double)k);
				}

				if (nm1d2 << 1 == n - 1) {
					idx->data[nm1d2] = ((double)anew + apnd) / 2.0;
				}
				else {
					idx->data[nm1d2] = (double)anew + -(double)nm1d2;
					idx->data[nm1d2 + 1] = apnd - (-(double)nm1d2);
				}
			}
		}

		emxInit_real_T(&c_this, 3);

		// 'CheckerboardImage:339' predictedPoints = predictPointsHorizontal(this, idx); 
		d_this = b_this;

		// 'CheckerboardImage:361' p1 = squeeze(this.BoardCoords(:, idx(2), :));
		n = d_this->BoardCoords->size[0];
		anew = d_this->BoardCoords->size[2];
		k = (int)idx->data[1];
		i18 = c_this->size[0] * c_this->size[1] * c_this->size[2];
		c_this->size[0] = n;
		c_this->size[1] = 1;
		c_this->size[2] = anew;
		emxEnsureCapacity((emxArray__common *)c_this, i18, (int)sizeof(double));
		for (i18 = 0; i18 < anew; i18++) {
			for (i19 = 0; i19 < n; i19++) {
				c_this->data[i19 + c_this->size[0] * c_this->size[1] * i18] =
					d_this->BoardCoords->data[(i19 + d_this->BoardCoords->size[0] * (k - 1))
					+ d_this->BoardCoords->size[0] * d_this->BoardCoords->size[1] * i18];
			}
		}

		emxInit_real_T(&e_this, 3);
		b_squeeze(c_this, predictedPoints);

		// 'CheckerboardImage:362' p2 = squeeze(this.BoardCoords(:, idx(1), :));
		n = d_this->BoardCoords->size[0];
		anew = d_this->BoardCoords->size[2];
		k = (int)idx->data[0];
		i18 = e_this->size[0] * e_this->size[1] * e_this->size[2];
		e_this->size[0] = n;
		e_this->size[1] = 1;
		e_this->size[2] = anew;
		emxEnsureCapacity((emxArray__common *)e_this, i18, (int)sizeof(double));
		emxFree_real_T(&c_this);
		for (i18 = 0; i18 < anew; i18++) {
			for (i19 = 0; i19 < n; i19++) {
				e_this->data[i19 + e_this->size[0] * e_this->size[1] * i18] =
					d_this->BoardCoords->data[(i19 + d_this->BoardCoords->size[0] * (k - 1))
					+ d_this->BoardCoords->size[0] * d_this->BoardCoords->size[1] * i18];
			}
		}

		emxInit_real_T(&b_p2, 2);
		b_squeeze(e_this, p2);

		// 'CheckerboardImage:363' newPoints = p2 + p2 - p1;
		// 'CheckerboardImage:340' newIndices = findClosestIndices(this, predictedPoints); 
		i18 = b_p2->size[0] * b_p2->size[1];
		b_p2->size[0] = p2->size[0];
		b_p2->size[1] = p2->size[1];
		emxEnsureCapacity((emxArray__common *)b_p2, i18, (int)sizeof(double));
		n = p2->size[0] * p2->size[1];
		emxFree_real_T(&e_this);
		for (i18 = 0; i18 < n; i18++) {
			b_p2->data[i18] = (p2->data[i18] + p2->data[i18]) - predictedPoints->
				data[i18];
		}

		CheckerboardImageFindClosest(b_this, b_p2, newIndices);

		// 'CheckerboardImage:341' [this.BoardIdx, this.BoardCoords] = expandBoardRight(this, newIndices); 
		d_this = b_this;

		// 'CheckerboardImage:418' newBoard = zeros(size(this.BoardIdx, 1), 1 + size(this.BoardIdx, 2)); 
		i18 = predictedPoints->size[0] * predictedPoints->size[1];
		predictedPoints->size[0] = d_this->BoardIdx->size[0];
		predictedPoints->size[1] = d_this->BoardIdx->size[1] + 1;
		emxEnsureCapacity((emxArray__common *)predictedPoints, i18, (int)sizeof
		(double));
		n = d_this->BoardIdx->size[0] * (d_this->BoardIdx->size[1] + 1);
		emxFree_real_T(&b_p2);
		for (i18 = 0; i18 < n; i18++) {
			predictedPoints->data[i18] = 0.0;
		}

		// 'CheckerboardImage:419' newBoard(:, end) = indices;
		n = predictedPoints->size[0];
		i18 = r10->size[0];
		r10->size[0] = n;
		emxEnsureCapacity((emxArray__common *)r10, i18, (int)sizeof(int));
		for (i18 = 0; i18 < n; i18++) {
			r10->data[i18] = i18;
		}

		k = r10->size[0];
		nm1d2 = predictedPoints->size[1] - 1;
		for (i18 = 0; i18 < k; i18++) {
			predictedPoints->data[r10->data[i18] + predictedPoints->size[0] * nm1d2] =
				newIndices->data[i18];
		}

		// 'CheckerboardImage:420' newBoard(:, 1:end-1) = this.BoardIdx;
		n = d_this->BoardIdx->size[1];
		for (i18 = 0; i18 < n; i18++) {
			anew = d_this->BoardIdx->size[0];
			for (i19 = 0; i19 < anew; i19++) {
				predictedPoints->data[i19 + predictedPoints->size[0] * i18] =
					d_this->BoardIdx->data[i19 + d_this->BoardIdx->size[0] * i18];
			}
		}

		// 'CheckerboardImage:422' newBoardCoords = zeros(size(this.BoardCoords, 1), ... 
		// 'CheckerboardImage:423'                 size(this.BoardCoords, 2) + 1, size(this.BoardCoords, 3)); 
		i18 = r9->size[0] * r9->size[1] * r9->size[2];
		r9->size[0] = d_this->BoardCoords->size[0];
		r9->size[1] = d_this->BoardCoords->size[1] + 1;
		r9->size[2] = d_this->BoardCoords->size[2];
		emxEnsureCapacity((emxArray__common *)r9, i18, (int)sizeof(double));
		n = d_this->BoardCoords->size[0] * (d_this->BoardCoords->size[1] + 1) *
			d_this->BoardCoords->size[2];
		for (i18 = 0; i18 < n; i18++) {
			r9->data[i18] = 0.0;
		}

		// 'CheckerboardImage:424' newBoardCoords(:, end, :) = this.Points(indices, :); 
		n = r9->size[0];
		i18 = r10->size[0];
		r10->size[0] = n;
		emxEnsureCapacity((emxArray__common *)r10, i18, (int)sizeof(int));
		for (i18 = 0; i18 < n; i18++) {
			r10->data[i18] = i18;
		}

		n = r9->size[2];
		i18 = r11->size[0];
		r11->size[0] = n;
		emxEnsureCapacity((emxArray__common *)r11, i18, (int)sizeof(int));
		for (i18 = 0; i18 < n; i18++) {
			r11->data[i18] = i18;
		}

		i18 = r12->size[0] * r12->size[1];
		r12->size[0] = newIndices->size[1];
		r12->size[1] = 2;
		emxEnsureCapacity((emxArray__common *)r12, i18, (int)sizeof(double));
		for (i18 = 0; i18 < 2; i18++) {
			n = newIndices->size[1];
			for (i19 = 0; i19 < n; i19++) {
				r12->data[i19 + r12->size[0] * i18] = d_this->Points->data[((int)
					newIndices->data[newIndices->size[0] * i19] + d_this->Points->size[0] *
					i18) - 1];
			}
		}

		k = r10->size[0];
		nm1d2 = r11->size[0];
		i18 = r9->size[1] - 1;
		for (i19 = 0; i19 < nm1d2; i19++) {
			for (i20 = 0; i20 < k; i20++) {
				r9->data[(r10->data[i20] + r9->size[0] * i18) + r9->size[0] * r9->size[1]
					* r11->data[i19]] = r12->data[i20 + k * i19];
			}
		}
		emxFree_real_T(&r12);
		emxFree_int32_T(&r11);
		emxFree_int32_T(&r10);

		// 'CheckerboardImage:425' newBoardCoords(:, 1:end-1, :) = this.BoardCoords; 
		n = d_this->BoardCoords->size[2];
		for (i18 = 0; i18 < n; i18++) {
			anew = d_this->BoardCoords->size[1];
			for (i19 = 0; i19 < anew; i19++) {
				nm1d2 = d_this->BoardCoords->size[0];
				for (i20 = 0; i20 < nm1d2; i20++) {
					r9->data[(i20 + r9->size[0] * i19) + r9->size[0] * r9->size[1] * i18] =
						d_this->BoardCoords->data[(i20 + d_this->BoardCoords->size[0] * i19)
						+ d_this->BoardCoords->size[0] * d_this->BoardCoords->size[1] * i18];
				}
			}
		}

		i18 = b_this->BoardIdx->size[0] * b_this->BoardIdx->size[1];
		b_this->BoardIdx->size[0] = predictedPoints->size[0];
		b_this->BoardIdx->size[1] = predictedPoints->size[1];
		emxEnsureCapacity((emxArray__common *)b_this->BoardIdx, i18, (int)sizeof
		(double));
		n = predictedPoints->size[0] * predictedPoints->size[1];
		for (i18 = 0; i18 < n; i18++) {
			b_this->BoardIdx->data[i18] = predictedPoints->data[i18];
		}

		i18 = b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] *
			b_this->BoardCoords->size[2];
		b_this->BoardCoords->size[0] = r9->size[0];
		b_this->BoardCoords->size[1] = r9->size[1];
		b_this->BoardCoords->size[2] = r9->size[2];
		emxEnsureCapacity((emxArray__common *)b_this->BoardCoords, i18, (int)sizeof
		(double));
		n = r9->size[0] * r9->size[1] * r9->size[2];
		for (i18 = 0; i18 < n; i18++) {
			b_this->BoardCoords->data[i18] = r9->data[i18];
		}

		// 'CheckerboardImage:342' idx = idx + 1;
		i18 = idx->size[0] * idx->size[1];
		idx->size[0] = 1;
		emxEnsureCapacity((emxArray__common *)idx, i18, (int)sizeof(double));
		k = idx->size[0];
		nm1d2 = idx->size[1];
		n = k * nm1d2;
		for (i18 = 0; i18 < n; i18++) {
			idx->data[i18]++;
		}

		emxInit_real_T(&r13, 3);

		// 'CheckerboardImage:343' newEnergy = computeNewEnergyHorizontal(this, idx, oldEnergy); 
		d_this = b_this;

		// ------------------------------------------------------------------
		//  find the nearest neighbor point in the direction of vector v
		//  compute normalized dot products
		//  dists is a linear combination of euclidean distances and
		//  "directional distances"
		//  eliminate points already in the board
		//  eliminate points "behind" the center
		//  find the nearest neighbor
		// ------------------------------------------------------------------
		//          function rot90(this, k)
		//              this.BoardIdx = rot90(this.BoardIdx, k);
		//              newBoardCoords1 = rot90(this.BoardCoords(:,:,1), k);
		//              newBoardCoords2 = rot90(this.BoardCoords(:,:,2), k);
		//              this.BoardCoords = cat(3, newBoardCoords1, newBoardCoords2); 
		//          end
		// ------------------------------------------------------------------
		//          function tf = isUpperLeftBlack(this, I)
		//              % check if the upper-left square of the board is black
		//
		//              % create a mask for the upper-left square
		//              upperLeftPolyX = [this.BoardCoords(1, 1, 1), ...
		//                  this.BoardCoords(1, 2, 1), this.BoardCoords(2, 2, 1), ... 
		//                  this.BoardCoords(2, 1, 1)];
		//              upperLeftPolyY = [this.BoardCoords(1, 1, 2), ...
		//                  this.BoardCoords(1, 2, 2), this.BoardCoords(2, 2, 2), ... 
		//                  this.BoardCoords(2, 1, 2)];
		//              upperLeftMask = poly2mask(upperLeftPolyX, upperLeftPolyY, ... 
		//                  size(I, 1), size(I, 2));
		//
		//              % create a mask for the square to the right of it
		//              nextSquarePolyX = [this.BoardCoords(1, 2, 1), ...
		//                  this.BoardCoords(1, 3, 1), this.BoardCoords(2, 3, 1), ... 
		//                  this.BoardCoords(2, 2, 1)];
		//              nextSquarePolyY = [this.BoardCoords(1, 2, 2), ...
		//                  this.BoardCoords(1, 3, 2), this.BoardCoords(2, 3, 2), ... 
		//                  this.BoardCoords(2, 2, 2)];
		//              nextSquareMask = poly2mask(nextSquarePolyX, nextSquarePolyY,... 
		//                  size(I, 1), size(I, 2));
		//
		//              % check if the first square is darker than the second
		//              tf = mean(I(upperLeftMask)) < mean(I(nextSquareMask));             
		//          end
		// ------------------------------------------------------------------
		// ------------------------------------------------------------------
		// directions = {'up', 'down', 'left', 'right'};
		// directions = [1 2 3 4];
		// ------------------------------------------------------------------
		// 'up'
		// 'down'
		// 'left'
		// 'right'
		// ------------------------------------------------------------------
		// 'up'
		// 'down'
		// 'left'
		// 'right'
		// ------------------------------------------------------------------
		// ------------------------------------------------------------------
		// ------------------------------------------------------------------
		//  returns indices of points closest to the predicted points
		// ------------------------------------------------------------------
		// ------------------------------------------------------------------
		// ------------------------------------------------------------------
		// ------------------------------------------------------------------
		// ------------------------------------------------------------------
		// ------------------------------------------------------------------
		// 'CheckerboardImage:447' num = squeeze(this.BoardCoords(:,idx(1),:) + this.BoardCoords(:,idx(3),:) ... 
		// 'CheckerboardImage:448'                 - 2*this.BoardCoords(:,idx(2),:)); 
		n = d_this->BoardCoords->size[0];
		anew = d_this->BoardCoords->size[2];
		k = (int)idx->data[0];
		i18 = r13->size[0] * r13->size[1] * r13->size[2];
		r13->size[0] = n;
		r13->size[1] = 1;
		r13->size[2] = anew;
		emxEnsureCapacity((emxArray__common *)r13, i18, (int)sizeof(double));
		for (i18 = 0; i18 < anew; i18++) {
			for (i19 = 0; i19 < n; i19++) {
				r13->data[i19 + r13->size[0] * r13->size[1] * i18] = d_this->
					BoardCoords->data[(i19 + d_this->BoardCoords->size[0] * (k - 1)) +
					d_this->BoardCoords->size[0] * d_this->BoardCoords->size[1] * i18];
			}
		}

		emxInit_real_T(&r14, 3);
		n = d_this->BoardCoords->size[0];
		anew = d_this->BoardCoords->size[2];
		k = (int)idx->data[2];
		i18 = r14->size[0] * r14->size[1] * r14->size[2];
		r14->size[0] = n;
		r14->size[1] = 1;
		r14->size[2] = anew;
		emxEnsureCapacity((emxArray__common *)r14, i18, (int)sizeof(double));
		for (i18 = 0; i18 < anew; i18++) {
			for (i19 = 0; i19 < n; i19++) {
				r14->data[i19 + r14->size[0] * r14->size[1] * i18] = d_this->
					BoardCoords->data[(i19 + d_this->BoardCoords->size[0] * (k - 1)) +
					d_this->BoardCoords->size[0] * d_this->BoardCoords->size[1] * i18];
			}
		}

		emxInit_real_T(&b, 3);
		n = d_this->BoardCoords->size[0];
		anew = d_this->BoardCoords->size[2];
		k = (int)idx->data[1];
		i18 = b->size[0] * b->size[1] * b->size[2];
		b->size[0] = n;
		b->size[1] = 1;
		b->size[2] = anew;
		emxEnsureCapacity((emxArray__common *)b, i18, (int)sizeof(double));
		for (i18 = 0; i18 < anew; i18++) {
			for (i19 = 0; i19 < n; i19++) {
				b->data[i19 + b->size[0] * b->size[1] * i18] = d_this->BoardCoords->
					data[(i19 + d_this->BoardCoords->size[0] * (k - 1)) +
					d_this->BoardCoords->size[0] * d_this->BoardCoords->size[1] * i18];
			}
		}

		emxInit_real_T(&r15, 3);
		i18 = r15->size[0] * r15->size[1] * r15->size[2];
		r15->size[0] = r13->size[0];
		r15->size[1] = 1;
		r15->size[2] = r13->size[2];
		emxEnsureCapacity((emxArray__common *)r15, i18, (int)sizeof(double));
		n = r13->size[0] * r13->size[1] * r13->size[2];
		for (i18 = 0; i18 < n; i18++) {
			r15->data[i18] = (r13->data[i18] + r14->data[i18]) - 2.0 * b->data[i18];
		}

		emxFree_real_T(&b);
		b_squeeze(r15, predictedPoints);

		// 'CheckerboardImage:449' denom = squeeze(this.BoardCoords(:,idx(1),:) - this.BoardCoords(:,idx(3),:)); 
		n = d_this->BoardCoords->size[0];
		anew = d_this->BoardCoords->size[2];
		k = (int)idx->data[0];
		i18 = r13->size[0] * r13->size[1] * r13->size[2];
		r13->size[0] = n;
		r13->size[1] = 1;
		r13->size[2] = anew;
		emxEnsureCapacity((emxArray__common *)r13, i18, (int)sizeof(double));
		emxFree_real_T(&r15);
		for (i18 = 0; i18 < anew; i18++) {
			for (i19 = 0; i19 < n; i19++) {
				r13->data[i19 + r13->size[0] * r13->size[1] * i18] = d_this->
					BoardCoords->data[(i19 + d_this->BoardCoords->size[0] * (k - 1)) +
					d_this->BoardCoords->size[0] * d_this->BoardCoords->size[1] * i18];
			}
		}

		n = d_this->BoardCoords->size[0];
		anew = d_this->BoardCoords->size[2];
		k = (int)idx->data[2];
		i18 = r14->size[0] * r14->size[1] * r14->size[2];
		r14->size[0] = n;
		r14->size[1] = 1;
		r14->size[2] = anew;
		emxEnsureCapacity((emxArray__common *)r14, i18, (int)sizeof(double));
		for (i18 = 0; i18 < anew; i18++) {
			for (i19 = 0; i19 < n; i19++) {
				r14->data[i19 + r14->size[0] * r14->size[1] * i18] = d_this->
					BoardCoords->data[(i19 + d_this->BoardCoords->size[0] * (k - 1)) +
					d_this->BoardCoords->size[0] * d_this->BoardCoords->size[1] * i18];
			}
		}

		emxInit_real_T(&r16, 3);
		i18 = r16->size[0] * r16->size[1] * r16->size[2];
		r16->size[0] = r13->size[0];
		r16->size[1] = 1;
		r16->size[2] = r13->size[2];
		emxEnsureCapacity((emxArray__common *)r16, i18, (int)sizeof(double));
		n = r13->size[0] * r13->size[1] * r13->size[2];
		for (i18 = 0; i18 < n; i18++) {
			r16->data[i18] = r13->data[i18] - r14->data[i18];
		}

		emxFree_real_T(&r14);
		emxFree_real_T(&r13);
		emxInit_real_T(&b_predictedPoints, 1);
		b_squeeze(r16, p2);

		// 'CheckerboardImage:450' newEnergy = max(oldEnergy, ...
		// 'CheckerboardImage:451'                 max(hypot(num(:, 1), num(:,2)) ./ hypot(denom(:, 1), denom(:, 2)))); 
		n = predictedPoints->size[0];
		i18 = b_predictedPoints->size[0];
		b_predictedPoints->size[0] = n;
		emxEnsureCapacity((emxArray__common *)b_predictedPoints, i18, (int)sizeof
		(double));
		emxFree_real_T(&r16);
		for (i18 = 0; i18 < n; i18++) {
			b_predictedPoints->data[i18] = predictedPoints->data[i18];
		}

		emxInit_real_T(&c_predictedPoints, 1);
		n = predictedPoints->size[0];
		i18 = c_predictedPoints->size[0];
		c_predictedPoints->size[0] = n;
		emxEnsureCapacity((emxArray__common *)c_predictedPoints, i18, (int)sizeof
		(double));
		for (i18 = 0; i18 < n; i18++) {
			c_predictedPoints->data[i18] = predictedPoints->data[i18 +
				predictedPoints->size[0]];
		}

		emxInit_real_T(&r17, 1);
		emxInit_real_T(&c_p2, 1);
		c_hypot(b_predictedPoints, c_predictedPoints, r17);
		n = p2->size[0];
		i18 = c_p2->size[0];
		c_p2->size[0] = n;
		emxEnsureCapacity((emxArray__common *)c_p2, i18, (int)sizeof(double));
		emxFree_real_T(&c_predictedPoints);
		emxFree_real_T(&b_predictedPoints);
		for (i18 = 0; i18 < n; i18++) {
			c_p2->data[i18] = p2->data[i18];
		}

		emxInit_real_T(&d_p2, 1);
		n = p2->size[0];
		i18 = d_p2->size[0];
		d_p2->size[0] = n;
		emxEnsureCapacity((emxArray__common *)d_p2, i18, (int)sizeof(double));
		for (i18 = 0; i18 < n; i18++) {
			d_p2->data[i18] = p2->data[i18 + p2->size[0]];
		}

		emxFree_real_T(&p2);

		emxInit_real_T(&varargin_2, 1);
		emxInit_real_T(&r18, 1);
		c_hypot(c_p2, d_p2, r18);
		rdivide(r17, r18, varargin_2);
		ndbl = varargin_2->data[0];
		emxFree_real_T(&d_p2);
		emxFree_real_T(&c_p2);
		emxFree_real_T(&r18);
		emxFree_real_T(&r17);
		if (varargin_2->size[0] > 1) {
			for (nm1d2 = 1; nm1d2 + 1 <= varargin_2->size[0]; nm1d2++) {
				if (varargin_2->data[nm1d2] > ndbl) {
					ndbl = varargin_2->data[nm1d2];
				}
			}
		}

		emxFree_real_T(&varargin_2);
		if (oldEnergy > ndbl) {
			newEnergy = oldEnergy;
		}
		else {
			newEnergy = (float)ndbl;
		}

		// 'CheckerboardImage:453' for i = 1:size(this.BoardCoords, 1)-2
		cdiff = (double)d_this->BoardCoords->size[0] - 2.0;
		i = 0;
		emxInit_real_T(&num, 3);
		emxInit_real_T(&denom, 3);
		emxInit_real_T(&r19, 3);
		while (i <= (int)cdiff - 1) {
			// 'CheckerboardImage:454' num = this.BoardCoords(i, idx(1), :) + this.BoardCoords(i+2, idx(1), :)... 
			// 'CheckerboardImage:455'                     - 2*this.BoardCoords(i+1, idx(1), :); 
			n = d_this->BoardCoords->size[2];
			k = (int)idx->data[0];
			i18 = num->size[0] * num->size[1] * num->size[2];
			num->size[0] = 1;
			num->size[1] = 1;
			num->size[2] = n;
			emxEnsureCapacity((emxArray__common *)num, i18, (int)sizeof(double));
			for (i18 = 0; i18 < n; i18++) {
				num->data[num->size[0] * num->size[1] * i18] = d_this->BoardCoords->
					data[(i + d_this->BoardCoords->size[0] * (k - 1)) +
					d_this->BoardCoords->size[0] * d_this->BoardCoords->size[1] * i18];
			}

			n = d_this->BoardCoords->size[2];
			k = (int)idx->data[0];
			i18 = r19->size[0] * r19->size[1] * r19->size[2];
			r19->size[0] = 1;
			r19->size[1] = 1;
			r19->size[2] = n;
			emxEnsureCapacity((emxArray__common *)r19, i18, (int)sizeof(double));
			for (i18 = 0; i18 < n; i18++) {
				r19->data[r19->size[0] * r19->size[1] * i18] = d_this->BoardCoords->
					data[(((int)((1.0 + (double)i) + 2.0) + d_this->BoardCoords->size[0] *
					(k - 1)) + d_this->BoardCoords->size[0] * d_this->
						BoardCoords->size[1] * i18) - 1];
			}

			n = d_this->BoardCoords->size[2];
			k = (int)idx->data[0];
			i18 = denom->size[0] * denom->size[1] * denom->size[2];
			denom->size[0] = 1;
			denom->size[1] = 1;
			denom->size[2] = n;
			emxEnsureCapacity((emxArray__common *)denom, i18, (int)sizeof(double));
			for (i18 = 0; i18 < n; i18++) {
				denom->data[denom->size[0] * denom->size[1] * i18] = d_this->
					BoardCoords->data[(((int)((1.0 + (double)i) + 1.0) +
						d_this->BoardCoords->size[0] * (k - 1)) +
						d_this->BoardCoords->size[0] * d_this->
						BoardCoords->size[1] * i18) - 1];
			}

			i18 = num->size[0] * num->size[1] * num->size[2];
			num->size[0] = 1;
			num->size[1] = 1;
			emxEnsureCapacity((emxArray__common *)num, i18, (int)sizeof(double));
			nm1d2 = num->size[0];
			k = num->size[1];
			anew = num->size[2];
			n = nm1d2 * k * anew;
			for (i18 = 0; i18 < n; i18++) {
				num->data[i18] = (num->data[i18] + r19->data[i18]) - 2.0 * denom->
					data[i18];
			}

			// 'CheckerboardImage:456' denom = this.BoardCoords(i, idx(1), :) - this.BoardCoords(i+2,idx(1),:); 
			n = d_this->BoardCoords->size[2];
			k = (int)idx->data[0];
			i18 = denom->size[0] * denom->size[1] * denom->size[2];
			denom->size[0] = 1;
			denom->size[1] = 1;
			denom->size[2] = n;
			emxEnsureCapacity((emxArray__common *)denom, i18, (int)sizeof(double));
			for (i18 = 0; i18 < n; i18++) {
				denom->data[denom->size[0] * denom->size[1] * i18] = d_this->
					BoardCoords->data[(i + d_this->BoardCoords->size[0] * (k - 1)) +
					d_this->BoardCoords->size[0] * d_this->BoardCoords->size[1] * i18];
			}

			n = d_this->BoardCoords->size[2];
			k = (int)idx->data[0];
			i18 = r19->size[0] * r19->size[1] * r19->size[2];
			r19->size[0] = 1;
			r19->size[1] = 1;
			r19->size[2] = n;
			emxEnsureCapacity((emxArray__common *)r19, i18, (int)sizeof(double));
			for (i18 = 0; i18 < n; i18++) {
				r19->data[r19->size[0] * r19->size[1] * i18] = d_this->BoardCoords->
					data[(((int)((1.0 + (double)i) + 2.0) + d_this->BoardCoords->size[0] *
					(k - 1)) + d_this->BoardCoords->size[0] * d_this->
						BoardCoords->size[1] * i18) - 1];
			}

			i18 = denom->size[0] * denom->size[1] * denom->size[2];
			denom->size[0] = 1;
			denom->size[1] = 1;
			emxEnsureCapacity((emxArray__common *)denom, i18, (int)sizeof(double));
			nm1d2 = denom->size[0];
			k = denom->size[1];
			anew = denom->size[2];
			n = nm1d2 * k * anew;
			for (i18 = 0; i18 < n; i18++) {
				denom->data[i18] -= r19->data[i18];
			}

			// 'CheckerboardImage:457' newEnergy = max(newEnergy, norm(num(:)) ./ norm(denom(:))); 
			b_num[0] = num->size[2];
			c_num = *num;
			c_num.size = (int *)&b_num;
			c_num.numDimensions = 1;
			ndbl = norm(&c_num);
			b_denom[0] = denom->size[2];
			c_num = *denom;
			c_num.size = (int *)&b_denom;
			c_num.numDimensions = 1;
			apnd = norm(&c_num);
			ndbl /= apnd;
			if (newEnergy > ndbl) {
			}
			else {
				newEnergy = (float)ndbl;
			}

			i++;
		}

		emxFree_real_T(&r19);
		emxFree_real_T(&denom);
		emxFree_real_T(&num);

		// 'CheckerboardImage:459' newEnergy = newEnergy * numel(this.BoardIdx) - numel(this.BoardIdx); 
		nm1d2 = d_this->BoardIdx->size[0] * d_this->BoardIdx->size[1];
		newEnergy = newEnergy * (float)nm1d2 - (float)(d_this->BoardIdx->size[0] *
			d_this->BoardIdx->size[1]);
		break;

	default:
		// 'CheckerboardImage:344' otherwise
		// 'CheckerboardImage:345' newEnergy = single(10e10);
		newEnergy = 1.0E+11F;
		break;
	}

	emxFree_real_T(&r12);
	emxFree_int32_T(&r11);
	emxFree_int32_T(&r10);
	emxFree_real_T(&p2);
	emxFree_real_T(&idx);
	emxFree_real_T(&r9);
	emxFree_real_T(&newIndices);
	emxFree_real_T(&predictedPoints);

	// 'CheckerboardImage:349' this.Energy = newEnergy;
	b_this->Energy = newEnergy;
}

//
// Arguments    : const CheckerboardImage *b_this
//                const emxArray_real_T *predictedPoints
//                emxArray_real_T *indices
// Return Type  : void
//
void CheckerboardImageFindClosest(const CheckerboardImage *b_this,
	const emxArray_real_T *predictedPoints, emxArray_real_T *indices)
{
	int i24;
	int csz_idx_0;
	int i;
	emxArray_real_T *p;
	emxArray_real32_T *diffs;
	emxArray_real32_T *a;
	emxArray_real32_T *av;
	emxArray_real32_T *cv;
	emxArray_real32_T *b_diffs;
	emxArray_real32_T *c_diffs;
	int na1;
	int k;
	int ak;
	int bk;
	int nc1;
	int ck;
	float mtmp;

	// 'CheckerboardImage:370' indices = zeros(1, size(predictedPoints, 1));
	i24 = indices->size[0] * indices->size[1];
	indices->size[0] = 1;
	indices->size[1] = predictedPoints->size[0];
	emxEnsureCapacity((emxArray__common *)indices, i24, (int)sizeof(double));
	csz_idx_0 = predictedPoints->size[0];
	for (i24 = 0; i24 < csz_idx_0; i24++) {
		indices->data[i24] = 0.0;
	}

	// 'CheckerboardImage:371' for i = 1:size(predictedPoints, 1)
	i = 0;
	emxInit_real_T(&p, 2);
	emxInit_real32_T(&diffs, 2);
	emxInit_real32_T(&a, 2);
	emxInit_real32_T(&av, 1);
	emxInit_real32_T(&cv, 1);
	emxInit_real32_T(&b_diffs, 1);
	emxInit_real32_T(&c_diffs, 1);
	while (i <= predictedPoints->size[0] - 1) {
		// 'CheckerboardImage:372' p = predictedPoints(i, :);
		csz_idx_0 = predictedPoints->size[1];
		i24 = p->size[0] * p->size[1];
		p->size[0] = 1;
		p->size[1] = csz_idx_0;
		emxEnsureCapacity((emxArray__common *)p, i24, (int)sizeof(double));
		for (i24 = 0; i24 < csz_idx_0; i24++) {
			p->data[p->size[0] * i24] = predictedPoints->data[i +
				predictedPoints->size[0] * i24];
		}

		// 'CheckerboardImage:373' diffs = bsxfun(@minus, this.Points, p);
		i24 = a->size[0] * a->size[1];
		a->size[0] = b_this->Points->size[0];
		a->size[1] = 2;
		emxEnsureCapacity((emxArray__common *)a, i24, (int)sizeof(float));
		csz_idx_0 = b_this->Points->size[0] * b_this->Points->size[1];
		for (i24 = 0; i24 < csz_idx_0; i24++) {
			a->data[i24] = b_this->Points->data[i24];
		}

		na1 = a->size[0];
		csz_idx_0 = a->size[0];
		i24 = diffs->size[0] * diffs->size[1];
		diffs->size[0] = csz_idx_0;
		diffs->size[1] = 2;
		emxEnsureCapacity((emxArray__common *)diffs, i24, (int)sizeof(float));
		if (diffs->size[0] == 0) {
		}
		else {
			k = a->size[0];
			i24 = av->size[0];
			av->size[0] = k;
			emxEnsureCapacity((emxArray__common *)av, i24, (int)sizeof(float));
			ak = 0;
			bk = 0;
			nc1 = diffs->size[0];
			i24 = (diffs->size[0] << 1) - diffs->size[0];
			for (ck = 0; ck <= i24; ck += nc1) {
				for (k = 0; k + 1 <= na1; k++) {
					av->data[k] = a->data[ak + k];
				}

				mtmp = (float)p->data[bk];
				k = cv->size[0];
				cv->size[0] = av->size[0];
				emxEnsureCapacity((emxArray__common *)cv, k, (int)sizeof(float));
				csz_idx_0 = av->size[0];
				for (k = 0; k < csz_idx_0; k++) {
					cv->data[k] = av->data[k] - mtmp;
				}

				for (k = 0; k + 1 <= nc1; k++) {
					diffs->data[ck + k] = cv->data[k];
				}

				ak += na1;
				bk++;
			}
		}

		// 'CheckerboardImage:374' dists = hypot(diffs(:, 1), diffs(:, 2));
		csz_idx_0 = diffs->size[0];
		i24 = b_diffs->size[0];
		b_diffs->size[0] = csz_idx_0;
		emxEnsureCapacity((emxArray__common *)b_diffs, i24, (int)sizeof(float));
		for (i24 = 0; i24 < csz_idx_0; i24++) {
			b_diffs->data[i24] = diffs->data[i24];
		}

		csz_idx_0 = diffs->size[0];
		i24 = c_diffs->size[0];
		c_diffs->size[0] = csz_idx_0;
		emxEnsureCapacity((emxArray__common *)c_diffs, i24, (int)sizeof(float));
		for (i24 = 0; i24 < csz_idx_0; i24++) {
			c_diffs->data[i24] = diffs->data[i24 + diffs->size[0]];
		}

		b_hypot(b_diffs, c_diffs, av);

		// 'CheckerboardImage:375' dists(indices(indices > 0)) = 10e10;
		csz_idx_0 = indices->size[1];
		for (k = 0; k < csz_idx_0; k++) {
			if ((int)indices->data[k] > 0) {
				av->data[(int)indices->data[k] - 1] = 1.0E+11F;
			}
		}

		// 'CheckerboardImage:376' [~, indices(i)] = min(dists);
		mtmp = av->data[0];
		csz_idx_0 = 0;
		if (av->size[0] > 1) {
			for (k = 1; k + 1 <= av->size[0]; k++) {
				if (av->data[k] < mtmp) {
					mtmp = av->data[k];
					csz_idx_0 = k;
				}
			}
		}

		indices->data[i] = csz_idx_0 + 1;

		// 'CheckerboardImage:376' ~
		i++;
	}

	emxFree_real32_T(&c_diffs);
	emxFree_real32_T(&b_diffs);
	emxFree_real32_T(&cv);
	emxFree_real32_T(&av);
	emxFree_real32_T(&a);
	emxFree_real32_T(&diffs);
	emxFree_real_T(&p);
}

//
// Arguments    : const CheckerboardImage *b_this
//                emxArray_real_T *newPoints
// Return Type  : void
//
void CheckerboardImagePredictPoint(const CheckerboardImage *b_this,
	emxArray_real_T *newPoints)
{
	emxArray_real_T *c_this;
	int loop_ub;
	int b_loop_ub;
	int i21;
	emxArray_real_T *p1;
	int i22;
	emxArray_real_T *d_this;
	emxInit_real_T(&c_this, 3);

	// 'CheckerboardImage:354' p1 = squeeze(this.BoardCoords(idx(2), :, :));
	loop_ub = b_this->BoardCoords->size[1];
	b_loop_ub = b_this->BoardCoords->size[2];
	i21 = c_this->size[0] * c_this->size[1] * c_this->size[2];
	c_this->size[0] = 1;
	c_this->size[1] = loop_ub;
	c_this->size[2] = b_loop_ub;
	emxEnsureCapacity((emxArray__common *)c_this, i21, (int)sizeof(double));
	for (i21 = 0; i21 < b_loop_ub; i21++) {
		for (i22 = 0; i22 < loop_ub; i22++) {
			c_this->data[c_this->size[0] * i22 + c_this->size[0] * c_this->size[1] *
				i21] = b_this->BoardCoords->data[(b_this->BoardCoords->size[0] * i22 +
					b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * i21) + 1];
		}
	}

	emxInit_real_T(&p1, 2);
	emxInit_real_T(&d_this, 3);
	squeeze(c_this, p1);
	emxFree_real_T(&c_this);

	// 'CheckerboardImage:355' p2 = squeeze(this.BoardCoords(idx(1), :, :));
	loop_ub = b_this->BoardCoords->size[1];
	b_loop_ub = b_this->BoardCoords->size[2];
	i21 = d_this->size[0] * d_this->size[1] * d_this->size[2];
	d_this->size[0] = 1;
	d_this->size[1] = loop_ub;
	d_this->size[2] = b_loop_ub;
	emxEnsureCapacity((emxArray__common *)d_this, i21, (int)sizeof(double));
	//emxFree_real_T(&c_this);
	for (i21 = 0; i21 < b_loop_ub; i21++) {
		for (i22 = 0; i22 < loop_ub; i22++) {
			d_this->data[d_this->size[0] * i22 + d_this->size[0] * d_this->size[1] *
				i21] = b_this->BoardCoords->data[b_this->BoardCoords->size[0] * i22 +
				b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * i21];
		}
	}

	squeeze(d_this, newPoints);
	emxFree_real_T(&d_this);

	// 'CheckerboardImage:356' newPoints = p2 + p2 - p1;
	i21 = newPoints->size[0] * newPoints->size[1];
	emxEnsureCapacity((emxArray__common *)newPoints, i21, (int)sizeof(double));
	loop_ub = newPoints->size[0];
	b_loop_ub = newPoints->size[1];
	loop_ub *= b_loop_ub;
	//emxFree_real_T(&d_this);
	for (i21 = 0; i21 < loop_ub; i21++) {
		newPoints->data[i21] = (newPoints->data[i21] + newPoints->data[i21]) -
			p1->data[i21];
	}

	emxFree_real_T(&p1);
}

//
// Arguments    : const emxArray_real_T *x
//                const emxArray_real_T *y
//                emxArray_real_T *r
// Return Type  : void
//
void c_hypot(const emxArray_real_T *x, const emxArray_real_T *y,
	emxArray_real_T *r)
{
	int k;
	int i30;
	if (x->size[0] <= y->size[0]) {
		k = x->size[0];
	}
	else {
		k = y->size[0];
	}

	i30 = r->size[0];
	r->size[0] = k;
	emxEnsureCapacity((emxArray__common *)r, i30, (int)sizeof(double));
	for (k = 0; k + 1 <= x->size[0]; k++) {
		r->data[k] = rt_hypotd(x->data[k], y->data[k]);
	}
}

//
// Arguments    : emxArray_real32_T *varargin_1
// Return Type  : void
//
void c_imfilter(emxArray_real32_T *varargin_1)
{
	double pad[2];
	emxArray_real32_T *A;
	int i;
	int iv3[2];
	int k;
	int lastRowA;
	int aidx;
	boolean_T b3;
	int cidx;
	int a;
	int b_a;
	int ma;
	int na;
	int firstColB;
	int lastColB;
	int lastRowB;
	int lastColA;
	int b_firstColB;
	int iC;
	int c;
	int iB;
	pad[0] = 0.0;
	pad[1] = 1.0;
	if ((varargin_1->size[0] == 0) || (varargin_1->size[1] == 0)) {
	}
	else {
		emxInit_real32_T(&A, 2);
		padImage(varargin_1, pad, A);
		for (i = 0; i < 2; i++) {
			pad[i] = ((double)A->size[i] - (1.0 + 2.0 * (double)i)) + 1.0;
		}

		for (k = 0; k < 2; k++) {
			if (0 >= (int)pad[k]) {
				iv3[k] = 0;
			}
			else {
				iv3[k] = (int)pad[k];
			}
		}

		i = varargin_1->size[0] * varargin_1->size[1];
		varargin_1->size[0] = iv3[0];
		varargin_1->size[1] = iv3[1];
		emxEnsureCapacity((emxArray__common *)varargin_1, i, (int)sizeof(float));
		lastRowA = iv3[1];
		for (i = 0; i < lastRowA; i++) {
			aidx = iv3[0];
			for (cidx = 0; cidx < aidx; cidx++) {
				varargin_1->data[cidx + varargin_1->size[0] * i] = 0.0F;
			}
		}

		if ((A->size[0] == 0) || (A->size[1] == 0) || ((iv3[0] == 0) || (iv3[1] == 0)))
		{
			b3 = true;
		}
		else {
			b3 = false;
		}

		if (!b3) {
			a = A->size[1];
			b_a = A->size[0];
			ma = A->size[0];
			na = A->size[1];
			if (A->size[1] < 2) {
				firstColB = 3 - A->size[1];
			}
			else {
				firstColB = 0;
			}

			if (3 <= A->size[1] - 1) {
				lastColB = 3;
			}
			else {
				lastColB = A->size[1];
			}

			if (1 <= A->size[0] - 1) {
				lastRowB = 1;
			}
			else {
				lastRowB = A->size[0];
			}

			while (firstColB <= lastColB - 1) {
				if ((firstColB + na) - 1 < a - 1) {
					lastColA = na;
				}
				else {
					lastColA = a - firstColB;
				}

				if (firstColB < 2) {
					k = 2 - firstColB;
				}
				else {
					k = 0;
				}

				while (k <= lastColA - 1) {
					if (firstColB + k > 2) {
						b_firstColB = (firstColB + k) - 2;
					}
					else {
						b_firstColB = 0;
					}

					iC = b_firstColB * A->size[0];
					c = k * ma;
					iB = firstColB + 1;
					i = 0;
					while (i <= lastRowB - 1) {
						if (ma <= b_a - 1) {
							lastRowA = ma;
						}
						else {
							lastRowA = b_a;
						}

						aidx = c;
						cidx = iC;
						for (i = 1; i <= lastRowA; i++) {
							varargin_1->data[cidx] += (-1.0F + ((float)iB - 1.0F)) * A->
								data[aidx];
							aidx++;
							cidx++;
						}

						iB++;
						iC++;
						i = 1;
					}

					k++;
				}

				firstColB++;
			}
		}

		emxFree_real32_T(&A);
	}
}


//
// Arguments    : int n
//                double a
//                const emxArray_real_T *x
//                int ix0
//                emxArray_real_T *y
//                int iy0
// Return Type  : void
//
void c_xaxpy(int n, double a, const emxArray_real_T *x, int ix0,
	emxArray_real_T *y, int iy0)
{
	int ix;
	int iy;
	int k;
	if ((n < 1) || (a == 0.0)) {
	}
	else {
		ix = ix0 - 1;
		iy = iy0 - 1;
		for (k = 0; k < n; k++) {
			y->data[iy] += a * x->data[ix];
			ix++;
			iy++;
		}
	}
}

//
// Arguments    : const emxArray_real_T *varargin_1
//                const emxArray_real_T *varargin_2
//                emxArray_real_T *y
// Return Type  : void
//
void cat(const emxArray_real_T *varargin_1, const emxArray_real_T
	*varargin_2, emxArray_real_T *y)
{
	unsigned int ysize_idx_0;
	unsigned int ysize_idx_1;
	int i37;
	int iy;
	int j;
	ysize_idx_0 = (unsigned int)varargin_1->size[0];
	ysize_idx_1 = (unsigned int)varargin_1->size[1];
	i37 = y->size[0] * y->size[1] * y->size[2];
	y->size[0] = (int)ysize_idx_0;
	y->size[1] = (int)ysize_idx_1;
	y->size[2] = 2;
	emxEnsureCapacity((emxArray__common *)y, i37, (int)sizeof(double));
	iy = -1;
	i37 = varargin_1->size[0] * varargin_1->size[1];
	for (j = 1; j <= i37; j++) {
		iy++;
		y->data[iy] = varargin_1->data[j - 1];
	}

	i37 = varargin_2->size[0] * varargin_2->size[1];
	for (j = 1; j <= i37; j++) {
		iy++;
		y->data[iy] = varargin_2->data[j - 1];
	}
}

//
// Arguments    : const CheckerboardImage *b_this
//                const emxArray_real_T *idx
//                float oldEnergy
// Return Type  : float
//
float CheckerboardImageComputeNewE2(const CheckerboardImage *b_this,
	const emxArray_real_T *idx, float oldEnergy)
{
	float newEnergy;
	emxArray_real_T *r33;
	int loop_ub;
	int ix;
	int b_idx;
	int i34;
	emxArray_real_T *r34;
	int num;
	emxArray_real_T *b;
	emxArray_real_T *r35;
	emxArray_real_T *b_num;
	emxArray_real_T *r36;
	emxArray_real_T *denom;
	emxArray_real_T *c_num;
	emxArray_real_T *d_num;
	emxArray_real_T *r37;
	emxArray_real_T *b_denom;
	emxArray_real_T *c_denom;
	emxArray_real_T *varargin_2;
	emxArray_real_T *r38;
	double mtmp;
	double d2;
	int i;
	emxArray_real_T *e_num;
	emxArray_real_T *d_denom;
	emxArray_real_T *r39;
	int f_num[1];
	emxArray_real_T g_num;
	int e_denom[1];
	double y;
	emxInit_real_T(&r33, 3);

	// 'CheckerboardImage:430' num = squeeze(this.BoardCoords(idx(1),:,:) + this.BoardCoords(idx(3),:,:) ... 
	// 'CheckerboardImage:431'                 - 2*this.BoardCoords(idx(2),:,:));
	loop_ub = b_this->BoardCoords->size[1];
	ix = b_this->BoardCoords->size[2];
	b_idx = (int)idx->data[0];
	i34 = r33->size[0] * r33->size[1] * r33->size[2];
	r33->size[0] = 1;
	r33->size[1] = loop_ub;
	r33->size[2] = ix;
	emxEnsureCapacity((emxArray__common *)r33, i34, (int)sizeof(double));
	for (i34 = 0; i34 < ix; i34++) {
		for (num = 0; num < loop_ub; num++) {
			r33->data[r33->size[0] * num + r33->size[0] * r33->size[1] * i34] =
				b_this->BoardCoords->data[((b_idx + b_this->BoardCoords->size[0] * num)
					+ b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * i34) - 1];
		}
	}

	emxInit_real_T(&r34, 3);
	loop_ub = b_this->BoardCoords->size[1];
	ix = b_this->BoardCoords->size[2];
	b_idx = (int)idx->data[2];
	i34 = r34->size[0] * r34->size[1] * r34->size[2];
	r34->size[0] = 1;
	r34->size[1] = loop_ub;
	r34->size[2] = ix;
	emxEnsureCapacity((emxArray__common *)r34, i34, (int)sizeof(double));
	for (i34 = 0; i34 < ix; i34++) {
		for (num = 0; num < loop_ub; num++) {
			r34->data[r34->size[0] * num + r34->size[0] * r34->size[1] * i34] =
				b_this->BoardCoords->data[((b_idx + b_this->BoardCoords->size[0] * num)
					+ b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * i34) - 1];
		}
	}

	emxInit_real_T(&b, 3);
	loop_ub = b_this->BoardCoords->size[1];
	ix = b_this->BoardCoords->size[2];
	b_idx = (int)idx->data[1];
	i34 = b->size[0] * b->size[1] * b->size[2];
	b->size[0] = 1;
	b->size[1] = loop_ub;
	b->size[2] = ix;
	emxEnsureCapacity((emxArray__common *)b, i34, (int)sizeof(double));
	for (i34 = 0; i34 < ix; i34++) {
		for (num = 0; num < loop_ub; num++) {
			b->data[b->size[0] * num + b->size[0] * b->size[1] * i34] =
				b_this->BoardCoords->data[((b_idx + b_this->BoardCoords->size[0] * num)
					+ b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * i34) - 1];
		}
	}

	emxInit_real_T(&r35, 3);
	i34 = r35->size[0] * r35->size[1] * r35->size[2];
	r35->size[0] = 1;
	r35->size[1] = r33->size[1];
	r35->size[2] = r33->size[2];
	emxEnsureCapacity((emxArray__common *)r35, i34, (int)sizeof(double));
	loop_ub = r33->size[0] * r33->size[1] * r33->size[2];
	for (i34 = 0; i34 < loop_ub; i34++) {
		r35->data[i34] = (r33->data[i34] + r34->data[i34]) - 2.0 * b->data[i34];
	}

	emxFree_real_T(&b);
	emxInit_real_T(&b_num, 2);
	squeeze(r35, b_num);
	emxFree_real_T(&r35);

	// 'CheckerboardImage:432' denom = squeeze(this.BoardCoords(idx(1),:,:) - this.BoardCoords(idx(3),:,:)); 
	loop_ub = b_this->BoardCoords->size[1];
	ix = b_this->BoardCoords->size[2];
	b_idx = (int)idx->data[0];
	i34 = r33->size[0] * r33->size[1] * r33->size[2];
	r33->size[0] = 1;
	r33->size[1] = loop_ub;
	r33->size[2] = ix;
	emxEnsureCapacity((emxArray__common *)r33, i34, (int)sizeof(double));
	
	for (i34 = 0; i34 < ix; i34++) {
		for (num = 0; num < loop_ub; num++) {
			r33->data[r33->size[0] * num + r33->size[0] * r33->size[1] * i34] =
				b_this->BoardCoords->data[((b_idx + b_this->BoardCoords->size[0] * num)
					+ b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * i34) - 1];
		}
	}

	loop_ub = b_this->BoardCoords->size[1];
	ix = b_this->BoardCoords->size[2];
	b_idx = (int)idx->data[2];
	i34 = r34->size[0] * r34->size[1] * r34->size[2];
	r34->size[0] = 1;
	r34->size[1] = loop_ub;
	r34->size[2] = ix;
	emxEnsureCapacity((emxArray__common *)r34, i34, (int)sizeof(double));
	for (i34 = 0; i34 < ix; i34++) {
		for (num = 0; num < loop_ub; num++) {
			r34->data[r34->size[0] * num + r34->size[0] * r34->size[1] * i34] =
				b_this->BoardCoords->data[((b_idx + b_this->BoardCoords->size[0] * num)
					+ b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * i34) - 1];
		}
	}

	emxInit_real_T(&r36, 3);
	i34 = r36->size[0] * r36->size[1] * r36->size[2];
	r36->size[0] = 1;
	r36->size[1] = r33->size[1];
	r36->size[2] = r33->size[2];
	emxEnsureCapacity((emxArray__common *)r36, i34, (int)sizeof(double));
	loop_ub = r33->size[0] * r33->size[1] * r33->size[2];
	for (i34 = 0; i34 < loop_ub; i34++) {
		r36->data[i34] = r33->data[i34] - r34->data[i34];
	}

	emxFree_real_T(&r34);
	emxFree_real_T(&r33);
	emxInit_real_T(&denom, 2);
	emxInit_real_T(&c_num, 1);
	squeeze(r36, denom);
	emxFree_real_T(&r36);

	// 'CheckerboardImage:433' newEnergy = max(oldEnergy, ...
	// 'CheckerboardImage:434'                 max(hypot(num(:, 1), num(:,2)) ./ hypot(denom(:, 1), denom(:, 2)))); 
	loop_ub = b_num->size[0];
	i34 = c_num->size[0];
	c_num->size[0] = loop_ub;
	emxEnsureCapacity((emxArray__common *)c_num, i34, (int)sizeof(double));
	//emxFree_real_T(&r36);
	for (i34 = 0; i34 < loop_ub; i34++) {
		c_num->data[i34] = b_num->data[i34];
	}

	emxInit_real_T(&d_num, 1);
	loop_ub = b_num->size[0];
	i34 = d_num->size[0];
	d_num->size[0] = loop_ub;
	emxEnsureCapacity((emxArray__common *)d_num, i34, (int)sizeof(double));
	for (i34 = 0; i34 < loop_ub; i34++) {
		d_num->data[i34] = b_num->data[i34 + b_num->size[0]];
	}

	emxFree_real_T(&b_num);
	emxInit_real_T(&r37, 1);
	emxInit_real_T(&b_denom, 1);
	c_hypot(c_num, d_num, r37);
	emxFree_real_T(&c_num);
	emxFree_real_T(&d_num);

	loop_ub = denom->size[0];
	i34 = b_denom->size[0];
	b_denom->size[0] = loop_ub;
	emxEnsureCapacity((emxArray__common *)b_denom, i34, (int)sizeof(double));
	//emxFree_real_T(&d_num);
	//emxFree_real_T(&c_num);
	for (i34 = 0; i34 < loop_ub; i34++) {
		b_denom->data[i34] = denom->data[i34];
	}

	emxInit_real_T(&c_denom, 1);
	loop_ub = denom->size[0];
	i34 = c_denom->size[0];
	c_denom->size[0] = loop_ub;
	emxEnsureCapacity((emxArray__common *)c_denom, i34, (int)sizeof(double));
	for (i34 = 0; i34 < loop_ub; i34++) {
		c_denom->data[i34] = denom->data[i34 + denom->size[0]];
	}

	emxFree_real_T(&denom);
	emxInit_real_T(&varargin_2, 1);
	emxInit_real_T(&r38, 1);
	c_hypot(b_denom, c_denom, r38);

	emxFree_real_T(&c_denom);
	emxFree_real_T(&b_denom);


	rdivide(r37, r38, varargin_2);
	emxFree_real_T(&r38);
	emxFree_real_T(&r37);

	mtmp = varargin_2->data[0];
	//emxFree_real_T(&c_denom);
	//emxFree_real_T(&b_denom);

	if (varargin_2->size[0] > 1) {
		for (ix = 1; ix + 1 <= varargin_2->size[0]; ix++) {
			if (varargin_2->data[ix] > mtmp) {
				mtmp = varargin_2->data[ix];
			}
		}
	}

	emxFree_real_T(&varargin_2);
	if (oldEnergy > mtmp) {
		newEnergy = oldEnergy;
	}
	else {
		newEnergy = (float)mtmp;
	}

	// 'CheckerboardImage:436' for i = 1:size(this.BoardCoords, 2)-2
	d2 = (double)b_this->BoardCoords->size[1] - 2.0;
	i = 0;
	emxInit_real_T(&e_num, 3);
	emxInit_real_T(&d_denom, 3);
	emxInit_real_T(&r39, 3);
	while (i <= (int)d2 - 1) {
		// 'CheckerboardImage:437' num = this.BoardCoords(idx(1), i, :) + this.BoardCoords(idx(1), i+2, :)... 
		// 'CheckerboardImage:438'                     - 2*this.BoardCoords(idx(1), i+1, :); 
		loop_ub = b_this->BoardCoords->size[2];
		b_idx = (int)idx->data[0];
		i34 = e_num->size[0] * e_num->size[1] * e_num->size[2];
		e_num->size[0] = 1;
		e_num->size[1] = 1;
		e_num->size[2] = loop_ub;
		emxEnsureCapacity((emxArray__common *)e_num, i34, (int)sizeof(double));
		for (i34 = 0; i34 < loop_ub; i34++) {
			e_num->data[e_num->size[0] * e_num->size[1] * i34] = b_this->
				BoardCoords->data[((b_idx + b_this->BoardCoords->size[0] * i) +
					b_this->BoardCoords->size[0] * b_this->
					BoardCoords->size[1] * i34) - 1];
		}

		loop_ub = b_this->BoardCoords->size[2];
		b_idx = (int)idx->data[0];
		i34 = r39->size[0] * r39->size[1] * r39->size[2];
		r39->size[0] = 1;
		r39->size[1] = 1;
		r39->size[2] = loop_ub;
		emxEnsureCapacity((emxArray__common *)r39, i34, (int)sizeof(double));
		for (i34 = 0; i34 < loop_ub; i34++) {
			r39->data[r39->size[0] * r39->size[1] * i34] = b_this->BoardCoords->data
				[((b_idx + b_this->BoardCoords->size[0] * ((int)((1.0 + (double)i) + 2.0)
					- 1)) + b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] *
					i34) - 1];
		}

		loop_ub = b_this->BoardCoords->size[2];
		b_idx = (int)idx->data[0];
		i34 = d_denom->size[0] * d_denom->size[1] * d_denom->size[2];
		d_denom->size[0] = 1;
		d_denom->size[1] = 1;
		d_denom->size[2] = loop_ub;
		emxEnsureCapacity((emxArray__common *)d_denom, i34, (int)sizeof(double));
		for (i34 = 0; i34 < loop_ub; i34++) {
			d_denom->data[d_denom->size[0] * d_denom->size[1] * i34] =
				b_this->BoardCoords->data[((b_idx + b_this->BoardCoords->size[0] * ((int)
				((1.0 + (double)i) + 1.0) - 1)) + b_this->BoardCoords->size[0] *
					b_this->BoardCoords->size[1] * i34) - 1];
		}

		i34 = e_num->size[0] * e_num->size[1] * e_num->size[2];
		e_num->size[0] = 1;
		e_num->size[1] = 1;
		emxEnsureCapacity((emxArray__common *)e_num, i34, (int)sizeof(double));
		ix = e_num->size[0];
		num = e_num->size[1];
		loop_ub = e_num->size[2];
		loop_ub *= ix * num;
		for (i34 = 0; i34 < loop_ub; i34++) {
			e_num->data[i34] = (e_num->data[i34] + r39->data[i34]) - 2.0 *
				d_denom->data[i34];
		}

		// 'CheckerboardImage:439' denom = this.BoardCoords(idx(1), i, :) - this.BoardCoords(idx(1),i+2,:); 
		loop_ub = b_this->BoardCoords->size[2];
		b_idx = (int)idx->data[0];
		i34 = d_denom->size[0] * d_denom->size[1] * d_denom->size[2];
		d_denom->size[0] = 1;
		d_denom->size[1] = 1;
		d_denom->size[2] = loop_ub;
		emxEnsureCapacity((emxArray__common *)d_denom, i34, (int)sizeof(double));
		for (i34 = 0; i34 < loop_ub; i34++) {
			d_denom->data[d_denom->size[0] * d_denom->size[1] * i34] =
				b_this->BoardCoords->data[((b_idx + b_this->BoardCoords->size[0] * i) +
					b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * i34) - 1];
		}

		loop_ub = b_this->BoardCoords->size[2];
		b_idx = (int)idx->data[0];
		i34 = r39->size[0] * r39->size[1] * r39->size[2];
		r39->size[0] = 1;
		r39->size[1] = 1;
		r39->size[2] = loop_ub;
		emxEnsureCapacity((emxArray__common *)r39, i34, (int)sizeof(double));
		for (i34 = 0; i34 < loop_ub; i34++) {
			r39->data[r39->size[0] * r39->size[1] * i34] = b_this->BoardCoords->data
				[((b_idx + b_this->BoardCoords->size[0] * ((int)((1.0 + (double)i) + 2.0)
					- 1)) + b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] *
					i34) - 1];
		}

		i34 = d_denom->size[0] * d_denom->size[1] * d_denom->size[2];
		d_denom->size[0] = 1;
		d_denom->size[1] = 1;
		emxEnsureCapacity((emxArray__common *)d_denom, i34, (int)sizeof(double));
		ix = d_denom->size[0];
		num = d_denom->size[1];
		loop_ub = d_denom->size[2];
		loop_ub *= ix * num;
		for (i34 = 0; i34 < loop_ub; i34++) {
			d_denom->data[i34] -= r39->data[i34];
		}

		// 'CheckerboardImage:440' newEnergy = max(newEnergy, norm(num(:)) ./ norm(denom(:))); 
		f_num[0] = e_num->size[2];
		g_num = *e_num;
		g_num.size = (int *)&f_num;
		g_num.numDimensions = 1;
		mtmp = norm(&g_num);
		e_denom[0] = d_denom->size[2];
		g_num = *d_denom;
		g_num.size = (int *)&e_denom;
		g_num.numDimensions = 1;
		y = norm(&g_num);
		mtmp /= y;
		if (newEnergy > mtmp) {
		}
		else {
			newEnergy = (float)mtmp;
		}

		i++;
	}

	emxFree_real_T(&r39);
	emxFree_real_T(&d_denom);
	emxFree_real_T(&e_num);

	// 'CheckerboardImage:442' newEnergy = newEnergy * numel(this.BoardIdx) - numel(this.BoardIdx); 
	ix = b_this->BoardIdx->size[0] * b_this->BoardIdx->size[1];
	return newEnergy * (float)ix - (float)(b_this->BoardIdx->size[0] *
		b_this->BoardIdx->size[1]);
}

//
// Arguments    : const CheckerboardImage *b_this
//                const emxArray_real_T *indices
//                emxArray_real_T *newBoard
//                emxArray_real_T *newBoardCoords
// Return Type  : void
//
void CheckerboardImageExpandBoard2(const CheckerboardImage *b_this,
	const emxArray_real_T *indices, emxArray_real_T *newBoard, emxArray_real_T
	*newBoardCoords)
{
	int i32;
	int loop_ub;
	int b_newBoard;
	int i33;
	emxArray_int32_T *r30;
	emxArray_int32_T *r31;
	emxArray_real_T *r32;
	int unnamed_idx_2;
	int b_newBoardCoords;

	// 'CheckerboardImage:394' newBoard = zeros(size(this.BoardIdx, 1)+1, size(this.BoardIdx, 2)); 
	i32 = newBoard->size[0] * newBoard->size[1];
	newBoard->size[0] = b_this->BoardIdx->size[0] + 1;
	newBoard->size[1] = b_this->BoardIdx->size[1];
	emxEnsureCapacity((emxArray__common *)newBoard, i32, (int)sizeof(double));
	loop_ub = (b_this->BoardIdx->size[0] + 1) * b_this->BoardIdx->size[1];
	for (i32 = 0; i32 < loop_ub; i32++) {
		newBoard->data[i32] = 0.0;
	}

	// 'CheckerboardImage:395' newBoard(end, :) = indices;
	b_newBoard = newBoard->size[0] - 1;
	loop_ub = indices->size[1];
	for (i32 = 0; i32 < loop_ub; i32++) {
		newBoard->data[b_newBoard + newBoard->size[0] * i32] = indices->data
			[indices->size[0] * i32];
	}

	// 'CheckerboardImage:396' newBoard(1:end-1, :) = this.BoardIdx;
	loop_ub = b_this->BoardIdx->size[1];
	for (i32 = 0; i32 < loop_ub; i32++) {
		b_newBoard = b_this->BoardIdx->size[0];
		for (i33 = 0; i33 < b_newBoard; i33++) {
			newBoard->data[i33 + newBoard->size[0] * i32] = b_this->BoardIdx->data[i33
				+ b_this->BoardIdx->size[0] * i32];
		}
	}

	// 'CheckerboardImage:398' newBoardCoords = zeros(size(this.BoardCoords, 1)+1, ... 
	// 'CheckerboardImage:399'                 size(this.BoardCoords, 2), size(this.BoardCoords, 3)); 
	i32 = newBoardCoords->size[0] * newBoardCoords->size[1] * newBoardCoords->
		size[2];
	newBoardCoords->size[0] = b_this->BoardCoords->size[0] + 1;
	newBoardCoords->size[1] = b_this->BoardCoords->size[1];
	newBoardCoords->size[2] = b_this->BoardCoords->size[2];
	emxEnsureCapacity((emxArray__common *)newBoardCoords, i32, (int)sizeof(double));
	loop_ub = (b_this->BoardCoords->size[0] + 1) * b_this->BoardCoords->size[1] *
		b_this->BoardCoords->size[2];
	for (i32 = 0; i32 < loop_ub; i32++) {
		newBoardCoords->data[i32] = 0.0;
	}

	emxInit_int32_T(&r30, 1);

	// 'CheckerboardImage:400' newBoardCoords(end, :, :) = this.Points(indices, :); 
	loop_ub = newBoardCoords->size[1];
	i32 = r30->size[0];
	r30->size[0] = loop_ub;
	emxEnsureCapacity((emxArray__common *)r30, i32, (int)sizeof(int));
	for (i32 = 0; i32 < loop_ub; i32++) {
		r30->data[i32] = i32;
	}

	emxInit_int32_T(&r31, 1);
	loop_ub = newBoardCoords->size[2];
	i32 = r31->size[0];
	r31->size[0] = loop_ub;
	emxEnsureCapacity((emxArray__common *)r31, i32, (int)sizeof(int));
	for (i32 = 0; i32 < loop_ub; i32++) {
		r31->data[i32] = i32;
	}

	emxInit_real_T(&r32, 2);
	i32 = r32->size[0] * r32->size[1];
	r32->size[0] = indices->size[1];
	r32->size[1] = 2;
	emxEnsureCapacity((emxArray__common *)r32, i32, (int)sizeof(double));
	for (i32 = 0; i32 < 2; i32++) {
		loop_ub = indices->size[1];
		for (i33 = 0; i33 < loop_ub; i33++) {
			r32->data[i33 + r32->size[0] * i32] = b_this->Points->data[((int)
				indices->data[indices->size[0] * i33] + b_this->Points->size[0] * i32) -
				1];
		}
	}

	b_newBoard = r30->size[0];
	unnamed_idx_2 = r31->size[0];
	b_newBoardCoords = newBoardCoords->size[0] - 1;
	for (i32 = 0; i32 < unnamed_idx_2; i32++) {
		for (i33 = 0; i33 < b_newBoard; i33++) {
			newBoardCoords->data[(b_newBoardCoords + newBoardCoords->size[0] *
				r30->data[i33]) + newBoardCoords->size[0] *
				newBoardCoords->size[1] * r31->data[i32]] = r32->data[i33 + b_newBoard *
				i32];
		}
	}

	emxFree_real_T(&r32);
	emxFree_int32_T(&r31);
	emxFree_int32_T(&r30);

	// 'CheckerboardImage:401' newBoardCoords(1:end-1, :, :) = this.BoardCoords;
	loop_ub = b_this->BoardCoords->size[2];
	for (i32 = 0; i32 < loop_ub; i32++) {
		b_newBoard = b_this->BoardCoords->size[1];
		for (i33 = 0; i33 < b_newBoard; i33++) {
			unnamed_idx_2 = b_this->BoardCoords->size[0];
			for (b_newBoardCoords = 0; b_newBoardCoords < unnamed_idx_2;
				b_newBoardCoords++) {
				newBoardCoords->data[(b_newBoardCoords + newBoardCoords->size[0] * i33)
					+ newBoardCoords->size[0] * newBoardCoords->size[1] * i32] =
					b_this->BoardCoords->data[(b_newBoardCoords + b_this->
						BoardCoords->size[0] * i33) + b_this->BoardCoords->size[0] *
					b_this->BoardCoords->size[1] * i32];
			}
		}
	}
}

//
// Arguments    : emxArray_real32_T *varargin_1
// Return Type  : void
//
void d_imfilter(emxArray_real32_T *varargin_1)
{
	double pad[2];
	double prePadSize[2];
	int firstRowA;
	emxArray_real32_T *a_prepad_tmp;
	emxArray_real32_T *A;
	int iv4[2];
	int k;
	int aidx;
	int cidx;
	boolean_T b4;
	int r;
	int a;
	int b_a;
	int ma;
	int na;
	int firstColB;
	int lastColB;
	int firstRowB;
	int lastRowB;
	int lastColA;
	int b_firstColB;
	int iC;
	int c;
	int i;
	int b_i;
	int a_length;
	pad[0] = 1.0;
	pad[1] = 1.0;
	if ((varargin_1->size[0] == 0) || (varargin_1->size[1] == 0)) {
	}
	else {
		for (firstRowA = 0; firstRowA < 2; firstRowA++) {
			prePadSize[firstRowA] = 0.0;
		}

		emxInit_real32_T(&a_prepad_tmp, 2);
		emxInit_real32_T(&A, 2);
		padarray(varargin_1, prePadSize, a_prepad_tmp);
		b_padarray(a_prepad_tmp, pad, A);
		emxFree_real32_T(&a_prepad_tmp);
		for (firstRowA = 0; firstRowA < 2; firstRowA++) {
			pad[firstRowA] = ((double)A->size[firstRowA] - 2.0) + 1.0;
		}

		for (k = 0; k < 2; k++) {
			if (0 >= (int)pad[k]) {
				iv4[k] = 0;
			}
			else {
				iv4[k] = (int)pad[k];
			}
		}

		firstRowA = varargin_1->size[0] * varargin_1->size[1];
		varargin_1->size[0] = iv4[0];
		varargin_1->size[1] = iv4[1];
		emxEnsureCapacity((emxArray__common *)varargin_1, firstRowA, (int)sizeof
		(float));
		aidx = iv4[1];
		for (firstRowA = 0; firstRowA < aidx; firstRowA++) {
			cidx = iv4[0];
			for (r = 0; r < cidx; r++) {
				varargin_1->data[r + varargin_1->size[0] * firstRowA] = 0.0F;
			}
		}

		if ((A->size[0] == 0) || (A->size[1] == 0) || ((iv4[0] == 0) || (iv4[1] == 0)))
		{
			b4 = true;
		}
		else {
			b4 = false;
		}

		if (!b4) {
			a = A->size[1];
			b_a = A->size[0];
			ma = A->size[0];
			na = A->size[1];
			if (A->size[1] < 1) {
				firstColB = 2;
			}
			else {
				firstColB = 0;
			}

			if (2 <= A->size[1] - 1) {
				lastColB = 2;
			}
			else {
				lastColB = A->size[1];
			}

			if (A->size[0] < 1) {
				firstRowB = 2;
			}
			else {
				firstRowB = 0;
			}

			if (2 <= A->size[0] - 1) {
				lastRowB = 2;
			}
			else {
				lastRowB = A->size[0];
			}

			while (firstColB <= lastColB - 1) {
				if ((firstColB + na) - 1 < a - 1) {
					lastColA = na;
				}
				else {
					lastColA = a - firstColB;
				}

				for (k = (firstColB < 1); k < lastColA; k++) {
					if (firstColB + k > 1) {
						b_firstColB = (firstColB + k) - 1;
					}
					else {
						b_firstColB = 0;
					}

					iC = b_firstColB * (A->size[0] - 1);
					c = k * ma;
					for (i = firstRowB; i < lastRowB; i++) {
						firstRowA = (i < 1);
						if (i + ma <= b_a - 1) {
							b_i = ma;
						}
						else {
							b_i = b_a - i;
						}

						a_length = b_i - firstRowA;
						aidx = c + firstRowA;
						cidx = iC;
						for (r = 1; r <= a_length; r++) {
							varargin_1->data[cidx] += 0.25F * A->data[aidx];
							aidx++;
							cidx++;
						}

						if (i >= 1) {
							iC++;
						}
					}
				}

				firstColB++;
			}
		}

		emxFree_real32_T(&A);
	}
}

//
// Arguments    : const emxArray_real32_T *I
//                double sigma
//                double peakThreshold
//                emxArray_real_T *points
//                double boardSize[2]
// Return Type  : void
//
void detectCheckerboardOnImage(const emxArray_real32_T *I, double sigma,
	double peakThreshold, emxArray_real_T *points, double boardSize[2])
{
	CheckerboardImage lobj_5;
	CheckerboardImage lobj_4;
	CheckerboardImage lobj_3;
	CheckerboardImage lobj_2;
	CheckerboardImage lobj_1;
	CheckerboardImage lobj_0;
	emxArray_real_T *G;
	emxArray_real32_T *Ig;
	int b;
	int loop_ub;
	emxArray_real32_T *Iy;
	emxArray_real32_T *I_45;
	emxArray_real32_T *Ixy;
	emxArray_real32_T *I_45_x;
	emxArray_real32_T *I_45_y;
	emxArray_real32_T *r0;
	emxArray_real32_T *b_Ig;
	emxArray_real32_T *b_b;
	double c;
	emxArray_real32_T *cxy;
	emxArray_int32_T *r1;
	int end;
	emxArray_real32_T *b_I_45_x;
	emxArray_real32_T *points0;
	emxArray_real32_T *varargin_1;
	emxArray_real32_T *varargin_2;
	int siz[2];
	emxArray_real_T *r2;
	emxArray_real32_T *b_cxy;
	CheckerboardImage *board0;
	emxArray_real32_T *c_I_45_x;
	CheckerboardImage *board45;
	//emxInitStruct_CheckerboardImage(&lobj_5);
	//emxInitStruct_CheckerboardImage(&lobj_4);
	//emxInitStruct_CheckerboardImage(&lobj_3);
	//emxInitStruct_CheckerboardImage(&lobj_2);
	//emxInitStruct_CheckerboardImage(&lobj_1);
	//emxInitStruct_CheckerboardImage(&lobj_0);
	emxInit_real_T(&G, 2);
	emxInit_real32_T(&Ig, 2);

	// 'detectCheckerboardOnImage:5' [cxy, c45, Ix, Iy] = secondDerivCornerMetricImage(I, sigma); 
	// 'secondDerivCornerMetricImage:3' assert(ismatrix(I));
	//  Low-pass filter the image
	// 'secondDerivCornerMetricImage:6' G = fspecial('gaussian', round(sigma * 7)+1, sigma); 
	fspecial(rt_roundd(sigma * 7.0) + 1.0, sigma, G);

	// 'secondDerivCornerMetricImage:7' Ig = imfilter(I, G, 'conv');
	b = Ig->size[0] * Ig->size[1];
	Ig->size[0] = I->size[0];
	Ig->size[1] = I->size[1];
	emxEnsureCapacity((emxArray__common *)Ig, b, (int)sizeof(float));
	loop_ub = I->size[0] * I->size[1];
	for (b = 0; b < loop_ub; b++) {
		Ig->data[b] = I->data[b];
	}

	emxInit_real32_T(&Iy, 2);
	imfilter(Ig, G);

	// 'secondDerivCornerMetricImage:9' derivFilter = [-1 0 1];
	//  first derivatives
	// 'secondDerivCornerMetricImage:12' Iy = imfilter(Ig, derivFilter', 'conv');
	b = Iy->size[0] * Iy->size[1];
	Iy->size[0] = Ig->size[0];
	Iy->size[1] = Ig->size[1];
	emxEnsureCapacity((emxArray__common *)Iy, b, (int)sizeof(float));
	loop_ub = Ig->size[0] * Ig->size[1];
	for (b = 0; b < loop_ub; b++) {
		Iy->data[b] = Ig->data[b];
	}

	emxInit_real32_T(&I_45, 2);
	b_imfilter(Iy);

	// 'secondDerivCornerMetricImage:13' Ix = imfilter(Ig, derivFilter, 'conv');
	c_imfilter(Ig);

	//  first derivative at 45 degrees
	// 'secondDerivCornerMetricImage:16' I_45 = Ix * cos(pi/4) + Iy * sin(pi/4);
	b = I_45->size[0] * I_45->size[1];
	I_45->size[0] = Ig->size[0];
	I_45->size[1] = Ig->size[1];
	emxEnsureCapacity((emxArray__common *)I_45, b, (int)sizeof(float));
	loop_ub = Ig->size[0] * Ig->size[1];
	for (b = 0; b < loop_ub; b++) {
		I_45->data[b] = Ig->data[b] * 0.707106769F + Iy->data[b] * 0.707106769F;
	}

	emxInit_real32_T(&Ixy, 2);

	// 'secondDerivCornerMetricImage:17' I_n45 = Ix * cos(-pi/4) + Iy * sin(-pi/4); 
	//  second derivative
	// 'secondDerivCornerMetricImage:20' Ixy = imfilter(Ix, derivFilter', 'conv'); 
	b = Ixy->size[0] * Ixy->size[1];
	Ixy->size[0] = Ig->size[0];
	Ixy->size[1] = Ig->size[1];
	emxEnsureCapacity((emxArray__common *)Ixy, b, (int)sizeof(float));
	loop_ub = Ig->size[0] * Ig->size[1];
	for (b = 0; b < loop_ub; b++) {
		Ixy->data[b] = Ig->data[b];
	}

	emxInit_real32_T(&I_45_x, 2);
	b_imfilter(Ixy);

	// 'secondDerivCornerMetricImage:22' I_45_x = imfilter(I_45, derivFilter, 'conv'); 
	b = I_45_x->size[0] * I_45_x->size[1];
	I_45_x->size[0] = I_45->size[0];
	I_45_x->size[1] = I_45->size[1];
	emxEnsureCapacity((emxArray__common *)I_45_x, b, (int)sizeof(float));
	loop_ub = I_45->size[0] * I_45->size[1];
	for (b = 0; b < loop_ub; b++) {
		I_45_x->data[b] = I_45->data[b];
	}

	emxInit_real32_T(&I_45_y, 2);
	c_imfilter(I_45_x);

	// 'secondDerivCornerMetricImage:23' I_45_y = imfilter(I_45, derivFilter', 'conv'); 
	b = I_45_y->size[0] * I_45_y->size[1];
	I_45_y->size[0] = I_45->size[0];
	I_45_y->size[1] = I_45->size[1];
	emxEnsureCapacity((emxArray__common *)I_45_y, b, (int)sizeof(float));
	loop_ub = I_45->size[0] * I_45->size[1];
	for (b = 0; b < loop_ub; b++) {
		I_45_y->data[b] = I_45->data[b];
	}

	emxInit_real32_T(&r0, 2);
	emxInit_real32_T(&b_Ig, 2);
	b_imfilter(I_45_y);

	// 'secondDerivCornerMetricImage:25' I_45_45 = I_45_x * cos(-pi/4) + I_45_y * sin(-pi/4); 
	//  suppress the outer corners
	// 'secondDerivCornerMetricImage:28' cxy = sigma^2 * abs(Ixy) - sigma * (abs(I_45) + abs(I_n45)); 
	b_abs(I_45, r0);
	b = b_Ig->size[0] * b_Ig->size[1];
	b_Ig->size[0] = Ig->size[0];
	b_Ig->size[1] = Ig->size[1];
	emxEnsureCapacity((emxArray__common *)b_Ig, b, (int)sizeof(float));
	loop_ub = Ig->size[0] * Ig->size[1];
	for (b = 0; b < loop_ub; b++) {
		b_Ig->data[b] = Ig->data[b] * 0.707106769F + Iy->data[b] * -0.707106769F;
	}

	emxInit_real32_T(&b_b, 2);
	b_abs(b_Ig, I_45);
	emxFree_real32_T(&b_Ig);
	c = sigma * sigma;
	b_abs(Ixy, b_b);
	b = b_b->size[0] * b_b->size[1];
	emxEnsureCapacity((emxArray__common *)b_b, b, (int)sizeof(float));
	loop_ub = b_b->size[0];
	b = b_b->size[1];
	loop_ub *= b;
	//emxFree_real32_T(&b_Ig);
	for (b = 0; b < loop_ub; b++) {
		b_b->data[b] *= (float)c;
	}

	b = r0->size[0] * r0->size[1];
	emxEnsureCapacity((emxArray__common *)r0, b, (int)sizeof(float));
	b = r0->size[0];
	loop_ub = r0->size[1];
	loop_ub *= b;
	for (b = 0; b < loop_ub; b++) {
		r0->data[b] = (float)sigma * (r0->data[b] + I_45->data[b]);
	}

	emxInit_real32_T(&cxy, 2);
	b = cxy->size[0] * cxy->size[1];
	cxy->size[0] = b_b->size[0];
	cxy->size[1] = b_b->size[1];
	emxEnsureCapacity((emxArray__common *)cxy, b, (int)sizeof(float));
	loop_ub = b_b->size[0] * b_b->size[1];
	for (b = 0; b < loop_ub; b++) {
		cxy->data[b] = b_b->data[b] - r0->data[b];
	}

	emxInit_int32_T(&r1, 1);

	// 'secondDerivCornerMetricImage:29' cxy(cxy < 0) = 0;
	b = G->size[0] * G->size[1];
	G->size[0] = b_b->size[0];
	G->size[1] = b_b->size[1];
	emxEnsureCapacity((emxArray__common *)G, b, (int)sizeof(double));
	loop_ub = b_b->size[0] * b_b->size[1];
	for (b = 0; b < loop_ub; b++) {
		G->data[b] = b_b->data[b] - r0->data[b];
	}

	end = G->size[0] * G->size[1] - 1;
	loop_ub = 0;
	for (b = 0; b <= end; b++) {
		if (b_b->data[b] - r0->data[b] < 0.0F) {
			loop_ub++;
		}
	}

	b = r1->size[0];
	r1->size[0] = loop_ub;
	emxEnsureCapacity((emxArray__common *)r1, b, (int)sizeof(int));
	loop_ub = 0;
	for (b = 0; b <= end; b++) {
		if (b_b->data[b] - r0->data[b] < 0.0F) {
			r1->data[loop_ub] = b + 1;
			loop_ub++;
		}
	}

	loop_ub = r1->size[0];
	for (b = 0; b < loop_ub; b++) {
		cxy->data[r1->data[b] - 1] = 0.0F;
	}

	emxInit_real32_T(&b_I_45_x, 2);

	// 'secondDerivCornerMetricImage:30' c45 = sigma^2 * abs(I_45_45) - sigma * (abs(Ix) + abs(Iy)); 
	b_abs(Ig, r0);
	b_abs(Iy, I_45);
	c = sigma * sigma;
	b = b_I_45_x->size[0] * b_I_45_x->size[1];
	b_I_45_x->size[0] = I_45_x->size[0];
	b_I_45_x->size[1] = I_45_x->size[1];
	emxEnsureCapacity((emxArray__common *)b_I_45_x, b, (int)sizeof(float));
	loop_ub = I_45_x->size[0] * I_45_x->size[1];
	for (b = 0; b < loop_ub; b++) {
		b_I_45_x->data[b] = I_45_x->data[b] * 0.707106769F + I_45_y->data[b] *
			-0.707106769F;
	}

	emxFree_real32_T(&I_45_y);
	b_abs(b_I_45_x, b_b);
	emxFree_real32_T(&b_I_45_x);
	b = b_b->size[0] * b_b->size[1];
	emxEnsureCapacity((emxArray__common *)b_b, b, (int)sizeof(float));
	loop_ub = b_b->size[0];
	b = b_b->size[1];
	loop_ub *= b;
	//emxFree_real32_T(&b_I_45_x);
	for (b = 0; b < loop_ub; b++) {
		b_b->data[b] *= (float)c;
	}

	b = r0->size[0] * r0->size[1];
	emxEnsureCapacity((emxArray__common *)r0, b, (int)sizeof(float));
	b = r0->size[0];
	loop_ub = r0->size[1];
	loop_ub *= b;
	for (b = 0; b < loop_ub; b++) {
		r0->data[b] = (float)sigma * (r0->data[b] + I_45->data[b]);
	}

	b = I_45_x->size[0] * I_45_x->size[1];
	I_45_x->size[0] = b_b->size[0];
	I_45_x->size[1] = b_b->size[1];
	emxEnsureCapacity((emxArray__common *)I_45_x, b, (int)sizeof(float));
	loop_ub = b_b->size[0] * b_b->size[1];
	for (b = 0; b < loop_ub; b++) {
		I_45_x->data[b] = b_b->data[b] - r0->data[b];
	}

	// 'secondDerivCornerMetricImage:31' c45(c45 < 0) = 0;
	b = G->size[0] * G->size[1];
	G->size[0] = b_b->size[0];
	G->size[1] = b_b->size[1];
	emxEnsureCapacity((emxArray__common *)G, b, (int)sizeof(double));
	loop_ub = b_b->size[0] * b_b->size[1];
	for (b = 0; b < loop_ub; b++) {
		G->data[b] = b_b->data[b] - r0->data[b];
	}

	end = G->size[0] * G->size[1] - 1;
	loop_ub = 0;
	emxFree_real_T(&G);
	for (b = 0; b <= end; b++) {
		if (b_b->data[b] - r0->data[b] < 0.0F) {
			loop_ub++;
		}
	}

	b = r1->size[0];
	r1->size[0] = loop_ub;
	emxEnsureCapacity((emxArray__common *)r1, b, (int)sizeof(int));
	loop_ub = 0;
	for (b = 0; b <= end; b++) {
		if (b_b->data[b] - r0->data[b] < 0.0F) {
			r1->data[loop_ub] = b + 1;
			loop_ub++;
		}
	}

	emxFree_real32_T(&b_b);
	emxFree_real32_T(&r0);
	loop_ub = r1->size[0];
	for (b = 0; b < loop_ub; b++) {
		I_45_x->data[r1->data[b] - 1] = 0.0F;
	}

	emxFree_int32_T(&r1);

	// 'detectCheckerboardOnImage:6' [Ix2, Iy2, Ixy] = computeJacobianEntries(Ix, Iy); 
	// 'computeJacobianEntries:3' Ix2 = Ix .^ 2;
	power(Ig, I_45);

	// 'computeJacobianEntries:4' Iy2 = Iy .^ 2;
	power(Iy, Ixy);

	// 'computeJacobianEntries:5' Ixy = Ix .* Iy;
	// 'computeJacobianEntries:7' G = fspecial('gaussian', 2, 15);
	// 'computeJacobianEntries:9' Ix2 = imfilter(Ix2, G);
	d_imfilter(I_45);

	// 'computeJacobianEntries:10' Iy2 = imfilter(Iy2, G);
	d_imfilter(Ixy);

	// 'computeJacobianEntries:11' Ixy = imfilter(Ixy, G);
	b = Ig->size[0] * Ig->size[1];
	emxEnsureCapacity((emxArray__common *)Ig, b, (int)sizeof(float));
	loop_ub = Ig->size[0];
	b = Ig->size[1];
	loop_ub *= b;
	for (b = 0; b < loop_ub; b++) {
		Ig->data[b] *= Iy->data[b];
	}

	emxFree_real32_T(&Iy);
	emxInit_real32_T(&points0, 2);
	emxInit_real32_T(&varargin_1, 1);
	d_imfilter(Ig);

	// 'detectCheckerboardOnImage:9' points0 = imgFindPeaks(cxy, peakThreshold);
	imgFindPeaks(cxy, peakThreshold, points0);

	// 'detectCheckerboardOnImage:10' scores0 = cxy(sub2ind(size(cxy), points0(:, 2), points0(:, 1))); 
	loop_ub = points0->size[0];
	b = varargin_1->size[0];
	varargin_1->size[0] = loop_ub;
	emxEnsureCapacity((emxArray__common *)varargin_1, b, (int)sizeof(float));
	for (b = 0; b < loop_ub; b++) {
		varargin_1->data[b] = points0->data[b + points0->size[0]];
	}

	emxInit_real32_T(&varargin_2, 1);
	loop_ub = points0->size[0];
	b = varargin_2->size[0];
	varargin_2->size[0] = loop_ub;
	emxEnsureCapacity((emxArray__common *)varargin_2, b, (int)sizeof(float));
	for (b = 0; b < loop_ub; b++) {
		varargin_2->data[b] = points0->data[b];
	}

	for (b = 0; b < 2; b++) {
		siz[b] = cxy->size[b];
	}

	emxInit_real_T(&r2, 1);
	b = r2->size[0];
	r2->size[0] = varargin_1->size[0];
	emxEnsureCapacity((emxArray__common *)r2, b, (int)sizeof(double));
	loop_ub = varargin_1->size[0];
	for (b = 0; b < loop_ub; b++) {
		r2->data[b] = (int)varargin_1->data[b] + siz[0] * ((int)varargin_2->data[b]
			- 1);
	}

	emxInit_real32_T(&b_cxy, 1);

	// 'detectCheckerboardOnImage:11' board0 = growCheckerboard(points0, scores0, Ix2, Iy2, Ixy, 0); 
	b = b_cxy->size[0];
	b_cxy->size[0] = r2->size[0];
	emxEnsureCapacity((emxArray__common *)b_cxy, b, (int)sizeof(float));
	loop_ub = r2->size[0];
	for (b = 0; b < loop_ub; b++) {
		b_cxy->data[b] = cxy->data[(int)r2->data[b] - 1];
	}
	emxInitStruct_CheckerboardImage(&lobj_2);
	emxInitStruct_CheckerboardImage(&lobj_1);
	emxInitStruct_CheckerboardImage(&lobj_0);

	board0 = growCheckerboard(points0, b_cxy, I_45, Ixy, Ig, 0.0, &lobj_0, &lobj_1,
		&lobj_2);


	emxFree_real32_T(&b_cxy);

	// 'detectCheckerboardOnImage:13' points45 = imgFindPeaks(c45, peakThreshold); 
	imgFindPeaks(I_45_x, peakThreshold, points0);

	// 'detectCheckerboardOnImage:14' scores45 = c45(sub2ind(size(c45), points45(:, 2), points45(:, 1))); 
	loop_ub = points0->size[0];
	b = varargin_1->size[0];
	varargin_1->size[0] = loop_ub;
	emxEnsureCapacity((emxArray__common *)varargin_1, b, (int)sizeof(float));
	//emxFree_real32_T(&b_cxy);
	for (b = 0; b < loop_ub; b++) {
		varargin_1->data[b] = points0->data[b + points0->size[0]];
	}

	loop_ub = points0->size[0];
	b = varargin_2->size[0];
	varargin_2->size[0] = loop_ub;
	emxEnsureCapacity((emxArray__common *)varargin_2, b, (int)sizeof(float));
	for (b = 0; b < loop_ub; b++) {
		varargin_2->data[b] = points0->data[b];
	}

	for (b = 0; b < 2; b++) {
		siz[b] = I_45_x->size[b];
	}

	b = r2->size[0];
	r2->size[0] = varargin_1->size[0];
	emxEnsureCapacity((emxArray__common *)r2, b, (int)sizeof(double));
	loop_ub = varargin_1->size[0];
	for (b = 0; b < loop_ub; b++) {
		r2->data[b] = (int)varargin_1->data[b] + siz[0] * ((int)varargin_2->data[b]
			- 1);
	}

	emxFree_real32_T(&varargin_2);
	emxFree_real32_T(&varargin_1);
	emxInit_real32_T(&c_I_45_x, 1);

	// 'detectCheckerboardOnImage:15' board45 = growCheckerboard(points45, scores45, Ix2, Iy2, Ixy, pi/4); 
	b = c_I_45_x->size[0];
	c_I_45_x->size[0] = r2->size[0];
	emxEnsureCapacity((emxArray__common *)c_I_45_x, b, (int)sizeof(float));
	loop_ub = r2->size[0];
	for (b = 0; b < loop_ub; b++) {
		c_I_45_x->data[b] = I_45_x->data[(int)r2->data[b] - 1];
	}

	emxFree_real_T(&r2);
	emxInitStruct_CheckerboardImage(&lobj_5);
	emxInitStruct_CheckerboardImage(&lobj_4);
	emxInitStruct_CheckerboardImage(&lobj_3);

	board45 = growCheckerboard(points0, c_I_45_x, I_45, Ixy, Ig,
		0.78539816339744828, &lobj_3, &lobj_4, &lobj_5);
	emxFree_real32_T(&c_I_45_x);
	emxFree_real32_T(&Ixy);
	emxFree_real32_T(&I_45);
	emxFree_real32_T(&Ig);
	emxFree_real32_T(&points0);


	// 'detectCheckerboardOnImage:17' points = [];
	b = points->size[0] * points->size[1];
	points->size[0] = 0;
	points->size[1] = 0;
	emxEnsureCapacity((emxArray__common *)points, b, (int)sizeof(double));

	// 'detectCheckerboardOnImage:18' boardSize = [0 0];
	//emxFree_real32_T(&c_I_45_x);
	/*emxFree_real32_T(&Ixy);
	emxFree_real32_T(&I_45);
	emxFree_real32_T(&Ig);
	emxFree_real32_T(&points0);*/
	for (b = 0; b < 2; b++) {
		boardSize[b] = 0.0;
	}

	// 'detectCheckerboardOnImage:19' if board0.isValid && board0.Energy < board45.Energy 
	if (board0->isValid && (board0->Energy < board45->Energy)) {
		emxFreeStruct_CheckerboardImage(&lobj_3);
		emxFreeStruct_CheckerboardImage(&lobj_4);
		emxFreeStruct_CheckerboardImage(&lobj_5);
		emxFree_real32_T(&I_45_x);
		// 'detectCheckerboardOnImage:20' board0 = orient(board0, I);
		board0 = orient(board0, I);

		// 'detectCheckerboardOnImage:21' [points, boardSize] = toPoints(board0);
		toPoints(board0, points, boardSize);

		emxFreeStruct_CheckerboardImage(&lobj_0);
		emxFreeStruct_CheckerboardImage(&lobj_1);
		emxFreeStruct_CheckerboardImage(&lobj_2);

		// 'detectCheckerboardOnImage:22' points = subPixelLocation(cxy, points);
		subPixelLocation(cxy, points);
		emxFree_real32_T(&cxy);
	}
	else {
		emxFreeStruct_CheckerboardImage(&lobj_0);
		emxFreeStruct_CheckerboardImage(&lobj_1);
		emxFreeStruct_CheckerboardImage(&lobj_2);
		emxFree_real32_T(&cxy);
		if (board45->isValid) {
			// 'detectCheckerboardOnImage:23' elseif board45.isValid
			// 'detectCheckerboardOnImage:24' board45 = orient(board45, I);
			board45 = orient(board45, I);

			// 'detectCheckerboardOnImage:25' [points, boardSize] = toPoints(board45); 
			toPoints(board45, points, boardSize);

			// 'detectCheckerboardOnImage:26' points = subPixelLocation(c45, points);
			subPixelLocation(I_45_x, points);
			
		}
		emxFree_real32_T(&I_45_x);

		emxFreeStruct_CheckerboardImage(&lobj_3);
		emxFreeStruct_CheckerboardImage(&lobj_4);
		emxFreeStruct_CheckerboardImage(&lobj_5);
	}

	//emxFree_real32_T(&I_45_x);
	//emxFree_real32_T(&cxy);
	//emxFreeStruct_CheckerboardImage(&lobj_0);
	//emxFreeStruct_CheckerboardImage(&lobj_1);
	//emxFreeStruct_CheckerboardImage(&lobj_2);
	//emxFreeStruct_CheckerboardImage(&lobj_3);
	//emxFreeStruct_CheckerboardImage(&lobj_4);
	//emxFreeStruct_CheckerboardImage(&lobj_5);
}

//
// Arguments    : const emxArray_real_T *v
//                emxArray_real_T *d
// Return Type  : void
//
void diag(const emxArray_real_T *v, emxArray_real_T *d)
{
	int j;
	int dlen;
	int stride;
	if ((v->size[0] == 1) && (v->size[1] == 1)) {
		j = d->size[0];
		d->size[0] = 1;
		emxEnsureCapacity((emxArray__common *)d, j, (int)sizeof(double));
		d->data[0] = v->data[0];
	}
	else {
		if (0 < v->size[1]) {
			if (v->size[0] <= v->size[1]) {
				dlen = v->size[0];
			}
			else {
				dlen = v->size[1];
			}

			stride = v->size[0] + 1;
		}
		else {
			dlen = 0;
			stride = 0;
		}

		j = d->size[0];
		d->size[0] = dlen;
		emxEnsureCapacity((emxArray__common *)d, j, (int)sizeof(double));
		for (j = 0; j + 1 <= dlen; j++) {
			d->data[j] = v->data[j * stride];
		}
	}
}

//
// Arguments    : int numerator
//                int denominator
// Return Type  : int
//
int div_s32_floor(int numerator, int denominator)
{
	int quotient;
	unsigned int absNumerator;
	unsigned int absDenominator;
	boolean_T quotientNeedsNegation;
	unsigned int tempAbsQuotient;
	if (denominator == 0) {
		if (numerator >= 0) {
			quotient = MAX_int;
		}
		else {
			quotient = MIN_int;
		}
	}
	else {
		if (numerator < 0) {
			absNumerator = ~(unsigned int)numerator + 1U;
		}
		else {
			absNumerator = (unsigned int)numerator;
		}

		if (denominator < 0) {
			absDenominator = ~(unsigned int)denominator + 1U;
		}
		else {
			absDenominator = (unsigned int)denominator;
		}

		quotientNeedsNegation = ((numerator < 0) != (denominator < 0));
		tempAbsQuotient = absNumerator / absDenominator;
		if (quotientNeedsNegation) {
			absNumerator %= absDenominator;
			if (absNumerator > 0U) {
				tempAbsQuotient++;
			}
		}

		if (quotientNeedsNegation) {
			quotient = -(int)tempAbsQuotient;
		}
		else {
			quotient = (int)tempAbsQuotient;
		}
	}

	return quotient;
}

//
// Arguments    : const CheckerboardImage *b_this
//                float oldEnergy
// Return Type  : float
//
float CheckerboardImageComputeNewE3(const CheckerboardImage *b_this,
	float oldEnergy)
{
	float newEnergy;
	emxArray_real_T *r40;
	int loop_ub;
	int ix;
	int i36;
	emxArray_real_T *r41;
	int num;
	emxArray_real_T *b;
	emxArray_real_T *r42;
	emxArray_real_T *b_num;
	emxArray_real_T *r43;
	emxArray_real_T *denom;
	emxArray_real_T *c_num;
	emxArray_real_T *d_num;
	emxArray_real_T *r44;
	emxArray_real_T *b_denom;
	emxArray_real_T *c_denom;
	emxArray_real_T *varargin_2;
	emxArray_real_T *r45;
	double mtmp;
	double d3;
	int i;
	emxArray_real_T *e_num;
	emxArray_real_T *d_denom;
	emxArray_real_T *r46;
	int f_num[1];
	emxArray_real_T g_num;
	int e_denom[1];
	double y;
	emxInit_real_T(&r40, 3);

	// ------------------------------------------------------------------
	//  find the nearest neighbor point in the direction of vector v
	//  compute normalized dot products
	//  dists is a linear combination of euclidean distances and
	//  "directional distances"
	//  eliminate points already in the board
	//  eliminate points "behind" the center
	//  find the nearest neighbor
	// ------------------------------------------------------------------
	//          function rot90(this, k)
	//              this.BoardIdx = rot90(this.BoardIdx, k);
	//              newBoardCoords1 = rot90(this.BoardCoords(:,:,1), k);
	//              newBoardCoords2 = rot90(this.BoardCoords(:,:,2), k);
	//              this.BoardCoords = cat(3, newBoardCoords1, newBoardCoords2);
	//          end
	// ------------------------------------------------------------------
	//          function tf = isUpperLeftBlack(this, I)
	//              % check if the upper-left square of the board is black
	//
	//              % create a mask for the upper-left square
	//              upperLeftPolyX = [this.BoardCoords(1, 1, 1), ...
	//                  this.BoardCoords(1, 2, 1), this.BoardCoords(2, 2, 1), ...
	//                  this.BoardCoords(2, 1, 1)];
	//              upperLeftPolyY = [this.BoardCoords(1, 1, 2), ...
	//                  this.BoardCoords(1, 2, 2), this.BoardCoords(2, 2, 2), ...
	//                  this.BoardCoords(2, 1, 2)];
	//              upperLeftMask = poly2mask(upperLeftPolyX, upperLeftPolyY, ...
	//                  size(I, 1), size(I, 2));
	//
	//              % create a mask for the square to the right of it
	//              nextSquarePolyX = [this.BoardCoords(1, 2, 1), ...
	//                  this.BoardCoords(1, 3, 1), this.BoardCoords(2, 3, 1), ...
	//                  this.BoardCoords(2, 2, 1)];
	//              nextSquarePolyY = [this.BoardCoords(1, 2, 2), ...
	//                  this.BoardCoords(1, 3, 2), this.BoardCoords(2, 3, 2), ...
	//                  this.BoardCoords(2, 2, 2)];
	//              nextSquareMask = poly2mask(nextSquarePolyX, nextSquarePolyY,... 
	//                  size(I, 1), size(I, 2));
	//
	//              % check if the first square is darker than the second
	//              tf = mean(I(upperLeftMask)) < mean(I(nextSquareMask));             
	//          end
	// ------------------------------------------------------------------
	// ------------------------------------------------------------------
	// directions = {'up', 'down', 'left', 'right'};
	// directions = [1 2 3 4];
	// ------------------------------------------------------------------
	// 'up'
	// 'down'
	// 'left'
	// 'right'
	// ------------------------------------------------------------------
	// 'up'
	// 'down'
	// 'left'
	// 'right'
	// ------------------------------------------------------------------
	// ------------------------------------------------------------------
	// ------------------------------------------------------------------
	//  returns indices of points closest to the predicted points
	// ------------------------------------------------------------------
	// ------------------------------------------------------------------
	// ------------------------------------------------------------------
	// ------------------------------------------------------------------
	// ------------------------------------------------------------------
	// ------------------------------------------------------------------
	// 'CheckerboardImage:447' num = squeeze(this.BoardCoords(:,idx(1),:) + this.BoardCoords(:,idx(3),:) ... 
	// 'CheckerboardImage:448'                 - 2*this.BoardCoords(:,idx(2),:));
	loop_ub = b_this->BoardCoords->size[0];
	ix = b_this->BoardCoords->size[2];
	i36 = r40->size[0] * r40->size[1] * r40->size[2];
	r40->size[0] = loop_ub;
	r40->size[1] = 1;
	r40->size[2] = ix;
	emxEnsureCapacity((emxArray__common *)r40, i36, (int)sizeof(double));
	for (i36 = 0; i36 < ix; i36++) {
		for (num = 0; num < loop_ub; num++) {
			r40->data[num + r40->size[0] * r40->size[1] * i36] = b_this->
				BoardCoords->data[num + b_this->BoardCoords->size[0] *
				b_this->BoardCoords->size[1] * i36];
		}
	}

	emxInit_real_T(&r41, 3);
	loop_ub = b_this->BoardCoords->size[0];
	ix = b_this->BoardCoords->size[2];
	i36 = r41->size[0] * r41->size[1] * r41->size[2];
	r41->size[0] = loop_ub;
	r41->size[1] = 1;
	r41->size[2] = ix;
	emxEnsureCapacity((emxArray__common *)r41, i36, (int)sizeof(double));
	for (i36 = 0; i36 < ix; i36++) {
		for (num = 0; num < loop_ub; num++) {
			r41->data[num + r41->size[0] * r41->size[1] * i36] = b_this->
				BoardCoords->data[(num + (b_this->BoardCoords->size[0] << 1)) +
				b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * i36];
		}
	}

	emxInit_real_T(&b, 3);
	loop_ub = b_this->BoardCoords->size[0];
	ix = b_this->BoardCoords->size[2];
	i36 = b->size[0] * b->size[1] * b->size[2];
	b->size[0] = loop_ub;
	b->size[1] = 1;
	b->size[2] = ix;
	emxEnsureCapacity((emxArray__common *)b, i36, (int)sizeof(double));
	for (i36 = 0; i36 < ix; i36++) {
		for (num = 0; num < loop_ub; num++) {
			b->data[num + b->size[0] * b->size[1] * i36] = b_this->BoardCoords->data
				[(num + b_this->BoardCoords->size[0]) + b_this->BoardCoords->size[0] *
				b_this->BoardCoords->size[1] * i36];
		}
	}

	emxInit_real_T(&r42, 3);
	i36 = r42->size[0] * r42->size[1] * r42->size[2];
	r42->size[0] = r40->size[0];
	r42->size[1] = 1;
	r42->size[2] = r40->size[2];
	emxEnsureCapacity((emxArray__common *)r42, i36, (int)sizeof(double));
	loop_ub = r40->size[0] * r40->size[1] * r40->size[2];
	for (i36 = 0; i36 < loop_ub; i36++) {
		r42->data[i36] = (r40->data[i36] + r41->data[i36]) - 2.0 * b->data[i36];
	}

	emxFree_real_T(&b);
	emxInit_real_T(&b_num, 2);
	b_squeeze(r42, b_num);
	emxFree_real_T(&r42);
	// 'CheckerboardImage:449' denom = squeeze(this.BoardCoords(:,idx(1),:) - this.BoardCoords(:,idx(3),:)); 
	loop_ub = b_this->BoardCoords->size[0];
	ix = b_this->BoardCoords->size[2];
	i36 = r40->size[0] * r40->size[1] * r40->size[2];
	r40->size[0] = loop_ub;
	r40->size[1] = 1;
	r40->size[2] = ix;
	emxEnsureCapacity((emxArray__common *)r40, i36, (int)sizeof(double));
	
	for (i36 = 0; i36 < ix; i36++) {
		for (num = 0; num < loop_ub; num++) {
			r40->data[num + r40->size[0] * r40->size[1] * i36] = b_this->
				BoardCoords->data[num + b_this->BoardCoords->size[0] *
				b_this->BoardCoords->size[1] * i36];
		}
	}

	loop_ub = b_this->BoardCoords->size[0];
	ix = b_this->BoardCoords->size[2];
	i36 = r41->size[0] * r41->size[1] * r41->size[2];
	r41->size[0] = loop_ub;
	r41->size[1] = 1;
	r41->size[2] = ix;
	emxEnsureCapacity((emxArray__common *)r41, i36, (int)sizeof(double));
	for (i36 = 0; i36 < ix; i36++) {
		for (num = 0; num < loop_ub; num++) {
			r41->data[num + r41->size[0] * r41->size[1] * i36] = b_this->
				BoardCoords->data[(num + (b_this->BoardCoords->size[0] << 1)) +
				b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * i36];
		}
	}

	emxInit_real_T(&r43, 3);
	i36 = r43->size[0] * r43->size[1] * r43->size[2];
	r43->size[0] = r40->size[0];
	r43->size[1] = 1;
	r43->size[2] = r40->size[2];
	emxEnsureCapacity((emxArray__common *)r43, i36, (int)sizeof(double));
	loop_ub = r40->size[0] * r40->size[1] * r40->size[2];
	for (i36 = 0; i36 < loop_ub; i36++) {
		r43->data[i36] = r40->data[i36] - r41->data[i36];
	}

	emxFree_real_T(&r41);
	emxFree_real_T(&r40);
	emxInit_real_T(&denom, 2);
	emxInit_real_T(&c_num, 1);
	b_squeeze(r43, denom);

	// 'CheckerboardImage:450' newEnergy = max(oldEnergy, ...
	// 'CheckerboardImage:451'                 max(hypot(num(:, 1), num(:,2)) ./ hypot(denom(:, 1), denom(:, 2)))); 
	loop_ub = b_num->size[0];
	i36 = c_num->size[0];
	c_num->size[0] = loop_ub;
	emxEnsureCapacity((emxArray__common *)c_num, i36, (int)sizeof(double));
	emxFree_real_T(&r43);
	for (i36 = 0; i36 < loop_ub; i36++) {
		c_num->data[i36] = b_num->data[i36];
	}

	emxInit_real_T(&d_num, 1);
	loop_ub = b_num->size[0];
	i36 = d_num->size[0];
	d_num->size[0] = loop_ub;
	emxEnsureCapacity((emxArray__common *)d_num, i36, (int)sizeof(double));
	for (i36 = 0; i36 < loop_ub; i36++) {
		d_num->data[i36] = b_num->data[i36 + b_num->size[0]];
	}

	emxFree_real_T(&b_num);
	emxInit_real_T(&r44, 1);
	emxInit_real_T(&b_denom, 1);
	c_hypot(c_num, d_num, r44);
	emxFree_real_T(&d_num);
	emxFree_real_T(&c_num);

	loop_ub = denom->size[0];
	i36 = b_denom->size[0];
	b_denom->size[0] = loop_ub;
	emxEnsureCapacity((emxArray__common *)b_denom, i36, (int)sizeof(double));
	
	
	for (i36 = 0; i36 < loop_ub; i36++) {
		b_denom->data[i36] = denom->data[i36];
	}

	emxInit_real_T(&c_denom, 1);
	loop_ub = denom->size[0];
	i36 = c_denom->size[0];
	c_denom->size[0] = loop_ub;
	emxEnsureCapacity((emxArray__common *)c_denom, i36, (int)sizeof(double));
	for (i36 = 0; i36 < loop_ub; i36++) {
		c_denom->data[i36] = denom->data[i36 + denom->size[0]];
	}

	emxFree_real_T(&denom);
	emxInit_real_T(&varargin_2, 1);
	emxInit_real_T(&r45, 1);
	c_hypot(b_denom, c_denom, r45);
	emxFree_real_T(&c_denom);
	emxFree_real_T(&b_denom);

	rdivide(r44, r45, varargin_2);
	emxFree_real_T(&r45);
	emxFree_real_T(&r44);


	mtmp = varargin_2->data[0];
	
	

	if (varargin_2->size[0] > 1) {
		for (ix = 1; ix + 1 <= varargin_2->size[0]; ix++) {
			if (varargin_2->data[ix] > mtmp) {
				mtmp = varargin_2->data[ix];
			}
		}
	}

	emxFree_real_T(&varargin_2);
	if (oldEnergy > mtmp) {
		newEnergy = oldEnergy;
	}
	else {
		newEnergy = (float)mtmp;
	}

	// 'CheckerboardImage:453' for i = 1:size(this.BoardCoords, 1)-2
	d3 = (double)b_this->BoardCoords->size[0] - 2.0;
	i = 0;
	emxInit_real_T(&e_num, 3);
	emxInit_real_T(&d_denom, 3);
	emxInit_real_T(&r46, 3);
	while (i <= (int)d3 - 1) {
		// 'CheckerboardImage:454' num = this.BoardCoords(i, idx(1), :) + this.BoardCoords(i+2, idx(1), :)... 
		// 'CheckerboardImage:455'                     - 2*this.BoardCoords(i+1, idx(1), :); 
		loop_ub = b_this->BoardCoords->size[2];
		i36 = e_num->size[0] * e_num->size[1] * e_num->size[2];
		e_num->size[0] = 1;
		e_num->size[1] = 1;
		e_num->size[2] = loop_ub;
		emxEnsureCapacity((emxArray__common *)e_num, i36, (int)sizeof(double));
		for (i36 = 0; i36 < loop_ub; i36++) {
			e_num->data[e_num->size[0] * e_num->size[1] * i36] = b_this->
				BoardCoords->data[i + b_this->BoardCoords->size[0] * b_this->
				BoardCoords->size[1] * i36];
		}

		loop_ub = b_this->BoardCoords->size[2];
		i36 = r46->size[0] * r46->size[1] * r46->size[2];
		r46->size[0] = 1;
		r46->size[1] = 1;
		r46->size[2] = loop_ub;
		emxEnsureCapacity((emxArray__common *)r46, i36, (int)sizeof(double));
		for (i36 = 0; i36 < loop_ub; i36++) {
			r46->data[r46->size[0] * r46->size[1] * i36] = b_this->BoardCoords->data
				[((int)((1.0 + (double)i) + 2.0) + b_this->BoardCoords->size[0] *
					b_this->BoardCoords->size[1] * i36) - 1];
		}

		loop_ub = b_this->BoardCoords->size[2];
		i36 = d_denom->size[0] * d_denom->size[1] * d_denom->size[2];
		d_denom->size[0] = 1;
		d_denom->size[1] = 1;
		d_denom->size[2] = loop_ub;
		emxEnsureCapacity((emxArray__common *)d_denom, i36, (int)sizeof(double));
		for (i36 = 0; i36 < loop_ub; i36++) {
			d_denom->data[d_denom->size[0] * d_denom->size[1] * i36] =
				b_this->BoardCoords->data[((int)((1.0 + (double)i) + 1.0) +
					b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] * i36) - 1];
		}

		i36 = e_num->size[0] * e_num->size[1] * e_num->size[2];
		e_num->size[0] = 1;
		e_num->size[1] = 1;
		emxEnsureCapacity((emxArray__common *)e_num, i36, (int)sizeof(double));
		ix = e_num->size[0];
		num = e_num->size[1];
		loop_ub = e_num->size[2];
		loop_ub *= ix * num;
		for (i36 = 0; i36 < loop_ub; i36++) {
			e_num->data[i36] = (e_num->data[i36] + r46->data[i36]) - 2.0 *
				d_denom->data[i36];
		}

		// 'CheckerboardImage:456' denom = this.BoardCoords(i, idx(1), :) - this.BoardCoords(i+2,idx(1),:); 
		loop_ub = b_this->BoardCoords->size[2];
		i36 = d_denom->size[0] * d_denom->size[1] * d_denom->size[2];
		d_denom->size[0] = 1;
		d_denom->size[1] = 1;
		d_denom->size[2] = loop_ub;
		emxEnsureCapacity((emxArray__common *)d_denom, i36, (int)sizeof(double));
		for (i36 = 0; i36 < loop_ub; i36++) {
			d_denom->data[d_denom->size[0] * d_denom->size[1] * i36] =
				b_this->BoardCoords->data[i + b_this->BoardCoords->size[0] *
				b_this->BoardCoords->size[1] * i36];
		}

		loop_ub = b_this->BoardCoords->size[2];
		i36 = r46->size[0] * r46->size[1] * r46->size[2];
		r46->size[0] = 1;
		r46->size[1] = 1;
		r46->size[2] = loop_ub;
		emxEnsureCapacity((emxArray__common *)r46, i36, (int)sizeof(double));
		for (i36 = 0; i36 < loop_ub; i36++) {
			r46->data[r46->size[0] * r46->size[1] * i36] = b_this->BoardCoords->data
				[((int)((1.0 + (double)i) + 2.0) + b_this->BoardCoords->size[0] *
					b_this->BoardCoords->size[1] * i36) - 1];
		}

		i36 = d_denom->size[0] * d_denom->size[1] * d_denom->size[2];
		d_denom->size[0] = 1;
		d_denom->size[1] = 1;
		emxEnsureCapacity((emxArray__common *)d_denom, i36, (int)sizeof(double));
		ix = d_denom->size[0];
		num = d_denom->size[1];
		loop_ub = d_denom->size[2];
		loop_ub *= ix * num;
		for (i36 = 0; i36 < loop_ub; i36++) {
			d_denom->data[i36] -= r46->data[i36];
		}

		// 'CheckerboardImage:457' newEnergy = max(newEnergy, norm(num(:)) ./ norm(denom(:))); 
		f_num[0] = e_num->size[2];
		g_num = *e_num;
		g_num.size = (int *)&f_num;
		g_num.numDimensions = 1;
		mtmp = norm(&g_num);
		e_denom[0] = d_denom->size[2];
		g_num = *d_denom;
		g_num.size = (int *)&e_denom;
		g_num.numDimensions = 1;
		y = norm(&g_num);
		mtmp /= y;
		if (newEnergy > mtmp) {
		}
		else {
			newEnergy = (float)mtmp;
		}

		i++;
	}

	emxFree_real_T(&r46);
	emxFree_real_T(&d_denom);
	emxFree_real_T(&e_num);

	// 'CheckerboardImage:459' newEnergy = newEnergy * numel(this.BoardIdx) - numel(this.BoardIdx); 
	ix = b_this->BoardIdx->size[0] * b_this->BoardIdx->size[1];
	return newEnergy * (float)ix - (float)(b_this->BoardIdx->size[0] *
		b_this->BoardIdx->size[1]);
}

//
// Arguments    : emxArray__common *emxArray
//                int oldNumel
//                int elementSize
// Return Type  : void
//
void emxEnsureCapacity(emxArray__common *emxArray, int oldNumel, int
	elementSize)
{
	int newNumel;
	int i;
	void *newData;
	newNumel = 1;
	for (i = 0; i < emxArray->numDimensions; i++) {
		newNumel *= emxArray->size[i];
	}

	if (newNumel > emxArray->allocatedSize) {
		i = emxArray->allocatedSize;
		if (i < 16) {
			i = 16;
		}

		while (i < newNumel) {
			if (i > 1073741823) {
				i = MAX_int;
			}
			else {
				i <<= 1;
			}
		}

		newData = calloc((unsigned int)i, (unsigned int)elementSize);
		if (emxArray->data != NULL) {
			memcpy(newData, emxArray->data, (unsigned int)(elementSize * oldNumel));
			if (emxArray->canFreeData) {
				free(emxArray->data);
			}
		}

		emxArray->data = newData;
		emxArray->allocatedSize = i;
		emxArray->canFreeData = true;
	}
}

//
// Arguments    : CheckerboardImage *pStruct
// Return Type  : void
//
void emxFreeStruct_CheckerboardImage(CheckerboardImage *pStruct)
{
	emxFree_real_T(&pStruct->BoardCoords);
	emxFree_real_T(&pStruct->BoardIdx);
	emxFree_real32_T(&pStruct->Points);
}

//
// Arguments    : emxArray_boolean_T **pEmxArray
// Return Type  : void
//
void emxFree_boolean_T(emxArray_boolean_T **pEmxArray)
{
	if (*pEmxArray != (emxArray_boolean_T *)NULL) {
		if (((*pEmxArray)->data != (boolean_T *)NULL) && (*pEmxArray)->canFreeData)
		{
			free((void *)(*pEmxArray)->data);
		}

		free((void *)(*pEmxArray)->size);
		free((void *)*pEmxArray);
		*pEmxArray = (emxArray_boolean_T *)NULL;
	}
}

//
// Arguments    : emxArray_int32_T **pEmxArray
// Return Type  : void
//
void emxFree_int32_T(emxArray_int32_T **pEmxArray)
{
	if (*pEmxArray != (emxArray_int32_T *)NULL) {
		if (((*pEmxArray)->data != (int *)NULL) && (*pEmxArray)->canFreeData) {
			free((void *)(*pEmxArray)->data);
		}

		free((void *)(*pEmxArray)->size);
		free((void *)*pEmxArray);
		*pEmxArray = (emxArray_int32_T *)NULL;
	}
}

//
// Arguments    : emxArray_real32_T **pEmxArray
// Return Type  : void
//
void emxFree_real32_T(emxArray_real32_T **pEmxArray)
{
	if (*pEmxArray != (emxArray_real32_T *)NULL) {
		if (((*pEmxArray)->data != (float *)NULL) && (*pEmxArray)->canFreeData) {
			free((void *)(*pEmxArray)->data);
		}

		free((void *)(*pEmxArray)->size);
		free((void *)*pEmxArray);
		*pEmxArray = (emxArray_real32_T *)NULL;
	}
}

//
// Arguments    : emxArray_real_T **pEmxArray
// Return Type  : void
//
void emxFree_real_T(emxArray_real_T **pEmxArray)
{
	if (*pEmxArray != (emxArray_real_T *)NULL) {
		if (((*pEmxArray)->data != (double *)NULL) && (*pEmxArray)->canFreeData) {
			free((void *)(*pEmxArray)->data);
		}

		free((void *)(*pEmxArray)->size);
		free((void *)*pEmxArray);
		*pEmxArray = (emxArray_real_T *)NULL;
	}
}

//
// Arguments    : emxArray_uint8_T **pEmxArray
// Return Type  : void
//
void emxFree_uint8_T(emxArray_uint8_T **pEmxArray)
{
	if (*pEmxArray != (emxArray_uint8_T *)NULL) {
		if (((*pEmxArray)->data != (unsigned char *)NULL) && (*pEmxArray)
			->canFreeData) {
			free((void *)(*pEmxArray)->data);
		}

		free((void *)(*pEmxArray)->size);
		free((void *)*pEmxArray);
		*pEmxArray = (emxArray_uint8_T *)NULL;
	}
}

//
// Arguments    : CheckerboardImage *pStruct
// Return Type  : void
//
void emxInitStruct_CheckerboardImage(CheckerboardImage *pStruct)
{
	emxInit_real_T(&pStruct->BoardCoords, 3);
	emxInit_real_T(&pStruct->BoardIdx, 2);
	emxInit_real32_T(&pStruct->Points, 2);
}

//
// Arguments    : emxArray_boolean_T **pEmxArray
//                int numDimensions
// Return Type  : void
//
void emxInit_boolean_T(emxArray_boolean_T **pEmxArray, int numDimensions)
{
	emxArray_boolean_T *emxArray;
	int i;
	*pEmxArray = (emxArray_boolean_T *)malloc(sizeof(emxArray_boolean_T));
	emxArray = *pEmxArray;
	emxArray->data = (boolean_T *)NULL;
	emxArray->numDimensions = numDimensions;
	emxArray->size = (int *)malloc((unsigned int)(sizeof(int) * numDimensions));
	emxArray->allocatedSize = 0;
	emxArray->canFreeData = true;
	for (i = 0; i < numDimensions; i++) {
		emxArray->size[i] = 0;
	}
}


//
// Arguments    : emxArray_int32_T **pEmxArray
//                int numDimensions
// Return Type  : void
//
void emxInit_int32_T(emxArray_int32_T **pEmxArray, int numDimensions)
{
	emxArray_int32_T *emxArray;
	int i;
	*pEmxArray = (emxArray_int32_T *)malloc(sizeof(emxArray_int32_T));
	emxArray = *pEmxArray;
	emxArray->data = (int *)NULL;
	emxArray->numDimensions = numDimensions;
	emxArray->size = (int *)malloc((unsigned int)(sizeof(int) * numDimensions));
	emxArray->allocatedSize = 0;
	emxArray->canFreeData = true;
	for (i = 0; i < numDimensions; i++) {
		emxArray->size[i] = 0;
	}
}

//
// Arguments    : emxArray_real32_T **pEmxArray
//                int numDimensions
// Return Type  : void
//
void emxInit_real32_T(emxArray_real32_T **pEmxArray, int numDimensions)
{
	emxArray_real32_T *emxArray;
	int i;
	*pEmxArray = (emxArray_real32_T *)malloc(sizeof(emxArray_real32_T));
	emxArray = *pEmxArray;
	emxArray->data = (float *)NULL;
	emxArray->numDimensions = numDimensions;
	emxArray->size = (int *)malloc((unsigned int)(sizeof(int) * numDimensions));
	emxArray->allocatedSize = 0;
	emxArray->canFreeData = true;
	for (i = 0; i < numDimensions; i++) {
		emxArray->size[i] = 0;
	}
}



//
// Arguments    : emxArray_real_T **pEmxArray
//                int numDimensions
// Return Type  : void
//
void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions)
{
	emxArray_real_T *emxArray;
	int i;
	*pEmxArray = (emxArray_real_T *)malloc(sizeof(emxArray_real_T));
	emxArray = *pEmxArray;
	emxArray->data = (double *)NULL;
	emxArray->numDimensions = numDimensions;
	emxArray->size = (int *)malloc((unsigned int)(sizeof(int) * numDimensions));
	emxArray->allocatedSize = 0;
	emxArray->canFreeData = true;
	for (i = 0; i < numDimensions; i++) {
		emxArray->size[i] = 0;
	}
}



//
// Arguments    : emxArray_uint8_T **pEmxArray
//                int numDimensions
// Return Type  : void
//
void emxInit_uint8_T(emxArray_uint8_T **pEmxArray, int numDimensions)
{
	emxArray_uint8_T *emxArray;
	int i;
	*pEmxArray = (emxArray_uint8_T *)malloc(sizeof(emxArray_uint8_T));
	emxArray = *pEmxArray;
	emxArray->data = (unsigned char *)NULL;
	emxArray->numDimensions = numDimensions;
	emxArray->size = (int *)malloc((unsigned int)(sizeof(int) * numDimensions));
	emxArray->allocatedSize = 0;
	emxArray->canFreeData = true;
	for (i = 0; i < numDimensions; i++) {
		emxArray->size[i] = 0;
	}
}

//
// Arguments    : double varargin_2
//                double varargin_3
//                emxArray_real_T *h
// Return Type  : void
//
void fspecial(double varargin_2, double varargin_3, emxArray_real_T *h)
{
	double b_varargin_2[2];
	double siz[2];
	int i1;
	double sumh;
	int ibtile;
	double ndbl;
	double apnd;
	double cdiff;
	double absa;
	emxArray_real_T *y;
	double absb;
	int nm1d2;
	int k;
	emxArray_real_T *b_y;
	emxArray_real_T *c_y;
	emxArray_real_T *a;
	int ibmat;
	emxArray_boolean_T *r3;
	int exitg2;
	emxArray_int32_T *r4;
	int exitg1;
	b_varargin_2[0] = varargin_2;
	b_varargin_2[1] = varargin_2;
	for (i1 = 0; i1 < 2; i1++) {
		siz[i1] = (b_varargin_2[i1] - 1.0) / 2.0;
	}

	if (siz[1] < -siz[1]) {
		ibtile = 0;
		sumh = -siz[1];
		apnd = siz[1];
	}
	else {
		sumh = -siz[1];
		ndbl = floor((siz[1] - (-siz[1])) + 0.5);
		apnd = -siz[1] + ndbl;
		cdiff = apnd - siz[1];
		absa = fabs(-siz[1]);
		absb = fabs(siz[1]);
		if (absa >= absb) {
			absb = absa;
		}

		if (fabs(cdiff) < 4.4408920985006262E-16 * absb) {
			ndbl++;
			apnd = siz[1];
		}
		else if (cdiff > 0.0) {
			apnd = -siz[1] + (ndbl - 1.0);
		}
		else {
			ndbl++;
		}

		if (ndbl >= 0.0) {
			ibtile = (int)ndbl;
		}
		else {
			ibtile = 0;
		}
	}

	emxInit_real_T(&y, 2);
	i1 = y->size[0] * y->size[1];
	y->size[0] = 1;
	y->size[1] = ibtile;
	emxEnsureCapacity((emxArray__common *)y, i1, (int)sizeof(double));
	if (ibtile > 0) {
		y->data[0] = sumh;
		if (ibtile > 1) {
			y->data[ibtile - 1] = apnd;
			nm1d2 = (ibtile - 1) / 2;
			for (k = 1; k < nm1d2; k++) {
				y->data[k] = sumh + (double)k;
				y->data[(ibtile - k) - 1] = apnd - (double)k;
			}

			if (nm1d2 << 1 == ibtile - 1) {
				y->data[nm1d2] = (sumh + apnd) / 2.0;
			}
			else {
				y->data[nm1d2] = sumh + (double)nm1d2;
				y->data[nm1d2 + 1] = apnd - (double)nm1d2;
			}
		}
	}

	if (siz[0] < -siz[0]) {
		ibtile = 0;
		sumh = -siz[0];
		apnd = siz[0];
	}
	else {
		sumh = -siz[0];
		ndbl = floor((siz[0] - (-siz[0])) + 0.5);
		apnd = -siz[0] + ndbl;
		cdiff = apnd - siz[0];
		absa = fabs(-siz[0]);
		absb = fabs(siz[0]);
		if (absa >= absb) {
			absb = absa;
		}

		if (fabs(cdiff) < 4.4408920985006262E-16 * absb) {
			ndbl++;
			apnd = siz[0];
		}
		else if (cdiff > 0.0) {
			apnd = -siz[0] + (ndbl - 1.0);
		}
		else {
			ndbl++;
		}

		if (ndbl >= 0.0) {
			ibtile = (int)ndbl;
		}
		else {
			ibtile = 0;
		}
	}

	emxInit_real_T(&b_y, 2);
	i1 = b_y->size[0] * b_y->size[1];
	b_y->size[0] = 1;
	b_y->size[1] = ibtile;
	emxEnsureCapacity((emxArray__common *)b_y, i1, (int)sizeof(double));
	if (ibtile > 0) {
		b_y->data[0] = sumh;
		if (ibtile > 1) {
			b_y->data[ibtile - 1] = apnd;
			nm1d2 = (ibtile - 1) / 2;
			for (k = 1; k < nm1d2; k++) {
				b_y->data[k] = sumh + (double)k;
				b_y->data[(ibtile - k) - 1] = apnd - (double)k;
			}

			if (nm1d2 << 1 == ibtile - 1) {
				b_y->data[nm1d2] = (sumh + apnd) / 2.0;
			}
			else {
				b_y->data[nm1d2] = sumh + (double)nm1d2;
				b_y->data[nm1d2 + 1] = apnd - (double)nm1d2;
			}
		}
	}

	emxInit_real_T(&c_y, 2);
	
	if ((y->size[1] == 0) || (b_y->size[1] == 0)) {
		emxFree_real_T(&y);
		i1 = h->size[0] * h->size[1];
		h->size[0] = 0;
		h->size[1] = 0;
		emxEnsureCapacity((emxArray__common *)h, i1, (int)sizeof(double));
		i1 = c_y->size[0] * c_y->size[1];
		c_y->size[0] = 0;
		c_y->size[1] = 0;
		emxEnsureCapacity((emxArray__common *)c_y, i1, (int)sizeof(double));
	}
	else {

		emxInit_real_T(&a, 2);
		ibmat = y->size[1];
		i1 = a->size[0] * a->size[1];
		a->size[0] = 1;
		a->size[1] = ibmat;
		emxEnsureCapacity((emxArray__common *)a, i1, (int)sizeof(double));
		for (i1 = 0; i1 < ibmat; i1++) {
			a->data[a->size[0] * i1] = y->data[i1];
		}

		siz[0] = b_y->size[1];
		siz[1] = y->size[1];
		emxFree_real_T(&y);


		nm1d2 = a->size[1];
		i1 = h->size[0] * h->size[1];
		h->size[0] = (int)siz[0];
		h->size[1] = nm1d2;
		emxEnsureCapacity((emxArray__common *)h, i1, (int)sizeof(double));
		nm1d2 = (int)siz[0];
		for (ibtile = 0; ibtile + 1 <= a->size[1]; ibtile++) {
			ibmat = ibtile * nm1d2;
			for (k = 1; k <= nm1d2; k++) {
				h->data[(ibmat + k) - 1] = a->data[ibtile];
			}
		}
		emxFree_real_T(&a);



		ibmat = b_y->size[1];
		i1 = c_y->size[0] * c_y->size[1];
		c_y->size[0] = ibmat;
		c_y->size[1] = (int)siz[1];
		emxEnsureCapacity((emxArray__common *)c_y, i1, (int)sizeof(double));
		ibmat = b_y->size[1];
		for (nm1d2 = 1; nm1d2 <= (int)siz[1]; nm1d2++) {
			ibtile = (nm1d2 - 1) * ibmat;
			for (k = 1; k <= ibmat; k++) {
				c_y->data[(ibtile + k) - 1] = b_y->data[k - 1];
			}
		}
	}

	//emxFree_real_T(&a);
	emxFree_real_T(&b_y);
	/*emxFree_real_T(&y);*/
	sumh = 2.0 * varargin_3 * varargin_3;
	i1 = h->size[0] * h->size[1];
	emxEnsureCapacity((emxArray__common *)h, i1, (int)sizeof(double));
	nm1d2 = h->size[0];
	ibtile = h->size[1];
	nm1d2 *= ibtile;
	for (i1 = 0; i1 < nm1d2; i1++) {
		h->data[i1] = -(h->data[i1] * h->data[i1] + c_y->data[i1] * c_y->data[i1]) /
			sumh;
	}

	emxFree_real_T(&c_y);
	nm1d2 = h->size[0] * h->size[1];
	for (k = 0; k + 1 <= nm1d2; k++) {
		h->data[k] = exp(h->data[k]);
	}

	sumh = h->data[0];
	ibtile = h->size[0] * h->size[1];
	if (ibtile > 1) {
		ibtile = h->size[0] * h->size[1];
		if (1 < ibtile) {
			nm1d2 = 1;
			do {
				exitg2 = 0;
				ibtile = h->size[0] * h->size[1];
				if (nm1d2 + 1 <= ibtile) {
					if (h->data[nm1d2] > sumh) {
						sumh = h->data[nm1d2];
					}

					nm1d2++;
				}
				else {
					exitg2 = 1;
				}
			} while (exitg2 == 0);
		}
	}

	emxInit_boolean_T(&r3, 2);
	i1 = r3->size[0] * r3->size[1];
	r3->size[0] = h->size[0];
	r3->size[1] = h->size[1];
	emxEnsureCapacity((emxArray__common *)r3, i1, (int)sizeof(boolean_T));
	sumh *= 2.2204460492503131E-16;
	nm1d2 = h->size[0] * h->size[1];
	for (i1 = 0; i1 < nm1d2; i1++) {
		r3->data[i1] = (h->data[i1] < sumh);
	}

	k = r3->size[0] * r3->size[1] - 1;
	nm1d2 = 0;
	for (ibmat = 0; ibmat <= k; ibmat++) {
		if (r3->data[ibmat]) {
			nm1d2++;
		}
	}

	emxInit_int32_T(&r4, 1);
	i1 = r4->size[0];
	r4->size[0] = nm1d2;
	emxEnsureCapacity((emxArray__common *)r4, i1, (int)sizeof(int));
	ibtile = 0;
	for (ibmat = 0; ibmat <= k; ibmat++) {
		if (r3->data[ibmat]) {
			r4->data[ibtile] = ibmat + 1;
			ibtile++;
		}
	}

	emxFree_boolean_T(&r3);
	nm1d2 = r4->size[0];
	for (i1 = 0; i1 < nm1d2; i1++) {
		h->data[r4->data[i1] - 1] = 0.0;
	}

	emxFree_int32_T(&r4);
	ibtile = h->size[0] * h->size[1];
	if (ibtile == 0) {
		sumh = 0.0;
	}
	else {
		sumh = h->data[0];
		k = 2;
		do {
			exitg1 = 0;
			ibtile = h->size[0] * h->size[1];
			if (k <= ibtile) {
				sumh += h->data[k - 1];
				k++;
			}
			else {
				exitg1 = 1;
			}
		} while (exitg1 == 0);
	}

	if (sumh != 0.0) {
		i1 = h->size[0] * h->size[1];
		emxEnsureCapacity((emxArray__common *)h, i1, (int)sizeof(double));
		nm1d2 = h->size[0];
		ibtile = h->size[1];
		nm1d2 *= ibtile;
		for (i1 = 0; i1 < nm1d2; i1++) {
			h->data[i1] /= sumh;
		}
	}
}

//
// Exit immediately if no corner points were found
// Arguments    : const emxArray_real32_T *points
//                const emxArray_real32_T *scores
//                const emxArray_real32_T *Ix2
//                const emxArray_real32_T *Iy2
//                const emxArray_real32_T *Ixy
//                double theta
//                CheckerboardImage *iobj_0
//                CheckerboardImage *iobj_1
//                CheckerboardImage *iobj_2
// Return Type  : CheckerboardImage *
//
CheckerboardImage *growCheckerboard(const emxArray_real32_T *points,
	const emxArray_real32_T *scores, const emxArray_real32_T *Ix2, const
	emxArray_real32_T *Iy2, const emxArray_real32_T *Ixy, double theta,
	CheckerboardImage *iobj_0, CheckerboardImage *iobj_1, CheckerboardImage
	*iobj_2)
{
	CheckerboardImage *board;
	int ndbl;
	int apnd;
	int cdiff;
	int absb;
	emxArray_real_T *seedIdx;
	int i10;
	emxArray_real32_T *unusedU0;
	int loop_ub;
	emxArray_int32_T *iidx;
	emxArray_real_T *b_seedIdx;
	int i11;
	CheckerboardImage *b_this;
	CheckerboardImage *previousBoard;
	CheckerboardImage *currentBoard;
	int i;
	emxArray_real_T *b_currentBoard;
	emxArray_real_T *c_currentBoard;
	emxArray_real_T *d_currentBoard;
	emxArray_real_T *e_currentBoard;
	emxArray_real_T *f_currentBoard;
	emxArray_real_T *g_currentBoard;
	emxArray_real_T *h_currentBoard;
	emxArray_real_T *i_currentBoard;
	float x[2];
	float df;
	float adf;
	float tb;
	float ab;
	float y;
	float rt;
	int sgn1;
	float cs;
	int sgn2;
	float ct;
	float sn1;
	float cs1;
	float tn;
	float v2[2];
	boolean_T hasExpanded;
	CheckerboardImage *tmpBoard;
	int b_i;
	int exitg1;
	boolean_T guard1 = false;
	double d0;
	int i12;
	int i13;

	// 'detectCheckerboardOnImage:35' if isempty(scores)
	if (scores->size[0] == 0) {
		// 'detectCheckerboardOnImage:36' board = CheckerboardImage;
		board = c_CheckerboardImage_Checkerboar(iobj_2);
	}
	else {
		//  only use corners with high scores as seeds to reduce computation
		// 'detectCheckerboardOnImage:43' seedIdx = 1:size(points, 1);
		if (points->size[0] < 1) {
			ndbl = 0;
			apnd = 0;
		}
		else {
			ndbl = (int)floor(((double)points->size[0] - 1.0) + 0.5);
			apnd = ndbl + 1;
			cdiff = (ndbl - points->size[0]) + 1;
			absb = points->size[0];
			if (fabs((double)cdiff) < 4.4408920985006262E-16 * (double)absb) {
				ndbl++;
				apnd = points->size[0];
			}
			else if (cdiff > 0) {
				apnd = ndbl;
			}
			else {
				ndbl++;
			}
		}

		emxInit_real_T(&seedIdx, 2);
		i10 = seedIdx->size[0] * seedIdx->size[1];
		seedIdx->size[0] = 1;
		seedIdx->size[1] = ndbl;
		emxEnsureCapacity((emxArray__common *)seedIdx, i10, (int)sizeof(double));
		if (ndbl > 0) {
			seedIdx->data[0] = 1.0;
			if (ndbl > 1) {
				seedIdx->data[ndbl - 1] = apnd;
				cdiff = (ndbl - 1) / 2;
				for (absb = 1; absb < cdiff; absb++) {
					seedIdx->data[absb] = 1.0 + (double)absb;
					seedIdx->data[(ndbl - absb) - 1] = apnd - absb;
				}

				if (cdiff << 1 == ndbl - 1) {
					seedIdx->data[cdiff] = (1.0 + (double)apnd) / 2.0;
				}
				else {
					seedIdx->data[cdiff] = 1.0 + (double)cdiff;
					seedIdx->data[cdiff + 1] = apnd - cdiff;
				}
			}
		}

		emxInit_real32_T(&unusedU0, 1);

		// 'detectCheckerboardOnImage:44' [~, sortedIdx] = sort(scores(seedIdx), 'descend'); 
		i10 = unusedU0->size[0];
		unusedU0->size[0] = seedIdx->size[1];
		emxEnsureCapacity((emxArray__common *)unusedU0, i10, (int)sizeof(float));
		loop_ub = seedIdx->size[1];
		for (i10 = 0; i10 < loop_ub; i10++) {
			unusedU0->data[i10] = scores->data[(int)seedIdx->data[seedIdx->size[0] *
				i10] - 1];
		}

		emxInit_int32_T(&iidx, 1);
		emxInit_real_T(&b_seedIdx, 2);
		sort(unusedU0, iidx);
		emxFree_real32_T(&unusedU0);

		// 'detectCheckerboardOnImage:44' ~
		// 'detectCheckerboardOnImage:45' seedIdx = seedIdx(sortedIdx);
		i10 = b_seedIdx->size[0] * b_seedIdx->size[1];
		b_seedIdx->size[0] = 1;
		b_seedIdx->size[1] = iidx->size[0];
		emxEnsureCapacity((emxArray__common *)b_seedIdx, i10, (int)sizeof(double));
		loop_ub = iidx->size[0];
		
		for (i10 = 0; i10 < loop_ub; i10++) {
			b_seedIdx->data[b_seedIdx->size[0] * i10] = seedIdx->data[iidx->data[i10]
				- 1];
		}

		emxFree_int32_T(&iidx);
		i10 = seedIdx->size[0] * seedIdx->size[1];
		seedIdx->size[0] = 1;
		seedIdx->size[1] = b_seedIdx->size[1];
		emxEnsureCapacity((emxArray__common *)seedIdx, i10, (int)sizeof(double));
		loop_ub = b_seedIdx->size[1];
		for (i10 = 0; i10 < loop_ub; i10++) {
			seedIdx->data[seedIdx->size[0] * i10] = b_seedIdx->data[b_seedIdx->size[0]
				* i10];
		}

		emxFree_real_T(&b_seedIdx);

		// 'detectCheckerboardOnImage:46' seedIdx = seedIdx(1:round(numel(seedIdx / 2))); 
		i10 = (int)rt_roundd((double)seedIdx->size[1]);
		if (1 > i10) {
			i10 = 0;
		}

		i11 = seedIdx->size[0] * seedIdx->size[1];
		seedIdx->size[1] = i10;
		emxEnsureCapacity((emxArray__common *)seedIdx, i11, (int)sizeof(double));

		// 'detectCheckerboardOnImage:48' angleThreshold = 3 * pi / 16;
		// 'detectCheckerboardOnImage:51' previousBoard =CheckerboardImage;
		b_this = iobj_0;
		previousBoard = b_this;
		b_this->isValid = false;
		b_this->Energy = 1.0E+11F;

		// 'CheckerboardImage:20' this.BoardIdx = 0;
		i11 = b_this->BoardIdx->size[0] * b_this->BoardIdx->size[1];
		b_this->BoardIdx->size[0] = 1;
		b_this->BoardIdx->size[1] = 1;
		emxEnsureCapacity((emxArray__common *)b_this->BoardIdx, i11, (int)sizeof
		(double));
		b_this->BoardIdx->data[0] = 0.0;

		// 'CheckerboardImage:21' this.BoardIdx = zeros(3, 3);
		i11 = b_this->BoardIdx->size[0] * b_this->BoardIdx->size[1];
		b_this->BoardIdx->size[0] = 3;
		b_this->BoardIdx->size[1] = 3;
		emxEnsureCapacity((emxArray__common *)b_this->BoardIdx, i11, (int)sizeof
		(double));
		for (i11 = 0; i11 < 9; i11++) {
			b_this->BoardIdx->data[i11] = 0.0;
		}

		// 'CheckerboardImage:22' this.BoardCoords = 0;
		i11 = b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] *
			b_this->BoardCoords->size[2];
		b_this->BoardCoords->size[0] = 1;
		b_this->BoardCoords->size[1] = 1;
		b_this->BoardCoords->size[2] = 1;
		emxEnsureCapacity((emxArray__common *)b_this->BoardCoords, i11, (int)sizeof
		(double));
		b_this->BoardCoords->data[0] = 0.0;

		// 'CheckerboardImage:23' this.BoardCoords = zeros(3, 3, 2);
		i11 = b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] *
			b_this->BoardCoords->size[2];
		b_this->BoardCoords->size[0] = 3;
		b_this->BoardCoords->size[1] = 3;
		b_this->BoardCoords->size[2] = 2;
		emxEnsureCapacity((emxArray__common *)b_this->BoardCoords, i11, (int)sizeof
		(double));
		for (i11 = 0; i11 < 18; i11++) {
			b_this->BoardCoords->data[i11] = 0.0;
		}

		// 'detectCheckerboardOnImage:52' currentBoard = CheckerboardImage;
		b_this = iobj_1;
		currentBoard = b_this;
		b_this->isValid = false;
		b_this->Energy = 1.0E+11F;

		// 'CheckerboardImage:20' this.BoardIdx = 0;
		i11 = b_this->BoardIdx->size[0] * b_this->BoardIdx->size[1];
		b_this->BoardIdx->size[0] = 1;
		b_this->BoardIdx->size[1] = 1;
		emxEnsureCapacity((emxArray__common *)b_this->BoardIdx, i11, (int)sizeof
		(double));
		b_this->BoardIdx->data[0] = 0.0;

		// 'CheckerboardImage:21' this.BoardIdx = zeros(3, 3);
		i11 = b_this->BoardIdx->size[0] * b_this->BoardIdx->size[1];
		b_this->BoardIdx->size[0] = 3;
		b_this->BoardIdx->size[1] = 3;
		emxEnsureCapacity((emxArray__common *)b_this->BoardIdx, i11, (int)sizeof
		(double));
		for (i11 = 0; i11 < 9; i11++) {
			b_this->BoardIdx->data[i11] = 0.0;
		}

		// 'CheckerboardImage:22' this.BoardCoords = 0;
		i11 = b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] *
			b_this->BoardCoords->size[2];
		b_this->BoardCoords->size[0] = 1;
		b_this->BoardCoords->size[1] = 1;
		b_this->BoardCoords->size[2] = 1;
		emxEnsureCapacity((emxArray__common *)b_this->BoardCoords, i11, (int)sizeof
		(double));
		b_this->BoardCoords->data[0] = 0.0;

		// 'CheckerboardImage:23' this.BoardCoords = zeros(3, 3, 2);
		i11 = b_this->BoardCoords->size[0] * b_this->BoardCoords->size[1] *
			b_this->BoardCoords->size[2];
		b_this->BoardCoords->size[0] = 3;
		b_this->BoardCoords->size[1] = 3;
		b_this->BoardCoords->size[2] = 2;
		emxEnsureCapacity((emxArray__common *)b_this->BoardCoords, i11, (int)sizeof
		(double));
		for (i11 = 0; i11 < 18; i11++) {
			b_this->BoardCoords->data[i11] = 0.0;
		}

		// 'detectCheckerboardOnImage:53' for i = 1:numel(seedIdx)
		i = 0;
		emxInit_real_T(&b_currentBoard, 2);
		emxInit_real_T(&c_currentBoard, 3);
		emxInit_real_T(&d_currentBoard, 2);
		emxInit_real_T(&e_currentBoard, 3);
		emxInit_real_T(&f_currentBoard, 2);
		emxInit_real_T(&g_currentBoard, 3);
		emxInit_real_T(&h_currentBoard, 2);
		emxInit_real_T(&i_currentBoard, 3);
		while (i <= i10 - 1) {
			// 'detectCheckerboardOnImage:54' [v1, v2] = cornerOrientations(Ix2, Iy2, Ixy, round(points(seedIdx(i), :))); 
			cdiff = (int)seedIdx->data[i];
			for (i11 = 0; i11 < 2; i11++) {
				x[i11] = points->data[(cdiff + points->size[0] * i11) - 1];
			}

			for (absb = 0; absb < 2; absb++) {
				x[absb] = rt_roundf(x[absb]);
			}

			//  The orientation vectors are the eigen vectors of the
			//  structure tensor:
			//  [Ix^2  Ixy ]
			//  [Ixy   Iy^2]
			// -------------------------------------------------------------------------- 
			// 'detectCheckerboardOnImage:81' a = Ix2(p(2), p(1));
			// 'detectCheckerboardOnImage:82' b = Ixy(p(2), p(1));
			// 'detectCheckerboardOnImage:83' c = Iy2(p(2), p(1));
			//  % Computing eigenvectors "by hand", because the eig() function behaves 
			//  % differently in codegen.
			//  % Since the matrix is positive-semidefinite, its eigenvectors are
			//  % orthogonal. Compute the first eigenvector, then swap its elements and 
			//  % negate the y-component to make the second one.
			// 'detectCheckerboardOnImage:90' sm = a + c;
			// 'detectCheckerboardOnImage:91' df = a - c;
			df = Ix2->data[((int)x[1] + Ix2->size[0] * ((int)x[0] - 1)) - 1] -
				Iy2->data[((int)x[1] + Iy2->size[0] * ((int)x[0] - 1)) - 1];

			// 'detectCheckerboardOnImage:92' adf = abs(df);
			adf = (float)fabs((double)df);

			// 'detectCheckerboardOnImage:93' tb = b + b;
			tb = Ixy->data[((int)x[1] + Ixy->size[0] * ((int)x[0] - 1)) - 1] +
				Ixy->data[((int)x[1] + Ixy->size[0] * ((int)x[0] - 1)) - 1];

			// 'detectCheckerboardOnImage:94' ab = abs(tb);
			ab = (float)fabs((double)tb);

			// 'detectCheckerboardOnImage:96' if adf > ab
			if (adf > ab) {
				// 'detectCheckerboardOnImage:97' rt = adf * sqrt(1 + (ab/adf)^2);
				y = ab / adf;
				rt = adf * (float)sqrt((double)(1.0F + y * y));
			}
			else if (adf < ab) {
				// 'detectCheckerboardOnImage:98' elseif adf < ab
				// 'detectCheckerboardOnImage:99' rt = ab * sqrt(1 + (adf/ab)^2);
				y = adf / ab;
				rt = ab * (float)sqrt((double)(1.0F + y * y));
			}
			else {
				// 'detectCheckerboardOnImage:100' else
				// 'detectCheckerboardOnImage:101' rt = ab * sqrt(2);
				rt = ab * 1.41421354F;
			}

			// 'detectCheckerboardOnImage:104' if sm < 0
			if (Ix2->data[((int)x[1] + Ix2->size[0] * ((int)x[0] - 1)) - 1] +
				Iy2->data[((int)x[1] + Iy2->size[0] * ((int)x[0] - 1)) - 1] < 0.0F) {
				// 'detectCheckerboardOnImage:105' sgn1 = -1;
				sgn1 = -1;
			}
			else {
				// 'detectCheckerboardOnImage:106' else
				// 'detectCheckerboardOnImage:107' sgn1 = 1;
				sgn1 = 1;
			}

			// 'detectCheckerboardOnImage:110' if df > 0
			if (df > 0.0F) {
				// 'detectCheckerboardOnImage:111' cs = df + rt;
				cs = df + rt;

				// 'detectCheckerboardOnImage:112' sgn2 = 1;
				sgn2 = 1;
			}
			else {
				// 'detectCheckerboardOnImage:113' else
				// 'detectCheckerboardOnImage:114' cs = df - rt;
				cs = df - rt;

				// 'detectCheckerboardOnImage:115' sgn2 = -1;
				sgn2 = -1;
			}

			// 'detectCheckerboardOnImage:118' acs = abs(cs);
			// 'detectCheckerboardOnImage:119' if acs > ab
			if ((float)fabs((double)cs) > ab) {
				// 'detectCheckerboardOnImage:120' ct = -tb / cs;
				ct = -tb / cs;

				// 'detectCheckerboardOnImage:121' sn1 = 1 / sqrt(1 + ct * ct);
				sn1 = 1.0F / (float)sqrt((double)(1.0F + ct * ct));

				// 'detectCheckerboardOnImage:122' cs1 = ct * sn1;
				cs1 = ct * sn1;
			}
			else {
				// 'detectCheckerboardOnImage:123' else
				// 'detectCheckerboardOnImage:124' if ab == single(0)
				if (ab == 0.0F) {
					// 'detectCheckerboardOnImage:125' cs1 = single(1);
					cs1 = 1.0F;

					// 'detectCheckerboardOnImage:126' sn1 = single(0);
					sn1 = 0.0F;
				}
				else {
					// 'detectCheckerboardOnImage:127' else
					// 'detectCheckerboardOnImage:128' tn = -cs / tb;
					tn = -cs / tb;

					// 'detectCheckerboardOnImage:129' cs1 = 1 / sqrt(1 + tn * tn);
					cs1 = 1.0F / (float)sqrt((double)(1.0F + tn * tn));

					// 'detectCheckerboardOnImage:130' sn1 = tn * cs1;
					sn1 = tn * cs1;
				}
			}

			// 'detectCheckerboardOnImage:133' if sgn1 == sgn2
			if (sgn1 == sgn2) {
				// 'detectCheckerboardOnImage:134' tn = cs1;
				tn = cs1;

				// 'detectCheckerboardOnImage:135' cs1 = -sn1;
				cs1 = -sn1;

				// 'detectCheckerboardOnImage:136' sn1 = tn;
				sn1 = tn;
			}

			// 'detectCheckerboardOnImage:139' v1 = [-sn1, cs1];
			x[0] = -sn1;
			x[1] = cs1;

			// 'detectCheckerboardOnImage:140' v2 = [cs1, sn1];
			v2[0] = cs1;
			v2[1] = sn1;

			// 'detectCheckerboardOnImage:55' alpha1 = abs(atan2(v1(2), v1(1)));
			// 'detectCheckerboardOnImage:56' alpha2 = abs(atan2(v2(2), v2(1)));
			// 'detectCheckerboardOnImage:57' if abs(abs(alpha1 - pi) - theta) > angleThreshold && ... 
			// 'detectCheckerboardOnImage:58'             abs(abs(alpha2 - pi) - theta) > angleThreshold 
			if (((float)fabs((double)((float)fabs((double)((float)fabs((double)(float)
				atan2((double)cs1, (double)-sn1)) - 3.14159274F)) - (float)theta))
			> 0.58904862254808621) && ((float)fabs((double)((float)fabs((double)
				((float)fabs((double)(float)atan2((double)sn1, (double)cs1)) -
					3.14159274F)) - (float)theta)) > 0.58904862254808621)) {
			}
			else {
				// 'detectCheckerboardOnImage:62' currentBoard.initialize(seedIdx(i), points, v1, v2); 
				CheckerboardImageInitialize(currentBoard, seedIdx->data[i], points, x,
					v2);

				// 'detectCheckerboardOnImage:63' expandBoardFully(currentBoard);
				// 'CheckerboardImage:135' if ~this.isValid
				if (!currentBoard->isValid) {
				}
				else {
					// 'CheckerboardImage:139' hasExpanded = true;
					hasExpanded = true;

					// 'CheckerboardImage:140' while hasExpanded
					while (hasExpanded) {
						// 'CheckerboardImage:141' hasExpanded = this.expandBoardOnce();
						// 'CheckerboardImage:270' this.PreviousEnergy = this.Energy;
						currentBoard->PreviousEnergy = currentBoard->Energy;

						// 'CheckerboardImage:271' for i = 1:4
						b_i = 0;
						do {
							exitg1 = 0;
							if (b_i < 4) {
								// 'CheckerboardImage:272' if ~this.IsDirectionBad(i)
								guard1 = false;
								if (!currentBoard->IsDirectionBad[b_i]) {
									// 'CheckerboardImage:273' this.LastExpandDirection = i;
									currentBoard->LastExpandDirection = 1.0 + (double)b_i;

									// 'CheckerboardImage:274' expandBoardDirectionally(this, i);
									CheckerboardImageExpandBoard1(currentBoard, 1.0 + (double)
										b_i);

									// 'CheckerboardImage:275' if this.Energy < this.PreviousEnergy 
									if (currentBoard->Energy < currentBoard->PreviousEnergy) {
										// 'CheckerboardImage:276' success = true;
										exitg1 = 1;
									}
									else {
										// 'CheckerboardImage:278' else
										// 'CheckerboardImage:279' this.undoLastExpansion();
										// 'CheckerboardImage:289' this.Energy = this.PreviousEnergy; 
										currentBoard->Energy = currentBoard->PreviousEnergy;

										// 'CheckerboardImage:290' switch this.LastExpandDirection
										switch ((int)currentBoard->LastExpandDirection) {
										case 1:
											// 'CheckerboardImage:291' case 1 %'up'
											// 'CheckerboardImage:292' this.BoardIdx = this.BoardIdx(2:end, :); 
											i11 = currentBoard->BoardIdx->size[0];
											if (2 > i11) {
												i12 = 0;
												i11 = 0;
											}
											else {
												i12 = 1;
											}

											cdiff = currentBoard->BoardIdx->size[1];
											i13 = b_currentBoard->size[0] * b_currentBoard->size[1];
											b_currentBoard->size[0] = i11 - i12;
											b_currentBoard->size[1] = cdiff;
											emxEnsureCapacity((emxArray__common *)b_currentBoard, i13,
												(int)sizeof(double));
											for (i13 = 0; i13 < cdiff; i13++) {
												loop_ub = i11 - i12;
												for (ndbl = 0; ndbl < loop_ub; ndbl++) {
													b_currentBoard->data[ndbl + b_currentBoard->size[0] *
														i13] = currentBoard->BoardIdx->data[(i12 + ndbl) +
														currentBoard->BoardIdx->size[0] * i13];
												}
											}

											i11 = currentBoard->BoardIdx->size[0] *
												currentBoard->BoardIdx->size[1];
											currentBoard->BoardIdx->size[0] = b_currentBoard->size[0];
											currentBoard->BoardIdx->size[1] = b_currentBoard->size[1];
											emxEnsureCapacity((emxArray__common *)
												currentBoard->BoardIdx, i11, (int)sizeof
												(double));
											loop_ub = b_currentBoard->size[1];
											for (i11 = 0; i11 < loop_ub; i11++) {
												cdiff = b_currentBoard->size[0];
												for (i12 = 0; i12 < cdiff; i12++) {
													currentBoard->BoardIdx->data[i12 +
														currentBoard->BoardIdx->size[0] * i11] =
														b_currentBoard->data[i12 + b_currentBoard->size[0] *
														i11];
												}
											}

											// 'CheckerboardImage:293' this.BoardCoords = this.BoardCoords(2:end, :, :); 
											i11 = currentBoard->BoardCoords->size[0];
											if (2 > i11) {
												i12 = 0;
												i11 = 0;
											}
											else {
												i12 = 1;
											}

											cdiff = currentBoard->BoardCoords->size[1];
											absb = currentBoard->BoardCoords->size[2];
											i13 = c_currentBoard->size[0] * c_currentBoard->size[1] *
												c_currentBoard->size[2];
											c_currentBoard->size[0] = i11 - i12;
											c_currentBoard->size[1] = cdiff;
											c_currentBoard->size[2] = absb;
											emxEnsureCapacity((emxArray__common *)c_currentBoard, i13,
												(int)sizeof(double));
											for (i13 = 0; i13 < absb; i13++) {
												for (ndbl = 0; ndbl < cdiff; ndbl++) {
													loop_ub = i11 - i12;
													for (apnd = 0; apnd < loop_ub; apnd++) {
														c_currentBoard->data[(apnd + c_currentBoard->size[0]
															* ndbl) + c_currentBoard->
															size[0] * c_currentBoard->size[1] * i13] =
															currentBoard->BoardCoords->data[((i12 + apnd) +
																currentBoard->BoardCoords->size[0] * ndbl) +
															currentBoard->BoardCoords->size[0] *
															currentBoard->BoardCoords->size[1] * i13];
													}
												}
											}

											i11 = currentBoard->BoardCoords->size[0] *
												currentBoard->BoardCoords->size[1] *
												currentBoard->BoardCoords->size[2];
											currentBoard->BoardCoords->size[0] = c_currentBoard->size
												[0];
											currentBoard->BoardCoords->size[1] = c_currentBoard->size
												[1];
											currentBoard->BoardCoords->size[2] = c_currentBoard->size
												[2];
											emxEnsureCapacity((emxArray__common *)
												currentBoard->BoardCoords, i11, (int)
												sizeof(double));
											loop_ub = c_currentBoard->size[2];
											for (i11 = 0; i11 < loop_ub; i11++) {
												cdiff = c_currentBoard->size[1];
												for (i12 = 0; i12 < cdiff; i12++) {
													absb = c_currentBoard->size[0];
													for (i13 = 0; i13 < absb; i13++) {
														currentBoard->BoardCoords->data[(i13 +
															currentBoard->BoardCoords->size[0] * i12) +
															currentBoard->BoardCoords->size[0] *
															currentBoard->BoardCoords->size[1] * i11] =
															c_currentBoard->data[(i13 + c_currentBoard->size[0]
																* i12) + c_currentBoard->size[0] *
															c_currentBoard->size[1] * i11];
													}
												}
											}
											break;

										case 2:
											// 'CheckerboardImage:295' case 2 %'down'
											// 'CheckerboardImage:296' this.BoardIdx = this.BoardIdx(1:end-1, :); 
											d0 = (double)currentBoard->BoardIdx->size[0] - 1.0;
											if (1.0 > d0) {
												loop_ub = 0;
											}
											else {
												loop_ub = (int)d0;
											}

											cdiff = currentBoard->BoardIdx->size[1];
											i11 = d_currentBoard->size[0] * d_currentBoard->size[1];
											d_currentBoard->size[0] = loop_ub;
											d_currentBoard->size[1] = cdiff;
											emxEnsureCapacity((emxArray__common *)d_currentBoard, i11,
												(int)sizeof(double));
											for (i11 = 0; i11 < cdiff; i11++) {
												for (i12 = 0; i12 < loop_ub; i12++) {
													d_currentBoard->data[i12 + d_currentBoard->size[0] *
														i11] = currentBoard->BoardIdx->data[i12 +
														currentBoard->BoardIdx->size[0] * i11];
												}
											}

											i11 = currentBoard->BoardIdx->size[0] *
												currentBoard->BoardIdx->size[1];
											currentBoard->BoardIdx->size[0] = d_currentBoard->size[0];
											currentBoard->BoardIdx->size[1] = d_currentBoard->size[1];
											emxEnsureCapacity((emxArray__common *)
												currentBoard->BoardIdx, i11, (int)sizeof
												(double));
											loop_ub = d_currentBoard->size[1];
											for (i11 = 0; i11 < loop_ub; i11++) {
												cdiff = d_currentBoard->size[0];
												for (i12 = 0; i12 < cdiff; i12++) {
													currentBoard->BoardIdx->data[i12 +
														currentBoard->BoardIdx->size[0] * i11] =
														d_currentBoard->data[i12 + d_currentBoard->size[0] *
														i11];
												}
											}

											// 'CheckerboardImage:297' this.BoardCoords = this.BoardCoords(1:end-1, :, :); 
											d0 = (double)currentBoard->BoardCoords->size[0] - 1.0;
											if (1.0 > d0) {
												loop_ub = 0;
											}
											else {
												loop_ub = (int)d0;
											}

											cdiff = currentBoard->BoardCoords->size[1];
											absb = currentBoard->BoardCoords->size[2];
											i11 = e_currentBoard->size[0] * e_currentBoard->size[1] *
												e_currentBoard->size[2];
											e_currentBoard->size[0] = loop_ub;
											e_currentBoard->size[1] = cdiff;
											e_currentBoard->size[2] = absb;
											emxEnsureCapacity((emxArray__common *)e_currentBoard, i11,
												(int)sizeof(double));
											for (i11 = 0; i11 < absb; i11++) {
												for (i12 = 0; i12 < cdiff; i12++) {
													for (i13 = 0; i13 < loop_ub; i13++) {
														e_currentBoard->data[(i13 + e_currentBoard->size[0] *
															i12) + e_currentBoard->size[0]
															* e_currentBoard->size[1] * i11] =
															currentBoard->BoardCoords->data[(i13 +
																currentBoard->BoardCoords->size[0] * i12) +
															currentBoard->BoardCoords->size[0] *
															currentBoard->BoardCoords->size[1] * i11];
													}
												}
											}

											i11 = currentBoard->BoardCoords->size[0] *
												currentBoard->BoardCoords->size[1] *
												currentBoard->BoardCoords->size[2];
											currentBoard->BoardCoords->size[0] = e_currentBoard->size
												[0];
											currentBoard->BoardCoords->size[1] = e_currentBoard->size
												[1];
											currentBoard->BoardCoords->size[2] = e_currentBoard->size
												[2];
											emxEnsureCapacity((emxArray__common *)
												currentBoard->BoardCoords, i11, (int)
												sizeof(double));
											loop_ub = e_currentBoard->size[2];
											for (i11 = 0; i11 < loop_ub; i11++) {
												cdiff = e_currentBoard->size[1];
												for (i12 = 0; i12 < cdiff; i12++) {
													absb = e_currentBoard->size[0];
													for (i13 = 0; i13 < absb; i13++) {
														currentBoard->BoardCoords->data[(i13 +
															currentBoard->BoardCoords->size[0] * i12) +
															currentBoard->BoardCoords->size[0] *
															currentBoard->BoardCoords->size[1] * i11] =
															e_currentBoard->data[(i13 + e_currentBoard->size[0]
																* i12) + e_currentBoard->size[0] *
															e_currentBoard->size[1] * i11];
													}
												}
											}
											break;

										case 3:
											// 'CheckerboardImage:299' case 3 %'left'
											// 'CheckerboardImage:300' this.BoardIdx = this.BoardIdx(:, 2:end); 
											i11 = currentBoard->BoardIdx->size[1];
											if (2 > i11) {
												i12 = 0;
												i11 = 0;
											}
											else {
												i12 = 1;
											}

											cdiff = currentBoard->BoardIdx->size[0];
											i13 = f_currentBoard->size[0] * f_currentBoard->size[1];
											f_currentBoard->size[0] = cdiff;
											f_currentBoard->size[1] = i11 - i12;
											emxEnsureCapacity((emxArray__common *)f_currentBoard, i13,
												(int)sizeof(double));
											loop_ub = i11 - i12;
											for (i11 = 0; i11 < loop_ub; i11++) {
												for (i13 = 0; i13 < cdiff; i13++) {
													f_currentBoard->data[i13 + f_currentBoard->size[0] *
														i11] = currentBoard->BoardIdx->data[i13 +
														currentBoard->BoardIdx->size[0] * (i12 + i11)];
												}
											}

											i11 = currentBoard->BoardIdx->size[0] *
												currentBoard->BoardIdx->size[1];
											currentBoard->BoardIdx->size[0] = f_currentBoard->size[0];
											currentBoard->BoardIdx->size[1] = f_currentBoard->size[1];
											emxEnsureCapacity((emxArray__common *)
												currentBoard->BoardIdx, i11, (int)sizeof
												(double));
											loop_ub = f_currentBoard->size[1];
											for (i11 = 0; i11 < loop_ub; i11++) {
												cdiff = f_currentBoard->size[0];
												for (i12 = 0; i12 < cdiff; i12++) {
													currentBoard->BoardIdx->data[i12 +
														currentBoard->BoardIdx->size[0] * i11] =
														f_currentBoard->data[i12 + f_currentBoard->size[0] *
														i11];
												}
											}

											// 'CheckerboardImage:301' this.BoardCoords = this.BoardCoords(:, 2:end, :); 
											i11 = currentBoard->BoardCoords->size[1];
											if (2 > i11) {
												i12 = 0;
												i11 = 0;
											}
											else {
												i12 = 1;
											}

											cdiff = currentBoard->BoardCoords->size[0];
											absb = currentBoard->BoardCoords->size[2];
											i13 = g_currentBoard->size[0] * g_currentBoard->size[1] *
												g_currentBoard->size[2];
											g_currentBoard->size[0] = cdiff;
											g_currentBoard->size[1] = i11 - i12;
											g_currentBoard->size[2] = absb;
											emxEnsureCapacity((emxArray__common *)g_currentBoard, i13,
												(int)sizeof(double));
											for (i13 = 0; i13 < absb; i13++) {
												loop_ub = i11 - i12;
												for (ndbl = 0; ndbl < loop_ub; ndbl++) {
													for (apnd = 0; apnd < cdiff; apnd++) {
														g_currentBoard->data[(apnd + g_currentBoard->size[0]
															* ndbl) + g_currentBoard->
															size[0] * g_currentBoard->size[1] * i13] =
															currentBoard->BoardCoords->data[(apnd +
																currentBoard->BoardCoords->size[0] * (i12 + ndbl))
															+ currentBoard->BoardCoords->size[0] *
															currentBoard->BoardCoords->size[1] * i13];
													}
												}
											}

											i11 = currentBoard->BoardCoords->size[0] *
												currentBoard->BoardCoords->size[1] *
												currentBoard->BoardCoords->size[2];
											currentBoard->BoardCoords->size[0] = g_currentBoard->size
												[0];
											currentBoard->BoardCoords->size[1] = g_currentBoard->size
												[1];
											currentBoard->BoardCoords->size[2] = g_currentBoard->size
												[2];
											emxEnsureCapacity((emxArray__common *)
												currentBoard->BoardCoords, i11, (int)
												sizeof(double));
											loop_ub = g_currentBoard->size[2];
											for (i11 = 0; i11 < loop_ub; i11++) {
												cdiff = g_currentBoard->size[1];
												for (i12 = 0; i12 < cdiff; i12++) {
													absb = g_currentBoard->size[0];
													for (i13 = 0; i13 < absb; i13++) {
														currentBoard->BoardCoords->data[(i13 +
															currentBoard->BoardCoords->size[0] * i12) +
															currentBoard->BoardCoords->size[0] *
															currentBoard->BoardCoords->size[1] * i11] =
															g_currentBoard->data[(i13 + g_currentBoard->size[0]
																* i12) + g_currentBoard->size[0] *
															g_currentBoard->size[1] * i11];
													}
												}
											}
											break;

										case 4:
											// 'CheckerboardImage:303' case 4 %'right'
											// 'CheckerboardImage:304' this.BoardIdx = this.BoardIdx(:, 1:end-1); 
											d0 = (double)currentBoard->BoardIdx->size[1] - 1.0;
											if (1.0 > d0) {
												loop_ub = 0;
											}
											else {
												loop_ub = (int)d0;
											}

											cdiff = currentBoard->BoardIdx->size[0];
											i11 = h_currentBoard->size[0] * h_currentBoard->size[1];
											h_currentBoard->size[0] = cdiff;
											h_currentBoard->size[1] = loop_ub;
											emxEnsureCapacity((emxArray__common *)h_currentBoard, i11,
												(int)sizeof(double));
											for (i11 = 0; i11 < loop_ub; i11++) {
												for (i12 = 0; i12 < cdiff; i12++) {
													h_currentBoard->data[i12 + h_currentBoard->size[0] *
														i11] = currentBoard->BoardIdx->data[i12 +
														currentBoard->BoardIdx->size[0] * i11];
												}
											}

											i11 = currentBoard->BoardIdx->size[0] *
												currentBoard->BoardIdx->size[1];
											currentBoard->BoardIdx->size[0] = h_currentBoard->size[0];
											currentBoard->BoardIdx->size[1] = h_currentBoard->size[1];
											emxEnsureCapacity((emxArray__common *)
												currentBoard->BoardIdx, i11, (int)sizeof
												(double));
											loop_ub = h_currentBoard->size[1];
											for (i11 = 0; i11 < loop_ub; i11++) {
												cdiff = h_currentBoard->size[0];
												for (i12 = 0; i12 < cdiff; i12++) {
													currentBoard->BoardIdx->data[i12 +
														currentBoard->BoardIdx->size[0] * i11] =
														h_currentBoard->data[i12 + h_currentBoard->size[0] *
														i11];
												}
											}

											// 'CheckerboardImage:305' this.BoardCoords = this.BoardCoords(:, 1:end-1, :); 
											d0 = (double)currentBoard->BoardCoords->size[1] - 1.0;
											if (1.0 > d0) {
												loop_ub = 0;
											}
											else {
												loop_ub = (int)d0;
											}

											cdiff = currentBoard->BoardCoords->size[0];
											absb = currentBoard->BoardCoords->size[2];
											i11 = i_currentBoard->size[0] * i_currentBoard->size[1] *
												i_currentBoard->size[2];
											i_currentBoard->size[0] = cdiff;
											i_currentBoard->size[1] = loop_ub;
											i_currentBoard->size[2] = absb;
											emxEnsureCapacity((emxArray__common *)i_currentBoard, i11,
												(int)sizeof(double));
											for (i11 = 0; i11 < absb; i11++) {
												for (i12 = 0; i12 < loop_ub; i12++) {
													for (i13 = 0; i13 < cdiff; i13++) {
														i_currentBoard->data[(i13 + i_currentBoard->size[0] *
															i12) + i_currentBoard->size[0]
															* i_currentBoard->size[1] * i11] =
															currentBoard->BoardCoords->data[(i13 +
																currentBoard->BoardCoords->size[0] * i12) +
															currentBoard->BoardCoords->size[0] *
															currentBoard->BoardCoords->size[1] * i11];
													}
												}
											}

											i11 = currentBoard->BoardCoords->size[0] *
												currentBoard->BoardCoords->size[1] *
												currentBoard->BoardCoords->size[2];
											currentBoard->BoardCoords->size[0] = i_currentBoard->size
												[0];
											currentBoard->BoardCoords->size[1] = i_currentBoard->size
												[1];
											currentBoard->BoardCoords->size[2] = i_currentBoard->size
												[2];
											emxEnsureCapacity((emxArray__common *)
												currentBoard->BoardCoords, i11, (int)
												sizeof(double));
											loop_ub = i_currentBoard->size[2];
											for (i11 = 0; i11 < loop_ub; i11++) {
												cdiff = i_currentBoard->size[1];
												for (i12 = 0; i12 < cdiff; i12++) {
													absb = i_currentBoard->size[0];
													for (i13 = 0; i13 < absb; i13++) {
														currentBoard->BoardCoords->data[(i13 +
															currentBoard->BoardCoords->size[0] * i12) +
															currentBoard->BoardCoords->size[0] *
															currentBoard->BoardCoords->size[1] * i11] =
															i_currentBoard->data[(i13 + i_currentBoard->size[0]
																* i12) + i_currentBoard->size[0] *
															i_currentBoard->size[1] * i11];
													}
												}
											}
											break;
										}

										// 'CheckerboardImage:280' this.IsDirectionBad(i) = true;
										currentBoard->IsDirectionBad[b_i] = true;
										guard1 = true;
									}
								}
								else {
									guard1 = true;
								}

								if (guard1) {
									b_i++;
								}
							}
							else {
								// 'CheckerboardImage:284' success = false;
								hasExpanded = false;
								exitg1 = 1;
							}
						} while (exitg1 == 0);
					}
				}

				// 'detectCheckerboardOnImage:64' if currentBoard.Energy < previousBoard.Energy 
				if (currentBoard->Energy < previousBoard->Energy) {
					// 'detectCheckerboardOnImage:65' tmpBoard = previousBoard;
					tmpBoard = previousBoard;

					// 'detectCheckerboardOnImage:66' previousBoard = currentBoard;
					previousBoard = currentBoard;

					// 'detectCheckerboardOnImage:67' currentBoard = tmpBoard;
					currentBoard = tmpBoard;
				}
			}

			i++;
		}

		emxFree_real_T(&i_currentBoard);
		emxFree_real_T(&h_currentBoard);
		emxFree_real_T(&g_currentBoard);
		emxFree_real_T(&f_currentBoard);
		emxFree_real_T(&e_currentBoard);
		emxFree_real_T(&d_currentBoard);
		emxFree_real_T(&c_currentBoard);
		emxFree_real_T(&b_currentBoard);
		emxFree_real_T(&seedIdx);

		// 'detectCheckerboardOnImage:70' board = previousBoard;
		board = previousBoard;
	}

	return board;
}

//
// Arguments    : emxArray_real32_T *varargin_1
//                const emxArray_real_T *varargin_2
// Return Type  : void
//
void imfilter(emxArray_real32_T *varargin_1, const emxArray_real_T
	*varargin_2)
{
	double finalSize[2];
	double filter_center[2];
	double pad[2];
	emxArray_real_T *aaamp;
	emxArray_real_T *bbmpp;
	int r;
	emxArray_real_T *sampp;
	emxArray_real_T *b_samp;
	emxArray_boolean_T *xxtmp;
	emxArray_real_T *c_s;
	emxArray_boolean_T *r55;
	boolean_T guard1 = false;
	int firstRowA;
	boolean_T b_x[2];
	int aidx;
	int cidx;
	boolean_T densityFlag;
	int k;
	boolean_T exitg2;
	emxArray_real32_T *b_aampp;
	emxArray_real32_T *h;
	emxArray_real_T *d_s;
	double prePadSize[2];
	emxArray_real_T *hcol;
	double tol;
	boolean_T exitg1;
	emxArray_real32_T *a_prepad_tmp;
	emxArray_real_T *hrow;
	int iv1[2];
	emxArray_int32_T *r56;
	double out_size_row[2];
	int c_a;
	int d_a;
	int e_a;
	int f_a;
	int ma;
	unsigned int hrow_idx_0;
	int na;
	int c;
	int i;
	int firstColB;
	int lastColB;
	int firstRowB;
	int lastRowB;
	emxArray_boolean_T *connb;
	int lastColA;
	int b_firstColB;
	int iC;
	boolean_T tooBig;
	int b_c;
	int iB;
	int b_i;
	int a_length;
	double padSize[2];
	finalSize[0] = varargin_1->size[0];
	filter_center[0] = floor(((double)varargin_2->size[0] + 1.0) / 2.0);
	pad[0] = (double)varargin_2->size[0] - filter_center[0];
	finalSize[1] = varargin_1->size[1];
	filter_center[1] = floor(((double)varargin_2->size[1] + 1.0) / 2.0);
	pad[1] = (double)varargin_2->size[1] - filter_center[1];
	if ((varargin_1->size[0] == 0) || (varargin_1->size[1] == 0)) {
	}
	else if ((varargin_2->size[0] == 0) || (varargin_2->size[1] == 0)) {
		for (r = 0; r < 2; r++) {
			filter_center[r] = varargin_1->size[r];
		}

		r = varargin_1->size[0] * varargin_1->size[1];
		varargin_1->size[0] = (int)filter_center[0];
		varargin_1->size[1] = (int)filter_center[1];
		emxEnsureCapacity((emxArray__common *)varargin_1, r, (int)sizeof(float));
		firstRowA = (int)filter_center[1];
		for (r = 0; r < firstRowA; r++) {
			aidx = (int)filter_center[0];
			for (cidx = 0; cidx < aidx; cidx++) {
				varargin_1->data[cidx + varargin_1->size[0] * r] = 0.0F;
			}
		}
	}
	else {
		emxInit_real_T(&aaamp, 2);
		emxInit_real_T(&bbmpp, 2);
		emxInit_real_T(&sampp, 2);
		emxInit_real_T(&b_samp, 1);
		emxInit_boolean_T(&xxtmp, 1);
		emxInit_real_T(&c_s, 2);
		emxInit_boolean_T(&r55, 1);
		guard1 = false;
		if (varargin_2->size[0] * varargin_2->size[1] >= 289) {
			for (r = 0; r < 2; r++) {
				b_x[r] = (varargin_2->size[r] != 1);
			}

			densityFlag = true;
			k = 0;
			exitg2 = false;
			while ((!exitg2) && (k < 2)) {
				if (!b_x[k]) {
					densityFlag = false;
					exitg2 = true;
				}
				else {
					k++;
				}
			}

			if (densityFlag) {
				firstRowA = varargin_2->size[0] * varargin_2->size[1];
				r = r55->size[0];
				r55->size[0] = firstRowA;
				emxEnsureCapacity((emxArray__common *)r55, r, (int)sizeof(boolean_T));
				for (r = 0; r < firstRowA; r++) {
					r55->data[r] = true;
				}

				if (all(r55)) {
					emxFree_boolean_T(&r55);
					svd(varargin_2, aaamp, sampp, bbmpp);
					firstRowA = sampp->size[0];
					aidx = sampp->size[1];
					r = c_s->size[0] * c_s->size[1];
					c_s->size[0] = firstRowA;
					c_s->size[1] = aidx;
					emxEnsureCapacity((emxArray__common *)c_s, r, (int)sizeof(double));
					for (r = 0; r < aidx; r++) {
						for (cidx = 0; cidx < firstRowA; cidx++) {
							c_s->data[cidx + c_s->size[0] * r] = sampp->data[cidx + sampp->size[0] * r];
						}
					}

					diag(c_s, b_samp);
					emxFree_real_T(&c_s);
					firstRowA = varargin_2->size[0];
					aidx = varargin_2->size[1];
					if (firstRowA >= aidx) {
						aidx = firstRowA;
					}

					tol = b_samp->data[0];
					if (b_samp->size[0] > 1) {
						for (firstRowA = 1; firstRowA + 1 <= b_samp->size[0]; firstRowA++) {
							if (b_samp->data[firstRowA] > tol) {
								tol = b_samp->data[firstRowA];
							}
						}
					}

					tol = (double)aidx * tol * 2.2204460492503131E-16;
					r = xxtmp->size[0];
					xxtmp->size[0] = b_samp->size[0];
					emxEnsureCapacity((emxArray__common *)xxtmp, r, (int)sizeof(boolean_T));
					firstRowA = b_samp->size[0];
					for (r = 0; r < firstRowA; r++) {
						xxtmp->data[r] = (b_samp->data[r] > tol);
					}

					if (xxtmp->size[0] == 0) {
						tol = 0.0;
					}
					else {
						tol = xxtmp->data[0];
						for (k = 2; k <= xxtmp->size[0]; k++) {
							tol += (double)xxtmp->data[k - 1];
						}
					}

					densityFlag = (tol == 1.0);
				}
				else {
					guard1 = true;
				}
			}
			else {
				guard1 = true;
			}
		}
		else {
			emxFree_boolean_T(&r55);
			emxFree_real_T(&c_s);
			guard1 = true;
		}

		if (guard1) {
			densityFlag = false;
		}

		//emxFree_boolean_T(&r55);
		//emxFree_real_T(&c_s);




		emxInit_real32_T(&b_aampp, 2);
		if (densityFlag) {
			emxInit_real_T(&d_s, 2);
			padImage(varargin_1, pad, b_aampp);
			svd(varargin_2, aaamp, sampp, bbmpp);
			firstRowA = sampp->size[0];
			aidx = sampp->size[1];
			r = d_s->size[0] * d_s->size[1];
			d_s->size[0] = firstRowA;
			d_s->size[1] = aidx;
			emxEnsureCapacity((emxArray__common *)d_s, r, (int)sizeof(double));
			for (r = 0; r < aidx; r++) {
				for (cidx = 0; cidx < firstRowA; cidx++) {
					d_s->data[cidx + d_s->size[0] * r] = sampp->data[cidx + sampp->size[0] * r];
				}
			}

			
			emxFree_real_T(&sampp);

			emxInit_real_T(&hcol, 1);
			diag(d_s, b_samp);
			tol = sqrt(b_samp->data[0]);
			firstRowA = aaamp->size[0];
			r = hcol->size[0];
			hcol->size[0] = firstRowA;
			emxEnsureCapacity((emxArray__common *)hcol, r, (int)sizeof(double));
			emxFree_real_T(&d_s);
			for (r = 0; r < firstRowA; r++) {
				hcol->data[r] = aaamp->data[r] * tol;
			}

			emxInit_real_T(&hrow, 2);
			tol = sqrt(b_samp->data[0]);
			firstRowA = bbmpp->size[0];
			r = hrow->size[0] * hrow->size[1];
			hrow->size[0] = 1;
			hrow->size[1] = firstRowA;
			emxEnsureCapacity((emxArray__common *)hrow, r, (int)sizeof(double));
			for (r = 0; r < firstRowA; r++) {
				hrow->data[hrow->size[0] * r] = bbmpp->data[r] * tol;
			}

			r = aaamp->size[0] * aaamp->size[1];
			aaamp->size[0] = b_aampp->size[0];
			aaamp->size[1] = b_aampp->size[1];
			emxEnsureCapacity((emxArray__common *)aaamp, r, (int)sizeof(double));
			firstRowA = b_aampp->size[0] * b_aampp->size[1];
			for (r = 0; r < firstRowA; r++) {
				aaamp->data[r] = b_aampp->data[r];
			}

			emxFree_real32_T(&b_aampp);

			emxInit_int32_T(&r56, 1);
			out_size_row[0] = aaamp->size[0];
			out_size_row[1] = finalSize[1];
			filter_center[0] = 0.0;
			filter_center[1] = (int)pad[1];
			hrow_idx_0 = (unsigned int)hrow->size[1];
			aidx = (int)hrow_idx_0 - 1;
			firstRowA = 0;
			for (i = 0; i <= aidx; i++) {
				if (hrow->data[i] != 0.0) {
					firstRowA++;
				}
			}

			r = r56->size[0];
			r56->size[0] = firstRowA;
			emxEnsureCapacity((emxArray__common *)r56, r, (int)sizeof(int));
			firstRowA = 0;
			for (i = 0; i <= aidx; i++) {
				if (hrow->data[i] != 0.0) {
					r56->data[firstRowA] = i + 1;
					firstRowA++;
				}
			}

			r = b_samp->size[0];
			b_samp->size[0] = r56->size[0];
			emxEnsureCapacity((emxArray__common *)b_samp, r, (int)sizeof(double));
			firstRowA = r56->size[0];
			for (r = 0; r < firstRowA; r++) {
				b_samp->data[r] = hrow->data[r56->data[r] - 1];
			}

			emxInit_boolean_T(&connb, 2);
			r = connb->size[0] * connb->size[1];
			connb->size[0] = 1;
			connb->size[1] = hrow->size[1];
			emxEnsureCapacity((emxArray__common *)connb, r, (int)sizeof(boolean_T));
			firstRowA = hrow->size[0] * hrow->size[1];
			for (r = 0; r < firstRowA; r++) {
				connb->data[r] = (hrow->data[r] != 0.0);
			}

			densityFlag = false;
			if ((double)r56->size[0] / (double)hrow->size[1] > 0.05) {
				densityFlag = true;
			}

			tooBig = true;
			for (i = 0; i < 2; i++) {
				if (tooBig && (out_size_row[i] > 65500.0)) {
					tooBig = true;
				}
				else {
					tooBig = false;
				}
			}

			if (densityFlag && (!tooBig)) {
				densityFlag = true;
			}
			else {
				densityFlag = false;
			}

			r = bbmpp->size[0] * bbmpp->size[1];
			bbmpp->size[0] = (int)out_size_row[0];
			bbmpp->size[1] = (int)finalSize[1];
			emxEnsureCapacity((emxArray__common *)bbmpp, r, (int)sizeof(double));
			if (densityFlag) {
				for (r = 0; r < 2; r++) {
					padSize[r] = aaamp->size[r];
				}

				for (r = 0; r < 2; r++) {
					filter_center[r] = hrow->size[r];
				}

				ippfilter_real64(&aaamp->data[0], &bbmpp->data[0], out_size_row, 2.0, padSize,
					&hrow->data[0], filter_center, true);
			}
			else {
				for (r = 0; r < 2; r++) {
					padSize[r] = aaamp->size[r];
				}

				for (r = 0; r < 2; r++) {
					prePadSize[r] = connb->size[r];
				}

				imfilter_real64(&aaamp->data[0], &bbmpp->data[0], 2.0, out_size_row, 2.0,
					padSize, &b_samp->data[0], (double)r56->size[0],
					&connb->data[0], 2.0, prePadSize, filter_center, 2.0,
					true, true);
			}

			emxFree_boolean_T(&connb);
			emxFree_real_T(&hrow);
			filter_center[0] = (int)pad[0];
			filter_center[1] = 0.0;
			hrow_idx_0 = (unsigned int)hcol->size[0];
			aidx = (int)hrow_idx_0 - 1;
			firstRowA = 0;
			for (i = 0; i <= aidx; i++) {
				if (hcol->data[i] != 0.0) {
					firstRowA++;
				}
			}

			r = r56->size[0];
			r56->size[0] = firstRowA;
			emxEnsureCapacity((emxArray__common *)r56, r, (int)sizeof(int));
			firstRowA = 0;
			for (i = 0; i <= aidx; i++) {
				if (hcol->data[i] != 0.0) {
					r56->data[firstRowA] = i + 1;
					firstRowA++;
				}
			}

			r = b_samp->size[0];
			b_samp->size[0] = r56->size[0];
			emxEnsureCapacity((emxArray__common *)b_samp, r, (int)sizeof(double));
			firstRowA = r56->size[0];
			for (r = 0; r < firstRowA; r++) {
				b_samp->data[r] = hcol->data[r56->data[r] - 1];
			}

			r = xxtmp->size[0];
			xxtmp->size[0] = hcol->size[0];
			emxEnsureCapacity((emxArray__common *)xxtmp, r, (int)sizeof(boolean_T));
			firstRowA = hcol->size[0];
			for (r = 0; r < firstRowA; r++) {
				xxtmp->data[r] = (hcol->data[r] != 0.0);
			}

			densityFlag = false;
			if ((double)r56->size[0] / (double)hcol->size[0] > 0.05) {
				densityFlag = true;
			}

			tooBig = true;
			for (i = 0; i < 2; i++) {
				if (tooBig && (finalSize[i] > 65500.0)) {
					tooBig = true;
				}
				else {
					tooBig = false;
				}
			}

			if (densityFlag && (!tooBig)) {
				densityFlag = true;
			}
			else {
				densityFlag = false;
			}

			r = aaamp->size[0] * aaamp->size[1];
			aaamp->size[0] = bbmpp->size[0];
			aaamp->size[1] = bbmpp->size[1];
			emxEnsureCapacity((emxArray__common *)aaamp, r, (int)sizeof(double));
			firstRowA = bbmpp->size[0] * bbmpp->size[1];
			for (r = 0; r < firstRowA; r++) {
				aaamp->data[r] = bbmpp->data[r];
			}

			r = bbmpp->size[0] * bbmpp->size[1];
			bbmpp->size[0] = (int)finalSize[0];
			bbmpp->size[1] = (int)finalSize[1];
			emxEnsureCapacity((emxArray__common *)bbmpp, r, (int)sizeof(double));
			if (densityFlag) {
				for (r = 0; r < 2; r++) {
					padSize[r] = aaamp->size[r];
				}

				filter_center[0] = hcol->size[0];
				filter_center[1] = 1.0;
				ippfilter_real64(&aaamp->data[0], &bbmpp->data[0], finalSize, 2.0, padSize,
					&hcol->data[0], filter_center, true);
			}
			else {
				for (r = 0; r < 2; r++) {
					padSize[r] = aaamp->size[r];
				}

				prePadSize[0] = xxtmp->size[0];
				prePadSize[1] = 1.0;
				imfilter_real64(&aaamp->data[0], &bbmpp->data[0], 2.0, finalSize, 2.0, padSize,
					&b_samp->data[0], (double)r56->size[0], &xxtmp->data[0], 2.0,
					prePadSize, filter_center, 2.0, true, true);
			}
			emxFree_real_T(&aaamp);

			emxFree_boolean_T(&xxtmp);
			emxFree_real_T(&b_samp);

			emxFree_int32_T(&r56);
			emxFree_real_T(&hcol);
			r = varargin_1->size[0] * varargin_1->size[1];
			varargin_1->size[0] = bbmpp->size[0];
			varargin_1->size[1] = bbmpp->size[1];
			emxEnsureCapacity((emxArray__common *)varargin_1, r, (int)sizeof(float));
			firstRowA = bbmpp->size[1];
			for (r = 0; r < firstRowA; r++) {
				aidx = bbmpp->size[0];
				for (cidx = 0; cidx < aidx; cidx++) {
					varargin_1->data[cidx + varargin_1->size[0] * r] = (float)bbmpp->data[cidx
						+ bbmpp->size[0] * r];
				}
			}
			emxFree_real_T(&bbmpp);
		}
		else {
			emxFree_boolean_T(&xxtmp);

			emxFree_real_T(&b_samp);
			emxFree_real_T(&bbmpp);
			emxFree_real_T(&sampp);
			emxFree_real_T(&aaamp);

			emxInit_real32_T(&h, 2);
			r = h->size[0] * h->size[1];
			h->size[0] = varargin_2->size[0];
			h->size[1] = varargin_2->size[1];
			emxEnsureCapacity((emxArray__common *)h, r, (int)sizeof(float));
			firstRowA = varargin_2->size[0] * varargin_2->size[1];
			for (r = 0; r < firstRowA; r++) {
				h->data[r] = (float)varargin_2->data[r];
			}

			for (r = 0; r < 2; r++) {
				prePadSize[r] = h->size[r];
			}

			for (k = 0; k < 2; k++) {
				tol = 1.0 - ((double)(unsigned int)prePadSize[k] - floor((double)
					(unsigned int)prePadSize[k] / 2.0) * 2.0);
				prePadSize[k] = pad[k] - tol;
				filter_center[k] = tol;
			}

			densityFlag = false;
			k = 0;
			exitg1 = false;
			while ((!exitg1) && (k < 2)) {
				if (!!(filter_center[k] == 1.0)) {
					densityFlag = true;
					exitg1 = true;
				}
				else {
					k++;
				}
			}

			if (densityFlag) {
				emxInit_real32_T(&a_prepad_tmp, 2);
				padarray(varargin_1, prePadSize, a_prepad_tmp);
				b_padarray(a_prepad_tmp, pad, b_aampp);
				emxFree_real32_T(&a_prepad_tmp);
			}
			else {
				padImage(varargin_1, pad, b_aampp);
			}

			for (r = 0; r < 2; r++) {
				filter_center[r] = h->size[r];
			}

			for (r = 0; r < 2; r++) {
				tol = ((double)b_aampp->size[r] - filter_center[r]) + (double)
					(filter_center[r] > 0.0);
				filter_center[r] = tol;
			}

			for (k = 0; k < 2; k++) {
				if (0 >= (int)filter_center[k]) {
					iv1[k] = 0;
				}
				else {
					iv1[k] = (int)filter_center[k];
				}
			}

			r = varargin_1->size[0] * varargin_1->size[1];
			varargin_1->size[0] = iv1[0];
			varargin_1->size[1] = iv1[1];
			emxEnsureCapacity((emxArray__common *)varargin_1, r, (int)sizeof(float));
			firstRowA = iv1[1];
			for (r = 0; r < firstRowA; r++) {
				aidx = iv1[0];
				for (cidx = 0; cidx < aidx; cidx++) {
					varargin_1->data[cidx + varargin_1->size[0] * r] = 0.0F;
				}
			}

			if ((b_aampp->size[0] == 0) || (b_aampp->size[1] == 0) || ((iv1[0] == 0) || (iv1[1]
				== 0))) {
				densityFlag = true;
			}
			else {
				densityFlag = false;
			}

			if (!densityFlag) {
				c_a = h->size[1] - 1;
				d_a = b_aampp->size[1];
				e_a = h->size[0] - 1;
				f_a = b_aampp->size[0];
				ma = b_aampp->size[0];
				na = b_aampp->size[1];
				c = b_aampp->size[0] - h->size[0];
				if (b_aampp->size[1] < h->size[1] - 1) {
					firstColB = h->size[1] - b_aampp->size[1];
				}
				else {
					firstColB = 0;
				}

				if (h->size[1] <= b_aampp->size[1] - 1) {
					lastColB = h->size[1];
				}
				else {
					lastColB = b_aampp->size[1];
				}

				if (b_aampp->size[0] < h->size[0] - 1) {
					firstRowB = h->size[0] - b_aampp->size[0];
				}
				else {
					firstRowB = 0;
				}

				if (h->size[0] <= b_aampp->size[0] - 1) {
					lastRowB = h->size[0];
				}
				else {
					lastRowB = b_aampp->size[0];
				}

				while (firstColB <= lastColB - 1) {
					if ((firstColB + na) - 1 < d_a - 1) {
						lastColA = na;
					}
					else {
						lastColA = d_a - firstColB;
					}

					if (firstColB < c_a) {
						k = c_a - firstColB;
					}
					else {
						k = 0;
					}

					while (k <= lastColA - 1) {
						if (firstColB + k > c_a) {
							b_firstColB = (firstColB + k) - c_a;
						}
						else {
							b_firstColB = 0;
						}

						iC = b_firstColB * (c + 1);
						b_c = k * ma;
						iB = firstRowB + firstColB * h->size[0];
						for (i = firstRowB; i < lastRowB; i++) {
							if (i < e_a) {
								firstRowA = e_a - i;
							}
							else {
								firstRowA = 0;
							}

							if (i + ma <= f_a - 1) {
								b_i = ma;
							}
							else {
								b_i = f_a - i;
							}

							a_length = b_i - firstRowA;
							aidx = b_c + firstRowA;
							cidx = iC;
							for (r = 1; r <= a_length; r++) {
								varargin_1->data[cidx] += h->data[iB] * b_aampp->data[aidx];
								aidx++;
								cidx++;
							}

							iB++;
							if (i >= e_a) {
								iC++;
							}
						}

						k++;
					}

					firstColB++;
				}
			}
			emxFree_real32_T(&b_aampp);
			emxFree_real32_T(&h);
		}

		//emxFree_boolean_T(&xxtmp);
		//emxFree_real_T(&b_samp);
		//emxFree_real_T(&sampp);
		//emxFree_real_T(&bbmpp);
		//emxFree_real_T(&aaamp);
		//emxFree_real32_T(&b_aampp);
	}
}

//
// Returns local maxima in the image metric. quality is a threshold
//  expressed as a fraction of the maximum value of metric.
// Arguments    : const emxArray_real32_T *metric
//                double quality
//                emxArray_real32_T *loc
// Return Type  : void
//
void imgFindPeaks(const emxArray_real32_T *metric, double quality,
	emxArray_real32_T *loc)
{
	float mtmp;
	int k;
	int nx;
	emxArray_boolean_T *bw;
	int i8;
	int exitg4;
	double imSize[2];
	boolean_T conn[9];
	double connSize[2];
	float threshold;
	emxArray_boolean_T *last_aout;
	boolean_T p;
	boolean_T b_p;
	int exitg3;
	boolean_T exitg2;
	emxArray_int32_T *ii;
	int idx;
	boolean_T exitg1;
	boolean_T guard1 = false;
	emxArray_real_T *b_idx;
	emxArray_int32_T *vk;

	// 'imgFindPeaks:5' maxMetric = max(metric(:));
	mtmp = metric->data[0];
	k = metric->size[0] * metric->size[1];
	if (k > 1) {
		k = metric->size[0] * metric->size[1];
		if (1 < k) {
			nx = 1;
			do {
				exitg4 = 0;
				k = metric->size[0] * metric->size[1];
				if (nx + 1 <= k) {
					if (metric->data[nx] > mtmp) {
						mtmp = metric->data[nx];
					}

					nx++;
				}
				else {
					exitg4 = 1;
				}
			} while (exitg4 == 0);
		}
	}

	// 'imgFindPeaks:6' if maxMetric <= eps(0)
	if (mtmp <= 4.94065645841247E-324) {
		// 'imgFindPeaks:7' loc = zeros(0,2,'single');
		i8 = loc->size[0] * loc->size[1];
		loc->size[0] = 0;
		loc->size[1] = 2;
		emxEnsureCapacity((emxArray__common *)loc, i8, (int)sizeof(float));
	}
	else {
		emxInit_boolean_T(&bw, 2);

		// 'imgFindPeaks:8' else
		// 'imgFindPeaks:10' bw = imregionalmax(metric, 8);
		for (i8 = 0; i8 < 2; i8++) {
			k = bw->size[0] * bw->size[1];
			bw->size[i8] = metric->size[i8];
			emxEnsureCapacity((emxArray__common *)bw, k, (int)sizeof(boolean_T));
		}

		for (i8 = 0; i8 < 2; i8++) {
			imSize[i8] = metric->size[i8];
		}

		for (i8 = 0; i8 < 9; i8++) {
			conn[i8] = true;
		}

		for (i8 = 0; i8 < 2; i8++) {
			connSize[i8] = 3.0;
		}

		imregionalmax_real32(&metric->data[0], &bw->data[0], 2.0, imSize, conn, 2.0,
			connSize);

		// 'imgFindPeaks:12' threshold = quality * maxMetric;
		threshold = (float)quality * mtmp;

		// 'imgFindPeaks:13' bw(metric < threshold) = 0;
		k = metric->size[0] * metric->size[1];
		for (nx = 0; nx < k; nx++) {
			if (metric->data[nx] < threshold) {
				bw->data[nx] = false;
			}
		}

		// 'imgFindPeaks:15' bw = bwmorphShrink(bw);
		// 'bwmorphShrink:2' img = (img ~= 0);
		// 'bwmorphShrink:4' bwout = img;
		i8 = bw->size[0] * bw->size[1];
		emxEnsureCapacity((emxArray__common *)bw, i8, (int)sizeof(boolean_T));

		// 'bwmorphShrink:6' while true
		emxInit_boolean_T(&last_aout, 2);
		do {
			// 'bwmorphShrink:7' last_aout   = bwout;
			i8 = last_aout->size[0] * last_aout->size[1];
			last_aout->size[0] = bw->size[0];
			last_aout->size[1] = bw->size[1];
			emxEnsureCapacity((emxArray__common *)last_aout, i8, (int)sizeof(boolean_T));
			k = bw->size[0] * bw->size[1];
			for (i8 = 0; i8 < k; i8++) {
				last_aout->data[i8] = bw->data[i8];
			}

			// 'bwmorphShrink:8' bwout = bwmorphApplyOnce(bwout);
			bwmorphApplyOnce(bw);

			// 'bwmorphShrink:10' if(isequal(last_aout, bwout))
			p = false;
			b_p = false;
			k = 0;
			do {
				exitg3 = 0;
				if (k < 2) {
					if (last_aout->size[k] != bw->size[k]) {
						exitg3 = 1;
					}
					else {
						k++;
					}
				}
				else {
					b_p = true;
					exitg3 = 1;
				}
			} while (exitg3 == 0);

			if (b_p && (!((last_aout->size[0] == 0) || (last_aout->size[1] == 0))) &&
				(!((bw->size[0] == 0) || (bw->size[1] == 0)))) {
				k = 0;
				exitg2 = false;
				while ((!exitg2) && (k <= bw->size[0] * bw->size[1] - 1)) {
					if (last_aout->data[k] != bw->data[k]) {
						b_p = false;
						exitg2 = true;
					}
					else {
						k++;
					}
				}
			}

			if (!b_p) {
			}
			else {
				p = true;
			}
		} while (!p);

		emxFree_boolean_T(&last_aout);

		// > the output is not changing anymore
		//  Exclude points on the border
		// 'imgFindPeaks:18' bw(1, :) = 0;
		k = bw->size[1];
		for (i8 = 0; i8 < k; i8++) {
			bw->data[bw->size[0] * i8] = false;
		}

		// 'imgFindPeaks:19' bw(end, :) = 0;
		k = bw->size[1];
		nx = bw->size[0] - 1;
		for (i8 = 0; i8 < k; i8++) {
			bw->data[nx + bw->size[0] * i8] = false;
		}

		// 'imgFindPeaks:20' bw(:, 1) = 0;
		k = bw->size[0];
		for (i8 = 0; i8 < k; i8++) {
			bw->data[i8] = false;
		}

		// 'imgFindPeaks:21' bw(:, end) = 0;
		k = bw->size[0];
		nx = bw->size[1] - 1;
		for (i8 = 0; i8 < k; i8++) {
			bw->data[i8 + bw->size[0] * nx] = false;
		}

		emxInit_int32_T(&ii, 1);

		//  Find location of the peaks
		// 'imgFindPeaks:24' idx = find(bw);
		nx = bw->size[0] * bw->size[1];
		idx = 0;
		i8 = ii->size[0];
		ii->size[0] = nx;
		emxEnsureCapacity((emxArray__common *)ii, i8, (int)sizeof(int));
		k = 1;
		exitg1 = false;
		while ((!exitg1) && (k <= nx)) {
			guard1 = false;
			if (bw->data[k - 1]) {
				idx++;
				ii->data[idx - 1] = k;
				if (idx >= nx) {
					exitg1 = true;
				}
				else {
					guard1 = true;
				}
			}
			else {
				guard1 = true;
			}

			if (guard1) {
				k++;
			}
		}

		emxFree_boolean_T(&bw);
		if (nx == 1) {
			if (idx == 0) {
				i8 = ii->size[0];
				ii->size[0] = 0;
				emxEnsureCapacity((emxArray__common *)ii, i8, (int)sizeof(int));
			}
		}
		else {
			i8 = ii->size[0];
			if (1 > idx) {
				ii->size[0] = 0;
			}
			else {
				ii->size[0] = idx;
			}

			emxEnsureCapacity((emxArray__common *)ii, i8, (int)sizeof(int));
		}

		emxInit_real_T(&b_idx, 1);
		i8 = b_idx->size[0];
		b_idx->size[0] = ii->size[0];
		emxEnsureCapacity((emxArray__common *)b_idx, i8, (int)sizeof(double));
		k = ii->size[0];
		for (i8 = 0; i8 < k; i8++) {
			b_idx->data[i8] = ii->data[i8];
		}

		// 'imgFindPeaks:25' loc = zeros([length(idx) 2], 'like', metric );
		imSize[0] = b_idx->size[0];
		i8 = loc->size[0] * loc->size[1];
		loc->size[0] = (int)imSize[0];
		loc->size[1] = 2;
		emxEnsureCapacity((emxArray__common *)loc, i8, (int)sizeof(float));
		k = (int)imSize[0] << 1;
		for (i8 = 0; i8 < k; i8++) {
			loc->data[i8] = 0.0F;
		}

		// 'imgFindPeaks:26' [loc(:, 2), loc(:, 1)] = ind2sub(size(metric), idx);
		for (i8 = 0; i8 < 2; i8++) {
			connSize[i8] = metric->size[i8];
		}

		i8 = ii->size[0];
		ii->size[0] = b_idx->size[0];
		emxEnsureCapacity((emxArray__common *)ii, i8, (int)sizeof(int));
		k = b_idx->size[0];
		for (i8 = 0; i8 < k; i8++) {
			ii->data[i8] = (int)b_idx->data[i8];
		}

		emxFree_real_T(&b_idx);
		i8 = ii->size[0];
		emxEnsureCapacity((emxArray__common *)ii, i8, (int)sizeof(int));
		k = ii->size[0];
		for (i8 = 0; i8 < k; i8++) {
			ii->data[i8]--;
		}

		emxInit_int32_T(&vk, 1);
		i8 = vk->size[0];
		vk->size[0] = ii->size[0];
		emxEnsureCapacity((emxArray__common *)vk, i8, (int)sizeof(int));
		k = ii->size[0];
		for (i8 = 0; i8 < k; i8++) {
			vk->data[i8] = ii->data[i8] / (int)(unsigned int)connSize[0];
		}

		i8 = ii->size[0];
		emxEnsureCapacity((emxArray__common *)ii, i8, (int)sizeof(int));
		k = ii->size[0];
		for (i8 = 0; i8 < k; i8++) {
			ii->data[i8] -= vk->data[i8] * (int)(unsigned int)connSize[0];
		}

		k = ii->size[0];
		for (i8 = 0; i8 < k; i8++) {
			loc->data[i8 + loc->size[0]] = (float)(ii->data[i8] + 1);
		}

		emxFree_int32_T(&ii);
		k = vk->size[0];
		for (i8 = 0; i8 < k; i8++) {
			loc->data[i8] = (float)(vk->data[i8] + 1);
		}

		emxFree_int32_T(&vk);
	}
}

//
// Arguments    : const emxArray_real32_T *x
// Return Type  : float
//
float mean(const emxArray_real32_T *x)
{
	float y;
	int k;
	if (x->size[0] == 0) {
		y = 0.0F;
	}
	else {
		y = x->data[0];
		for (k = 2; k <= x->size[0]; k++) {
			y += x->data[k - 1];
		}
	}

	y /= (float)x->size[0];
	return y;
}

//
// Arguments    : emxArray_int32_T *idx
//                emxArray_real32_T *x
//                int offset
//                int np
//                int nq
//                emxArray_int32_T *iwork
//                emxArray_real32_T *xwork
// Return Type  : void
//
void merge(emxArray_int32_T *idx, emxArray_real32_T *x, int offset, int
	np, int nq, emxArray_int32_T *iwork, emxArray_real32_T *xwork)
{
	int n;
	int qend;
	int p;
	int iout;
	int exitg1;
	if (nq == 0) {
	}
	else {
		n = np + nq;
		for (qend = 0; qend + 1 <= n; qend++) {
			iwork->data[qend] = idx->data[offset + qend];
			xwork->data[qend] = x->data[offset + qend];
		}

		p = 0;
		n = np;
		qend = np + nq;
		iout = offset - 1;
		do {
			exitg1 = 0;
			iout++;
			if (xwork->data[p] >= xwork->data[n]) {
				idx->data[iout] = iwork->data[p];
				x->data[iout] = xwork->data[p];
				if (p + 1 < np) {
					p++;
				}
				else {
					exitg1 = 1;
				}
			}
			else {
				idx->data[iout] = iwork->data[n];
				x->data[iout] = xwork->data[n];
				if (n + 1 < qend) {
					n++;
				}
				else {
					n = (iout - p) + 1;
					while (p + 1 <= np) {
						idx->data[n + p] = iwork->data[p];
						x->data[n + p] = xwork->data[p];
						p++;
					}

					exitg1 = 1;
				}
			}
		} while (exitg1 == 0);
	}
}

//
// Arguments    : emxArray_int32_T *idx
//                emxArray_real32_T *x
//                int offset
//                int n
//                int preSortLevel
//                emxArray_int32_T *iwork
//                emxArray_real32_T *xwork
// Return Type  : void
//
void merge_block(emxArray_int32_T *idx, emxArray_real32_T *x, int offset,
	int n, int preSortLevel, emxArray_int32_T *iwork, emxArray_real32_T *xwork)
{
	int nPairs;
	int bLen;
	int tailOffset;
	int nTail;
	nPairs = n >> preSortLevel;
	bLen = 1 << preSortLevel;
	while (nPairs > 1) {
		if ((nPairs & 1) != 0) {
			nPairs--;
			tailOffset = bLen * nPairs;
			nTail = n - tailOffset;
			if (nTail > bLen) {
				merge(idx, x, offset + tailOffset, bLen, nTail - bLen, iwork, xwork);
			}
		}

		tailOffset = bLen << 1;
		nPairs >>= 1;
		for (nTail = 1; nTail <= nPairs; nTail++) {
			merge(idx, x, offset + (nTail - 1) * tailOffset, bLen, bLen, iwork, xwork);
		}

		bLen = tailOffset;
	}

	if (n > bLen) {
		merge(idx, x, offset, bLen, n - bLen, iwork, xwork);
	}
}

//
// Arguments    : const emxArray_real_T *x
// Return Type  : double
//
double norm(const emxArray_real_T *x)
{
	double y;
	double scale;
	int k;
	double absxk;
	double t;
	if (x->size[0] == 0) {
		y = 0.0;
	}
	else {
		y = 0.0;
		if (x->size[0] == 1) {
			y = fabs(x->data[0]);
		}
		else {
			scale = 2.2250738585072014E-308;
			for (k = 1; k <= x->size[0]; k++) {
				absxk = fabs(x->data[k - 1]);
				if (absxk > scale) {
					t = scale / absxk;
					y = 1.0 + y * t * t;
					scale = absxk;
				}
				else {
					t = absxk / scale;
					y += t * t;
				}
			}

			y = scale * sqrt(y);
		}
	}

	return y;
}

//
// Arguments    : CheckerboardImage *board
//                const emxArray_real32_T *I
// Return Type  : CheckerboardImage *
//
CheckerboardImage *orient(CheckerboardImage *board, const
	emxArray_real32_T *I)
{
	CheckerboardImage *b_board;
	emxArray_real_T *r47;
	emxArray_boolean_T *upperLeftMask;
	emxArray_boolean_T *nextSquareMask;
	CheckerboardImage *c_board;
	emxArray_int32_T *r48;
	int j;
	int i1;
	double upperLeftPolyX[4];
	emxArray_real_T *d_board;
	double upperLeftPolyY[4];
	int k;
	emxArray_real_T *newBoardCoords1;
	int vspread;
	emxArray_real_T *e_board;
	emxArray_real_T *newBoardCoords2;
	emxArray_real_T *r49;
	emxArray_int32_T *r50;
	emxArray_real32_T *b_I;
	emxArray_real32_T *c_I;
	float x;
	float b_x;
	emxArray_real_T *r51;
	emxArray_real_T *r52;
	emxArray_boolean_T *c_x;
	boolean_T y;
	int ix;
	boolean_T exitg1;
	boolean_T b1;
	b_board = board;

	// --------------------------------------------------------------------------
	// 'detectCheckerboardOnImage:168' if board.Energy~=10e10
	if (b_board->Energy != 1.0E+11) {
		//  orient the board so that the long side is the X-axis
		// 'detectCheckerboardOnImage:170' if size(board.BoardCoords, 1) < size(board.BoardCoords, 2) 
		if (b_board->BoardCoords->size[0] < b_board->BoardCoords->size[1]) {
			emxInit_real_T(&r47, 2);

			// 'detectCheckerboardOnImage:171' board = rot90_checkerboard(board, 1);
			c_board = b_board;
			b_board = c_board;

			// -------------------------------------------------------------------------- 
			// 'detectCheckerboardOnImage:192' board.BoardIdx = rot90(board.BoardIdx, k); 
			rot90(b_board->BoardIdx, r47);
			j = b_board->BoardIdx->size[0] * b_board->BoardIdx->size[1];
			b_board->BoardIdx->size[0] = r47->size[0];
			b_board->BoardIdx->size[1] = r47->size[1];
			emxEnsureCapacity((emxArray__common *)b_board->BoardIdx, j, (int)sizeof
			(double));
			i1 = r47->size[0] * r47->size[1];
			for (j = 0; j < i1; j++) {
				b_board->BoardIdx->data[j] = r47->data[j];
			}

			emxFree_real_T(&r47);
			emxInit_real_T(&d_board, 2);

			// 'detectCheckerboardOnImage:193' newBoardCoords1 = rot90(board.BoardCoords(:,:,1), k); 
			i1 = b_board->BoardCoords->size[0];
			k = b_board->BoardCoords->size[1];
			j = d_board->size[0] * d_board->size[1];
			d_board->size[0] = i1;
			d_board->size[1] = k;
			emxEnsureCapacity((emxArray__common *)d_board, j, (int)sizeof(double));
			for (j = 0; j < k; j++) {
				for (vspread = 0; vspread < i1; vspread++) {
					d_board->data[vspread + d_board->size[0] * j] = b_board->
						BoardCoords->data[vspread + b_board->BoardCoords->size[0] * j];
				}
			}

			emxInit_real_T(&newBoardCoords1, 2);
			emxInit_real_T(&e_board, 2);
			rot90(d_board, newBoardCoords1);
			emxFree_real_T(&d_board);
			// 'detectCheckerboardOnImage:194' newBoardCoords2 = rot90(board.BoardCoords(:,:,2), k); 
			i1 = b_board->BoardCoords->size[0];
			k = b_board->BoardCoords->size[1];
			j = e_board->size[0] * e_board->size[1];
			e_board->size[0] = i1;
			e_board->size[1] = k;
			emxEnsureCapacity((emxArray__common *)e_board, j, (int)sizeof(double));
			
			for (j = 0; j < k; j++) {
				for (vspread = 0; vspread < i1; vspread++) {
					e_board->data[vspread + e_board->size[0] * j] = b_board->
						BoardCoords->data[(vspread + b_board->BoardCoords->size[0] * j) +
						b_board->BoardCoords->size[0] * b_board->BoardCoords->size[1]];
				}
			}

			emxInit_real_T(&newBoardCoords2, 2);
			emxInit_real_T(&r49, 3);
			rot90(e_board, newBoardCoords2);

			// 'detectCheckerboardOnImage:195' board.BoardCoords = cat(3, newBoardCoords1, newBoardCoords2); 
			cat(newBoardCoords1, newBoardCoords2, r49);
			j = b_board->BoardCoords->size[0] * b_board->BoardCoords->size[1] *
				b_board->BoardCoords->size[2];
			b_board->BoardCoords->size[0] = r49->size[0];
			b_board->BoardCoords->size[1] = r49->size[1];
			b_board->BoardCoords->size[2] = 2;
			emxEnsureCapacity((emxArray__common *)b_board->BoardCoords, j, (int)sizeof
			(double));
			i1 = r49->size[0] * r49->size[1] * r49->size[2];
			emxFree_real_T(&e_board);
			emxFree_real_T(&newBoardCoords2);
			emxFree_real_T(&newBoardCoords1);
			for (j = 0; j < i1; j++) {
				b_board->BoardCoords->data[j] = r49->data[j];
			}

			emxFree_real_T(&r49);
		}

		emxInit_boolean_T(&upperLeftMask, 2);
		emxInit_boolean_T(&nextSquareMask, 2);
		emxInit_int32_T(&r48, 1);

		//  try to orient the board so that (0,0) is on a black square
		// 'detectCheckerboardOnImage:175' if ~isUpperLeftBlack(board, I)
		c_board = b_board;

		//  check if the upper-left square of the board is black
		// -------------------------------------------------------------------------- 
		//  create a mask for the upper-left square
		// 'detectCheckerboardOnImage:204' upperLeftPolyX = [this.BoardCoords(1, 1, 1), ... 
		// 'detectCheckerboardOnImage:205'     this.BoardCoords(1, 2, 1), this.BoardCoords(2, 2, 1), ... 
		// 'detectCheckerboardOnImage:206'     this.BoardCoords(2, 1, 1)];
		upperLeftPolyX[0] = c_board->BoardCoords->data[0];
		upperLeftPolyX[1] = c_board->BoardCoords->data[c_board->BoardCoords->size[0]];
		upperLeftPolyX[2] = c_board->BoardCoords->data[1 + c_board->
			BoardCoords->size[0]];
		upperLeftPolyX[3] = c_board->BoardCoords->data[1];

		// 'detectCheckerboardOnImage:207' upperLeftPolyY = [this.BoardCoords(1, 1, 2), ... 
		// 'detectCheckerboardOnImage:208'     this.BoardCoords(1, 2, 2), this.BoardCoords(2, 2, 2), ... 
		// 'detectCheckerboardOnImage:209'     this.BoardCoords(2, 1, 2)];
		upperLeftPolyY[0] = c_board->BoardCoords->data[c_board->BoardCoords->size[0]
			* c_board->BoardCoords->size[1]];
		upperLeftPolyY[1] = c_board->BoardCoords->data[c_board->BoardCoords->size[0]
			+ c_board->BoardCoords->size[0] * c_board->BoardCoords->size[1]];
		upperLeftPolyY[2] = c_board->BoardCoords->data[(c_board->BoardCoords->size[0]
			+ c_board->BoardCoords->size[0] * c_board->BoardCoords->size[1]) + 1];
		upperLeftPolyY[3] = c_board->BoardCoords->data[1 + c_board->
			BoardCoords->size[0] * c_board->BoardCoords->size[1]];

		// 'detectCheckerboardOnImage:210' upperLeftMask = poly2RectMask(upperLeftPolyX, upperLeftPolyY, ... 
		// 'detectCheckerboardOnImage:211'     size(I, 1), size(I, 2));
		poly2RectMask(upperLeftPolyX, upperLeftPolyY, (double)I->size[0], (double)
			I->size[1], upperLeftMask);

		//  create a mask for the square to the right of it
		// 'detectCheckerboardOnImage:214' nextSquarePolyX = [this.BoardCoords(1, 2, 1), ... 
		// 'detectCheckerboardOnImage:215'     this.BoardCoords(1, 3, 1), this.BoardCoords(2, 3, 1), ... 
		// 'detectCheckerboardOnImage:216'     this.BoardCoords(2, 2, 1)];
		upperLeftPolyX[0] = c_board->BoardCoords->data[c_board->BoardCoords->size[0]];
		upperLeftPolyX[1] = c_board->BoardCoords->data[c_board->BoardCoords->size[0]
			<< 1];
		upperLeftPolyX[2] = c_board->BoardCoords->data[1 + (c_board->
			BoardCoords->size[0] << 1)];
		upperLeftPolyX[3] = c_board->BoardCoords->data[1 + c_board->
			BoardCoords->size[0]];

		// 'detectCheckerboardOnImage:217' nextSquarePolyY = [this.BoardCoords(1, 2, 2), ... 
		// 'detectCheckerboardOnImage:218'     this.BoardCoords(1, 3, 2), this.BoardCoords(2, 3, 2), ... 
		// 'detectCheckerboardOnImage:219'     this.BoardCoords(2, 2, 2)];
		upperLeftPolyY[0] = c_board->BoardCoords->data[c_board->BoardCoords->size[0]
			+ c_board->BoardCoords->size[0] * c_board->BoardCoords->size[1]];
		upperLeftPolyY[1] = c_board->BoardCoords->data[(c_board->BoardCoords->size[0]
			<< 1) + c_board->BoardCoords->size[0] * c_board->BoardCoords->size[1]];
		upperLeftPolyY[2] = c_board->BoardCoords->data[((c_board->BoardCoords->size
			[0] << 1) + c_board->BoardCoords->size[0] * c_board->BoardCoords->size[1])
			+ 1];
		upperLeftPolyY[3] = c_board->BoardCoords->data[(c_board->BoardCoords->size[0]
			+ c_board->BoardCoords->size[0] * c_board->BoardCoords->size[1]) + 1];

		// 'detectCheckerboardOnImage:220' nextSquareMask = poly2RectMask(nextSquarePolyX, nextSquarePolyY,... 
		// 'detectCheckerboardOnImage:221'     size(I, 1), size(I, 2));
		poly2RectMask(upperLeftPolyX, upperLeftPolyY, (double)I->size[0], (double)
			I->size[1], nextSquareMask);

		//  check if the first square is darker than the second
		// 'detectCheckerboardOnImage:224' tf = mean(mean(I(upperLeftMask))) < mean(mean(I(nextSquareMask))); 
		i1 = upperLeftMask->size[0] * upperLeftMask->size[1] - 1;
		k = 0;
		for (vspread = 0; vspread <= i1; vspread++) {
			if (upperLeftMask->data[vspread]) {
				k++;
			}
		}

		j = r48->size[0];
		r48->size[0] = k;
		emxEnsureCapacity((emxArray__common *)r48, j, (int)sizeof(int));
		k = 0;
		for (vspread = 0; vspread <= i1; vspread++) {
			if (upperLeftMask->data[vspread]) {
				r48->data[k] = vspread + 1;
				k++;
			}
		}

		emxFree_boolean_T(&upperLeftMask);
		emxInit_int32_T(&r50, 1);
		i1 = nextSquareMask->size[0] * nextSquareMask->size[1] - 1;
		k = 0;
		for (vspread = 0; vspread <= i1; vspread++) {
			if (nextSquareMask->data[vspread]) {
				k++;
			}
		}

		j = r50->size[0];
		r50->size[0] = k;
		emxEnsureCapacity((emxArray__common *)r50, j, (int)sizeof(int));
		k = 0;
		for (vspread = 0; vspread <= i1; vspread++) {
			if (nextSquareMask->data[vspread]) {
				r50->data[k] = vspread + 1;
				k++;
			}
		}

		emxFree_boolean_T(&nextSquareMask);
		emxInit_real32_T(&b_I, 1);
		j = b_I->size[0];
		b_I->size[0] = r48->size[0];
		emxEnsureCapacity((emxArray__common *)b_I, j, (int)sizeof(float));
		i1 = r48->size[0];
		for (j = 0; j < i1; j++) {
			b_I->data[j] = I->data[r48->data[j] - 1];
		}

		emxFree_int32_T(&r48);
		emxInit_real32_T(&c_I, 1);
		x = mean(b_I);
		emxFree_real32_T(&b_I);

		j = c_I->size[0];
		c_I->size[0] = r50->size[0];
		emxEnsureCapacity((emxArray__common *)c_I, j, (int)sizeof(float));
		i1 = r50->size[0];
		
		for (j = 0; j < i1; j++) {
			c_I->data[j] = I->data[r50->data[j] - 1];
		}

		emxFree_int32_T(&r50);
		b_x = mean(c_I);
		emxFree_real32_T(&c_I);
		if (!(x < b_x)) {
			// 'detectCheckerboardOnImage:176' board = rot90_checkerboard(board, 2);
			b_board = rot90_checkerboard(b_board);
		}

		//  if both sides are odd or both sides are even, make sure
		//  that (0,0) is at the upper-left corner.
		// 'detectCheckerboardOnImage:181' if ~xor(mod(size(board.BoardCoords, 1), 2) == 0,...  
		// 'detectCheckerboardOnImage:182'             mod(size(board.BoardCoords, 2), 2) == 0) 
		k = b_board->BoardCoords->size[0];
		vspread = b_board->BoardCoords->size[1];
		if (!((((double)k - floor((double)k / 2.0) * 2.0 == 0.0) ^ ((double)vspread
			- floor((double)vspread / 2.0) * 2.0 == 0.0)) != 0)) {
			emxInit_real_T(&r51, 3);

			// 'detectCheckerboardOnImage:183' if any(board.BoardCoords(1,1,:) > board.BoardCoords(end, end, :)) 
			i1 = b_board->BoardCoords->size[2];
			j = r51->size[0] * r51->size[1] * r51->size[2];
			r51->size[0] = 1;
			r51->size[1] = 1;
			r51->size[2] = i1;
			emxEnsureCapacity((emxArray__common *)r51, j, (int)sizeof(double));
			for (j = 0; j < i1; j++) {
				r51->data[r51->size[0] * r51->size[1] * j] = b_board->BoardCoords->
					data[b_board->BoardCoords->size[0] * b_board->BoardCoords->size[1] * j];
			}

			emxInit_real_T(&r52, 3);
			i1 = b_board->BoardCoords->size[2];
			k = b_board->BoardCoords->size[1];
			vspread = b_board->BoardCoords->size[0];
			j = r52->size[0] * r52->size[1] * r52->size[2];
			r52->size[0] = 1;
			r52->size[1] = 1;
			r52->size[2] = i1;
			emxEnsureCapacity((emxArray__common *)r52, j, (int)sizeof(double));
			for (j = 0; j < i1; j++) {
				r52->data[r52->size[0] * r52->size[1] * j] = b_board->BoardCoords->data
					[((vspread + b_board->BoardCoords->size[0] * (k - 1)) +
						b_board->BoardCoords->size[0] * b_board->BoardCoords->size[1] * j) -
					1];
			}

			emxInit_boolean_T(&c_x, 3);
			j = c_x->size[0] * c_x->size[1] * c_x->size[2];
			c_x->size[0] = 1;
			c_x->size[1] = 1;
			c_x->size[2] = r51->size[2];
			emxEnsureCapacity((emxArray__common *)c_x, j, (int)sizeof(boolean_T));
			i1 = r51->size[0] * r51->size[1] * r51->size[2];
			for (j = 0; j < i1; j++) {
				c_x->data[j] = (r51->data[j] > r52->data[j]);
			}

			emxFree_real_T(&r52);
			emxFree_real_T(&r51);
			y = false;
			k = 3;
			while ((k > 2) && (c_x->size[2] == 1)) {
				k = 2;
			}

			if (3 > k) {
				k = c_x->size[2];
			}
			else {
				k = 1;
			}

			vspread = (c_x->size[2] - 1) * k;
			i1 = 0;
			for (j = 1; j <= k; j++) {
				i1++;
				vspread++;
				ix = i1;
				exitg1 = false;
				while ((!exitg1) && ((k > 0) && (ix <= vspread))) {
					b1 = !c_x->data[ix - 1];
					if (!b1) {
						y = true;
						exitg1 = true;
					}
					else {
						ix += k;
					}
				}
			}

			emxFree_boolean_T(&c_x);
			if (y) {
				// 'detectCheckerboardOnImage:184' board = rot90_checkerboard(board, 2); 
				b_board = rot90_checkerboard(b_board);
			}
		}
	}

	return b_board;
}

//
// Arguments    : const emxArray_real32_T *a_tmp
//                const double pad[2]
//                emxArray_real32_T *a
// Return Type  : void
//
void padImage(const emxArray_real32_T *a_tmp, const double pad[2],
	emxArray_real32_T *a)
{
	double sizeB[2];
	int i2;
	double b_sizeB[2];
	int j;
	int i;
	int i3;
	if ((a_tmp->size[0] == 0) || (a_tmp->size[1] == 0)) {
		for (i2 = 0; i2 < 2; i2++) {
			sizeB[i2] = (double)a_tmp->size[i2] + 2.0 * pad[i2];
		}

		b_sizeB[0] = sizeB[0];
		b_sizeB[1] = sizeB[1];
		repmat(b_sizeB, a);
	}
	else {
		for (i2 = 0; i2 < 2; i2++) {
			sizeB[i2] = (double)a_tmp->size[i2] + 2.0 * pad[i2];
		}

		i2 = a->size[0] * a->size[1];
		a->size[0] = (int)sizeB[0];
		a->size[1] = (int)sizeB[1];
		emxEnsureCapacity((emxArray__common *)a, i2, (int)sizeof(float));
		for (j = 0; j < (int)pad[1]; j++) {
			i2 = a->size[0];
			for (i = 0; i < i2; i++) {
				a->data[i + a->size[0] * j] = 0.0F;
			}
		}

		i2 = a->size[1];
		for (j = a_tmp->size[1] + (int)pad[1]; j + 1 <= i2; j++) {
			i3 = a->size[0];
			for (i = 0; i < i3; i++) {
				a->data[i + a->size[0] * j] = 0.0F;
			}
		}

		for (j = 0; j < a_tmp->size[1]; j++) {
			for (i = 0; i < (int)pad[0]; i++) {
				a->data[i + a->size[0] * (j + (int)pad[1])] = 0.0F;
			}
		}

		for (j = 0; j < a_tmp->size[1]; j++) {
			i2 = a->size[0];
			for (i = (int)pad[0] + a_tmp->size[0]; i + 1 <= i2; i++) {
				a->data[i + a->size[0] * (j + (int)pad[1])] = 0.0F;
			}
		}

		for (j = 0; j < a_tmp->size[1]; j++) {
			for (i = 0; i < a_tmp->size[0]; i++) {
				a->data[(i + (int)pad[0]) + a->size[0] * (j + (int)pad[1])] =
					a_tmp->data[i + a_tmp->size[0] * j];
			}
		}
	}
}

//
// Arguments    : const emxArray_real32_T *varargin_1
//                const double varargin_2[2]
//                emxArray_real32_T *b
// Return Type  : void
//
void padarray(const emxArray_real32_T *varargin_1, const double
	varargin_2[2], emxArray_real32_T *b)
{
	double sizeB[2];
	int i5;
	double b_sizeB[2];
	int j;
	int i;
	if ((varargin_1->size[0] == 0) || (varargin_1->size[1] == 0)) {
		for (i5 = 0; i5 < 2; i5++) {
			sizeB[i5] = (double)varargin_1->size[i5] + varargin_2[i5];
		}

		b_sizeB[0] = sizeB[0];
		b_sizeB[1] = sizeB[1];
		repmat(b_sizeB, b);
	}
	else {
		for (i5 = 0; i5 < 2; i5++) {
			sizeB[i5] = (double)varargin_1->size[i5] + varargin_2[i5];
		}

		i5 = b->size[0] * b->size[1];
		b->size[0] = (int)sizeB[0];
		b->size[1] = (int)sizeB[1];
		emxEnsureCapacity((emxArray__common *)b, i5, (int)sizeof(float));
		for (j = 0; j < (int)varargin_2[1]; j++) {
			i5 = b->size[0];
			for (i = 0; i < i5; i++) {
				b->data[i + b->size[0] * j] = 0.0F;
			}
		}

		i5 = b->size[1];
		for (j = (int)varargin_2[1]; j + 1 <= i5; j++) {
			for (i = 0; i < (int)varargin_2[0]; i++) {
				b->data[i + b->size[0] * j] = 0.0F;
			}
		}

		for (j = 0; j < varargin_1->size[1]; j++) {
			for (i = 0; i < varargin_1->size[0]; i++) {
				b->data[(i + (int)varargin_2[0]) + b->size[0] * (j + (int)varargin_2[1])]
					= varargin_1->data[i + varargin_1->size[0] * j];
			}
		}
	}
}

//
// Arguments    : double b_X[4]
//                double Y[4]
//                double height
//                double width
//                emxArray_boolean_T *mask
// Return Type  : void
//
void poly2RectMask(double b_X[4], double Y[4], double height, double
	width, emxArray_boolean_T *mask)
{
	int i38;
	int loop_ub;
	int i39;
	int i40;
	int i41;
	int unnamed_idx_0;

	// --------------------------------------------------------------------------
	// 'detectCheckerboardOnImage:229' X = sort(X);


	// 'detectCheckerboardOnImage:230' Y = sort(Y);
	std::sort(b_X, b_X + 4);
	std::sort(Y, Y + 4);

	// 'detectCheckerboardOnImage:231' x1 = X(2);
	// 'detectCheckerboardOnImage:232' x2 = X(3);
	// 'detectCheckerboardOnImage:233' y1 = Y(2);
	// 'detectCheckerboardOnImage:234' y2 = Y(3);
	// 'detectCheckerboardOnImage:235' mask = false(height, width);
	i38 = mask->size[0] * mask->size[1];
	mask->size[0] = (int)height;
	mask->size[1] = (int)width;
	emxEnsureCapacity((emxArray__common *)mask, i38, (int)sizeof(boolean_T));
	loop_ub = (int)height * (int)width;
	for (i38 = 0; i38 < loop_ub; i38++) {
		mask->data[i38] = false;
	}

	// 'detectCheckerboardOnImage:236' mask(y1:y2, x1:x2) = true;
	if (Y[1] > Y[2]) {
		i38 = 1;
		i39 = 1;
	}
	else {
		i38 = (int)Y[1];
		i39 = (int)Y[2] + 1;
	}

	if (b_X[1] > b_X[2]) {
		i40 = 1;
		i41 = 1;
	}
	else {
		i40 = (int)b_X[1];
		i41 = (int)b_X[2] + 1;
	}

	unnamed_idx_0 = i39 - i38;
	loop_ub = i41 - i40;
	for (i39 = 0; i39 < loop_ub; i39++) {
		for (i41 = 0; i41 < unnamed_idx_0; i41++) {
			mask->data[((i38 + i41) + mask->size[0] * ((i40 + i39) - 1)) - 1] = true;
		}
	}
}

//
// Arguments    : const emxArray_real32_T *a
//                emxArray_real32_T *y
// Return Type  : void
//
void power(const emxArray_real32_T *a, emxArray_real32_T *y)
{
	emxArray_real32_T *x;
	int ub_loop;
	int loop_ub;
	unsigned int uv2[2];
	int k;
	int b_k = 0;
	emxInit_real32_T(&x, 2);
	ub_loop = x->size[0] * x->size[1];
	x->size[0] = a->size[0];
	x->size[1] = a->size[1];
	emxEnsureCapacity((emxArray__common *)x, ub_loop, (int)sizeof(float));
	loop_ub = a->size[0] * a->size[1];
	for (ub_loop = 0; ub_loop < loop_ub; ub_loop++) {
		x->data[ub_loop] = a->data[ub_loop];
	}

	for (ub_loop = 0; ub_loop < 2; ub_loop++) {
		uv2[ub_loop] = (unsigned int)a->size[ub_loop];
	}

	ub_loop = y->size[0] * y->size[1];
	y->size[0] = (int)uv2[0];
	y->size[1] = (int)uv2[1];
	emxEnsureCapacity((emxArray__common *)y, ub_loop, (int)sizeof(float));
	ub_loop = a->size[0] * a->size[1];

#pragma omp parallel for \
 num_threads(omp_get_max_threads()) \
 private(b_k)

	for (k = 1; k <= ub_loop; k++) {
		b_k = k;
		y->data[b_k - 1] = x->data[b_k - 1] * x->data[b_k - 1];
	}

	emxFree_real32_T(&x);
}

//
// Arguments    : const emxArray_real_T *x
//                const emxArray_real_T *y
//                emxArray_real_T *z
// Return Type  : void
//
void rdivide(const emxArray_real_T *x, const emxArray_real_T *y,
	emxArray_real_T *z)
{
	int i31;
	int loop_ub;
	i31 = z->size[0];
	z->size[0] = x->size[0];
	emxEnsureCapacity((emxArray__common *)z, i31, (int)sizeof(double));
	loop_ub = x->size[0];
	for (i31 = 0; i31 < loop_ub; i31++) {
		z->data[i31] = x->data[i31] / y->data[i31];
	}
}

//
// Arguments    : const double varargin_1[2]
//                emxArray_real32_T *b
// Return Type  : void
//
void repmat(const double varargin_1[2], emxArray_real32_T *b)
{
	int i4;
	int loop_ub;
	i4 = b->size[0] * b->size[1];
	b->size[0] = (int)varargin_1[0];
	b->size[1] = (int)varargin_1[1];
	emxEnsureCapacity((emxArray__common *)b, i4, (int)sizeof(float));
	loop_ub = (int)varargin_1[0] * (int)varargin_1[1];
	for (i4 = 0; i4 < loop_ub; i4++) {
		b->data[i4] = 0.0F;
	}
}

//
// Arguments    : const emxArray_real_T *A
//                emxArray_real_T *B
// Return Type  : void
//
void rot90(const emxArray_real_T *A, emxArray_real_T *B)
{
	int n;
	int sizeB_idx_0;
	int sizeB_idx_1;
	int A_idx_0;
	int B_idx_0;
	n = A->size[1];
	sizeB_idx_0 = A->size[1];
	sizeB_idx_1 = A->size[0];
	A_idx_0 = B->size[0] * B->size[1];
	B->size[0] = sizeB_idx_0;
	B->size[1] = sizeB_idx_1;
	emxEnsureCapacity((emxArray__common *)B, A_idx_0, (int)sizeof(double));
	for (sizeB_idx_0 = 1; sizeB_idx_0 <= n; sizeB_idx_0++) {
		for (sizeB_idx_1 = 0; sizeB_idx_1 + 1 <= A->size[0]; sizeB_idx_1++) {
			A_idx_0 = A->size[0];
			B_idx_0 = B->size[0];
			B->data[(sizeB_idx_0 + B_idx_0 * sizeB_idx_1) - 1] = A->data[sizeB_idx_1 +
				A_idx_0 * (n - sizeB_idx_0)];
		}
	}
}

//
// Arguments    : CheckerboardImage *board
// Return Type  : CheckerboardImage *
//
CheckerboardImage *rot90_checkerboard(CheckerboardImage *board)
{
	CheckerboardImage *b_board;
	emxArray_real_T *r53;
	int i42;
	int loop_ub;
	emxArray_real_T *c_board;
	int b_loop_ub;
	emxArray_real_T *newBoardCoords1;
	int i43;
	emxArray_real_T *d_board;
	emxArray_real_T *newBoardCoords2;
	emxArray_real_T *r54;
	emxInit_real_T(&r53, 2);
	b_board = board;

	// --------------------------------------------------------------------------
	// 'detectCheckerboardOnImage:192' board.BoardIdx = rot90(board.BoardIdx, k);
	b_rot90(b_board->BoardIdx, r53);
	i42 = b_board->BoardIdx->size[0] * b_board->BoardIdx->size[1];
	b_board->BoardIdx->size[0] = r53->size[0];
	b_board->BoardIdx->size[1] = r53->size[1];
	emxEnsureCapacity((emxArray__common *)b_board->BoardIdx, i42, (int)sizeof
	(double));
	loop_ub = r53->size[0] * r53->size[1];
	for (i42 = 0; i42 < loop_ub; i42++) {
		b_board->BoardIdx->data[i42] = r53->data[i42];
	}

	emxFree_real_T(&r53);
	emxInit_real_T(&c_board, 2);

	// 'detectCheckerboardOnImage:193' newBoardCoords1 = rot90(board.BoardCoords(:,:,1), k); 
	loop_ub = b_board->BoardCoords->size[0];
	b_loop_ub = b_board->BoardCoords->size[1];
	i42 = c_board->size[0] * c_board->size[1];
	c_board->size[0] = loop_ub;
	c_board->size[1] = b_loop_ub;
	emxEnsureCapacity((emxArray__common *)c_board, i42, (int)sizeof(double));
	for (i42 = 0; i42 < b_loop_ub; i42++) {
		for (i43 = 0; i43 < loop_ub; i43++) {
			c_board->data[i43 + c_board->size[0] * i42] = b_board->BoardCoords->
				data[i43 + b_board->BoardCoords->size[0] * i42];
		}
	}

	emxInit_real_T(&newBoardCoords1, 2);
	emxInit_real_T(&d_board, 2);
	b_rot90(c_board, newBoardCoords1);
	emxFree_real_T(&c_board);
	// 'detectCheckerboardOnImage:194' newBoardCoords2 = rot90(board.BoardCoords(:,:,2), k); 
	loop_ub = b_board->BoardCoords->size[0];
	b_loop_ub = b_board->BoardCoords->size[1];
	i42 = d_board->size[0] * d_board->size[1];
	d_board->size[0] = loop_ub;
	d_board->size[1] = b_loop_ub;
	emxEnsureCapacity((emxArray__common *)d_board, i42, (int)sizeof(double));
	
	for (i42 = 0; i42 < b_loop_ub; i42++) {
		for (i43 = 0; i43 < loop_ub; i43++) {
			d_board->data[i43 + d_board->size[0] * i42] = b_board->BoardCoords->data
				[(i43 + b_board->BoardCoords->size[0] * i42) + b_board->
				BoardCoords->size[0] * b_board->BoardCoords->size[1]];
		}
	}

	emxInit_real_T(&newBoardCoords2, 2);
	emxInit_real_T(&r54, 3);
	b_rot90(d_board, newBoardCoords2);
	emxFree_real_T(&d_board);

	// 'detectCheckerboardOnImage:195' board.BoardCoords = cat(3, newBoardCoords1, newBoardCoords2); 
	cat(newBoardCoords1, newBoardCoords2, r54);
	emxFree_real_T(&newBoardCoords2);
	emxFree_real_T(&newBoardCoords1);

	i42 = b_board->BoardCoords->size[0] * b_board->BoardCoords->size[1] *
		b_board->BoardCoords->size[2];
	b_board->BoardCoords->size[0] = r54->size[0];
	b_board->BoardCoords->size[1] = r54->size[1];
	b_board->BoardCoords->size[2] = 2;
	emxEnsureCapacity((emxArray__common *)b_board->BoardCoords, i42, (int)sizeof
	(double));
	loop_ub = r54->size[0] * r54->size[1] * r54->size[2];
	
	
	
	for (i42 = 0; i42 < loop_ub; i42++) {
		b_board->BoardCoords->data[i42] = r54->data[i42];
	}

	emxFree_real_T(&r54);
	return b_board;
}

//
// Arguments    : double u0
//                double u1
// Return Type  : double
//
double rt_hypotd(double u0, double u1)
{
	double y;
	double a;
	double b;
	a = fabs(u0);
	b = fabs(u1);
	if (a < b) {
		a /= b;
		y = b * sqrt(a * a + 1.0);
	}
	else if (a > b) {
		b /= a;
		y = a * sqrt(b * b + 1.0);
	}
	else {
		y = a * 1.4142135623730951;
	}

	return y;
}

//
// Arguments    : float u0
//                float u1
// Return Type  : float
//
float rt_hypotf(float u0, float u1)
{
	float y;
	float a;
	float b;
	a = (float)fabs((double)u0);
	b = (float)fabs((double)u1);
	if (a < b) {
		a /= b;
		y = b * (float)sqrt((double)(a * a + 1.0F));
	}
	else if (a > b) {
		b /= a;
		y = a * (float)sqrt((double)(b * b + 1.0F));
	}
	else {
		y = a * 1.41421354F;
	}

	return y;
}

//
// Arguments    : double u
// Return Type  : double
//
double rt_roundd(double u)
{
	double y;
	if (fabs(u) < 4.503599627370496E+15) {
		if (u >= 0.5) {
			y = floor(u + 0.5);
		}
		else if (u > -0.5) {
			y = 0.0;
		}
		else {
			y = ceil(u - 0.5);
		}
	}
	else {
		y = u;
	}

	return y;
}

//
// Arguments    : float u
// Return Type  : float
//
float rt_roundf(float u)
{
	float y;
	if ((float)fabs((double)u) < 8.388608E+6F) {
		if (u >= 0.5F) {
			y = (float)floor((double)(u + 0.5F));
		}
		else if (u > -0.5F) {
			y = 0.0F;
		}
		else {
			y = (float)ceil((double)(u - 0.5F));
		}
	}
	else {
		y = u;
	}

	return y;
}

//
// Arguments    : emxArray_real32_T *x
//                emxArray_int32_T *idx
// Return Type  : void
//
void sort(emxArray_real32_T *x, emxArray_int32_T *idx)
{
	int dim;
	dim = 2;
	if (x->size[0] != 1) {
		dim = 1;
	}

	b_sort(x, dim, idx);
}

//
// Arguments    : emxArray_real32_T *x
//                emxArray_int32_T *idx
// Return Type  : void
//
void sortIdx(emxArray_real32_T *x, emxArray_int32_T *idx)
{
	emxArray_real32_T *b_x;
	int ib;
	int p;
	int preSortLevel;
	int n;
	float x4[4];
	int idx4[4];
	emxArray_int32_T *iwork;
	emxArray_real32_T *xwork;
	int k;
	signed char perm[4];
	int nBlocks;
	int i4;
	int b_iwork[256];
	float b_xwork[256];
	int b;
	int bLen;
	int bLen2;
	int nPairs;
	int exitg1;
	emxInit_real32_T(&b_x, 1);
	ib = x->size[0];
	p = b_x->size[0];
	b_x->size[0] = x->size[0];
	emxEnsureCapacity((emxArray__common *)b_x, p, (int)sizeof(float));
	preSortLevel = x->size[0];
	for (p = 0; p < preSortLevel; p++) {
		b_x->data[p] = x->data[p];
	}

	p = idx->size[0];
	idx->size[0] = ib;
	emxEnsureCapacity((emxArray__common *)idx, p, (int)sizeof(int));
	for (p = 0; p < ib; p++) {
		idx->data[p] = 0;
	}

	n = x->size[0];
	for (preSortLevel = 0; preSortLevel < 4; preSortLevel++) {
		x4[preSortLevel] = 0.0F;
		idx4[preSortLevel] = 0;
	}

	emxInit_int32_T(&iwork, 1);
	p = iwork->size[0];
	iwork->size[0] = ib;
	emxEnsureCapacity((emxArray__common *)iwork, p, (int)sizeof(int));
	preSortLevel = iwork->size[0];
	p = iwork->size[0];
	iwork->size[0] = preSortLevel;
	emxEnsureCapacity((emxArray__common *)iwork, p, (int)sizeof(int));
	for (p = 0; p < preSortLevel; p++) {
		iwork->data[p] = 0;
	}

	emxInit_real32_T(&xwork, 1);
	preSortLevel = x->size[0];
	p = xwork->size[0];
	xwork->size[0] = preSortLevel;
	emxEnsureCapacity((emxArray__common *)xwork, p, (int)sizeof(float));
	preSortLevel = xwork->size[0];
	p = xwork->size[0];
	xwork->size[0] = preSortLevel;
	emxEnsureCapacity((emxArray__common *)xwork, p, (int)sizeof(float));
	for (p = 0; p < preSortLevel; p++) {
		xwork->data[p] = 0.0F;
	}

	ib = 0;
	for (k = 0; k + 1 <= n; k++) {
		ib++;
		idx4[ib - 1] = k + 1;
		x4[ib - 1] = b_x->data[k];
		if (ib == 4) {
			if (x4[0] >= x4[1]) {
				preSortLevel = 1;
				ib = 2;
			}
			else {
				preSortLevel = 2;
				ib = 1;
			}

			if (x4[2] >= x4[3]) {
				p = 3;
				i4 = 4;
			}
			else {
				p = 4;
				i4 = 3;
			}

			if (x4[preSortLevel - 1] >= x4[p - 1]) {
				if (x4[ib - 1] >= x4[p - 1]) {
					perm[0] = (signed char)preSortLevel;
					perm[1] = (signed char)ib;
					perm[2] = (signed char)p;
					perm[3] = (signed char)i4;
				}
				else if (x4[ib - 1] >= x4[i4 - 1]) {
					perm[0] = (signed char)preSortLevel;
					perm[1] = (signed char)p;
					perm[2] = (signed char)ib;
					perm[3] = (signed char)i4;
				}
				else {
					perm[0] = (signed char)preSortLevel;
					perm[1] = (signed char)p;
					perm[2] = (signed char)i4;
					perm[3] = (signed char)ib;
				}
			}
			else if (x4[preSortLevel - 1] >= x4[i4 - 1]) {
				if (x4[ib - 1] >= x4[i4 - 1]) {
					perm[0] = (signed char)p;
					perm[1] = (signed char)preSortLevel;
					perm[2] = (signed char)ib;
					perm[3] = (signed char)i4;
				}
				else {
					perm[0] = (signed char)p;
					perm[1] = (signed char)preSortLevel;
					perm[2] = (signed char)i4;
					perm[3] = (signed char)ib;
				}
			}
			else {
				perm[0] = (signed char)p;
				perm[1] = (signed char)i4;
				perm[2] = (signed char)preSortLevel;
				perm[3] = (signed char)ib;
			}

			idx->data[k - 3] = idx4[perm[0] - 1];
			idx->data[k - 2] = idx4[perm[1] - 1];
			idx->data[k - 1] = idx4[perm[2] - 1];
			idx->data[k] = idx4[perm[3] - 1];
			b_x->data[k - 3] = x4[perm[0] - 1];
			b_x->data[k - 2] = x4[perm[1] - 1];
			b_x->data[k - 1] = x4[perm[2] - 1];
			b_x->data[k] = x4[perm[3] - 1];
			ib = 0;
		}
	}

	if (ib > 0) {
		for (preSortLevel = 0; preSortLevel < 4; preSortLevel++) {
			perm[preSortLevel] = 0;
		}

		if (ib == 1) {
			perm[0] = 1;
		}
		else if (ib == 2) {
			if (x4[0] >= x4[1]) {
				perm[0] = 1;
				perm[1] = 2;
			}
			else {
				perm[0] = 2;
				perm[1] = 1;
			}
		}
		else if (x4[0] >= x4[1]) {
			if (x4[1] >= x4[2]) {
				perm[0] = 1;
				perm[1] = 2;
				perm[2] = 3;
			}
			else if (x4[0] >= x4[2]) {
				perm[0] = 1;
				perm[1] = 3;
				perm[2] = 2;
			}
			else {
				perm[0] = 3;
				perm[1] = 1;
				perm[2] = 2;
			}
		}
		else if (x4[0] >= x4[2]) {
			perm[0] = 2;
			perm[1] = 1;
			perm[2] = 3;
		}
		else if (x4[1] >= x4[2]) {
			perm[0] = 2;
			perm[1] = 3;
			perm[2] = 1;
		}
		else {
			perm[0] = 3;
			perm[1] = 2;
			perm[2] = 1;
		}

		for (k = 1; k <= ib; k++) {
			idx->data[((n - ib) + k) - 1] = idx4[perm[k - 1] - 1];
			b_x->data[((n - ib) + k) - 1] = x4[perm[k - 1] - 1];
		}
	}

	preSortLevel = 2;
	if (x->size[0] > 1) {
		if (x->size[0] >= 256) {
			nBlocks = x->size[0] >> 8;
			if (nBlocks > 0) {
				for (i4 = 1; i4 <= nBlocks; i4++) {
					n = ((i4 - 1) << 8) - 1;
					for (b = 0; b < 6; b++) {
						bLen = 1 << (b + 2);
						bLen2 = bLen << 1;
						nPairs = 256 >> (b + 3);
						for (k = 1; k <= nPairs; k++) {
							ib = n + (k - 1) * bLen2;
							for (preSortLevel = 1; preSortLevel <= bLen2; preSortLevel++) {
								b_iwork[preSortLevel - 1] = idx->data[ib + preSortLevel];
								b_xwork[preSortLevel - 1] = b_x->data[ib + preSortLevel];
							}

							p = 0;
							preSortLevel = bLen;
							do {
								exitg1 = 0;
								ib++;
								if (b_xwork[p] >= b_xwork[preSortLevel]) {
									idx->data[ib] = b_iwork[p];
									b_x->data[ib] = b_xwork[p];
									if (p + 1 < bLen) {
										p++;
									}
									else {
										exitg1 = 1;
									}
								}
								else {
									idx->data[ib] = b_iwork[preSortLevel];
									b_x->data[ib] = b_xwork[preSortLevel];
									if (preSortLevel + 1 < bLen2) {
										preSortLevel++;
									}
									else {
										preSortLevel = ib - p;
										while (p + 1 <= bLen) {
											idx->data[(preSortLevel + p) + 1] = b_iwork[p];
											b_x->data[(preSortLevel + p) + 1] = b_xwork[p];
											p++;
										}

										exitg1 = 1;
									}
								}
							} while (exitg1 == 0);
						}
					}
				}

				preSortLevel = nBlocks << 8;
				ib = x->size[0] - preSortLevel;
				if (ib > 0) {
					merge_block(idx, b_x, preSortLevel, ib, 2, iwork, xwork);
				}

				preSortLevel = 8;
			}
		}

		merge_block(idx, b_x, 0, x->size[0], preSortLevel, iwork, xwork);
	}

	emxFree_real32_T(&xwork);
	emxFree_int32_T(&iwork);
	p = x->size[0];
	x->size[0] = b_x->size[0];
	emxEnsureCapacity((emxArray__common *)x, p, (int)sizeof(float));
	preSortLevel = b_x->size[0];
	for (p = 0; p < preSortLevel; p++) {
		x->data[p] = b_x->data[p];
	}

	emxFree_real32_T(&b_x);
}

//
// Arguments    : const emxArray_real_T *a
//                emxArray_real_T *b
// Return Type  : void
//
void squeeze(const emxArray_real_T *a, emxArray_real_T *b)
{
	int k;
	int sqsz[3];
	int i23;
	k = 3;
	while ((k > 2) && (a->size[2] == 1)) {
		k = 2;
	}

	if (k <= 2) {
		sqsz[1] = a->size[1];
		i23 = b->size[0] * b->size[1];
		b->size[0] = 1;
		b->size[1] = sqsz[1];
		emxEnsureCapacity((emxArray__common *)b, i23, (int)sizeof(double));
		i23 = a->size[1] * a->size[2];
		for (k = 0; k + 1 <= i23; k++) {
			b->data[k] = a->data[k];
		}
	}
	else {
		for (i23 = 0; i23 < 3; i23++) {
			sqsz[i23] = 1;
		}

		k = 0;
		if (a->size[1] != 1) {
			sqsz[0] = a->size[1];
			k = 1;
		}

		if (a->size[2] != 1) {
			sqsz[k] = a->size[2];
		}

		i23 = b->size[0] * b->size[1];
		b->size[0] = sqsz[0];
		b->size[1] = sqsz[1];
		emxEnsureCapacity((emxArray__common *)b, i23, (int)sizeof(double));
		i23 = a->size[1] * a->size[2];
		for (k = 0; k + 1 <= i23; k++) {
			b->data[k] = a->data[k];
		}
	}
}

//
// Arguments    : const emxArray_real32_T *metric
//                emxArray_real_T *loc
// Return Type  : void
//
void subPixelLocation(const emxArray_real32_T *metric, emxArray_real_T
	*loc)
{
	int i59;
	int id;
	emxArray_real32_T *b_metric;
	int loop_ub;
	int i60;
	double loc_data[2];
	int b_loop_ub;
	boolean_T b_loc_data[2];
	int loc_size[1];
	emxArray_boolean_T c_loc_data;
	float subPixelLoc_data[2];
	int i61;
	int i62;
	int i63;
	int i64;
	int c_loop_ub;
	float beta[6];
	float b_beta;
	float x;
	float y;
	float d_loc_data[2];
	float b_x[2];

	// 'subPixelLocation:5' for id = 1: size(loc,1)
	i59 = loc->size[0];
	id = 0;
	emxInit_real32_T(&b_metric, 2);
	while (id <= i59 - 1) {
		// 'subPixelLocation:6' loc(id,:) = subPixelLocationImpl(metric, loc(id,:)); 
		loop_ub = loc->size[1];
		for (i60 = 0; i60 < loop_ub; i60++) {
			loc_data[i60] = loc->data[id + loc->size[0] * i60];
		}

		//  subPixelLocation(metric, loc) fits a quadratic function to a patch in the 
		//  image metric around the pixel specified by loc. metric is a grayscale
		//  2-D image, and loc is a two-element vector [x y]. subPixelLoc is a
		//  two-element vector [x y], corresponding to the maximum of the quadratic  
		//  function.
		//  The size of the patch is halfPatchSize * 2 + 1
		// 'subPixelLocation:17' halfPatchSize = 2;
		//  Check if the patch is outside the image
		// 'subPixelLocation:20' if any(loc(:) < halfPatchSize + 1) || loc(1) > size(metric, 2) - halfPatchSize - 1 ... 
		// 'subPixelLocation:21'     || loc(2) > size(metric, 1) - halfPatchSize -1
		b_loop_ub = loc->size[1];
		loc_size[0] = b_loop_ub;
		for (i60 = 0; i60 < b_loop_ub; i60++) {
			b_loc_data[i60] = (loc_data[i60] < 3.0);
		}

		c_loc_data.data = (boolean_T *)&b_loc_data;
		c_loc_data.size = (int *)&loc_size;
		c_loc_data.allocatedSize = 2;
		c_loc_data.numDimensions = 1;
		c_loc_data.canFreeData = false;
		if (any(&c_loc_data) || (loc->data[id] > ((double)metric->size[1] - 2.0) -
			1.0) || (loc->data[id + loc->size[0]] > ((double)metric->size[0] - 2.0)
				- 1.0)) {

			
			// 'subPixelLocation:22' subPixelLoc = single(loc);
			for (i60 = 0; i60 < loop_ub; i60++) {
				subPixelLoc_data[i60] = (float)loc_data[i60];
			}
		}
		else {
			//  Get the patch
			// 'subPixelLocation:27' patch = metric(loc(2)-halfPatchSize:loc(2)+halfPatchSize, ... 
			// 'subPixelLocation:28'                loc(1)-halfPatchSize:loc(1)+halfPatchSize); 
			if (loc->data[id + loc->size[0]] - 2.0 > loc->data[id + loc->size[0]] +
				2.0) {
				i60 = 0;
				i61 = 0;
			}
			else {
				i60 = (int)(loc->data[id + loc->size[0]] - 2.0) - 1;
				i61 = (int)(loc->data[id + loc->size[0]] + 2.0);
			}

			if (loc->data[id] - 2.0 > loc->data[id] + 2.0) {
				i62 = 0;
				i63 = 0;
			}
			else {
				i62 = (int)(loc->data[id] - 2.0) - 1;
				i63 = (int)(loc->data[id] + 2.0);
			}

			//  Create the matrix for the normal equations used to solve for the
			//  coefficients of the quadratic function. This matrix depends only on the  
			//  patch size, and thus needs to be computed only once.
			// 'subPixelLocation:34' if isempty(X)
			//  Get a least-squares solution for the coefficients
			// 'subPixelLocation:39' y = patch(:);
			// 'subPixelLocation:40' beta = X * y;
			i64 = b_metric->size[0] * b_metric->size[1];
			b_metric->size[0] = i61 - i60;
			b_metric->size[1] = i63 - i62;
			emxEnsureCapacity((emxArray__common *)b_metric, i64, (int)sizeof(float));
			b_loop_ub = i63 - i62;
			for (i63 = 0; i63 < b_loop_ub; i63++) {
				c_loop_ub = i61 - i60;
				for (i64 = 0; i64 < c_loop_ub; i64++) {
					b_metric->data[i64 + b_metric->size[0] * i63] = metric->data[(i60 +
						i64) + metric->size[0] * (i62 + i63)];
				}
			}

			for (i60 = 0; i60 < 6; i60++) {
				beta[i60] = 0.0F;
				for (i61 = 0; i61 < 25; i61++) {
					b_beta = beta[i60] + (float)X[i60 + 6 * i61] * b_metric->data[i61];
					beta[i60] = b_beta;
				}
			}

			//  f(x,y) = Ax^2 + By^2 + Cx + Dy + Exy + F
			// 'subPixelLocation:43' A = beta(1);
			// 'subPixelLocation:44' B = beta(2);
			// 'subPixelLocation:45' C = beta(3);
			// 'subPixelLocation:46' D = beta(4);
			// 'subPixelLocation:47' E = beta(5);
			//  F = beta(6), but we do not need it
			//  Solve for the maximum of the quadratic, in patch-based coordinates
			// 'subPixelLocation:51' x = -(2*B*C - D*E) / (4*A*B - E^2);
			x = -(2.0F * beta[1] * beta[2] - beta[3] * beta[4]) / (4.0F * beta[0] *
				beta[1] - beta[4] * beta[4]);

			// 'subPixelLocation:52' y = -(2*A*D - C*E) / (4*A*B - E^2);
			y = -(2.0F * beta[0] * beta[3] - beta[2] * beta[4]) / (4.0F * beta[0] *
				beta[1] - beta[4] * beta[4]);

			// 'subPixelLocation:53' if ~isfinite(x) || abs(x) > halfPatchSize || ~isfinite(y) || abs(y) > halfPatchSize 
			if (((float)fabs((double)x) > 2.0F) || ((float)fabs((double)y) > 2.0F)) {
				// 'subPixelLocation:54' x = single(0);
				x = 0.0F;

				// 'subPixelLocation:55' y = single(0);
				y = 0.0F;
			}

			//  Get the sub-pixel location
			// 'subPixelLocation:59' subPixelLoc = single(loc) + [x y];
			for (i60 = 0; i60 < loop_ub; i60++) {
				d_loc_data[i60] = (float)loc_data[i60];
			}

			b_x[0] = x;
			b_x[1] = y;
			loop_ub = 2;
			for (i60 = 0; i60 < 2; i60++) {
				subPixelLoc_data[i60] = d_loc_data[i60] + b_x[i60];
			}
		}

		for (i60 = 0; i60 < loop_ub; i60++) {
			loc->data[id + loc->size[0] * i60] = subPixelLoc_data[i60];
		}

		id++;
	}

	emxFree_real32_T(&b_metric);
}
//
// Arguments    : void
// Return Type  : void
//
void subPixelLocationImpl_init()
{
	const double dv0[150] = { 0.028571428571428574, 0.028571428571428574,
	-0.04, -0.04, 0.04, -0.0742857142857143, 0.028571428571428574,
	-0.014285714285714285, -0.04, -0.02, 0.02, 0.01142857142857142,
	0.028571428571428574, -0.028571428571428571, -0.04, 0.0, 0.0,
	0.039999999999999994, 0.028571428571428574, -0.014285714285714285, -0.04,
	0.02, -0.02, 0.01142857142857142, 0.028571428571428574, 0.028571428571428574,
	-0.04, 0.04, -0.04, -0.0742857142857143, -0.014285714285714287,
	0.028571428571428571, -0.02, -0.04, 0.02, 0.011428571428571429,
	-0.014285714285714285, -0.014285714285714284, -0.02, -0.02, 0.01,
	0.097142857142857142, -0.01428571428571429, -0.028571428571428574, -0.02,
	0.0, 0.0, 0.12571428571428572, -0.014285714285714285, -0.014285714285714284,
	-0.02, 0.02, -0.01, 0.097142857142857142, -0.014285714285714287,
	0.028571428571428571, -0.02, 0.04, -0.02, 0.011428571428571429,
	-0.028571428571428574, 0.028571428571428571, 0.0, -0.04, 0.0,
	0.040000000000000008, -0.028571428571428574, -0.014285714285714287, 0.0,
	-0.02, 0.0, 0.12571428571428572, -0.028571428571428574,
	-0.028571428571428574, 0.0, 0.0, 0.0, 0.1542857142857143,
	-0.028571428571428574, -0.014285714285714287, 0.0, 0.02, 0.0,
	0.12571428571428572, -0.028571428571428574, 0.028571428571428571, 0.0, 0.04,
	0.0, 0.040000000000000008, -0.014285714285714287, 0.028571428571428571, 0.02,
	-0.04, -0.02, 0.011428571428571429, -0.014285714285714285,
	-0.014285714285714284, 0.02, -0.02, -0.01, 0.097142857142857142,
	-0.01428571428571429, -0.028571428571428574, 0.02, 0.0, 0.0,
	0.12571428571428572, -0.014285714285714285, -0.014285714285714284, 0.02,
	0.02, 0.01, 0.097142857142857142, -0.014285714285714287,
	0.028571428571428571, 0.02, 0.04, 0.02, 0.011428571428571429,
	0.028571428571428574, 0.028571428571428574, 0.04, -0.04, -0.04,
	-0.0742857142857143, 0.028571428571428574, -0.014285714285714285, 0.04,
	-0.02, -0.02, 0.01142857142857142, 0.028571428571428574,
	-0.028571428571428571, 0.04, 0.0, 0.0, 0.039999999999999994,
	0.028571428571428574, -0.014285714285714285, 0.04, 0.02, 0.02,
	0.01142857142857142, 0.028571428571428574, 0.028571428571428574, 0.04, 0.04,
	0.04, -0.0742857142857143 };

	memcpy(&X[0], &dv0[0], 150U * sizeof(double));

	// 'subPixelLocation:35' X = createNormalEquationsMatrix(halfPatchSize);
}



//
// returns the points as an Mx2 matrix of x,y coordinates, and
//  the size of the board
// Arguments    : const CheckerboardImage *b_this
//                emxArray_real_T *points
//                double boardSize[2]
// Return Type  : void
//
void toPoints(const CheckerboardImage *b_this, emxArray_real_T *points,
	double boardSize[2])
{
	emxArray_boolean_T *c_this;
	int i44;
	int loop_ub;
	double numPoints;
	emxArray_real_T *x;
	int b_loop_ub;
	int i45;
	emxInit_boolean_T(&c_this, 1);

	// --------------------------------------------------------------------------
	// 'detectCheckerboardOnImage:149' if any(this.BoardIdx(:) == 0)
	i44 = c_this->size[0];
	c_this->size[0] = b_this->BoardIdx->size[0] * b_this->BoardIdx->size[1];
	emxEnsureCapacity((emxArray__common *)c_this, i44, (int)sizeof(boolean_T));
	loop_ub = b_this->BoardIdx->size[0] * b_this->BoardIdx->size[1];
	for (i44 = 0; i44 < loop_ub; i44++) {
		c_this->data[i44] = (b_this->BoardIdx->data[i44] == 0.0);
	}

	if (any(c_this)) {
		emxFree_boolean_T(&c_this);
		// 'detectCheckerboardOnImage:150' points = [];
		i44 = points->size[0] * points->size[1];
		points->size[0] = 0;
		points->size[1] = 0;
		emxEnsureCapacity((emxArray__common *)points, i44, (int)sizeof(double));

		// 'detectCheckerboardOnImage:151' boardSize = [0 0];
		for (i44 = 0; i44 < 2; i44++) {
			boardSize[i44] = 0.0;
		}
	}
	else {
		emxFree_boolean_T(&c_this);
		// 'detectCheckerboardOnImage:155' numPoints = size(this.BoardCoords, 1) * size(this.BoardCoords, 2); 
		numPoints = (double)b_this->BoardCoords->size[0] * (double)
			b_this->BoardCoords->size[1];

		// 'detectCheckerboardOnImage:156' points = zeros(numPoints, 2);
		i44 = points->size[0] * points->size[1];
		points->size[0] = (int)numPoints;
		points->size[1] = 2;
		emxEnsureCapacity((emxArray__common *)points, i44, (int)sizeof(double));
		loop_ub = (int)numPoints << 1;
		for (i44 = 0; i44 < loop_ub; i44++) {
			points->data[i44] = 0.0;
		}

		emxInit_real_T(&x, 2);

		// 'detectCheckerboardOnImage:157' x = this.BoardCoords(:, :, 1)';
		loop_ub = b_this->BoardCoords->size[0];
		b_loop_ub = b_this->BoardCoords->size[1];
		i44 = x->size[0] * x->size[1];
		x->size[0] = b_loop_ub;
		x->size[1] = loop_ub;
		emxEnsureCapacity((emxArray__common *)x, i44, (int)sizeof(double));
		for (i44 = 0; i44 < loop_ub; i44++) {
			for (i45 = 0; i45 < b_loop_ub; i45++) {
				x->data[i45 + x->size[0] * i44] = b_this->BoardCoords->data[i44 +
					b_this->BoardCoords->size[0] * i45];
			}
		}

		// 'detectCheckerboardOnImage:158' points(:, 1) = x(:);
		loop_ub = x->size[0] * x->size[1];
		for (i44 = 0; i44 < loop_ub; i44++) {
			points->data[i44] = x->data[i44];
		}

		// 'detectCheckerboardOnImage:159' y = this.BoardCoords(:, :, 2)';
		loop_ub = b_this->BoardCoords->size[0];
		b_loop_ub = b_this->BoardCoords->size[1];
		i44 = x->size[0] * x->size[1];
		x->size[0] = b_loop_ub;
		x->size[1] = loop_ub;
		emxEnsureCapacity((emxArray__common *)x, i44, (int)sizeof(double));
		for (i44 = 0; i44 < loop_ub; i44++) {
			for (i45 = 0; i45 < b_loop_ub; i45++) {
				x->data[i45 + x->size[0] * i44] = b_this->BoardCoords->data[(i44 +
					b_this->BoardCoords->size[0] * i45) + b_this->BoardCoords->size[0] *
					b_this->BoardCoords->size[1]];
			}
		}

		// 'detectCheckerboardOnImage:160' points(:, 2) = y(:);
		loop_ub = x->size[0] * x->size[1];
		for (i44 = 0; i44 < loop_ub; i44++) {
			points->data[i44 + points->size[0]] = x->data[i44];
		}

		emxFree_real_T(&x);

		// 'detectCheckerboardOnImage:161' boardSize = [size(this.BoardCoords, 2)+1, size(this.BoardCoords, 1)+1]; 
		boardSize[0] = (double)b_this->BoardCoords->size[1] + 1.0;
		boardSize[1] = (double)b_this->BoardCoords->size[0] + 1.0;
	}

	
}

//
// Arguments    : int n
//                double a
//                int ix0
//                emxArray_real_T *y
//                int iy0
// Return Type  : void
//
void xaxpy(int n, double a, int ix0, emxArray_real_T *y, int iy0)
{
	int ix;
	int iy;
	int k;
	if ((n < 1) || (a == 0.0)) {
	}
	else {
		ix = ix0 - 1;
		iy = iy0 - 1;
		for (k = 0; k < n; k++) {
			y->data[iy] += a * y->data[ix];
			ix++;
			iy++;
		}
	}
}

//
// Arguments    : int n
//                const emxArray_real_T *x
//                int ix0
//                const emxArray_real_T *y
//                int iy0
// Return Type  : double
//
double xdotc(int n, const emxArray_real_T *x, int ix0, const
	emxArray_real_T *y, int iy0)
{
	double d;
	int ix;
	int iy;
	int k;
	d = 0.0;
	if (n < 1) {
	}
	else {
		ix = ix0;
		iy = iy0;
		for (k = 1; k <= n; k++) {
			d += x->data[ix - 1] * y->data[iy - 1];
			ix++;
			iy++;
		}
	}

	return d;
}

//
// Arguments    : int n
//                const emxArray_real_T *x
//                int ix0
// Return Type  : double
//
double xnrm2(int n, const emxArray_real_T *x, int ix0)
{
	double y;
	double scale;
	int kend;
	int k;
	double absxk;
	double t;
	y = 0.0;
	if (n < 1) {
	}
	else if (n == 1) {
		y = fabs(x->data[ix0 - 1]);
	}
	else {
		scale = 2.2250738585072014E-308;
		kend = (ix0 + n) - 1;
		for (k = ix0; k <= kend; k++) {
			absxk = fabs(x->data[k - 1]);
			if (absxk > scale) {
				t = scale / absxk;
				y = 1.0 + y * t * t;
				scale = absxk;
			}
			else {
				t = absxk / scale;
				y += t * t;
			}
		}

		y = scale * sqrt(y);
	}

	return y;
}

//
// Arguments    : int n
//                emxArray_real_T *x
//                int ix0
//                int iy0
//                double c
//                double s
// Return Type  : void
//
void xrot(int n, emxArray_real_T *x, int ix0, int iy0, double c, double s)
{
	int ix;
	int iy;
	int k;
	double temp;
	if (n < 1) {
	}
	else {
		ix = ix0 - 1;
		iy = iy0 - 1;
		for (k = 1; k <= n; k++) {
			temp = c * x->data[ix] + s * x->data[iy];
			x->data[iy] = c * x->data[iy] - s * x->data[ix];
			x->data[ix] = temp;
			iy++;
			ix++;
		}
	}
}

//
// Arguments    : double *a
//                double *b
//                double *c
//                double *s
// Return Type  : void
//
void xrotg(double *a, double *b, double *c, double *s)
{
	double roe;
	double absa;
	double absb;
	double scale;
	double ads;
	double bds;
	roe = *b;
	absa = fabs(*a);
	absb = fabs(*b);
	if (absa > absb) {
		roe = *a;
	}

	scale = absa + absb;
	if (scale == 0.0) {
		*s = 0.0;
		*c = 1.0;
		scale = 0.0;
		*b = 0.0;
	}
	else {
		ads = absa / scale;
		bds = absb / scale;
		scale *= sqrt(ads * ads + bds * bds);
		if (roe < 0.0) {
			scale = -scale;
		}

		*c = *a / scale;
		*s = *b / scale;
		if (absa > absb) {
			*b = *s;
		}
		else if (*c != 0.0) {
			*b = 1.0 / *c;
		}
		else {
			*b = 1.0;
		}
	}

	*a = scale;
}

//
// Arguments    : int n
//                double a
//                emxArray_real_T *x
//                int ix0
// Return Type  : void
//
void xscal(int n, double a, emxArray_real_T *x, int ix0)
{
	int i46;
	int k;
	i46 = (ix0 + n) - 1;
	for (k = ix0; k <= i46; k++) {
		x->data[k - 1] *= a;
	}
}

//
// Arguments    : int n
//                emxArray_real_T *x
//                int ix0
//                int iy0
// Return Type  : void
//
void xswap(int n, emxArray_real_T *x, int ix0, int iy0)
{
	int ix;
	int iy;
	int k;
	double temp;
	ix = ix0 - 1;
	iy = iy0 - 1;
	for (k = 1; k <= n; k++) {
		temp = x->data[ix];
		x->data[ix] = x->data[iy];
		x->data[iy] = temp;
		ix++;
		iy++;
	}
}

//
// Arguments    : const emxArray_uint8_T *I
//                double sigma
//                double minCornerMetric
//                emxArray_real_T *points
//                double boardSize[2]
// Return Type  : void
//
void detectCheckerboardPointsOnImage(const emxArray_uint8_T *I, double sigma,
	double minCornerMetric, emxArray_real_T *points, double boardSize[2])
{
	emxArray_real32_T *b_I;
	int i0;
	int loop_ub;
	emxInit_real32_T(&b_I, 2);

	// 'detectCheckerboardPointsOnImage:3' I=im2single(I);
	i0 = b_I->size[0] * b_I->size[1];
	b_I->size[0] = I->size[0];
	b_I->size[1] = I->size[1];
	emxEnsureCapacity((emxArray__common *)b_I, i0, (int)sizeof(float));
	loop_ub = I->size[0] * I->size[1];
	for (i0 = 0; i0 < loop_ub; i0++) {
		b_I->data[i0] = (float)I->data[i0] / 255.0F;
	}

	// 'detectCheckerboardPointsOnImage:4' [points, boardSize] = detectCheckerboardOnImage(I, sigma, minCornerMetric); 
	detectCheckerboardOnImage(b_I, sigma, minCornerMetric, points, boardSize);

	// 'detectCheckerboardPointsOnImage:6' if isempty(points)
	if ((points->size[0] == 0) || (points->size[1] == 0)) {
		// 'detectCheckerboardPointsOnImage:7' sigma = sigma*2;
		sigma *= 2.0;

		// 'detectCheckerboardPointsOnImage:8' [points, boardSize] = detectCheckerboardOnImage(I, sigma, minCornerMetric); 
		detectCheckerboardOnImage(b_I, sigma, minCornerMetric, points, boardSize);
	}

	emxFree_real32_T(&b_I);
}

//
// Arguments    : void
// Return Type  : void
//
void detectCheckerboardInitialize()
{
	omp_init_nest_lock(&emlrtNestLockGlobal);
	subPixelLocationImpl_init();
}

//
// Arguments    : void
// Return Type  : void
//
void CheckerboardPointsTerminate()
{
	omp_destroy_nest_lock(&emlrtNestLockGlobal);
}


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
	emxInit_uint8_T(&emx, numDimensions);
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












