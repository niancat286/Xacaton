#include "Predicat.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <float.h>

int isEqual(PTYPE a, PTYPE b){
    PTYPE diff = fabsf(a - b);
    PTYPE scale = fmaxf(1.0f, fabsf(a) + fabsf(b));
    return diff <= (FLT_EPSILON * scale * 10.0f);
}

int isEqual2(TVECT a, TVECT b){
    return isEqual(a.x,b.x) && isEqual(a.y,b.y) && isEqual(a.z, b.z);
}
