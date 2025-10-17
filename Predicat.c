#include "Predicat.h"
#include <stdio.h>
#include <assert.h>

int isEqual(PTYPE a, PTYPE b){
    return fabs(a-b) < __DBL_EPSILON__;
}

int isEqual2(TVECT a, TVECT b){
    return isEqual(a.x,b.x) && isEqual(a.y,b.y) && isEqual(a.z, b.z);
}
