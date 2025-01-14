#shader vertex
#version 330 core
        
layout(location = 0) in vec3 position;
//layout(location = 1) in vec2 texCoord;

out vec4 out_pos;

//Projection matrices
uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(position,1.0);
    out_pos = u_MVP * vec4(position,1.0);
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in vec4 out_pos;

//uniform vec4 u_Color;

void main()
{
    vec4 out_pos_norm = out_pos / out_pos.w;
    out_pos_norm.y = 0.5 * (out_pos_norm.y + 1);
    if(out_pos_norm.y <= 1 && out_pos_norm.y > 2/3)
        color = vec4(out_pos_norm.y, 1 - out_pos_norm.y, 0, 1.0);
    if(out_pos_norm.y <= 2/3 && out_pos_norm.y > 1/3)
        color = vec4(0, out_pos_norm.y, 1 - out_pos_norm.y, 1.0);
    if(out_pos_norm.y <= 1/3 && out_pos_norm.y >= 0)
        color = vec4(0, 1 - out_pos_norm.y, out_pos_norm.y, 1.0);
};