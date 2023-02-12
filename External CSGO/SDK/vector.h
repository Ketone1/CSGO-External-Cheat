#pragma once

struct Vector {
    Vector() noexcept
        : x(), y(), z() {}

    Vector(float x, float y, float z) noexcept
        : x(x), y(y), z(z) {}

    Vector& operator+(const Vector& v) noexcept {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector& operator-(const Vector& v) noexcept {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    float x, y, z;
};

struct Vector2
{
    float x = { }, y = { };
};