#type vertex
#version 460 core

layout(location=0)in vec3 a_position;
layout(location=1)in vec4 a_color;
layout(location=2)in int a_entity_id;

layout(std140, binding =2) uniform Matrix
{
    mat4 projection;
    mat4 view;
};

out vec4 v_color;
out flat int v_entity_id;


void main()
{
    v_color=a_color;
    v_entity_id= a_entity_id;

    gl_Position=projection*view*vec4(a_position, 1);
}

#type fragment
#version 460 core


in vec4 v_color;
in flat int v_entity_id;


layout(location=0)out vec4 color;
layout(location=1)out int id;

void main()
{
    color = v_color;
    id=v_entity_id;
}

