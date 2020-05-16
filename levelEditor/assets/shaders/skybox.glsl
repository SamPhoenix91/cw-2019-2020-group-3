#region Vertex

#version 440 core
			
layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec3 a_vertexNormal;
layout(location = 2) in vec3 a_texCoord;

out vec3 fragmentPos;
out vec3 normal;
out vec3 texCoord;

layout(std140) uniform Matrices
{
	mat4 u_VP;
};

uniform mat4 u_model;

void main()
{
	fragmentPos = vec3(u_model * vec4(a_vertexPosition, 1.0));
	normal = mat3(transpose(inverse(u_model))) * a_vertexNormal;
	texCoord = vec3(a_texCoord.x, a_texCoord.y, a_texCoord.z);
	gl_Position =  u_VP * u_model * vec4(a_vertexPosition, 1.0);
}
				
#region Fragment

#version 440 core

in vec3 normal;
in vec3 fragmentPos;
in vec3 texCoord;
out vec4 colour;
uniform samplerCube u_skybox;

void main()
{
	colour = texture(u_skybox, texCoord);
}