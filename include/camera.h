#pragma once

#include "math/vector3f.h"
#include "math/matrix4f.h"

typedef enum Camera_Direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
} Camera_Direction;

typedef struct Fly_Camera {
    float speed;
    float fov;

    Vector3f position;
    Vector3f front;
    Vector3f up;

    float last_mouse_x;
    float last_mouse_y;

    Matrix4f view;
    Matrix4f projection;
} Fly_Camera;

Fly_Camera createFlyCamera(int window_width, int window_height);
void updateFlyCamera(Fly_Camera* camera);
void moveFlyCameraPosition(Fly_Camera* camera, Camera_Direction direction);
void moveFlyCameraAngle(Fly_Camera* camera, double new_x, double new_y);