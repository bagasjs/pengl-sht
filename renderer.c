#include "renderer.h"
#include "gm.h"

#include <glad/glad.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void DumpVertex(R_Vertex v)
{
    printf("pos=(%f,%f), color=(%f,%f,%f,%f), uv=(%f,%f), tex=%f\n",
            v.pos.x, v.pos.y,
            v.color.r, v.color.g, v.color.b, v.color.a,
            v.uv.u, v.uv.v,
            v.tex);
}

void CheckOpenGLError(void)
{
    int exit_on_error = 1;
    GLenum err = GL_NO_ERROR;
    printf("CHECKING OPENGL ERROR\n");
    while((err = glGetError()) != GL_NO_ERROR) {
        if(err == GL_NO_ERROR) {
            break;
        } else if(err == GL_INVALID_ENUM) {
            fprintf(stderr, "OPENGL ERROR %d: GL_INVALID_ENUM\n", err);
        } else if(err == GL_INVALID_VALUE) {
            fprintf(stderr, "OPENGL ERROR %d: GL_INVALID_ENUM\n", err);
        } else if(err == GL_INVALID_OPERATION) {
            fprintf(stderr, "OPENGL ERROR %d: GL_INVALID_ENUM\n", err);
        } else if(err == GL_STACK_OVERFLOW) {
            fprintf(stderr, "OPENGL ERROR %d: GL_INVALID_ENUM\n", err);
        } else if(err == GL_STACK_UNDERFLOW) {
            fprintf(stderr, "OPENGL ERROR %d: GL_INVALID_ENUM\n", err);
        } else if(err == GL_OUT_OF_MEMORY) {
            fprintf(stderr, "OPENGL ERROR %d: GL_INVALID_ENUM\n", err);
        } else if(err == GL_INVALID_FRAMEBUFFER_OPERATION) {
            fprintf(stderr, "OPENGL ERROR %d: GL_INVALID_ENUM\n", err);
        } else {
            fprintf(stderr, "OPENGL ERROR: %d\n", err);
        }
        if(exit_on_error) {
            exit(EXIT_FAILURE);
        }
    }
}

int r_init(R* r)
{
    memset(r, 0, sizeof(R));
    glGenVertexArrays(1, &r->vao);
    glBindVertexArray(r->vao);

    glGenBuffers(1, &r->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(r->vertices), r->vertices, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(R_Vertex), (GLvoid*)offsetof(R_Vertex, pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(R_Vertex), (GLvoid*)offsetof(R_Vertex, color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(R_Vertex), (GLvoid*)offsetof(R_Vertex, uv));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(R_Vertex), (GLvoid*)offsetof(R_Vertex, tex));

    glGenBuffers(1, &r->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(r->elements), r->elements, GL_DYNAMIC_DRAW);

    return 0;
}

void r_deinit(R* r)
{
    glDeleteBuffers(1, &r->vbo);
    glDeleteBuffers(1, &r->ebo);
    glDeleteVertexArrays(1, &r->vao);
}

void r_sync(R* r)
{
    glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, r->vertices_count*sizeof(*r->vertices), r->vertices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r->ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, r->elements_count*sizeof(*r->elements), r->elements);
}

void r_draw(R* r)
{
    glBindVertexArray(r->vao);
    glUseProgram(r->shader_programs[r->current_shader_program]);
    for(uint32_t i=0; i<R_COUNT_TEXTURES; ++i) {
        if(r->texture_samplers[i] != 0) {
            glActiveTexture(GL_TEXTURE0 + r->texture_samplers[i]);
            glBindTexture(GL_TEXTURE_2D, r->textures[i]);
        }
    }

    int location = glGetUniformLocation(r->shader_programs[r->current_shader_program], "u_textures");
    if(location >= 0) {
        glUniform1iv(location, R_COUNT_TEXTURES, r->texture_samplers);
    } else {
        fprintf(stderr, "Invalid uniform location\n");
    }
    location = glGetUniformLocation(r->shader_programs[r->current_shader_program], "u_MVP");
    if(location >= 0) {
        glUniformMatrix4fv(location, 1, GL_FALSE, r->projection.elements);
    } else {
        fprintf(stderr, "Invalid uniform location\n");
    }
    glDrawElements(GL_TRIANGLES, r->elements_count, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r->ebo);
}

void r_clean(R* r)
{
    (void)r;
    glClear(GL_COLOR_BUFFER_BIT);
}

void r_flush(R* r)
{
    r_sync(r);
    r_draw(r);
    r->elements_count = 0;
    r->vertices_count = 0;
    memset(r->texture_samplers, 0, sizeof(r->texture_samplers));
}

void r_viewport(R* r, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
    glViewport(x, y, w, h);
    r->projection = m4f_ortho(x, w, y, h, 0.0f, 100.0f);
}

