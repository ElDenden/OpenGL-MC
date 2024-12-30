#shader vertex
#version 330 core
        
layout(location = 0) in vec2 position;
//layout(location = 1) in vec2 texCoord;

//Outputting data into the fragment shader
//out vec2 v_TexCoord;

//Projection matrices
//uniform mat4 u_MVP;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    //v_TexCoord = texCoord;
}

#shader fragment
#version 330 core
        
out vec4 color;

//in vec2 v_TexCoord;

uniform vec4 u_Color;
//uniform sampler2D u_Texture;

void main()
{
    //vec4 texColor = texture(u_Texture, v_TexCoord);
    color = u_Color;
}