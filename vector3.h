#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <vector>

struct Vector2 {
    float x, y;
    inline Vector2() : x(0), y(0) {}
    inline Vector2(float x, float y) : x(x), y(y) {}
};

struct Vector3 {
    union {
        struct { float x, y, z; };
        float data[3];
    };
    inline Vector3() : x(0), y(0), z(0) {}
    inline Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    inline float magnitude() const { return sqrtf(x * x + y * y + z * z); }
    inline Vector3 operator-(const Vector3& o) const { return Vector3(x - o.x, y - o.y, z - o.z); }
};

struct Quaternion {
    float x, y, z, w;
    inline Quaternion() : x(0), y(0), z(0), w(1) {}
};