void r_triangle(R* r, 
        V2f p0, V4f c0, V2f uv0, float tex0,
        V2f p1, V4f c1, V2f uv1, float tex1,
        V2f p2, V4f c2, V2f uv2, float tex2)
{
    r->elements[r->elements_count + 0] = r->vertices_count + 0;
    r->elements[r->elements_count + 1] = r->vertices_count + 1;
    r->elements[r->elements_count + 2] = r->vertices_count + 2;
    r->elements_count += 3;

    r->vertices[r->vertices_count + 0].pos = v3f(p0.x, p0.y, 0.0f);
    r->vertices[r->vertices_count + 0].color = c0;
    r->vertices[r->vertices_count + 0].uv = uv0;
    r->vertices[r->vertices_count + 0].tex = tex0;
    r->vertices[r->vertices_count + 1].pos = v3f(p1.x, p1.y, 0.0f);
    r->vertices[r->vertices_count + 1].color = c1;
    r->vertices[r->vertices_count + 1].uv = uv1;
    r->vertices[r->vertices_count + 1].tex = tex1;
    r->vertices[r->vertices_count + 2].pos = v3f(p2.x, p2.y, 0.0f);
    r->vertices[r->vertices_count + 2].color = c2;
    r->vertices[r->vertices_count + 2].uv = uv2;
    r->vertices[r->vertices_count + 2].tex = tex2;
    r->vertices_count += 3;
}

void r_quad(R* r, 
        V2f p0, V4f c0, V2f uv0, float tex0,
        V2f p1, V4f c1, V2f uv1, float tex1,
        V2f p2, V4f c2, V2f uv2, float tex2,
        V2f p3, V4f c3, V2f uv3, float tex3)
{
    r->elements[r->elements_count + 0] = r->vertices_count + 0;
    r->elements[r->elements_count + 1] = r->vertices_count + 1;
    r->elements[r->elements_count + 2] = r->vertices_count + 2;
    r->elements[r->elements_count + 3] = r->vertices_count + 2;
    r->elements[r->elements_count + 4] = r->vertices_count + 3;
    r->elements[r->elements_count + 5] = r->vertices_count + 0;
    r->elements_count += 6;

    r->vertices[r->vertices_count + 0].pos = v3f(p0.x, p0.y, 0.0f);
    r->vertices[r->vertices_count + 0].color = c0;
    r->vertices[r->vertices_count + 0].uv = uv0;
    r->vertices[r->vertices_count + 0].tex = tex0;
    r->vertices[r->vertices_count + 1].pos = v3f(p1.x, p1.y, 0.0f);
    r->vertices[r->vertices_count + 1].color = c1;
    r->vertices[r->vertices_count + 1].uv = uv1;
    r->vertices[r->vertices_count + 1].tex = tex1;
    r->vertices[r->vertices_count + 2].pos = v3f(p2.x, p2.y, 0.0f);
    r->vertices[r->vertices_count + 2].color = c2;
    r->vertices[r->vertices_count + 2].uv = uv2;
    r->vertices[r->vertices_count + 2].tex = tex2;
    r->vertices[r->vertices_count + 3].pos = v3f(p3.x, p3.y, 0.0f);
    r->vertices[r->vertices_count + 3].color = c3;
    r->vertices[r->vertices_count + 3].uv = uv3;
    r->vertices[r->vertices_count + 3].tex = tex3;
    r->vertices_count += 4;
}

void r_load_texture(R* r, int texture_index, const uint8_t* data, uint32_t w, uint32_t h, uint32_t channels)
{
    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);

    GLenum internal_format = GL_RGBA8;
    GLenum data_format = channels == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, h, 0, data_format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    r->textures[texture_index] = texture;
}

void r_rect(R* r, V2f pos, V2f size, V4f color)
{
    V2f zeros = v2f_zeros();
    r_quad(r, 
        v2f(pos.x, pos.y), color, zeros, 0.0f,
        v2f(pos.x + size.x, pos.y), color, zeros, 0.0f,
        v2f(pos.x + size.x, pos.y + size.y), color, zeros, 0.0f,
        v2f(pos.x, pos.y + size.y), color, zeros, 0.0f);
}

void r_image(R* r, V2f pos, V2f size, int texture_index)
{
    V4f color = v4f_zeros();
    r_quad(r,
        v2f(pos.x, pos.y), color, v2f(0.0f, 0.0f), texture_index,
        v2f(pos.x + size.x, pos.y), color, v2f(1.0f, 0.0f), texture_index,
        v2f(pos.x + size.x, pos.y + size.y), color, v2f(1.0f, 1.0f), texture_index,
        v2f(pos.x, pos.y + size.y), color, v2f(0.0f, 1.0f), texture_index);
    r->texture_samplers[texture_index] = texture_index;
}

void r_use_shader(R* r, int shader_index)
{
    r->current_shader_program = shader_index;
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

void r_load_shader_from_file(R* r, int shader_index, const char* vert_path, const char* frag_path)
{
    char* vert_src = load_file_text(vert_path, NULL);
    char* frag_src = load_file_text(frag_path, NULL);
    r_load_shader(r, shader_index, vert_src, frag_src);
    free(vert_src);
    free(frag_src);
}

void r_load_shader(R* r, int shader_index, const char* vert_src, const char* frag_src)
{
    GLuint shader_program, vert_shader, frag_shader;
    GLint success;
    GLchar info_log[512];

    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, (const GLchar* const*)&vert_src, NULL);
    glCompileShader(vert_shader);
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vert_shader, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "VERTEX SHADER: %s\n", info_log);
    }

    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, (const GLchar* const*)&frag_src, NULL);
    glCompileShader(frag_shader);
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(frag_shader, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "FRAGMENT SHADER: %s\n", info_log);
    }

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vert_shader);
    glAttachShader(shader_program, frag_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shader_program, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "SHADER PROGRAM LINKING: %s\n", info_log);
    }
    glUseProgram(shader_program);

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    r->shader_programs[shader_index] = shader_program;
    r->current_shader_program = shader_index;
}
