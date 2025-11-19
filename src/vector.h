#ifndef VECTOR_H

#define VECTOR_H

#include <cmath>
#include <iostream>
#include <cstring>

class Vector2
{
public:
    float x;
    float y;
    Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
    ~Vector2() {}
    float length() const { return std::sqrt(x * x + y * y); }

    void normalize()
    {
        float len = length();
        if (len > 0)
        {
            x /= len;
            y /= len;
        }
    }

    static Vector2 zero()
    {
        return Vector2(0.0f, 0.0f);
    }

    static Vector2 one()
    {
        return Vector2(1.0f, 1.0f);
    }

    Vector2 normalized() const
    {
        float len = length();
        return (len > 0) ? Vector2(x / len, y / len) : Vector2(0, 0);
    }

    float dot(const Vector2 &v) const { return x * v.x + y * v.y; }

    Vector2 operator+(const Vector2 &v) const { return Vector2(x + v.x, y + v.y); }
    Vector2 operator-(const Vector2 &v) const { return Vector2(x - v.x, y - v.y); }
    Vector2 operator*(float s) const { return Vector2(x * s, y * s); }
    Vector2 operator/(float s) const { return Vector2(x / s, y / s); }

    Vector2 &operator+=(const Vector2 &v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }
    Vector2 &operator-=(const Vector2 &v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    void print() const { std::cout << "(" << x << ", " << y << ")"; }
};

class Vector3
{
public:
    float x;
    float y;
    float z;
    Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
    ~Vector3() {}

    float length() const { return std::sqrt(x * x + y * y + z * z); }

    void normalize()
    {
        float len = length();
        if (len > 0)
        {
            x /= len;
            y /= len;
            z /= len;
        }
    }

    static Vector3 zero()
    {
        return Vector3(0.0f, 0.0f, 0.0f);
    }

    static Vector3 one()
    {
        return Vector3(1.0f, 1.0f, 1.0f);
    }

    Vector3 normalized() const
    {
        float len = length();
        return (len > 0) ? Vector3(x / len, y / len, z / len) : Vector3(0, 0, 0);
    }

    float dot(const Vector3 &v) const { return x * v.x + y * v.y + z * v.z; }

    Vector3 cross(const Vector3 &v) const
    {
        return Vector3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x);
    }

    Vector3 operator+(const Vector3 &v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3 operator-(const Vector3 &v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
    Vector3 operator*(float s) const { return Vector3(x * s, y * s, z * s); }
    Vector3 operator/(float s) const { return Vector3(x / s, y / s, z / s); }

    Vector3 &operator+=(const Vector3 &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    Vector3 &operator-=(const Vector3 &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    void print() const { std::cout << "(" << x << ", " << y << ", " << z << ")"; }
};

class Vector4
{
public:
    float x;
    float y;
    float z;
    float w;
    Vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) : x(x), y(y), z(z), w(w) {}
    ~Vector4() {}

    float length() const { return std::sqrt(x * x + y * y + z * z + w * w); }

    void normalize()
    {
        float len = length();
        if (len > 0)
        {
            x /= len;
            y /= len;
            z /= len;
            w /= len;
        }
    }

    static Vector4 zero()
    {
        return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    static Vector4 one()
    {
        return Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    Vector4 normalized() const
    {
        float len = length();
        return (len > 0) ? Vector4(x / len, y / len, z / len, w / len) : Vector4(0, 0, 0, 0);
    }

    float dot(const Vector4 &v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }

    Vector4 operator+(const Vector4 &v) const { return Vector4(x + v.x, y + v.y, z + v.z, w + v.w); }
    Vector4 operator-(const Vector4 &v) const { return Vector4(x - v.x, y - v.y, z - v.z, w - v.w); }
    Vector4 operator*(float s) const { return Vector4(x * s, y * s, z * s, w * s); }
    Vector4 operator/(float s) const { return Vector4(x / s, y / s, z / s, w / s); }

    Vector4 &operator+=(const Vector4 &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return *this;
    }
    Vector4 &operator-=(const Vector4 &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
        return *this;
    }

    void print() const { std::cout << "(" << x << ", " << y << ", " << z << ", " << w << ")"; }
};

struct Mat4
{
    float m[16];

    Mat4()
    {
        loadIdentity();
    }

    void loadIdentity()
    {
        memset(m, 0, sizeof(m));
        m[0] = m[5] = m[10] = m[15] = 1.0f;
    }

    static Mat4 identity()
    {
        return Mat4();
    }

    // Translation matrix
    static Mat4 translate(float x, float y, float z)
    {
        Mat4 r;
        r.m[12] = x;
        r.m[13] = y;
        r.m[14] = z;
        return r;
    }

    static Mat4 ortho(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        Mat4 r;
        r.loadIdentity();
        r.m[0] = 2.0f / (right - left);
        r.m[5] = 2.0f / (top - bottom);
        r.m[10] = -2.0f / (zFar - zNear);
        r.m[12] = -(right + left) / (right - left);
        r.m[13] = -(top + bottom) / (top - bottom);
        r.m[14] = -(zFar + zNear) / (zFar - zNear);
        return r;
    }

    // Add this inside the Mat4 struct
    static Mat4 perspective(float fovDegrees, float aspect, float zNear, float zFar)
    {
        Mat4 r;
        r.loadIdentity();

        float fovRad = fovDegrees * 3.14159265359f / 180.0f;
        float f = 1.0f / tan(fovRad / 2.0f);

        r.m[0] = f / aspect;
        r.m[1] = 0.0f;
        r.m[2] = 0.0f;
        r.m[3] = 0.0f;

        r.m[4] = 0.0f;
        r.m[5] = f;
        r.m[6] = 0.0f;
        r.m[7] = 0.0f;

        r.m[8] = 0.0f;
        r.m[9] = 0.0f;
        r.m[10] = (zFar + zNear) / (zNear - zFar);
        r.m[11] = -1.0f;

        r.m[12] = 0.0f;
        r.m[13] = 0.0f;
        r.m[14] = (2.0f * zFar * zNear) / (zNear - zFar);
        r.m[15] = 0.0f;

        return r;
    }

    Mat4 operator*(const Mat4 &o) const
    {
        Mat4 r;
        for (int row = 0; row < 4; row++)
            for (int col = 0; col < 4; col++)
                r.m[col + row * 4] =
                    m[row * 4 + 0] * o.m[col + 0] +
                    m[row * 4 + 1] * o.m[col + 4] +
                    m[row * 4 + 2] * o.m[col + 8] +
                    m[row * 4 + 3] * o.m[col + 12];
        return r;
    }
};
#endif