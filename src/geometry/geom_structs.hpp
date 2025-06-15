#ifndef GEOM_STRUCTS_HPP_INCLUDED
#define GEOM_STRUCTS_HPP_INCLUDED

struct Point2d
{
    float x;
    float y;

    Point2d() = default;
    Point2d(float x, float y) : x(x), y(y) {}
    Point2d(float *arr) : x(arr[0]), y(arr[1]) {}
};

struct Point3d
{
    float x;
    float y;
    float z;

    Point3d() = default;
    Point3d(float x, float y, float z) : x(x), y(y), z(z) {}
    Point3d(float *arr) : x(arr[0]), y(arr[1]), z(arr[2]) {}
};

// region R = {(x, y, z) | |c.x-x|<=rx, |c.y-y|<=ry, |c.z-z|<=rz }
struct AABB3d
{
    Point3d c;
    float r[3];
};

// Bounding volume with inexpensive intersection test.
// Region R = { (x, y, z) | (x-c.x)^2 + (y-c.y)^2 + (z-c.z)^2 <= r^2 }
struct Sphere
{
    Point3d c;
    float r;
};

class Matrix33
{
public:
    // returns the pointer of the requested row.
    float *operator[](int row)
    {
        return &m[row * 3];
    }

    const float *operator[](int row) const
    {
        return &m[row * 3];
    }

    Matrix33 transpose()
    {
        Matrix33 trg;
        trg.m[0 * 3 + 0] = m[0 * 3 + 0];
        trg.m[1 * 3 + 0] = m[0 * 3 + 1];
        trg.m[2 * 3 + 0] = m[0 * 3 + 2];
        trg.m[0 * 3 + 1] = m[1 * 3 + 0];
        trg.m[1 * 3 + 1] = m[1 * 3 + 1];
        trg.m[2 * 3 + 1] = m[1 * 3 + 2];
        trg.m[0 * 3 + 2] = m[2 * 3 + 0];
        trg.m[1 * 3 + 2] = m[2 * 3 + 1];
        trg.m[2 * 3 + 2] = m[2 * 3 + 2];
        return trg;
    }

    bool isorthonormal();
    bool isdiagonal();

    friend Matrix33 operator*(const Matrix33 &a, const Matrix33 &b);
private:
    float m[9] = {0.};
};


#endif