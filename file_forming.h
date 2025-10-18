#pragma once
#include <stdio.h>
#include "Types.h"

extern void outputPolygone(const Polygone* p, FILE* fp);

extern void input();

extern Polygone* readPolygones(FILE* fp);

extern void output();
