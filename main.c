#include "gm.h"
#include "renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WND_WIDTH 800
#define WND_HEIGHT 600
#define WND_TITLE "OpenGL Shit"

void debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    (void) source;
    (void) id;
    (void) length;
    (void) userParam;
    fprintf(stderr, "[OpenGL Error](%d)(%d) %s\n", severity, type, message);
}

int main(void)
{
    assert(glfwInit() && "Failed to initialize GLFW");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* wnd = glfwCreateWindow(WND_WIDTH, WND_HEIGHT, WND_TITLE, NULL, NULL);
    assert(wnd != NULL && "Failed to create window");
    glfwMakeContextCurrent(wnd);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glDebugMessageCallback(debug_message_callback, 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    R ren = {0};
    R* r = &ren;
    r_init(r);
    r_viewport(r, 0, 0, WND_WIDTH, WND_HEIGHT);
    r_load_shader_from_file(r, R_SHADER0, "shaders/main.vert", "shaders/main.frag");

    V4f red = v4f(1.0f, 0.0f, 0.0f, 1.0f);
    V4f green = v4f(0.0f, 1.0f, 0.0f, 1.0f);
    V4f blue = v4f(0.0f, 0.0f, 1.0f, 1.0f);

    int w, h, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load("./images/sample.jpg", &w, &h, &channels, 0);

    r_load_texture(r, R_TEXTURE1, data, w, h, channels);

    while(!glfwWindowShouldClose(wnd)) {
        r_clean(r);
        r_rect(r, v2f(0.0f, 0.0f), v2f(100.0f, 100.0f), red);
        r_image(r, v2f(0.0f, 0.0f), v2f((float)w/2.5, (float)h/2.5), R_TEXTURE1);
        r_flush(r);
        glfwSwapBuffers(wnd);
        glfwPollEvents();
    }

    r_deinit(r);

    glfwDestroyWindow(wnd);
    glfwTerminate();
}


