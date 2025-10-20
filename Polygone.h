#ifndef _POLYGONE_H_
#define _POLYGONE_H_

#include <stdio.h>
#include "Types.h"

// task а)
void add_polygone_from_console(const char *filename);

// task б)
void append_polygons_from_file(const char *dest_filename, const char *src_filename);

// task в)
void display_all_polygons(const char *filename);

// task г)
void display_polygone_by_index(const char *filename, NTYPE index);

// task д)
int delete_polygone_by_index(const char *filename, NTYPE index);

// task е)
int is_present_in_file(const char *filename, const Polygone *p);

// task є)
int find_max_perimeter_polygone(const char *filename, Polygone *result);

// task ж)
int find_min_area_polygone(const char *filename, Polygone *result);

// task з)
NTYPE count_convex_polygons(const char *filename);

//task і)
NTYPE count_polygons_containing_point(const char *filename, TPoint p);

// task й)
void filter_polygons(const char *file_a, const char *file_b, int (*predicate)(const Polygone *));

PTYPE perimeter_polygone(const Polygone *p);
PTYPE area_polygone(const Polygone *p);
int is_convex(const Polygone *p);
int is_point_inside(const Polygone *p, TPoint point);
int isEqualPoint(TPoint a, TPoint b);
int isEqualPolygone(const Polygone *p1, const Polygone *p2);
void free_polygone(Polygone *p);

#endif // _POLYGONE_H_