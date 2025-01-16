#include "window.h"

#include <stdlib.h>
#include <stdio.h>
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
    glfwSwapInterval(1);

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
    window.delta_time  = 0.0f;
    window.frames = 0;

    window.fps_calculation_rate = 5.0f; // five times per second

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
    double current_time = glfwGetTime();
    window->frame_time = current_time - window->fps_previous_time;
    window->frames++;
    window->fps_previous_time = current_time;

    if (window->frame_time >= 1.0 / window->fps_calculation_rate) {
        window->fps = window->frames / window->frame_time;
        printf("FPS: %f\n", window->fps);
        printf("Frame Time: %f\n", window->frame_time * 100.0f);
        window->frames = 0;
    }

    glfwPollEvents();
    glfwSwapBuffers(window->glfw_window);
}

void window_destroy(Window* window) {
    glfwDestroyWindow(window->glfw_window);
    glfwTerminate();
}