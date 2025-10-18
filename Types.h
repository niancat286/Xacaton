#ifndef _TYPES_H_
#define _TYPES_H_

// #ifdef __cpluslus
// extern "C"{
// #endif

typedef unsigned int NTYPE;
typedef float PTYPE;

#define TRUE 1
#define FALSE 0

typedef struct TPoint
{
   PTYPE x;
   PTYPE y;
} TPoint;

typedef struct Polygone
{
   NTYPE n; // number of vertices
   TPoint *vertice;
} Polygone;

typedef struct Line
{
   PTYPE a;
   PTYPE b;
   PTYPE c;
} TLine;

typedef struct Ray
{
   TPoint start_point;
   TPoint direction_point;
} TRay;

typedef struct Segment
{
   TPoint pointA;
   TPoint pointB;
} TSegment;

typedef struct Triangle
{
   TPoint pointA;
   TPoint pointB;
   TPoint pointC;
} TTriangle;

// #ifdef __cpluslus
// }
// #endif /// end of __cpluslus

#endif // end of _TYPES_H_