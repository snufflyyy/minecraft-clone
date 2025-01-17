#include "camera.h"

#include "math/matrix4f.h"
#include "window.h"

#include <math.h>

Fly_Camera fly_camera_create(int window_width, int window_height) {
    Fly_Camera camera = {0};

    camera.sensitivity = 0.3f;
    camera.speed = 10.0f;
    camera.fov = 75.0f;

    camera.position = (Vector3f) {0.0f, 0.0f, 0.0f};
    camera.front = (Vector3f) {0.0f, 0.0f, -1.0f};
    camera.up = (Vector3f) {0.0f, 1.0f, 0.0f};

    camera.yaw = -90.0f;
    camera.pitch = 0.0f;

    camera.last_mouse_x = (float) window_width / 2;
    camera.last_mouse_y = (float) window_height / 2;

    camera.view = matrix4f_identity();
    camera.projection = matrix4f_perspective(camera.fov, (float) window_width / (float) window_height, 0.1f, 1000.0f);

    return camera;
}

void fly_camera_keyboard_mouse_movement(Fly_Camera* camera, Window* window, float delta_time) {
    if (glfwGetKey(window->glfw_window, GLFW_KEY_W) == GLFW_PRESS) {
        fly_camera_move(camera, FORWARD, delta_time);
    }
    if (glfwGetKey(window->glfw_window, GLFW_KEY_S) == GLFW_PRESS) {
        fly_camera_move(camera, BACKWARD, delta_time);
    }
    if (glfwGetKey(window->glfw_window, GLFW_KEY_A) == GLFW_PRESS) {
        fly_camera_move(camera, LEFT, delta_time);
    }
    if (glfwGetKey(window->glfw_window, GLFW_KEY_D) == GLFW_PRESS) {
        fly_camera_move(camera, RIGHT, delta_time);
    }

    double mouse_x, mouse_y;
    glfwGetCursorPos(window->glfw_window, &mouse_x, &mouse_y);
    fly_camera_change_angle(camera, mouse_x, mouse_y);
}

void fly_camera_update(Fly_Camera* camera) {
    Vector3f center = vector3f_add(&camera->position, &camera->front);
    camera->view = matrix4f_look_at(camera->position, center, camera->up);
}

void fly_camera_move(Fly_Camera* camera, Camera_Direction direction, float delta_time) {
    switch (direction) {
        case FORWARD: {
            Vector3f up = {camera->front.x, camera->front.y, camera->front.z};
            vector3f_scale(&up, camera->speed * delta_time);
            camera->position = vector3f_add(&camera->position, &up);
            break;
        } 
        case BACKWARD: {
            Vector3f down = {camera->front.x, camera->front.y, camera->front.z};
            vector3f_scale(&down, camera->speed * delta_time);
            camera->position = vector3f_subtract(&camera->position, &down);
            break;
        } 
        case LEFT: {
            Vector3f left = vector3f_cross_product(&camera->front, &camera->up);
            vector3f_normalize(&left);
            vector3f_scale(&left, camera->speed * delta_time);
            camera->position = vector3f_subtract(&camera->position, &left);
            break;
        }
        case RIGHT: {
            Vector3f right = vector3f_cross_product(&camera->front, &camera->up);
            vector3f_normalize(&right);
            vector3f_scale(&right, camera->speed * delta_time);
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