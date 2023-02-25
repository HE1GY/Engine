#type vertex
#version 460 core

layout(location=0)in vec3 a_position;
layout(location=1)in vec4 a_color;
layout(location=2)in vec2 a_tex_coord;
layout(location=3)in int a_tex_slot;
layout(location=4)in int a_entity_id;

layout(std140, binding =2) uniform Matrix
{
    mat4 projection;
    mat4 view;
};


out vec4 v_color;
out vec2 v_tex_coord;
out flat int v_tex_slot;
out flat int v_entity_id;

void main()
{
    v_color=a_color;
    v_tex_coord=a_tex_coord;
    v_tex_slot=a_tex_slot;
    v_entity_id= a_entity_id;


    gl_Position=projection*view*vec4(a_position, 1);
}

#type fragment
#version 460 core

in vec4 v_color;
in vec2 v_tex_coord;
in flat int v_tex_slot;
in flat int v_entity_id;

uniform sampler2D u_texture2D[32];

layout(location=0)out vec4 color;
layout(location=1)out int id;

void main()
{
    color=texture(u_texture2D[v_tex_slot], v_tex_coord)*v_color;
    // color=vec4(float(v_tex_slot), 0, 0, 1);
    id=v_entity_id;
}

