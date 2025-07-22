#pragma once
#include <cmath>
#include "Vector3D.h"

class MyQuaternion {
public:
    float x, y, z, w;

    // Default constructor: identity MyQuaternion
    MyQuaternion() : x(0), y(0), z(0), w(1) {}

    // Constructor from components
    MyQuaternion(float x, float y, float z, float w)
        : x(x), y(y), z(z), w(w) {
    }

    // Construct from Euler angles (in radians)
    static MyQuaternion fromEulerXYZ(Vector3D xyz) {
        float cx = cos(xyz.x * 0.5f);
        float sx = sin(xyz.x * 0.5f);
        float cy = cos(xyz.y * 0.5f);
        float sy = sin(xyz.y * 0.5f);
        float cz = cos(xyz.z * 0.5f);
        float sz = sin(xyz.z * 0.5f);

        MyQuaternion q;
        q.w = cx * cy * cz + sx * sy * sz;
        q.x = sx * cy * cz - cx * sy * sz;
        q.y = cx * sy * cz + sx * cy * sz;
        q.z = cx * cy * sz - sx * sy * cz;
        return q;
    }

    // Normalize the MyQuaternion
    void normalize() {
        float mag = std::sqrt(x * x + y * y + z * z + w * w);
        if (mag > 0.0f) {
            float invMag = 1.0f / mag;
            x *= invMag;
            y *= invMag;
            z *= invMag;
            w *= invMag;
        }
    }

    // MyQuaternion multiplication (composition)
    MyQuaternion operator*(const MyQuaternion& rhs) const {
        return MyQuaternion(
            w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
            w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
            w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w,
            w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z
        );
    }

    // Convert to 4x4 rotation matrix (row-major)
    void toMatrix(float m[16]) const {
        float xx = x * x;
        float yy = y * y;
        float zz = z * z;
        float xy = x * y;
        float xz = x * z;
        float yz = y * z;
        float wx = w * x;
        float wy = w * y;
        float wz = w * z;

        m[0] = 1.0f - 2.0f * (yy + zz);
        m[1] = 2.0f * (xy - wz);
        m[2] = 2.0f * (xz + wy);
        m[3] = 0.0f;

        m[4] = 2.0f * (xy + wz);
        m[5] = 1.0f - 2.0f * (xx + zz);
        m[6] = 2.0f * (yz - wx);
        m[7] = 0.0f;

        m[8] = 2.0f * (xz - wy);
        m[9] = 2.0f * (yz + wx);
        m[10] = 1.0f - 2.0f * (xx + yy);
        m[11] = 0.0f;

        m[12] = 0.0f;
        m[13] = 0.0f;
        m[14] = 0.0f;
        m[15] = 1.0f;
    }
};