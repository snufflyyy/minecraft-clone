#include "camera.h"

#include <glad/glad.h>

Fly_Camera createFlyCamera(int windowWidth, int windowHeight) {
    Fly_Camera camera = {0};

    camera.speed = 0.0005f;
    camera.fov = 75.0f;

    camera.position = (Vector3f) {0.0f, 0.0f, 0.0f};
    camera.front = (Vector3f) {0.0f, 0.0f, -1.0f};
    camera.up = (Vector3f) {0.0f, 1.0f, 0.0f};

    camera.last_mouse_x = (float) windowWidth / 2;
    camera.last_mouse_y = (float) windowHeight / 2;

    camera.view = matrix4f_identity();
    camera.projection = matrix4f_perspective(camera.fov, (float) windowWidth / (float) windowHeight, 0.01f, 1000.0f);

    return camera;
}

void updateFlyCamera(Fly_Camera* camera) {
    Vector3f center = vector3f_add(camera->position, camera->front);
    glm_lookat(camera->position, center, camera->up, camera->view);
}

void moveFlyCameraPosition(Fly_Camera* camera, Camera_Direction direction) {
    switch (direction) {
        case FORWARD:
            vec3 up = {camera->front[0], camera->front[1], camera->front[2]};
            glm_vec3_scale(up, camera->speed, up);
            glm_vec3_add(camera->position, up, camera->position);
            break;
        case BACKWARD:
            vec3 down = {camera->front[0], camera->front[1], camera->front[2]};
            glm_vec3_scale(down, camera->speed, down);
            glm_vec3_sub(camera->position, down, camera->position);
            break;
        case LEFT:
            vec3 left;
            glm_vec3_cross(camera->front, camera->up, left);
            glm_vec3_normalize(left); 
            glm_vec3_scale(left, camera->speed, left);
            glm_vec3_sub(camera->position, left, camera->position);
            break;
        case RIGHT:
            vec3 right;
            glm_vec3_cross(camera->front, camera->up, right);
            glm_vec3_normalize(right); 
            glm_vec3_scale(right, camera->speed, right);
            glm_vec3_add(camera->position, right, camera->position);
            break;
    }
}

void moveFlyCameraAngle(FlyCamera* camera, double newMouseX, double newMouseY) {
    float xOffset = newMouseX - camera->lastMouseX;
    float yOffset = camera->lastMouseY - newMouseY;

    camera->lastMouseX = newMouseX;
    camera->lastMouseY = newMouseY;

    const float sensitivity = 0.1f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    static float yaw = -90.0f;
    static float pitch = 0.0f;

    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    vec3 direction;
    direction[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    direction[1] = sin(glm_rad(pitch));
    direction[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
    glm_vec3_normalize_to(direction, camera->front);
}