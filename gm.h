#ifndef GM_H
#define GM_H

#define GM_PI 3.14159265358979323846f
#define GM_PI_2 2.0f * GMATH_PI
#define GM_HALF_PI 0.5f * GMATH_PI
#define GM_QUARTER_PI 0.25f * GMATH_PI
#define GM_ONE_OVER_PI 1.0f / GMATH_PI
#define GM_ONE_OVER_TWO_PI 1.0f / GMATH_PI_2
#define GM_SQRT_TWO 1.41421356237309504880f
#define GM_SQRT_THREE 1.73205080756887729352f
#define GM_SQRT_ONE_OVER_TWO 0.70710678118654752440f
#define GM_SQRT_ONE_OVER_THREE 0.57735026918962576450f
#define GM_DEG2RAD_MULTIPLIER GMATH_PI / 180.0f
#define GM_RAD2DEG_MULTIPLIER 180.0f / GMATH_PI
#define GM_SEC_TO_US_MULTIPLIER (1000.0f * 1000.0f)
#define GM_SEC_TO_MS_MULTIPLIER 1000.0f
#define GM_MS_TO_SEC_MULTIPLIER 0.001f
#define GM_INFINITY 1e30f
#define GM_EPSILON 1.192092896e-07f

#ifndef GM_STATIC_INLINE
    #define GM_STATIC_INLINE static inline
#endif

#include <math.h> // sqrt fabs

typedef unsigned char gm_bool;
#define GM_TRUE 1
#define GM_FALSE

typedef float gm_float;

typedef union V2f {
    gm_float elements[2];
    struct {
        union { gm_float x, r, s, u; };
        union { gm_float y, g, t, v; };
    };
} V2f;

GM_STATIC_INLINE V2f v2f(gm_float x, gm_float y) { return (V2f){ .x = x, .y = y, }; }
GM_STATIC_INLINE V2f v2f_zeros(void) { return (V2f){ .x=0.0f, .y=0.0f, }; }
GM_STATIC_INLINE V2f v2f_ones(void)  { return (V2f){ .x=1.0f, .y=1.0f, }; }
GM_STATIC_INLINE V2f v2f_add(V2f a, V2f b) { return (V2f){ .x=a.x+b.x, .y=a.y+b.y, }; }
GM_STATIC_INLINE V2f v2f_sub(V2f a, V2f b) { return (V2f){ .x=a.x-b.x, .y=a.y-b.y, }; }
GM_STATIC_INLINE V2f v2f_mul(V2f a, V2f b) { return (V2f){ .x=a.x*b.x, .y=a.y*b.y, }; }
GM_STATIC_INLINE V2f v2f_div(V2f a, V2f b) { return (V2f){ .x=a.x/b.x, .y=a.y/b.y, }; }
GM_STATIC_INLINE gm_float v2f_length(V2f a) { return (gm_float)sqrt((double)(a.x*a.x + a.y*a.y)); }
GM_STATIC_INLINE V2f v2f_normalize(V2f a) { gm_float l = v2f_length(a); return (V2f){ .x = a.x/l, .y = a.y/l, }; }
GM_STATIC_INLINE gm_float v2f_distance(V2f a, V2f b) { return v2f_length(v2f( a.x-b.x, a.y-b.y )); }
GM_STATIC_INLINE gm_bool v2f_cmp(V2f a, V2f b) { return fabs(a.x-b.x) <= GM_EPSILON && fabs(a.y-b.y) <= GM_EPSILON; }

typedef union V3f {
    gm_float elements[3];
    struct {
        union { gm_float x, r, s, u; };
        union { gm_float y, g, t, v; };
        union { gm_float z, b, p, w; };
    };
} V3f;

GM_STATIC_INLINE V3f v3f(gm_float x, gm_float y, gm_float z) { return (V3f){ .x=x, .y=y, .z=z }; }
GM_STATIC_INLINE V3f v3f_zeros(void) { return (V3f){ .x=0.0f, .y=0.0f, .z=0.0f }; }
GM_STATIC_INLINE V3f v3f_ones(void)  { return (V3f){ .x=1.0f, .y=1.0f, .z=1.0f }; }
GM_STATIC_INLINE V3f v3f_add(V3f a, V3f b) { return (V3f){ .x=a.x+b.x, .y=a.y+b.y, .z=a.z+b.z}; }
GM_STATIC_INLINE V3f v3f_sub(V3f a, V3f b) { return (V3f){ .x=a.x-b.x, .y=a.y-b.y, .z=a.z-b.z}; }
GM_STATIC_INLINE V3f v3f_mul(V3f a, V3f b) { return (V3f){ .x=a.x*b.x, .y=a.y*b.y, .z=a.z*b.z}; }
GM_STATIC_INLINE V3f v3f_div(V3f a, V3f b) { return (V3f){ .x=a.x/b.x, .y=a.y/b.y, .z=a.z/b.z}; }
GM_STATIC_INLINE gm_float v3f_length(V3f a) { return (gm_float)sqrt((double)(a.x*a.x + a.y*a.y + a.z*a.z)); }
GM_STATIC_INLINE V3f v3f_normalize(V3f a) { gm_float l = v3f_length(a); return (V3f){ .x = a.x/l, .y = a.y/l, .z=a.z/l }; }
GM_STATIC_INLINE gm_float v3f_distance(V3f a, V3f b) { return v3f_length(v3f( a.x-b.x, a.y-b.y, a.z-b.z )); }
GM_STATIC_INLINE gm_bool v3f_cmp(V3f a, V3f b) { return fabs(a.x-b.x) <= GM_EPSILON 
    && fabs(a.y-b.y) <= GM_EPSILON
    && fabs(a.z-b.z) <= GM_EPSILON; }

