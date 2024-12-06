#pragma once

class IntVector2 {

public:
    int x, y;

    IntVector2(int x, int y) : x(x), y(y) {}

    IntVector2 operator+(const IntVector2& other) const
    {
        return IntVector2(x + other.x, y + other.y);
    }

    IntVector2 operator-(const IntVector2& other) const
    {
        return IntVector2(x - other.x, y - other.y);
    }

    IntVector2 operator+=(const IntVector2& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    IntVector2 operator-=(const IntVector2& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    IntVector2 operator*(int scalar) const
    {
        return IntVector2(x * scalar, y * scalar);
    }

    bool operator==(const IntVector2& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const IntVector2& other) const
    {
        return x != other.x || y != other.y;
    }
};

