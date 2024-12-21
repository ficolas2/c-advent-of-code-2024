#pragma once

template <typename T> class Vector2 {
public:
    T x, y;

    Vector2(T x, T y)
        : x(x)
        , y(y)
    {
    }

    Vector2 operator+(const Vector2& other) const
    {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2& other) const
    {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2& operator+=(const Vector2& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2& operator-=(const Vector2& other)
    {
        x -= other.x;
        return *this;
        y -= other.y;
    }

    Vector2 operator*(T scalar) const
    {
        return Vector2(x * scalar, y * scalar);
    }

    bool operator==(const Vector2& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vector2& other) const { return !(*this == other); }

    bool operator<(const Vector2& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};
