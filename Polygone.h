#ifndef _POLYGONE_H_
#define _POLYGONE_H_

#include "stdio.h"
#include "stdlib.h"
#include "Types.h"
/*
#include "Predicat.h"


// Tasks
// Ввести багатокутник з консолі (якщо файлова змінна НУЛЛ) або з текстового файлу
extern int inputPolygone(Polygone* p, FILE* fp, );

//Вивести батокутник в консоль (якщо файлова змінна НУЛЛ) або в текстовий файл
extern void outputPolygone(const Polygone* p, FILE* fp);

extern int freePolygone(Polygone* p);

// if fp==NULL - print to console, else to binary file
extern int writePolygones(FILE* fp, Polygone* p, NTYPE n);

// delete k-th Polygone, 
// returns FALSE if not deleted
extern int deletePolygonesFile(FILE* fp, NTYPE k);

extern void showPolygoneFile(FILE* fp, NTYPE k);

extern void showPolygonesFile(FILE* fp);
*/
//чи однакові багатокутники?
extern int isEqualPoint(struct TPoint a, struct TPoint b);

extern int isEqualPolygone(const struct Polygone* p1,const struct Polygone* p2);

// чи є в нашому файлі
extern int isPresentPolygone(FILE* fp, const struct Polygone* p);

extern int isConvexPolygone(const Polygone* p);
/*
extern PTYPE perimeterPolygone(const Polygone* p);

extern PTYPE areaPolygone(const Polygone* p);

// Чи є багатокутник опуклим?

extern int maxPerimeterPolygone(FILE* fp, Polygone* p);

extern int minAreaPolygone(FILE* fp, Polygone* p);

extern NTYPE numberConvexPolygones(FILE* fp);

extern NTYPE conditionPolygones(FILE* fp, predicatPolygone Q, const char* fname);

extern NTYPE pointsPolygones(FILE* fp, TPoint p);

//Чи міститься точка Р всередині багатокутника  (на межі - теж всередині)
extern int pointsPolygoneInside(const Polygone* p, TPoint p0);


//Чи є багатокутник Трикутником?
extern int isTrianglePolygone(const Polygone* p);

//Чи є багатокутник Чотирикутником?
extern int isQuadrilateralPolygone(const Polygone* p);


// Трангуляція багатокутника
// Повертає кількість трикутників
extern NTYPE triangulatePolygone(const Polygone* p1, TTriangle* tr);

//Чи перетинаються два багатокутника
extern int isIntersectPolygones(const Polygone* p1, const Polygone* p2);
//Знайти багатокутник що утворений перетином двох багатокутників
extern Polygone intersect_polygone(const Polygone* p1, const Polygone* p2);

//Знайти два багатокутника, ща утворені перетином прямої та багатокутника
extern Polygone intersect_polygone_line_up(const Polygone* p1, const TLine* p2);
extern Polygone intersect_polygone_line_down(const Polygone* p1, const TLine* p2);

//Знайти найменший по площі опуклий багатокутник, шо містить даний багатокутник
extern Polygone convex_wrapper(const Polygone* p1);

//Знайти мінімальний по площі багатокутник, такий що кожна точка будь-якого ребра цього багаткутника знаходиться на відстані не більше 1 від даного багатокутника
extern Polygone wrapper_distance(const Polygone* p1, PTYPE dist);

*/
#endif // end of _TYPES_H_
