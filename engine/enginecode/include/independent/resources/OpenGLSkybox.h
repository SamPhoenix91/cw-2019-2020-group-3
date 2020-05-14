#pragma once
/*! \file OpenGLSkybox.h
\brief OpenGL implementation of a skybox.
*/

#include "resources/Skybox.h"

namespace Engine
{
	/*! \class OpenGLSkybox
	\brief OpenGL implementation of a skybox.
	*/
	class OpenGLSkybox : public Skybox
	{
	public:
		OpenGLSkybox();
		void draw(unsigned int texID);
		void stop();

		std::vector<std::string> faces
		{
			"Assets/Textures/Skybox/Space_Right.png",
			"Assets/Textures/Skybox/Space_left.png",
			"Assets/Textures/Skybox/Space_top.png",
			"Assets/Textures/Skybox/Space_bot.png",
			"Assets/Textures/Skybox/Space_front.png",
			"Assets/Textures/Skybox/Space_back.png"
		};
		unsigned int texID = 1;
	};
}