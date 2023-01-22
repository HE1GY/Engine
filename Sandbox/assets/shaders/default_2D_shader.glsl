#type vertex
#version 460 core
layout(location=0)in vec3 a_position;
layout(location=1)in vec4 a_color;
layout(location=2)in vec2 a_tex_coord;

uniform mat4 u_view_projection;

out vec4 v_color;
out vec2 v_tex_coord;
void main()
{
    v_color=a_color;
    v_tex_coord=a_tex_coord;
    gl_Position=u_view_projection*vec4(a_position, 1);
}

#type fragment
#version 460 core
in vec4 v_color;
in vec2 v_tex_coord;

uniform sampler2D u_texture2D;
uniform vec4 u_color;

layout(location=0)out vec4 color;

void main()
{
    //color=texture(u_texture2D, v_tex_coord*10)*v_color;
    color = v_color;
}

