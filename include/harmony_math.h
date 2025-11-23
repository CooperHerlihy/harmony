/*
 * =============================================================================
 *
 * Copyright (c) 2025 Cooper Herlihy
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * =============================================================================
 */

#ifndef HARMONY_MATH_H
#define HARMONY_MATH_H

#include "harmony.h"

// random number generators : TODO

// hash functions : TODO

/**
 * Creates a model matrix for 2D graphics
 *
 * Parameters
 * - position The position of the model
 * - scale The scale of the model
 * - rotation The rotation of the model
 * Returns
 * - The created matrix
 */
Mat4 harmony_model_matrix_2d(Vec3 position, Vec2 scale, f32 rotation);

/**
 * Creates a model matrix for 3D graphics
 *
 * Parameters
 * - position The position of the model
 * - scale The scale of the model
 * - rotation The rotation of the model
 * Returns
 * - The created matrix
 */
Mat4 harmony_model_matrix_3d(Vec3 position, Vec3 scale, Quat rotation);

/**
 * Creates a view matrix
 *
 * Parameters
 * - position The position of the camera
 * - zoom The zoom of the camera
 * - rotation The rotation of the camera
 * Returns
 * - The created matrix
 */
Mat4 harmony_view_matrix(Vec3 position, f32 zoom, Quat rotation);

/**
 * Creates an orthographic projection matrix
 *
 * Parameters
 * - left The left-hand side of the view frustum
 * - right The right-hand side of the view frustum
 * - top The top of the view frustum
 * - bottom The bottom of the view frustum
 * - near The near plane of the view frustum
 * - far The far plane of the view frustum
 * Returns
 * - The created matrix
 */
Mat4 harmony_orthographic_projection(f32 left, f32 right, f32 top, f32 bottom, f32 near, f32 far);

/**
 * Creates a perspective projection matrix
 *
 * Parameters
 * - fov The field of view of the projection in radians
 * - aspect The aspect ratio of the projection
 * - near The near plane of the projection, must be greater than 0.0f
 * - far The far plane of the projection, must be greater than near
 * Returns
 * - The created matrix
 */
Mat4 harmony_perspective_projection(f32 fov, f32 aspect, f32 near, f32 far);

#if defined(HARMONY_IMPLEMENTATION_MATH) || defined(HARMONY_IMPLEMENTATION_ALL)

Mat4 harmony_model_matrix_2d(Vec3 position, Vec2 scale, f32 rotation) {
    Mat2 m2 = smat2(1.0f);
    m2.x.x = scale.x;
    m2.y.y = scale.y;
    m2 = mmul2((Mat2){
        {cosf(rotation), sinf(rotation)},
        {-sinf(rotation), cosf(rotation)},
    }, m2);
    Mat4 m4 = mat2to4(m2);
    m4.w.x = position.x;
    m4.w.y = position.y;
    m4.w.z = position.z;
    return m4;
}

Mat4 harmony_model_matrix_3d(Vec3 position, Vec3 scale, Quat rotation) {
    Mat3 m3 = smat3(1.0f);
    m3.x.x = scale.x;
    m3.y.y = scale.y;
    m3.z.z = scale.z;
    m3 = rotate_mat3(rotation, m3);
    Mat4 m4 = mat3to4(m3);
    m4.w.x = position.x;
    m4.w.y = position.y;
    m4.w.z = position.z;
    return m4;
}

Mat4 harmony_view_matrix(Vec3 position, f32 zoom, Quat rotation) {
    Mat4 rot = mat3to4(rotate_mat3(qconj(rotation), smat3(1.0f)));
    Mat4 pos = smat4(1.0f);
    pos.x.x = zoom;
    pos.y.y = zoom;
    pos.w.x = -position.x;
    pos.w.y = -position.y;
    pos.w.z = -position.z;
    return mmul4(rot, pos);
}

Mat4 harmony_orthographic_projection(f32 left, f32 right, f32 top, f32 bottom, f32 near, f32 far) {
    return (Mat4){
        {2.0f / (right - left), 0.0f, 0.0f, 0.0f},
        {0.0f, 2.0f / (bottom - top), 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f / (far - near), 0.0f},
        {-(right + left) / (right - left), -(bottom + top) / (bottom - top), -(near) / (far - near), 1.0f},
    };
}

Mat4 harmony_perspective_projection(f32 fov, f32 aspect, f32 near, f32 far) {
    harmony_assert(near > 0.0f);
    harmony_assert(far > near);
    f32 scale = 1 / tanf(fov / 2);
    return (Mat4){
        {scale / aspect, 0.0f, 0.0f, 0.0f},
        {0.0f, scale, 0.0f, 0.0f},
        {0.0f, 0.0f, far / (far - near), 1.0f},
        {0.0f, 0.0f, -(far * near) / (far - near), 0.0f},
    };
}

#endif // defined(HARMONY_IMPLEMENTATION_MATH) || defined(HARMONY_IMPLEMENTATION_ALL)

#endif // HARMONY_MATH_H
