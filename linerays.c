#include "LineRays.h"
#include <math.h>
#include <float.h>


static TPoint invalid_point() {
    TPoint p = {NAN, NAN};
    return p;
}

static int is_valid(TPoint p) {
    return !isnan(p.x) && !isnan(p.y);
}

static TLine line_from_points(TPoint p1, TPoint p2) {
    TLine line;
    line.a = p2.y - p1.y;
    line.b = p1.x - p2.x;
    line.c = -line.a * p1.x - line.b * p1.y;
    return line;
}


PTYPE length_point_line(TPoint p, TLine line) {
    PTYPE denominator = sqrtf(line.a * line.a + line.b * line.b);
    if (fabsf(denominator) < FLT_EPSILON) {
        return 0.0f;
    }
    return fabsf(line.a * p.x + line.b * p.y + line.c) / denominator;
}


PTYPE length_point_segment(TPoint p, TSegment seg) {
    TPoint A = seg.pointA;
    TPoint B = seg.pointB;

    TPoint AB = {B.x - A.x, B.y - A.y};
    TPoint AP = {p.x - A.x, p.y - A.y};

    PTYPE ab_len_sq = AB.x * AB.x + AB.y * AB.y;
    if (fabsf(ab_len_sq) < FLT_EPSILON) {
        return sqrtf(AP.x * AP.x + AP.y * AP.y);
    }

    PTYPE t = (AP.x * AB.x + AP.y * AB.y) / ab_len_sq;

    if (t < 0.0f) {
        return sqrtf(powf(p.x - A.x, 2) + powf(p.y - A.y, 2));
    }
    if (t > 1.0f) {
        return sqrtf(powf(p.x - B.x, 2) + powf(p.y - B.y, 2));
    }

    return length_point_line(p, line_from_points(A, B));
}


PTYPE length_point_ray(TPoint p, TRay ray) {
    TPoint O = ray.start_point;
    TPoint D_pt = ray.direction_point;
    TPoint D = {D_pt.x - O.x, D_pt.y - O.y};
    TPoint OP = {p.x - O.x, p.y - O.y};

    PTYPE d_len_sq = D.x * D.x + D.y * D.y;
    if (fabsf(d_len_sq) < FLT_EPSILON) {
        return sqrtf(OP.x * OP.x + OP.y * OP.y);
    }

    PTYPE t = (OP.x * D.x + OP.y * D.y) / d_len_sq;

    if (t < 0.0f) {
        return sqrtf(powf(p.x - O.x, 2) + powf(p.y - O.y, 2));
    }

    return length_point_line(p, line_from_points(O, D_pt));
}

TPoint intersect_lines(TLine l1, TLine l2) {
    PTYPE det = l1.a * l2.b - l2.a * l1.b;

    if (fabsf(det) < FLT_EPSILON) {
        return invalid_point();
    }

    TPoint p;
    p.x = (l2.b * -l1.c - l1.b * -l2.c) / det;
    p.y = (l1.a * -l2.c - l2.a * -l1.c) / det;
    return p;
}


TPoint intersect_rays(TRay r1, TRay r2) {
    TPoint o1 = r1.start_point;
    TPoint d1 = {r1.direction_point.x - o1.x, r1.direction_point.y - o1.y};
    TPoint o2 = r2.start_point;
    TPoint d2 = {r2.direction_point.x - o2.x, r2.direction_point.y - o2.y};

    PTYPE det = d1.x * d2.y - d1.y * d2.x;

    if (fabsf(det) < FLT_EPSILON) {
        return invalid_point();
    }

    PTYPE t = ((o2.x - o1.x) * d2.y - (o2.y - o1.y) * d2.x) / det;
    PTYPE u = ((o2.x - o1.x) * d1.y - (o2.y - o1.y) * d1.x) / det;

    if (t >= 0.0f && u >= 0.0f) {
        TPoint p = {o1.x + t * d1.x, o1.y + t * d1.y};
        return p;
    }

    return invalid_point();
}

TPoint intersect_segments(TSegment seg1, TSegment seg2) {
    TLine l1 = line_from_points(seg1.pointA, seg1.pointB);
    TLine l2 = line_from_points(seg2.pointA, seg2.pointB);
    TPoint p = intersect_lines(l1, l2);

    if (!is_valid(p)) {
        return invalid_point();
    }

    int on_segment1 =
        (p.x >= fminf(seg1.pointA.x, seg1.pointB.x) - FLT_EPSILON) &&
        (p.x <= fmaxf(seg1.pointA.x, seg1.pointB.x) + FLT_EPSILON) &&
        (p.y >= fminf(seg1.pointA.y, seg1.pointB.y) - FLT_EPSILON) &&
        (p.y <= fmaxf(seg1.pointA.y, seg1.pointB.y) + FLT_EPSILON);

    int on_segment2 =
        (p.x >= fminf(seg2.pointA.x, seg2.pointB.x) - FLT_EPSILON) &&
        (p.x <= fmaxf(seg2.pointA.x, seg2.pointB.x) + FLT_EPSILON) &&
        (p.y >= fminf(seg2.pointA.y, seg2.pointB.y) - FLT_EPSILON) &&
        (p.y <= fmaxf(seg2.pointA.y, seg2.pointB.y) + FLT_EPSILON);

    if (on_segment1 && on_segment2) {
        return p;
    }

    return invalid_point();
}


PTYPE length_segments(TSegment line1, TSegment line2) {
    if (is_valid(intersect_segments(line1, line2))) {
        return 0.0f;
    }

    PTYPE d1 = length_point_segment(line1.pointA, line2);
    PTYPE d2 = length_point_segment(line1.pointB, line2);
    PTYPE d3 = length_point_segment(line2.pointA, line1);
    PTYPE d4 = length_point_segment(line2.pointB, line1);

    return fminf(fminf(d1, d2), fminf(d3, d4));
}