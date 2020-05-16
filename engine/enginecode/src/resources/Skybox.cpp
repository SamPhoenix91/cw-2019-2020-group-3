#include "engine_pch.h"

#include <glad\glad.h>
#include "resources\Skybox.h"

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "rendering/Buffers.h"
#include "resources/SkyboxAPI.h"
#include "resources/OpenGLSkybox.h"

namespace Engine
{
	Skybox::Skybox()
	{
		m_paths.push_back("Assets/Textures/Skybox/Space_Right.png");
		m_paths.push_back("Assets/Textures/Skybox/Space_left.png");
		m_paths.push_back("Assets/Textures/Skybox/Space_top.png");
		m_paths.push_back("Assets/Textures/Skybox/Space_bot.png");
		m_paths.push_back("Assets/Textures/Skybox/Space_front.png");
		m_paths.push_back("Assets/Textures/Skybox/Space_back.png");
	}


	void Skybox::createSkyboxCube()
	{
		float skyboxVerticesTwo[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
		};

		//generate buffers
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);

		//bind buffers
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVerticesTwo), &skyboxVerticesTwo, GL_STATIC_DRAW);

		//process positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	}

	Skybox * Skybox::createSkybox(std::shared_ptr<Shader> defSkyboxShader)
	{
		switch (SkyboxAPI::getAPI())
		{
		case SkyboxAPI::API::None:
			LogError("Lack of graphics API not supported!");
			break;
		case SkyboxAPI::API::OpenGL:
			return new OpenGLSkybox(defSkyboxShader);
			break;
		case SkyboxAPI::API::Direct3D:
			LogError("Direct3D is not supported.");
		}
	}


	void Skybox::draw(unsigned int texID)
	{
		glDepthFunc(GL_LEQUAL); //change depth function so depth test passes when values are equal to depth buffer's content
		glBindVertexArray(vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
		glDepthFunc(GL_LESS); //change depth check back to normal
	}

	void Skybox::stop()
	{
		glDeleteBuffers(1, &vao);
		glDeleteBuffers(1, &vbo);
	}

	Skybox::~Skybox()
	{
	}
}