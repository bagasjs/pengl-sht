#version 330

layout(location=0) in vec3 a_position;
layout(location=1) in vec4 a_color;
layout(location=2) in vec2 a_uv;
layout(location=3) in float a_tex;

out vec4 v_color;
out vec2 v_uv;
out float v_tex;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(a_position, 1.0);
    v_uv = a_uv;
    v_color = a_color;
    v_tex = a_tex;
}
