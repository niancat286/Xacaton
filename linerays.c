#ifndef _LINERAYS_H_
#define _LINERAYS_H_

#include "Types.h"

PTYPE length_point_line(TPoint p, TLine line)
{
    // TODO: Реалізувати логіку обчислення відстані
    (void)p;
    (void)line;
    return 0.0f;
}

PTYPE length_point_segment(TPoint p, TSegment line)
{
    // TODO: тут заглушки, тому реалізуйте друзі
    (void)p;
    (void)line;
    return 0.0f;
}

PTYPE length_point_ray(TPoint p, TRay ray)
{
    // TODO: тут заглушки, тому реалізуйте друзі
    (void)p;
    (void)ray;
    return 0.0f;
}

PTYPE length_segments(TSegment line1, TSegment line2)
{
    // TODO: тут заглушки, тому реалізуйте друзі
    (void)line1;
    (void)line2;
    return 0.0f;
}

TPoint intersect_rays(TRay r1, TRay r2)
{
    // TODO: тут заглушки, тому реалізуйте друзі
    (void)r1;
    (void)r2;
    TPoint no_intersection = {0, 0};
    return no_intersection;
}

TPoint intersect_lines(TLine l1, TLine l2)
{
    // TODO: тут заглушки, тому реалізуйте друзі
    (void)l1;
    (void)l2;
    TPoint no_intersection = {0, 0};
    return no_intersection;
}

TPoint intersect_segments(TSegment line1, TSegment line2)
{
    // TODO: тут заглушки, тому реалізуйте друзі
    (void)line1;
    (void)line2;
    TPoint no_intersection = {0, 0};
    return no_intersection;
}

#endif