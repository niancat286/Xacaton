#ifndef _VECTOR_OPERATIONS_H_
#define _VECTOR_OPERATIONS_H_

#ifdef __cpluslus
extern "C"
{
#endif

#include "Types.h"

    typedef struct TVECT
    {
        PTYPE x;
        PTYPE y;
        PTYPE z;
    } TVECT;

    extern TVECT setVector(TPoint x, TPoint y);

    extern PTYPE lengthVector(TVECT v);

    extern TVECT addVector(TVECT v1, TVECT v2);

    extern TVECT subVector(TVECT v1, TVECT v2);

    extern PTYPE scalarMultVector(TVECT v1, TVECT v2);

    extern TVECT vectorMultVector(TVECT v1, TVECT v2);

    extern TVECT multVector(PTYPE a, TVECT x);

    extern int isEqual(PTYPE a, PTYPE b);
    extern int isEqual2(TVECT a, TVECT b);
#ifdef __cplusplus
}
#endif

#endif