#ifndef LA_H_
#define LA_H_

#include <stdlib.h>
#include <math.h>

typedef struct { float c[2]; } V2f;
V2f v2f_sum(V2f a, V2f b);
V2f v2f_sub(V2f a, V2f b);
V2f v2f_mul(V2f a, V2f b);
V2f v2f_div(V2f a, V2f b);
V2f v2f(float x0, float x1);
V2f v2fs(float x);
V2f v2f_sqrt(V2f v0);
V2f v2f_pow(V2f v0, V2f v1);
V2f v2f_sin(V2f v0);
V2f v2f_cos(V2f v0);

typedef struct { double c[2]; } V2d;
V2d v2d_sum(V2d a, V2d b);
V2d v2d_sub(V2d a, V2d b);
V2d v2d_mul(V2d a, V2d b);
V2d v2d_div(V2d a, V2d b);
V2d v2d(double x0, double x1);
V2d v2ds(double x);
V2d v2d_sqrt(V2d v0);
V2d v2d_pow(V2d v0, V2d v1);
V2d v2d_sin(V2d v0);
V2d v2d_cos(V2d v0);

typedef struct { int c[2]; } V2i;
V2i v2i_sum(V2i a, V2i b);
V2i v2i_sub(V2i a, V2i b);
V2i v2i_mul(V2i a, V2i b);
V2i v2i_div(V2i a, V2i b);
V2i v2i(int x0, int x1);
V2i v2is(int x);

typedef struct { size_t c[2]; } V2s;
V2s v2s_sum(V2s a, V2s b);
V2s v2s_sub(V2s a, V2s b);
V2s v2s_mul(V2s a, V2s b);
V2s v2s_div(V2s a, V2s b);
V2s v2s(size_t x0, size_t x1);
V2s v2ss(size_t x);

typedef struct { float c[3]; } V3f;
V3f v3f_sum(V3f a, V3f b);
V3f v3f_sub(V3f a, V3f b);
V3f v3f_mul(V3f a, V3f b);
V3f v3f_div(V3f a, V3f b);
V3f v3f(float x0, float x1, float x2);
V3f v3fs(float x);
V3f v3f_sqrt(V3f v0);
V3f v3f_pow(V3f v0, V3f v1);
V3f v3f_sin(V3f v0);
V3f v3f_cos(V3f v0);

typedef struct { double c[3]; } V3d;
V3d v3d_sum(V3d a, V3d b);
V3d v3d_sub(V3d a, V3d b);
V3d v3d_mul(V3d a, V3d b);
V3d v3d_div(V3d a, V3d b);
V3d v3d(double x0, double x1, double x2);
V3d v3ds(double x);
V3d v3d_sqrt(V3d v0);
V3d v3d_pow(V3d v0, V3d v1);
V3d v3d_sin(V3d v0);
V3d v3d_cos(V3d v0);

typedef struct { int c[3]; } V3i;
V3i v3i_sum(V3i a, V3i b);
V3i v3i_sub(V3i a, V3i b);
V3i v3i_mul(V3i a, V3i b);
V3i v3i_div(V3i a, V3i b);
V3i v3i(int x0, int x1, int x2);
V3i v3is(int x);

typedef struct { size_t c[3]; } V3s;
V3s v3s_sum(V3s a, V3s b);
V3s v3s_sub(V3s a, V3s b);
V3s v3s_mul(V3s a, V3s b);
V3s v3s_div(V3s a, V3s b);
V3s v3s(size_t x0, size_t x1, size_t x2);
V3s v3ss(size_t x);

typedef struct { float c[4]; } V4f;
V4f v4f_sum(V4f a, V4f b);
V4f v4f_sub(V4f a, V4f b);
V4f v4f_mul(V4f a, V4f b);
V4f v4f_div(V4f a, V4f b);
V4f v4f(float x0, float x1, float x2, float x3);
V4f v4fs(float x);
V4f v4f_sqrt(V4f v0);
V4f v4f_pow(V4f v0, V4f v1);
V4f v4f_sin(V4f v0);
V4f v4f_cos(V4f v0);

