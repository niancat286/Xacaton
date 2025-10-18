#pragma once

int testVectors(); // test all in 1 file they are simple

int testInputPolygone(); // 3 tests enough

int testWritePolygones(); // 3 tests

int testDeletePolygone(); // 1 test

int testShowPolygoneFile(); // 3 tests


//These all requires at least 3 tests each
extern int isPresentPolygone(FILE* fp, const Polygone* p);

int isEqualPolygone(const Polygone* p1, const Polygone* p2);

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

// Tests for new functions
extern int test_inputPolygone_file();
extern int test_inputPolygone_invalid();
extern int test_writePolygone();
extern int test_addPolygonesFromFile();
extern int test_addSinglePolygonFromConsole_success();
extern int test_addSinglePolygonFromConsole_duplicate();
extern int test_addSinglePolygonFromConsole_invalid();
extern int test_isEqualPolygone();
extern int test_perimeterPolygone();

// Test utilities
extern void createTestFile(const char* filename);
