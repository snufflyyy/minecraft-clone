#include "vector3f.h"

Vector3f vector3f_add(Vector3f vector1, Vector3f vector2) {
    Vector3f vector = {0};

    vector.x = vector1.x + vector2.x;
    vector.y = vector1.y + vector2.y;
    vector.z = vector1.z + vector2.z;

    return vector;
}