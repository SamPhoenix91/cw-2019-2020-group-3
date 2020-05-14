#pragma once

/*! \file SkyboxAPI.h
\brief Defines what skybox API is used
*/

namespace Engine
{
	/*! \class SkyboxAPI
	\brief Defines current skybox API
	*/
	class SkyboxAPI
	{
	public:
		/*! \enum API
		\brief Set of possible skybox APIs
		*/
		enum class API
		{
			None = 0, OpenGL = 1, Direct3D = 2
		};
		//! Get which API is currently in use. \return current skybox API
		inline static API getAPI() { return s_api; }

	private:
		static API s_api; //!< Current skybox API
	};
}