#ifndef _TYPES_H_
#define _TYPES_H_

#include "stdio.h"
#include "stdlib.h"

#include "Predicat.h"

// Input polygon from console (if file pointer is NULL) or from text file
// Returns TRUE on success, FALSE on error
extern int inputPolygone(Polygone* p, FILE* fp);

// Output polygon to console (if file pointer is NULL) or to text file
extern void outputPolygone(const Polygone* p, FILE* fp);

// Free memory allocated for polygon
// Returns 0 on success
extern int freePolygone(Polygone* p);

// Write polygons to file (binary format)
// if fp==NULL - print to console, else to binary file
// Returns number of polygons successfully written
extern int writePolygones(FILE* fp, Polygone* p, NTYPE n);

// Write single polygon to file (text format)
// Returns TRUE on success, FALSE on error
extern int writePolygone(FILE* fp, Polygone* p);

// Add all polygons from source file to destination file
// Returns number of polygons successfully added
extern int addPolygonesFromFile(const char* source_filename, const char* dest_filename);

// Read polygons from file
// Returns array of polygons, NULL on error
extern Polygone* readPolygones(FILE* fp);

// Delete k-th polygon from file
// Returns FALSE if not deleted
extern int deletePolygonesFile(FILE* fp, NTYPE k);

// Display specific polygon by index
extern void showPolygoneFile(FILE* fp, NTYPE k);

// Display all polygons from file
extern void showPolygonesFile(FILE* fp);


// Check if two polygons are equal
extern int isEqualPolygone(const Polygone* p1, const Polygone* p2);

// Validate polygon structure and data
// Returns TRUE if polygon is valid, FALSE otherwise
extern int isValidPolygon(const Polygone* p);

// Check if polygon exists in file
extern int isPresentPolygone(FILE* fp, const Polygone* p);


// Calculate polygon perimeter
extern PTYPE perimeterPolygone(const Polygone* p);

// Calculate polygon area
extern PTYPE areaPolygone(const Polygone* p);


// Check if polygon is convex
extern int isConvexPolygone(const Polygone* p);

// Check if polygon is a triangle
extern int isTrianglePolygone(const Polygone* p);

// Check if polygon is a quadrilateral
extern int isQuadrilateralPolygone(const Polygone* p);


// Find polygon with maximum perimeter in file
// Returns TRUE if found, FALSE otherwise
extern int maxPerimeterPolygone(FILE* fp, Polygone* p);

// Find polygon with minimum area in file
// Returns TRUE if found, FALSE otherwise
extern int minAreaPolygone(FILE* fp, Polygone* p);

// Count convex polygons in file
extern NTYPE numberConvexPolygones(FILE* fp);

// Filter polygons by condition and write to file
// Returns number of polygons matching condition
extern NTYPE conditionPolygones(FILE* fp, predicatPolygone Q, const char* fname);

// Count polygons containing given point
extern NTYPE pointsPolygones(FILE* fp, TPoint p);

// Check if point P is inside polygon (on boundary is considered inside)
extern int pointsPolygoneInside(const Polygone* p, TPoint p0);

// Polygon triangulation
// Returns number of triangles
extern NTYPE triangulatePolygone(const Polygone* p1, TTriangle* tr);

// Check if two polygons intersect
extern int isIntersectPolygones(const Polygone* p1, const Polygone* p2);

// Find polygon formed by intersection of two polygons
extern Polygone intersect_polygone(const Polygone* p1, const Polygone* p2);

// Find polygons formed by intersection of line and polygon
extern Polygone intersect_polygone_line_up(const Polygone* p1, const TLine* p2);
extern Polygone intersect_polygone_line_down(const Polygone* p1, const TLine* p2);

// Find smallest convex polygon by area that contains given polygon
extern Polygone convex_wrapper(const Polygone* p1);

// Find minimal area polygon such that every point of any edge of this polygon
// is at distance no more than 'dist' from given polygon
extern Polygone wrapper_distance(const Polygone* p1, PTYPE dist);

// Text file reading function
extern int readPolygoneFromText(FILE* fp, Polygone* p);

// Polygon validation
extern int isValidPolygon(const Polygone* p);

// File operations for point b)
extern int addPolygonesFromFile(const char* source_filename, const char* dest_filename);

#endif // end of _TYPES_H_