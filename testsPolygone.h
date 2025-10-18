#pragma once
#include "Polygone.h"

#ifndef _TESTS_POLYGONE_H_
#define _TESTS_POLYGONE_H_

/**
 * @brief Runs all defined tests and prints a summary.
 */
void run_all_tests();

#endif // _TESTS_POLYGONE_H_
/*
int testVectors(); // test all in 1 file they are simple

int testInputPolygone(); // 3 tests enough

int testWritePolygones(); // 3 tests

int testDeletePolygone(); // 1 test

int testShowPolygoneFile(); // 3 tests
*/

// These all requires at least 3 tests each
// extern int test_isPresentPolygone();

// extern int test_isEqualPolygone();
/*
extern PTYPE perimeterPolygone(const Polygone* p);

extern PTYPE areaPolygone(const Polygone* p);

extern int isConvexPolygone(const Polygone* p);

extern int maxPerimeterPolygone(FILE* fp, Polygone* p);

extern int minAreaPolygone(FILE* fp, Polygone* p);

extern NTYPE numberConvexPolygones(FILE* fp);

extern NTYPE conditionPolygones(FILE* fp, predicatPolygone Q, const char* fname);

extern NTYPE pointsPolygones(FILE* fp, TPoint p);
extern NTYPE linePolygones(FILE* fp, TLine L);
extern NTYPE rayPolygones(FILE* fp, TRay R);
....


*/