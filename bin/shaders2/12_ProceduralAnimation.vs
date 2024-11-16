#version 330 core
layout (location = 0) in vec3  aPos;
layout (location = 1) in vec3  aNormal;
layout (location = 2) in vec2  aTexCoords;
layout (location = 3) in vec3  tangent;
layout (location = 4) in vec3  bitangent;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
uniform float radius;
uniform float height;
//Variables para proy
uniform float amplitud;
uniform float numpi;
uniform float frec;
uniform float angTheta;
uniform float k;

void main()
{
    
    vec4 PosL = vec4(aPos, 1.0f);
    //Actividad 2
    //PosL.x += radius * cos(time);
    //PosL.y += radius * sin(time);
    //PosL.z += height;

    //Proy
    PosL.x += time;
    //PosL.x = clamp(PosL.x, -20.0f, 20.0f);
    PosL.z += amplitud * (abs(sin(2*numpi*frec*time + angTheta)) * exp(-k*time));
    

    //Experimento 2
    //PosL.x += radius * cos(time);
    //PosL.y += radius * sin(time);
    //PosL.z += tan(3.0f * time);
    //PosL.z = clamp(PosL.z, -5.0f, 5.0f); //clamp(PosL.z, minVal, maxVal) ORIGINAL x: Es el valor que quieres restringir. minVal: Es el valor mínimo permitido. maxVal: Es el valor máximo permitido.
    
    gl_Position = projection * view * model * PosL;

    TexCoords = aTexCoords;  
}