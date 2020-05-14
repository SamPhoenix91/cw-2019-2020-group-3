#include "engine_pch.h"
#include <glad/glad.h>
#include "resources/OpenGLSkybox.h"

Engine::OpenGLSkybox::OpenGLSkybox()
{
	m_paths.push_back("Assets/Textures/Skybox/Space_Right.png");
	m_paths.push_back("Assets/Textures/Skybox/Space_left.png");
	m_paths.push_back("Assets/Textures/Skybox/Space_top.png");
	m_paths.push_back("Assets/Textures/Skybox/Space_bot.png");
	m_paths.push_back("Assets/Textures/Skybox/Space_front.png");
	m_paths.push_back("Assets/Textures/Skybox/Space_back.png");

	glDepthFunc(GL_LEQUAL); //change depth function so depth test passes when values are equal to depth buffer's content
	glBindVertexArray(vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
	glDepthFunc(GL_LESS); //change depth check back to normal
}

void Engine::OpenGLSkybox::draw(unsigned int texID)
{
	glDepthFunc(GL_LEQUAL); //change depth function so depth test passes when values are equal to depth buffer's content
	glBindVertexArray(vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
	glDepthFunc(GL_LESS); //change depth check back to normal
}

void Engine::OpenGLSkybox::stop()
{
	glDeleteBuffers(1, &vao);
	glDeleteBuffers(1, &vbo);
}
