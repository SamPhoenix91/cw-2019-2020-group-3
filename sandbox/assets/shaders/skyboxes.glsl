#region Vertex
#version 440 core

layout(location = 0) in vec3 aPos;
out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	TexCoords = aPos;
	vec4 pos = projection * view * vec4(aPos, 1.0);
	gl_Position = pos.xyww;
}

#region Tessellation Control
#version 450 core
layout (vertices = 3) out;

in vec3 TexCoords[];
out vec3 TexCoordsTC[];

layout(std140) uniform CameraPosition
{
	vec3 u_camPos;
};

float GetTessLevel(float Distance0, float Distance1)
{
	float avgDist = (Distance0 + Distance1) / 2.0f;
	float tess = 0.0f;


	float exponent = -0.105f * avgDist;
	tess = exp(exponent) * 15.0f;
	if(tess < 1.0f)
	{
		return 1.0f;
	}
	tess = 5.0f;
	return tess;
}

void main()
{
   float eyeToVertexDist1 = distance(u_camPos, fragPosV[1]);
   float eyeToVertexDist2 = distance(u_camPos, fragPosV[2]);
   if (gl_InvocationID==0)
   {
		  fragPosTC[gl_InvocationID]  = fragPosV[gl_InvocationID];

		   // Calculate the tessellation levels
          gl_TessLevelOuter[0] = GetTessLevel(eyeToVertexDist1, eyeToVertexDist2); 
          gl_TessLevelOuter[1] = gl_TessLevelOuter[0]; 
          gl_TessLevelOuter[2] = gl_TessLevelOuter[0];
          gl_TessLevelInner[0] = gl_TessLevelOuter[0]; 

   }
   TexCoordsTC[gl_InvocationID] = TexCoords[gl_InvocationID];
}

#region Tessellation Eval
#version 450 core

layout(triangles, fractional_even_spacing, ccw) in;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2);
vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2);

layout(std140) uniform Matrices
{
	mat4 u_VP;
};

in vec3 TexCoordsTC[];
out vec3 TexCoordsTE[];

void main()
{
	TexCoordsTE = interpolate2D(texCoordTC[0], texCoordTC[1], texCoordTC[2]);
	vec4 pos = projection * view * vec4(aPos, 1.0);
	gl_Position = pos.xyww;
}

//basic linear interpolation
vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
   	return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
   	return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

#region Geometry

#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 TexCoordsTE[];
out vec3 TexCoordsG;

void main()
{
   for(int i = 0 ; i < 3; i++)
   {
      gl_Position = gl_in[i].gl_Position ;
      TexCoordsG = TexCoordsTE[i];
      EmitVertex() ;
  }
     EndPrimitive() ;
}

#region Fragment

#version 440 core
			
layout(location = 0) out vec4 colour;
in vec3 TexCoordsG;
out vec4 FragColour;

uniform samplerCube skybox;

void main()
{
	FragColour = texture(skybox, TexCoordsG);
}