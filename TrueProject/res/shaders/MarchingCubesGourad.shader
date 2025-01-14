#shader vertex
#version 330 core
        
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
//layout(location = 1) in vec2 texCoord;

out vec4 in_color;

//Projection matrices
uniform mat4 u_MVP;

//Scene uniforms
uniform vec3 ai;
uniform vec3 cameraPos;

//Light uniforms
uniform vec3 id;
uniform vec3 lightPos;

//Material uniforms
uniform float ka;
uniform float kd;
uniform vec3 ac;
uniform vec3 dc;


void main()
{
    vec4 vertPos4 = u_MVP * vec4(position,1.0);

    vec3 V = normalize(cameraPos-position);


    vec3 normal_signed = normalize(normal);
    
    if(dot(V, normal_signed) < 0)
    {
        normal_signed *= -1;
    }

    gl_Position = vertPos4;

    vec3 L = normalize(lightPos - position);

    // Lambert's cosine law
    float lambertian = max(dot(normal_signed, L), 0.0);

    if(lambertian > 0)
    {
        in_color = vec4(ka * ac * ai + kd * lambertian * dc * id, 1.0);
    }
    else
    {
        in_color = vec4(0.0,0.0,0.0,1.0);
    }
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in vec4 in_color;

//uniform vec4 u_Color;

void main()
{
    color = clamp(in_color, 0, 1);
};