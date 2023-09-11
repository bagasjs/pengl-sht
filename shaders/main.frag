# version 330

layout(location=0) out vec4 o_color;

uniform sampler2D u_textures[8];

in vec4 v_color;
in vec2 v_uv;
in float v_tex;

void main()
{
    int index = int(v_tex);
    if(index == 1) {
        o_color = texture(u_textures[1], v_uv);
    } else if(index == 2) {
        o_color = texture(u_textures[2], v_uv);
    } else if(index == 3) {
        o_color = texture(u_textures[3], v_uv);
    } else if(index == 4) {
        o_color = texture(u_textures[4], v_uv);
    } else if(index == 5) {
        o_color = texture(u_textures[5], v_uv);
    } else if(index == 6) {
        o_color = texture(u_textures[6], v_uv);
    } else if(index == 7) {
        o_color = texture(u_textures[7], v_uv);
    } else {
        o_color = v_color;
    }
    
    // if(v_tex > 0) {
    //     o_color = texture(u_textures[v_tex], v_uv);
    // } else {
    //     o_color = v_color;
    // }
}
