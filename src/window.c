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

    window.glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window.glfw_window) {
        printf("ERROR: Failed to create GLFW window!\n");
        exit(-2);
    }

    glfwMakeContextCurrent(window.glfw_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("ERROR: Failed to initialize GLAD\n");
        exit(-3);
    }  

    glViewport(0, 0, width, height);

    glfwSetWindowUserPointer(window.glfw_window, &window);
    glfwSetFramebufferSizeCallback(window.glfw_window, window_resize_callback);

    return window;
}

void window_resize_callback(GLFWwindow* window, int width, int height) {
    Window* window_struct = (Window*) glfwGetWindowUserPointer(window);
    window_struct->just_resized = true;

    glViewport(0, 0, width, height);
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
    window->just_resized = false;

    glfwPollEvents();
    glfwSwapBuffers(window->glfw_window);
}

void window_destory(Window* window) {
    glfwDestroyWindow(window->glfw_window);
    glfwTerminate();
}