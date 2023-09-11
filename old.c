#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

static char* load_file_text(const char* file_path, size_t* size) {
    FILE* f = fopen(file_path, "r");
    size_t sz = 0;
    sz = 0;
    fseek(f, 0L, SEEK_END);
    sz = ftell(f);
    fseek(f, 0L, SEEK_SET);
    char* result = (char*)malloc(sz + 1);
    memset(result, 0, sz + 1);
    for(size_t i = 0; i < sz; ++i)
        result[i] = fgetc(f);
    if(size) *size = sz;
    return result;
}

void load_shader()
{
    GLuint shader_program, vert_shader, frag_shader;
    GLint success;
    GLchar info_log[512];

    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    char* vert_src = load_file_text("./shaders/basic.vert", NULL);
    glShaderSource(vert_shader, 1, (const GLchar* const*)&vert_src, NULL);
    glCompileShader(vert_shader);
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vert_shader, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "%s\n", info_log);
    }

    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    char* frag_src = load_file_text("./shaders/basic.frag", NULL);
    glShaderSource(frag_shader, 1, (const GLchar* const*)&frag_src, NULL);
    glCompileShader(frag_shader);
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(frag_shader, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "%s\n", info_log);
    }

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vert_shader);
    glAttachShader(shader_program, frag_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shader_program, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "%s\n", info_log);
    }
    glUseProgram(shader_program);

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
    free(vert_src);
    free(frag_src);
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
    if(glDebugMessageCallback != NULL) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(debug_message_callback, 0);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        +0.5f, -0.5f, 0.0f,
        +0.5f, +0.5f, 0.0f,
        -0.5f, +0.5f, 0.0f,
    };

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (const void*)vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (const void*)0);
    glEnableVertexAttribArray(0);

    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0,
    };
    GLuint elements_count = sizeof(elements)/sizeof(GLuint);
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), (const void*)elements, GL_STATIC_DRAW);

    load_shader();

    while(!glfwWindowShouldClose(wnd)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(wnd);
        glfwPollEvents();
    }

    glfwDestroyWindow(wnd);
    glfwTerminate();
}


