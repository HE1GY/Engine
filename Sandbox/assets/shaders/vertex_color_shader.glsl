#type vertex
#version 460 core
layout(location=0)in vec3 a_position;
layout(location=1)in vec4 a_color;
uniform mat4 u_view_project;
uniform mat4 u_model;

out vec4 v_color;
void main()
{
    v_color=a_color;
    gl_Position=u_view_project*u_model*vec4(a_position, 1);
}

#type fragment
#version 460 core
in vec4 v_color;

out vec4 color;

void main()
{
    color=v_color;
}