#type vertex
#version 460 core
layout(location=0)in vec3 a_position;
uniform mat4 u_view_project;
uniform mat4 u_model;

out vec3 v_color;
void main()
{
    gl_Position=u_view_project*u_model*vec4(a_position, 1);
}

#type fragment
#version 460 core
layout(location=0)out vec4 color;
uniform vec4 u_color;
void main()
{
    color=u_color;
}