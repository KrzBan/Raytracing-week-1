#pragma once

#include <cmath>
#include <iostream>

class vec3 {
public:
    double points[3];
public:
    vec3() : points{ 0,0,0 } {}
    vec3(double e0, double e1, double e2) : points{ e0, e1, e2 } {}

    double x() const { return points[0]; }
    double y() const { return points[1]; }
    double z() const { return points[2]; }

    vec3 operator-() const { return vec3(-points[0], -points[1], -points[2]); }
    double operator[](int i) const { return points[i]; }
    double& operator[](int i) { return points[i]; }

    vec3& operator+=(const vec3& v) {
        points[0] += v.points[0];
        points[1] += v.points[1];
        points[2] += v.points[2];
        return *this;
    }

    vec3& operator*=(const double t) {
        points[0] *= t;
        points[1] *= t;
        points[2] *= t;
        return *this;
    }

    vec3& operator/=(const double t) {
        return *this *= 1 / t;
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    double length_squared() const {
        return points[0] * points[0] + points[1] * points[1] + points[2] * points[2];
    }

    inline static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    inline static vec3 random(double min, double max) {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }
};

// Type aliases for vec3
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color

// vec3 Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.points[0] << ' ' << v.points[1] << ' ' << v.points[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.points[0] + v.points[0], u.points[1] + v.points[1], u.points[2] + v.points[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.points[0] - v.points[0], u.points[1] - v.points[1], u.points[2] - v.points[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.points[0] * v.points[0], u.points[1] * v.points[1], u.points[2] * v.points[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t * v.points[0], t * v.points[1], t * v.points[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1 / t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
    return u.points[0] * v.points[0]
        + u.points[1] * v.points[1]
        + u.points[2] * v.points[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.points[1] * v.points[2] - u.points[2] * v.points[1],
        u.points[2] * v.points[0] - u.points[0] * v.points[2],
        u.points[0] * v.points[1] - u.points[1] * v.points[0]);
}

inline vec3 normalized(vec3 v) {
    return v / v.length();
}

vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1, 1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

vec3 random_unit_vector() {
    return normalized(random_in_unit_sphere());
}