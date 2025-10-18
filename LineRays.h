#ifndef _LINERAYS_H_
#define _LINERAYS_H_

#include "Types.h"

// Відстань від точки до прямої
PTYPE length_point_line(TPoint p, TLine line);

// Відстань від точки до відрізку
PTYPE length_point_segment(TPoint p, TSegment line);

// Відстань від точки до луча
PTYPE length_point_ray(TPoint p, TRay ray);

// Найменша відстань від між двома відрізками
PTYPE length_segments(TSegment line1, TSegment line2);

// Точка перетину між лучами (якщо ні - повертає НУЛЛ)
TPoint intersect_rays(TRay r1, TRay r2);

// Точка перетину прямих (якщо ні - повертає НУЛЛ)
TPoint intersect_lines(TLine r1, TLine r2);

// Точка перетину відрізків (якщо немає - повертає НУЛЛ)
TPoint intersect_segments(TSegment line1, TSegment line2);
#endif