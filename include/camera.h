#pragma once

#include "math/vector3f.h"
#include "math/matrix4f.h"
#include "window.h"

typedef enum Camera_Direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
} Camera_Direction;

typedef struct Fly_Camera {
    float sensitivity;
    float speed;
    float fov;

    Vector3f position;
    Vector3f front;
    Vector3f up;

    float yaw;
    float pitch;

    float last_mouse_x;
    float last_mouse_y;

    Matrix4f view;
    Matrix4f projection;
} Fly_Camera;

Fly_Camera fly_camera_create(int window_width, int window_height);
void fly_camera_keyboard_mouse_movement(Fly_Camera* camera, Window* window, float delta_time);
void fly_camera_update(Fly_Camera* camera);
void fly_camera_move(Fly_Camera* camera, Camera_Direction direction, float delta_time);
void fly_camera_change_angle(Fly_Camera* camera, float new_mouse_x, float new_mouse_y);