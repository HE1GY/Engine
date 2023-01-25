#type vertex
#version 460 core

layout(location=0)in vec3 a_position;
layout(location=1)in vec4 a_color;
layout(location=2)in vec2 a_tex_coord;
layout(location=3)in float a_tex_slot;

uniform mat4 u_view_projection;

out vec4 v_color;
out vec2 v_tex_coord;
out float v_tex_slot;
void main()
{
    v_color=a_color;
    v_tex_coord=a_tex_coord;
    v_tex_slot=a_tex_slot;

    gl_Position=u_view_projection*vec4(a_position, 1);
}

#type fragment
#version 460 core

in vec4 v_color;
in vec2 v_tex_coord;
in float v_tex_slot;

uniform sampler2D u_texture2D[32];

layout(location=0)out vec4 color;

void main()
{
    color=texture(u_texture2D[int(v_tex_slot)], v_tex_coord)*v_color;
    //color=vec4(v_tex_coord, 0, 1);
}

