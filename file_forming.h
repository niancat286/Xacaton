#pragma once
#include <stdio.h>
#include "Types.h"

extern void outputPolygon(Polygone p);

extern void input();

extern Polygone* readPolygones(FILE* fp);

extern void output();