#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float vertexScalarData;
//layout (location = 2) in vec3 aNormal;


out vec3 FragPos;
out float fragScalarData;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    //Normal = mat3(transpose(inverse(model))) * aNormal;  
    fragScalarData = vertexScalarData;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}