#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>
#include "gm.h"

#define R_MAX_VERTICES (1024)
#define R_MAX_ELEMENTS (1024)

typedef struct {
    V3f pos;
    V4f color;
    V2f uv;
    float tex;
} R_Vertex;

typedef enum {
    R_SHADER0 = 0,
    R_SHADER1,
    R_SHADER2,
    R_SHADER3,
    R_SHADER4,
    R_COUNT_SHADERS
} R_Shader;

typedef enum {
    R_TEXTURE0 = 0,
    R_TEXTURE1, R_TEXTURE2, R_TEXTURE3, R_TEXTURE4, R_TEXTURE5, R_TEXTURE6, R_TEXTURE7,
    R_COUNT_TEXTURES,
} R_Textures;

typedef struct {
    uint32_t vao, vbo, ebo;
    uint32_t shader_programs[R_COUNT_SHADERS];
    uint32_t current_shader_program;

    uint32_t textures[R_COUNT_TEXTURES];
    int texture_samplers[R_COUNT_TEXTURES];

    R_Vertex vertices[R_MAX_VERTICES];
    uint32_t elements[R_MAX_ELEMENTS];
    uint32_t vertices_count, elements_count;
    M4f projection;
} R;

int r_init(R* r);
void r_deinit(R* r);
void r_viewport(R* r, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

void r_load_texture(R* r, int texture_index, const uint8_t* data, uint32_t w, uint32_t h, uint32_t channels);

void r_load_shader(R* r, int shader_index, const char* vert_src, const char* frag_src);
void r_load_shader_from_file(R* r, int shader_index, const char* vert_path, const char* frag_path);
void r_use_shader(R* r, int shader_index);

void r_clean(R* r);
void r_sync(R* r);
void r_draw(R* r);
void r_flush(R* r);

void r_triangle(R* r, 
        V2f p0, V4f c0, V2f uv0, float tex0,
        V2f p1, V4f c1, V2f uv1, float tex1,
        V2f p2, V4f c2, V2f uv2, float tex2);

void r_quad(R* r, 
        V2f p0, V4f c0, V2f uv0, float tex0,
        V2f p1, V4f c1, V2f uv1, float tex1,
        V2f p2, V4f c2, V2f uv2, float tex2,
        V2f p3, V4f c3, V2f uv3, float tex3);

void r_rect(R* r, V2f pos, V2f size, V4f color);
void r_image(R* r, V2f pos, V2f size, int texture_index);

#endif // RENDERER_H
