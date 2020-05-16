#region Vertex
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;


layout (std140) uniform Matrices
{
	mat4 u_VP;
	mat4 u_camPos;
};

void main()
{
    TexCoords = aPos;
    vec4 pos = u_VP * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}

#region Fragment
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube u_skybox;

void main()
{    
    FragColor = texture(u_skybox, TexCoords);
}