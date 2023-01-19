#type vertex
#version 460 core
layout(location=0)in vec3 a_position;
layout(location=1)in vec2 a_tex_coord;

uniform mat4 u_view_projection;
uniform mat4 u_model;

out vec2 v_tex_coord;
void main()
{
    v_tex_coord=a_tex_coord;
    gl_Position=u_view_projection* u_model*vec4(a_position, 1);
}

#type fragment
#version 460 core
in vec2 v_tex_coord;

uniform sampler2D u_texture2D;
uniform vec4 u_color;

layout(location=0)out vec4 color;

void main()
{
    color=texture(u_texture2D, v_tex_coord*10)*u_color;
}

