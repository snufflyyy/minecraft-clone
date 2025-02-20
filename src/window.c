#include "window.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void window_resize_callback(GLFWwindow* window, int width, int height);

Window window_create(int width, int height, const char* title) {
    Window window = {0};

    if (!glfwInit()) {
        printf("ERROR: Failed to initialize GLFW!\n");
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);
    #endif

    window.glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window.glfw_window) {
        printf("ERROR: Failed to create GLFW window!\n");
        exit(-2);
    }

    glfwMakeContextCurrent(window.glfw_window);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("ERROR: Failed to initialize GLAD\n");
        exit(-3);
    }  

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glfwSetWindowUserPointer(window.glfw_window, &window);
    glfwSetFramebufferSizeCallback(window.glfw_window, window_resize_callback);
    glfwSetInputMode(window.glfw_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwSetInputMode(window.glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    window.fps = 0.0f;
    window.frame_time = 0.0f;
    window.delta_time = 0.0f;
    window.last_frame_time = 0.0f;
    window.last_fps_print_time = 0.0f;

    return window;
}

void window_resize_callback(GLFWwindow* window, int width, int height) {
    Window* window_struct = (Window*) glfwGetWindowUserPointer(window);

    glViewport(0, 0, width, height);

    window_struct->just_resized = true;
}

bool window_should_close(Window* window) {
    return glfwWindowShouldClose(window->glfw_window);
}

int window_get_width(Window* window) {
    int width;
    glfwGetWindowSize(window->glfw_window, &width, NULL);
    return width;
}

int window_get_height(Window* window) {
    int height;
    glfwGetWindowSize(window->glfw_window, NULL, &height);
    return height;
}

void window_update(Window* window) {
    float current_time = (float) glfwGetTime();
    window->delta_time = current_time - window->last_frame_time;
    window->frame_time = window->delta_time * 1000.0f;
    window->fps = 1.0f / window->delta_time;
    window->last_frame_time = current_time;

    if (current_time - window->last_fps_print_time >= 1.0f / 10.0f) {
        printf("FPS: %0.2f Frame Time: %0.2f\n", window->fps, window->frame_time);
        window->last_fps_print_time = current_time;
    }

    glfwPollEvents();
    glfwSwapBuffers(window->glfw_window);
}

void window_destroy(Window* window) {
    glfwDestroyWindow(window->glfw_window);
    glfwTerminate();
}