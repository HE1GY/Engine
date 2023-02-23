#type vertex
#version 460 core

layout(location=0)in vec3 a_position;
layout(location=1)in vec3 a_local;
layout(location=2)in vec4 a_color;
layout(location=3)in float a_thickness;
layout(location=4)in float a_fade;
layout(location=5)in int a_entity_id;

uniform mat4 u_view_projection;

out vec3 v_local;
out vec4 v_color;
out float v_thickness;
out float v_fade;
out flat int v_entity_id;


void main()
{
    v_local=a_local;
    v_color=a_color;
    v_thickness=a_thickness;
    v_fade=a_fade;
    v_entity_id= a_entity_id;

    gl_Position=u_view_projection*vec4(a_position, 1);
}

#type fragment
#version 460 core

in vec3 v_local;
in vec4 v_color;
in float v_thickness;
in float v_fade;
in flat int v_entity_id;


layout(location=0)out vec4 color;
layout(location=1)out int id;

void main()
{
    float length_to_center =1.0f-length(v_local);

    float alpha =smoothstep(0.0f, v_fade, length_to_center);

    float sickness_factor =1.0f-smoothstep(v_thickness, v_thickness+v_fade, length_to_center);

    if (sickness_factor==0.0f || alpha==0.0f)
    {
        discard;
    }

    color = vec4(v_color)*alpha*sickness_factor;
    id=v_entity_id;
}