typedef struct { double c[4]; } V4d;
V4d v4d_sum(V4d a, V4d b);
V4d v4d_sub(V4d a, V4d b);
V4d v4d_mul(V4d a, V4d b);
V4d v4d_div(V4d a, V4d b);
V4d v4d(double x0, double x1, double x2, double x3);
V4d v4ds(double x);
V4d v4d_sqrt(V4d v0);
V4d v4d_pow(V4d v0, V4d v1);
V4d v4d_sin(V4d v0);
V4d v4d_cos(V4d v0);

typedef struct { int c[4]; } V4i;
V4i v4i_sum(V4i a, V4i b);
V4i v4i_sub(V4i a, V4i b);
V4i v4i_mul(V4i a, V4i b);
V4i v4i_div(V4i a, V4i b);
V4i v4i(int x0, int x1, int x2, int x3);
V4i v4is(int x);

typedef struct { size_t c[4]; } V4s;
V4s v4s_sum(V4s a, V4s b);
V4s v4s_sub(V4s a, V4s b);
V4s v4s_mul(V4s a, V4s b);
V4s v4s_div(V4s a, V4s b);
V4s v4s(size_t x0, size_t x1, size_t x2, size_t x3);
V4s v4ss(size_t x);

#endif //LA_H_

#ifdef LA_IMPLEMENTATION

