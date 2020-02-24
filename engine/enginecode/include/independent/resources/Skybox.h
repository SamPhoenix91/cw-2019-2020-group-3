#pragma once

// Creates skybox (basically a cube)

#include <vector>

namespace Engine
{
	class Skybox
	{
	private:
		std::vector<std::string> m_paths; // paths to skybox's textures
		unsigned int vao, vbo; // buffers IDs
	public:
		Skybox(); // Init skybox

		void createSkyboxCube(); // Creates skybox cube
		inline std::vector<std::string> getPaths() { return m_paths; } // Gets all file paths
		void draw(unsigned int texID); // Draw skybox
		void stop(); // Free resources

		std::vector<std::string> faces
		{
			"Assets/Textures/Skybox/Space_Right.png",
			"Assets/Textures/Skybox/Space_left.png",
			"Assets/Textures/Skybox/Space_top.png",
			"Assets/Textures/Skybox/Space_bot.png",
			"Assets/Textures/Skybox/Space_front.png",
			"Assets/Textures/Skybox/Space_back.png"
		};

		~Skybox();
	};
}
