#include <bits/stdc++.h>

#include "kyopro/geometry.hpp"

using namespace std;
using namespace kyopro::geometry;

int main() {
    {
        assert(sign(1e-12L) == 0);
        assert(sign(1e-7L) == 1);
        assert(sign(-1e-7L) == -1);
        assert(equal(1.0L, 1.0L + 1e-12L));
        assert(lt(1.0L, 1.1L));
        assert(le(1.0L, 1.0L + 1e-12L));
        assert(gt(1.1L, 1.0L));
        assert(ge(1.0L, 1.0L - 1e-12L));
    }

    {
        point a(0, 0), b(2, 2), c(1, 1), d(1, 1 + 1e-12L);
        assert(same_point(c, d));
        assert(equal(dot(point(1, 2), point(3, 4)), 11));
        assert(equal(cross(point(1, 0), point(0, 1)), 1));
        assert(equal(norm2(point(3, 4)), 25));
        assert(equal(distance(a, point(3, 4)), 5));

        line l(a, b);
        assert(on_line(l, c));
        assert(on_line(l, d));
        assert(on_segment(a, b, c));
        assert(!on_segment(a, b, point(3, 3)));
        assert(parallel(l, line(point(10, 10), point(12, 12))));
        assert(same_line(l, line(point(10, 10), point(12, 12))));
        assert(!same_line(l, line(point(10, 11), point(12, 13))));
        assert(line_line_relation(l, line(point(10, 10), point(12, 12))) == 1);
        assert(line_line_relation(l, line(point(10, 11), point(12, 13))) == 0);
        assert(line_line_relation(l, line(point(0, 1), point(1, 0))) == 2);
        assert(orthogonal(line(point(0, 0), point(1, 0)),
                          line(point(0, 0), point(0, 1))));
        assert(equal(distance_line_point(line(point(0, 0), point(2, 0)),
                                     point(1, 3)),
                     3));
        assert(equal(distance_segment_point(point(0, 0), point(2, 0),
                                            point(3, 4)),
                     sqrt((long double)17)));
    }

    {
        assert(intersect_segments(point(0, 0), point(2, 2),
                                  point(0, 2), point(2, 0)));
        assert(intersect_segments(point(0, 0), point(2, 0),
                                  point(1, 0), point(3, 0)));
        assert(!intersect_segments(point(0, 0), point(1, 0),
                                   point(2, 0), point(3, 0)));
    }

    {
        circle c(point(0, 0), 5);
        assert(same_circle(c, circle(point(1e-12L, -1e-12L), 5 + 1e-12L)));
        assert(on_circle(c, point(3, 4)));
        assert(circle_position(c, point(1, 1)) == -1);
        assert(circle_position(c, point(3, 4)) == 0);
        assert(circle_position(c, point(6, 0)) == 1);
        assert(in_circle(c, point(3, 4)));
        assert(!in_circle(c, point(3, 4), true));
        assert(in_circle(c, point(1, 1), true));

        assert(line_circle_relation(line(point(-10, 5), point(10, 5)), c) == 1);
        assert(line_circle_relation(line(point(-10, 6), point(10, 6)), c) == 0);
        assert(line_circle_relation(line(point(-10, 0), point(10, 0)), c) == 2);
        assert(segment_circle_relation(point(6, 0), point(7, 0), c) == 0);
        assert(segment_circle_relation(point(5, 0), point(7, 0), c) == 1);
        assert(segment_circle_relation(point(-10, 0), point(10, 0), c) == 2);
    }

    {
        circle a(point(0, 0), 5);
        assert(circle_circle_relation(a, circle(point(11, 0), 5)) == 0);
        assert(circle_circle_relation(a, circle(point(10, 0), 5)) == 1);
        assert(circle_circle_relation(a, circle(point(7, 0), 5)) == 2);
        assert(circle_circle_relation(a, circle(point(2, 0), 3)) == 3);
        assert(circle_circle_relation(a, circle(point(1, 0), 1)) == 4);
        assert(circle_circle_relation(a, circle(point(0, 0), 5)) == 5);
    }

    return 0;
}
