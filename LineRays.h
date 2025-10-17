


// Відстань від точки до прямої
PTYPE length_point_line(TPoint p, TLine line);

// Відстань від точки до відрізку
PTYPE length_point_segment(TPoint p, TSegement line);

// Відстань від точки до луча
PTYPE length_point_ray(TPoint p, TRay ray);


// Найменша відстань від між двома відрізками
PTYPE length_segments(TSegement line1, TSegement line2);

// Точка перетину між лучами (якщо ні - повертає НУЛЛ)
Tpoint intersect_rays(TRay r1, TRay r2);

// Точка перетину прямих (якщо ні - повертає НУЛЛ)
Tpoint intersect_lines(TLine r1, TLine r2);

// Точка перетину відрізків (якщо немає - повертає НУЛЛ)
Tpoint intersect_segments(TSegement line1, TSegement line2);