V2f v2f_sum(V2f a, V2f b)
{
	for(int i = 0; i < 2; ++i) a.c[i] += b.c[i];
	return a;
}
V2f v2f_sub(V2f a, V2f b)
{
	for(int i = 0; i < 2; ++i) a.c[i] -= b.c[i];
	return a;
}
V2f v2f_mul(V2f a, V2f b)
{
	for(int i = 0; i < 2; ++i) a.c[i] *= b.c[i];
	return a;
}
V2f v2f_div(V2f a, V2f b)
{
	for(int i = 0; i < 2; ++i) a.c[i] /= b.c[i];
	return a;
}
V2f v2f(float x0, float x1)
{
	V2f result;
	result.c[0] = x0;
	result.c[1] = x1;
	return result;
}
V2f v2fs(float x)
{
	return v2f(x, x);
}
V2f v2f_sqrt(V2f v0)
{
	for(int i = 0; i < 2; ++i) v0.c[i] = sqrtf(v0.c[i]);
	return v0;
}
V2f v2f_pow(V2f v0, V2f v1)
{
	for(int i = 0; i < 2; ++i) v0.c[i] = powf(v0.c[i], v1.c[i]);
	return v0;
}
V2f v2f_sin(V2f v0)
{
	for(int i = 0; i < 2; ++i) v0.c[i] = sinf(v0.c[i]);
	return v0;
}
V2f v2f_cos(V2f v0)
{
	for(int i = 0; i < 2; ++i) v0.c[i] = cosf(v0.c[i]);
	return v0;
}
V2d v2d_sum(V2d a, V2d b)
{
	for(int i = 0; i < 2; ++i) a.c[i] += b.c[i];
	return a;
}
V2d v2d_sub(V2d a, V2d b)
{
	for(int i = 0; i < 2; ++i) a.c[i] -= b.c[i];
	return a;
}
V2d v2d_mul(V2d a, V2d b)
{
	for(int i = 0; i < 2; ++i) a.c[i] *= b.c[i];
	return a;
}
V2d v2d_div(V2d a, V2d b)
{
	for(int i = 0; i < 2; ++i) a.c[i] /= b.c[i];
	return a;
}
V2d v2d(double x0, double x1)
{
	V2d result;
	result.c[0] = x0;
	result.c[1] = x1;
	return result;
}
V2d v2ds(double x)
{
	return v2d(x, x);
}
V2d v2d_sqrt(V2d v0)
{
	for(int i = 0; i < 2; ++i) v0.c[i] = sqrt(v0.c[i]);
	return v0;
}
V2d v2d_pow(V2d v0, V2d v1)
{
	for(int i = 0; i < 2; ++i) v0.c[i] = pow(v0.c[i], v1.c[i]);
	return v0;
}
V2d v2d_sin(V2d v0)
{
	for(int i = 0; i < 2; ++i) v0.c[i] = sin(v0.c[i]);
	return v0;
}
V2d v2d_cos(V2d v0)
{
	for(int i = 0; i < 2; ++i) v0.c[i] = cos(v0.c[i]);
	return v0;
}
V2i v2i_sum(V2i a, V2i b)
{
	for(int i = 0; i < 2; ++i) a.c[i] += b.c[i];
	return a;
}
V2i v2i_sub(V2i a, V2i b)
{
	for(int i = 0; i < 2; ++i) a.c[i] -= b.c[i];
	return a;
}
V2i v2i_mul(V2i a, V2i b)
{
	for(int i = 0; i < 2; ++i) a.c[i] *= b.c[i];
	return a;
}
V2i v2i_div(V2i a, V2i b)
{
	for(int i = 0; i < 2; ++i) a.c[i] /= b.c[i];
	return a;
}
V2i v2i(int x0, int x1)
{
	V2i result;
	result.c[0] = x0;
	result.c[1] = x1;
	return result;
}
V2i v2is(int x)
{
	return v2i(x, x);
}
V2s v2s_sum(V2s a, V2s b)
{
	for(int i = 0; i < 2; ++i) a.c[i] += b.c[i];
	return a;
}
V2s v2s_sub(V2s a, V2s b)
{
	for(int i = 0; i < 2; ++i) a.c[i] -= b.c[i];
	return a;
}
V2s v2s_mul(V2s a, V2s b)
{
	for(int i = 0; i < 2; ++i) a.c[i] *= b.c[i];
	return a;
}
V2s v2s_div(V2s a, V2s b)
{
	for(int i = 0; i < 2; ++i) a.c[i] /= b.c[i];
	return a;
}
V2s v2s(size_t x0, size_t x1)
{
	V2s result;
	result.c[0] = x0;
	result.c[1] = x1;
	return result;
}
V2s v2ss(size_t x)
{
	return v2s(x, x);
}
V3f v3f_sum(V3f a, V3f b)
{
	for(int i = 0; i < 3; ++i) a.c[i] += b.c[i];
	return a;
}
V3f v3f_sub(V3f a, V3f b)
{
	for(int i = 0; i < 3; ++i) a.c[i] -= b.c[i];
	return a;
}
V3f v3f_mul(V3f a, V3f b)
{
	for(int i = 0; i < 3; ++i) a.c[i] *= b.c[i];
	return a;
}
V3f v3f_div(V3f a, V3f b)
{
	for(int i = 0; i < 3; ++i) a.c[i] /= b.c[i];
	return a;
}
V3f v3f(float x0, float x1, float x2)
{
	V3f result;
	result.c[0] = x0;
	result.c[1] = x1;
	result.c[2] = x2;
	return result;
}
V3f v3fs(float x)
{
	return v3f(x, x, x);
}
V3f v3f_sqrt(V3f v0)
{
	for(int i = 0; i < 3; ++i) v0.c[i] = sqrtf(v0.c[i]);
	return v0;
}
V3f v3f_pow(V3f v0, V3f v1)
{
	for(int i = 0; i < 3; ++i) v0.c[i] = powf(v0.c[i], v1.c[i]);
	return v0;
}
V3f v3f_sin(V3f v0)
{
	for(int i = 0; i < 3; ++i) v0.c[i] = sinf(v0.c[i]);
	return v0;
}
V3f v3f_cos(V3f v0)
{
	for(int i = 0; i < 3; ++i) v0.c[i] = cosf(v0.c[i]);
	return v0;
}
V3d v3d_sum(V3d a, V3d b)
{
	for(int i = 0; i < 3; ++i) a.c[i] += b.c[i];
	return a;
}
V3d v3d_sub(V3d a, V3d b)
{
	for(int i = 0; i < 3; ++i) a.c[i] -= b.c[i];
	return a;
}
V3d v3d_mul(V3d a, V3d b)
{
	for(int i = 0; i < 3; ++i) a.c[i] *= b.c[i];
	return a;
}
V3d v3d_div(V3d a, V3d b)
{
	for(int i = 0; i < 3; ++i) a.c[i] /= b.c[i];
	return a;
}
V3d v3d(double x0, double x1, double x2)
{
	V3d result;
	result.c[0] = x0;
	result.c[1] = x1;
	result.c[2] = x2;
	return result;
}
V3d v3ds(double x)
{
	return v3d(x, x, x);
}
V3d v3d_sqrt(V3d v0)
{
	for(int i = 0; i < 3; ++i) v0.c[i] = sqrt(v0.c[i]);
	return v0;
}
V3d v3d_pow(V3d v0, V3d v1)
{
	for(int i = 0; i < 3; ++i) v0.c[i] = pow(v0.c[i], v1.c[i]);
	return v0;
}
V3d v3d_sin(V3d v0)
{
	for(int i = 0; i < 3; ++i) v0.c[i] = sin(v0.c[i]);
	return v0;
}
V3d v3d_cos(V3d v0)
{
	for(int i = 0; i < 3; ++i) v0.c[i] = cos(v0.c[i]);
	return v0;
}
V3i v3i_sum(V3i a, V3i b)
{
	for(int i = 0; i < 3; ++i) a.c[i] += b.c[i];
	return a;
}
V3i v3i_sub(V3i a, V3i b)
{
	for(int i = 0; i < 3; ++i) a.c[i] -= b.c[i];
	return a;
}
V3i v3i_mul(V3i a, V3i b)
{
	for(int i = 0; i < 3; ++i) a.c[i] *= b.c[i];
	return a;
}
V3i v3i_div(V3i a, V3i b)
{
	for(int i = 0; i < 3; ++i) a.c[i] /= b.c[i];
	return a;
}
V3i v3i(int x0, int x1, int x2)
{
	V3i result;
	result.c[0] = x0;
	result.c[1] = x1;
	result.c[2] = x2;
	return result;
}
V3i v3is(int x)
{
	return v3i(x, x, x);
}
V3s v3s_sum(V3s a, V3s b)
{
	for(int i = 0; i < 3; ++i) a.c[i] += b.c[i];
	return a;
}
V3s v3s_sub(V3s a, V3s b)
{
	for(int i = 0; i < 3; ++i) a.c[i] -= b.c[i];
	return a;
}
V3s v3s_mul(V3s a, V3s b)
{
	for(int i = 0; i < 3; ++i) a.c[i] *= b.c[i];
	return a;
}
V3s v3s_div(V3s a, V3s b)
{
	for(int i = 0; i < 3; ++i) a.c[i] /= b.c[i];
	return a;
}
V3s v3s(size_t x0, size_t x1, size_t x2)
{
	V3s result;
	result.c[0] = x0;
	result.c[1] = x1;
	result.c[2] = x2;
	return result;
}
V3s v3ss(size_t x)
{
	return v3s(x, x, x);
}
V4f v4f_sum(V4f a, V4f b)
{
	for(int i = 0; i < 4; ++i) a.c[i] += b.c[i];
	return a;
}
V4f v4f_sub(V4f a, V4f b)
{
	for(int i = 0; i < 4; ++i) a.c[i] -= b.c[i];
	return a;
}
V4f v4f_mul(V4f a, V4f b)
{
	for(int i = 0; i < 4; ++i) a.c[i] *= b.c[i];
	return a;
}
V4f v4f_div(V4f a, V4f b)
{
	for(int i = 0; i < 4; ++i) a.c[i] /= b.c[i];
	return a;
}
V4f v4f(float x0, float x1, float x2, float x3)
{
	V4f result;
	result.c[0] = x0;
	result.c[1] = x1;
	result.c[2] = x2;
	result.c[3] = x3;
	return result;
}
V4f v4fs(float x)
{
	return v4f(x, x, x, x);
}
V4f v4f_sqrt(V4f v0)
{
	for(int i = 0; i < 4; ++i) v0.c[i] = sqrtf(v0.c[i]);
	return v0;
}
V4f v4f_pow(V4f v0, V4f v1)
{
	for(int i = 0; i < 4; ++i) v0.c[i] = powf(v0.c[i], v1.c[i]);
	return v0;
}
V4f v4f_sin(V4f v0)
{
	for(int i = 0; i < 4; ++i) v0.c[i] = sinf(v0.c[i]);
	return v0;
}
V4f v4f_cos(V4f v0)
{
	for(int i = 0; i < 4; ++i) v0.c[i] = cosf(v0.c[i]);
	return v0;
}
V4d v4d_sum(V4d a, V4d b)
{
	for(int i = 0; i < 4; ++i) a.c[i] += b.c[i];
	return a;
}
V4d v4d_sub(V4d a, V4d b)
{
	for(int i = 0; i < 4; ++i) a.c[i] -= b.c[i];
	return a;
}
V4d v4d_mul(V4d a, V4d b)
{
	for(int i = 0; i < 4; ++i) a.c[i] *= b.c[i];
	return a;
}
V4d v4d_div(V4d a, V4d b)
{
	for(int i = 0; i < 4; ++i) a.c[i] /= b.c[i];
	return a;
}
V4d v4d(double x0, double x1, double x2, double x3)
{
	V4d result;
	result.c[0] = x0;
	result.c[1] = x1;
	result.c[2] = x2;
	result.c[3] = x3;
	return result;
}
V4d v4ds(double x)
{
	return v4d(x, x, x, x);
}
V4d v4d_sqrt(V4d v0)
{
	for(int i = 0; i < 4; ++i) v0.c[i] = sqrt(v0.c[i]);
	return v0;
}
V4d v4d_pow(V4d v0, V4d v1)
{
	for(int i = 0; i < 4; ++i) v0.c[i] = pow(v0.c[i], v1.c[i]);
	return v0;
}
V4d v4d_sin(V4d v0)
{
	for(int i = 0; i < 4; ++i) v0.c[i] = sin(v0.c[i]);
	return v0;
}
V4d v4d_cos(V4d v0)
{
	for(int i = 0; i < 4; ++i) v0.c[i] = cos(v0.c[i]);
	return v0;
}
V4i v4i_sum(V4i a, V4i b)
{
	for(int i = 0; i < 4; ++i) a.c[i] += b.c[i];
	return a;
}
V4i v4i_sub(V4i a, V4i b)
{
	for(int i = 0; i < 4; ++i) a.c[i] -= b.c[i];
	return a;
}
V4i v4i_mul(V4i a, V4i b)
{
	for(int i = 0; i < 4; ++i) a.c[i] *= b.c[i];
	return a;
}
V4i v4i_div(V4i a, V4i b)
{
	for(int i = 0; i < 4; ++i) a.c[i] /= b.c[i];
	return a;
}
V4i v4i(int x0, int x1, int x2, int x3)
{
	V4i result;
	result.c[0] = x0;
	result.c[1] = x1;
	result.c[2] = x2;
	result.c[3] = x3;
	return result;
}
V4i v4is(int x)
{
	return v4i(x, x, x, x);
}
V4s v4s_sum(V4s a, V4s b)
{
	for(int i = 0; i < 4; ++i) a.c[i] += b.c[i];
	return a;
}
V4s v4s_sub(V4s a, V4s b)
{
	for(int i = 0; i < 4; ++i) a.c[i] -= b.c[i];
	return a;
}
V4s v4s_mul(V4s a, V4s b)
{
	for(int i = 0; i < 4; ++i) a.c[i] *= b.c[i];
	return a;
}
V4s v4s_div(V4s a, V4s b)
{
	for(int i = 0; i < 4; ++i) a.c[i] /= b.c[i];
	return a;
}
V4s v4s(size_t x0, size_t x1, size_t x2, size_t x3)
{
	V4s result;
	result.c[0] = x0;
	result.c[1] = x1;
	result.c[2] = x2;
	result.c[3] = x3;
	return result;
}
V4s v4ss(size_t x)
{
	return v4s(x, x, x, x);
}
#endif //LA_IMPLEMENTATION