typedef union V4f {
    gm_float elements[4];
    struct {
        union { gm_float x, r, s; };
        union { gm_float y, g, t; };
        union { gm_float z, b, p; };
        union { gm_float w, a, q; };
    };
} V4f;

GM_STATIC_INLINE V4f v4f(gm_float x, gm_float y, gm_float z, gm_float w) { return (V4f){ .x=x, .y=y, .z=z, .w=w }; }
GM_STATIC_INLINE V4f v4f_zeros(void) { return (V4f){ .x=0.0f, .y=0.0f, .z=0.0f, .w=0.0f }; }
GM_STATIC_INLINE V4f v4f_ones(void)  { return (V4f){ .x=1.0f, .y=1.0f, .z=1.0f, .w=1.0f }; }
GM_STATIC_INLINE V4f v4f_add(V4f a, V4f b) { return (V4f){ .x=a.x+b.x, .y=a.y+b.y, .z=a.z+b.z, .w=a.w+b.w }; }
GM_STATIC_INLINE V4f v4f_sub(V4f a, V4f b) { return (V4f){ .x=a.x+b.x, .y=a.y+b.y, .z=a.z+b.z, .w=a.w+b.w }; }
GM_STATIC_INLINE V4f v4f_mul(V4f a, V4f b) { return (V4f){ .x=a.x+b.x, .y=a.y+b.y, .z=a.z+b.z, .w=a.w+b.w }; }
GM_STATIC_INLINE V4f v4f_div(V4f a, V4f b) { return (V4f){ .x=a.x+b.x, .y=a.y+b.y, .z=a.z+b.z, .w=a.w+b.w }; }
GM_STATIC_INLINE gm_float v4f_length(V4f a) { return (gm_float)sqrt((double)(a.x*a.x + a.y*a.y + a.z*a.z + a.w*a.w)); }
GM_STATIC_INLINE V4f v4f_normalize(V4f a) { gm_float l = v4f_length(a); return (V4f){ .x = a.x/l, .y = a.y/l, .z=a.z/l, .w=a.w/l }; }
GM_STATIC_INLINE gm_float v4f_distance(V4f a, V4f b) { return v4f_length(v4f( a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w )); }
GM_STATIC_INLINE gm_float v4f_dot(V4f a, V4f b) { return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w; }
GM_STATIC_INLINE gm_bool v4f_cmp(V4f a, V4f b) { return fabs(a.x-b.x) <= GM_EPSILON 
    && fabs(a.y-b.y) <= GM_EPSILON
    && fabs(a.w-b.w) <= GM_EPSILON
    && fabs(a.z-b.z) <= GM_EPSILON; }

typedef union M3f {
    V4f rows[3];
    gm_float elements[3*3];
} M3f;

typedef union M4f {
    V4f rows[4];
    gm_float elements[4*4];
} M4f;

GM_STATIC_INLINE M4f m4f_identity(void) {
    M4f res = {0};
    res.elements[0] = 1.0f;
    res.elements[5] = 1.0f;
    res.elements[10] = 1.0f;
    res.elements[15] = 1.0f;
    return res;
}

GM_STATIC_INLINE M4f m4f_ortho(gm_float left, gm_float right, gm_float bottom, gm_float top, gm_float near, gm_float far) {
    M4f res = m4f_identity();
    gm_float lr = 1.0f / (left - right);
    gm_float bt = 1.0f / (bottom - top);
    gm_float nf = 1.0f / (near - far);

    res.elements[0] = -2.0f * lr;
    res.elements[5] = -2.0f * bt;
    res.elements[10] = 2.0f * nf;

    res.elements[12] = (left + right) * lr;
    res.elements[13] = (top + bottom) * bt;
    res.elements[14] = (near + far) * nf;

    return res;
}

GM_STATIC_INLINE M4f m4f_perspective(gm_float fov, gm_float aspect_ratio, gm_float near, gm_float far) {
    float half_tan_fov = tan(fov*0.5f);
    M4f res = {0};
    res.elements[0] = 1.0f / (aspect_ratio * half_tan_fov);
    res.elements[5] = 1.0f / half_tan_fov;
    res.elements[10] = -((far + near) / (far - near));
    res.elements[11] = -1.0f;
    res.elements[14] = -((2.0f * far * near) / (far - near));
    return res;
}

GM_STATIC_INLINE M4f m4f_dot(M4f a, M4f b) {
    M4f res = m4f_identity();
    const float* a_ptr = a.elements;
    const float* b_ptr = b.elements;
    float* dst_ptr = res.elements;

    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            *dst_ptr =
                a_ptr[0] * b_ptr[0 + j] +
                a_ptr[1] * b_ptr[4 + j] +
                a_ptr[2] * b_ptr[8 + j] +
                a_ptr[3] * b_ptr[12 + j];
            dst_ptr+=1;
        }
        b_ptr+=1;
    }

    return res;
}

#include <stdio.h>
GM_STATIC_INLINE void m4f_dump(M4f m)
{
    for(size_t i=0; i<4; ++i) {
        printf("%f %f %f %f\n", m.rows[i].x, m.rows[i].y, m.rows[i].z, m.rows[i].w);
    }
}
#endif // GM_H

