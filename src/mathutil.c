#include "mathutil.h"
#include <math.h>
#include <ogc/gu.h>

void EulerToQuaternion(guQuaternion* q, const f32 rX, const f32 rY, const f32 rZ) {
	guVector vec;
	vec.x = rX; vec.y = rY; vec.z = rZ;
	guVecScale(&vec, &vec, 0.5f);

	f32 x[2], y[2], z[2];

	sincosf(vec.x, &x[0], &x[1]);
	sincosf(vec.y, &y[0], &y[1]);
	sincosf(vec.z, &z[0], &z[1]);

	ps_eulerQuat(x, y, z, q);
}

void AxisAngleToQuaternion(guQuaternion* q, guVector rAxis, const f32 rAngle) {
	f32 s, c;
	sincosf(rAngle / 2.0f, &s, &c);
	guVector out;
	guVecScale(&rAxis, &out, s);
	q->x = out.x;
	q->y = out.y;
	q->z = out.z;
	q->w = c;
}

void QUAT_lookat(guVector* forward, guVector *up, guQuaternion* out) {
	Mtx tmpMatrix;
	guVector zero = { 0, 0, 0 };
	guLookAt(tmpMatrix, forward, up, &zero);
	c_guQuatMtx(out, tmpMatrix);
}

void QUAT_slerp(guQuaternion* q0, guQuaternion* q1, const float t, guQuaternion* out) {
	if (t <= 0.0f) {
		*out = *q0;
		return;
	} else if (t >= 1.0f) {
		*out = *q1;
		return;
	}

	f32 cosOmega;
	QUAT_dotProduct(q0, q1, &cosOmega);
	guQuaternion q0c = *q0,
				 q1c = *q1;

	if (cosOmega < 0.0f) {
		f32 invert = -1.f;
		QUAT_scale(&q1c, &q1c, &invert);
		cosOmega = -cosOmega;
	}

	f32 k0, k1;
	if (cosOmega > 0.9999f) {
		k0 = 1.0f - t;
		k1 = t;
	} else {
		f32 sinOmega = sqrt(1.0f - cosOmega*cosOmega);
		f32 omega = atan2(sinOmega, cosOmega);
		f32 oneOverSinOmega = 1.0f / sinOmega;

		k0 = sin((1.0f - t) * omega) * oneOverSinOmega;
		k1 = sin(t * omega) * oneOverSinOmega;
	}

	QUAT_scale(&q0c, &q0c, &k0);
	QUAT_scale(&q1c, &q1c, &k1);
	guQuatAdd(&q0c, &q1c, out);
	return;
}

/* This should be in gu.h or something */

inline f32 guVecMag(guVector* vec) {
	return sqrt(guVecDotProduct(vec, vec));
}

inline f32 vecDistance(guVector* point1, guVector* point2) {
	guVector sub;
	guVecSub(point2, point1, &sub);
	return guVecMag(&sub);
}

u32 seed = 0;
inline f32 fioraRand() {
	seed = 214013 * seed + 2531011;
	union {
		u32 temp;
		f32 femp;
	} data;
	data.temp = (seed & 0x007FFFFF) | 0x3F800000;
	return data.femp - 1.0f;
}

inline void fioraSeed(u32 _seed) {
	seed = _seed;
}

guVector worldUp = { 0, 1, 0 };
guVector worldForward = { 0, 0, 1 };
guVector worldRight = { 1, 0, 0 };

inline void MakeMatrix(transform_t* t) {
	/* Reset matrix to identity */
	guMtxIdentity(t->matrix);

	/* Rotate, Scale, Translate */
	guQuatNormalize(&t->rotation, &t->rotation);
	c_guMtxQuat(t->matrix, &t->rotation);
	guMtxScaleApply(t->matrix, t->matrix, t->scale.x, t->scale.y, t->scale.z);
	guMtxTransApply(t->matrix, t->matrix, t->position.x, t->position.y, t->position.z);

	/* Calculate forward/up/left vectors */
	guVecMultiplySR(t->matrix, &worldUp, &t->up);
	guVecMultiplySR(t->matrix, &worldForward, &t->forward);
	guVecMultiplySR(t->matrix, &worldRight, &t->right);

	guVecNormalize(&t->up);
	guVecNormalize(&t->forward);
	guVecNormalize(&t->right);

	t->dirty = FALSE;
}

inline void SimpleMatrix(MtxP matrix, guQuaternion* rot, guVector* scale, guVector* pos) {
	guMtxIdentity(matrix);
	c_guMtxQuat(matrix, rot);
	guMtxScaleApply(matrix, matrix, scale->x, scale->y, scale->z);
	guMtxTransApply(matrix, matrix, pos->x, pos->y, pos->z);
}