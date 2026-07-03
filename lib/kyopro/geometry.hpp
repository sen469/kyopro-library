#ifndef KYOPRO_GEOMETRY_HPP
#define KYOPRO_GEOMETRY_HPP

#include <algorithm>
#include <cassert>
#include <cmath>

namespace kyopro {
namespace geometry {

constexpr long double default_eps = 1e-10L;

inline int sign(long double x, long double eps = default_eps) {
    if (x > eps) return 1;
    if (x < -eps) return -1;
    return 0;
}

inline bool equal(long double a, long double b, long double eps = default_eps) {
    return sign(a - b, eps) == 0;
}

inline bool lt(long double a, long double b, long double eps = default_eps) {
    return sign(a - b, eps) < 0;
}

inline bool le(long double a, long double b, long double eps = default_eps) {
    return sign(a - b, eps) <= 0;
}

inline bool gt(long double a, long double b, long double eps = default_eps) {
    return sign(a - b, eps) > 0;
}

inline bool ge(long double a, long double b, long double eps = default_eps) {
    return sign(a - b, eps) >= 0;
}

struct point {
    long double x;
    long double y;

    point() : x(0), y(0) {}
    point(long double x, long double y) : x(x), y(y) {}

    point& operator+=(const point& p) {
        x += p.x;
        y += p.y;
        return *this;
    }

    point& operator-=(const point& p) {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    point& operator*=(long double k) {
        x *= k;
        y *= k;
        return *this;
    }

    point& operator/=(long double k) {
        x /= k;
        y /= k;
        return *this;
    }

    point operator+() const { return *this; }
    point operator-() const { return point(-x, -y); }

    friend point operator+(point a, const point& b) { return a += b; }
    friend point operator-(point a, const point& b) { return a -= b; }
    friend point operator*(point p, long double k) { return p *= k; }
    friend point operator*(long double k, point p) { return p *= k; }
    friend point operator/(point p, long double k) { return p /= k; }
};

inline long double dot(point a, point b) {
    return a.x * b.x + a.y * b.y;
}

inline long double cross(point a, point b) {
    return a.x * b.y - a.y * b.x;
}

inline long double norm2(point p) {
    return dot(p, p);
}

inline long double norm(point p) {
    return std::sqrt(norm2(p));
}

inline long double distance(point a, point b) {
    return norm(a - b);
}

inline bool same_point(point a, point b, long double eps = default_eps) {
    return equal(a.x, b.x, eps) && equal(a.y, b.y, eps);
}

inline point rotate90(point p) {
    return point(-p.y, p.x);
}

inline point rotate(point p, long double rad) {
    long double c = std::cos(rad);
    long double s = std::sin(rad);
    return point(c * p.x - s * p.y, s * p.x + c * p.y);
}

struct line {
    point a;
    point b;

    line() : a(0, 0), b(1, 0) {}
    line(point a, point b) : a(a), b(b) {}
};

inline point direction(line l) {
    return l.b - l.a;
}

inline bool parallel(line l1, line l2, long double eps = default_eps) {
    return sign(cross(direction(l1), direction(l2)), eps) == 0;
}

inline bool orthogonal(line l1, line l2, long double eps = default_eps) {
    return sign(dot(direction(l1), direction(l2)), eps) == 0;
}

inline bool on_line(line l, point p, long double eps = default_eps) {
    return sign(cross(direction(l), p - l.a), eps) == 0;
}

inline bool same_line(line l1, line l2, long double eps = default_eps) {
    return parallel(l1, l2, eps) && on_line(l1, l2.a, eps);
}

inline int line_line_relation(line l1, line l2, long double eps = default_eps) {
    if (!parallel(l1, l2, eps)) return 2;
    return same_line(l1, l2, eps) ? 1 : 0;
}

inline bool on_segment(point a, point b, point p, long double eps = default_eps) {
    return on_line(line(a, b), p, eps) &&
           le(dot(p - a, p - b), 0, eps);
}

inline long double distance_line_point(line l, point p) {
    long double len = norm(direction(l));
    assert(len > 0);
    return std::abs(cross(direction(l), p - l.a)) / len;
}

inline long double distance_segment_point(point a, point b, point p) {
    if (same_point(a, b)) return distance(a, p);
    if (le(dot(b - a, p - a), 0)) return distance(a, p);
    if (le(dot(a - b, p - b), 0)) return distance(b, p);
    return distance_line_point(line(a, b), p);
}

inline bool intersect_segments(point a, point b, point c, point d,
                        long double eps = default_eps) {
    long double c1 = cross(b - a, c - a);
    long double c2 = cross(b - a, d - a);
    long double c3 = cross(d - c, a - c);
    long double c4 = cross(d - c, b - c);
    if (sign(c1, eps) == 0 && sign(c2, eps) == 0) {
        return le(std::max(std::min(a.x, b.x), std::min(c.x, d.x)),
                  std::min(std::max(a.x, b.x), std::max(c.x, d.x)), eps) &&
               le(std::max(std::min(a.y, b.y), std::min(c.y, d.y)),
                  std::min(std::max(a.y, b.y), std::max(c.y, d.y)), eps);
    }
    return le(c1 * c2, 0, eps) && le(c3 * c4, 0, eps);
}

struct circle {
    point center;
    long double r;

    circle() : center(0, 0), r(0) {}
    circle(point center, long double r) : center(center), r(r) {}
};

inline bool same_circle(circle c1, circle c2, long double eps = default_eps) {
    return same_point(c1.center, c2.center, eps) && equal(c1.r, c2.r, eps);
}

inline bool on_circle(circle c, point p, long double eps = default_eps) {
    return equal(distance(c.center, p), c.r, eps);
}

inline int circle_position(circle c, point p, long double eps = default_eps) {
    return sign(distance(c.center, p) - c.r, eps);
}

inline bool in_circle(circle c, point p, bool strict = false,
               long double eps = default_eps) {
    int s = circle_position(c, p, eps);
    return strict ? s < 0 : s <= 0;
}

inline int line_circle_relation(line l, circle c, long double eps = default_eps) {
    int s = sign(distance_line_point(l, c.center) - c.r, eps);
    if (s > 0) return 0;
    if (s == 0) return 1;
    return 2;
}

inline int segment_circle_relation(point a, point b, circle c,
                            long double eps = default_eps) {
    int s = sign(distance_segment_point(a, b, c.center) - c.r, eps);
    if (s > 0) return 0;
    if (s == 0) return 1;
    return 2;
}

inline int circle_circle_relation(circle c1, circle c2,
                           long double eps = default_eps) {
    if (same_circle(c1, c2, eps)) return 5;
    long double d = distance(c1.center, c2.center);
    long double rsum = c1.r + c2.r;
    long double rdiff = std::abs(c1.r - c2.r);
    if (gt(d, rsum, eps)) return 0;
    if (equal(d, rsum, eps)) return 1;
    if (gt(d, rdiff, eps)) return 2;
    if (equal(d, rdiff, eps)) return 3;
    return 4;
}

}  // namespace geometry
}  // namespace kyopro

#endif  // KYOPRO_GEOMETRY_HPP
