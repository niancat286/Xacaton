
TVECT setVector(TPoint x, TPoint y){
    TVECT v = {y.x - x.x, y.y - x.y, 0};
    return v;
}

PTYPE lengthVector(TVECT v) {
    PTYPE res = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return res;
}

TVECT addVector(TVECT v1, TVECT v2) {
    TVECT res = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
    return res;
}

TVECT multVector(PTYPE a, TVECT v) {
    TVECT res = {a * v.x, a * v.y, a * v.z};
    return res;
}

TVECT subVector(TVECT v1, TVECT v2) {
    return addVector(v1, multVector(-1, v2));
}

PTYPE scalarMultVector(TVECT v1, TVECT v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

TVECT vectorMultVector(TVECT v1, TVECT v2) {
    TVECT res;
    res.x = v1.y * v2.z - v1.z * v2.y;
    res.y = v1.z * v2.x - v1.x * v2.z;
    res.z = v1.x * v2.y - v1.y * v2.x;
    return res;
}
