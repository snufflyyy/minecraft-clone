#include "camera.h"

#include "math/matrix4f.h"

#include <math.h>

Fly_Camera fly_camera_create(int windowWidth, int windowHeight) {
    Fly_Camera camera = {0};

    camera.sensitivity = 0.1f;
    camera.speed = 0.1f;
    camera.fov = 60.0f;

    camera.position = (Vector3f) {0.0f, 0.0f, 0.0f};
    camera.front = (Vector3f) {0.0f, 0.0f, -1.0f};
    camera.up = (Vector3f) {0.0f, 1.0f, 0.0f};

    camera.yaw = -90.0f;
    camera.pitch = 0.0f;

    camera.last_mouse_x = (float) windowWidth / 2;
    camera.last_mouse_y = (float) windowHeight / 2;

    camera.view = matrix4f_identity();
    camera.projection = matrix4f_perspective(camera.fov, (float) windowWidth / (float) windowHeight, 0.01f, 1000.0f);

    return camera;
}

void fly_camera_update(Fly_Camera* camera) {
    Vector3f center = vector3f_add(&camera->position, &camera->front);
    camera->view = matrix4f_look_at(camera->position, center, camera->up);
}

void fly_camera_move(Fly_Camera* camera, Camera_Direction direction) {
    switch (direction) {
        case FORWARD: {
            Vector3f up = {camera->front.x, camera->front.y, camera->front.z};
            vector3f_scale(&up, camera->speed);
            camera->position = vector3f_add(&camera->position, &up);
            break;
        } 
        case BACKWARD: {
            Vector3f down = {camera->front.x, camera->front.y, camera->front.z};
            vector3f_scale(&down, camera->speed);
            camera->position = vector3f_subtract(&camera->position, &down);
            break;
        } 
        case LEFT: {
            Vector3f left = vector3f_cross_product(&camera->front, &camera->up);
            vector3f_normalize(&left);
            vector3f_scale(&left, camera->speed);
            camera->position = vector3f_subtract(&camera->position, &left);
            break;
        }
        case RIGHT: {
            Vector3f right = vector3f_cross_product(&camera->front, &camera->up);
            vector3f_normalize(&right);
            vector3f_scale(&right, camera->speed);
            camera->position = vector3f_add(&camera->position, &right);
            break;
        }
    }
}

void fly_camera_change_angle(Fly_Camera* camera, float new_mouse_x, float new_mouse_y) {
    float x_offset = new_mouse_x - camera->last_mouse_x;
    float y_offset = camera->last_mouse_y - new_mouse_y;

    camera->last_mouse_x = new_mouse_x;
    camera->last_mouse_y = new_mouse_y;

    x_offset *= camera->sensitivity;
    y_offset *= camera->sensitivity;

    camera->yaw   += x_offset;
    camera->pitch += y_offset;

    if (camera->pitch > 89.0f)  { camera->pitch = 89.0f; }
    if (camera->pitch < -89.0f) { camera->pitch = -89.0f; }

    Vector3f direction;
    direction.x = cosf(camera->yaw * ((float) M_PI / 180.0f)) * cosf(camera->pitch * ((float) M_PI / 180.0f));
    direction.y = sinf(camera->pitch * ((float) M_PI / 180.0f));
    direction.z = sinf(camera->yaw * ((float) M_PI / 180.0f)) * cosf(camera->pitch * ((float) M_PI / 180.0f));
    vector3f_normalize(&direction);
    camera->front = direction;
}