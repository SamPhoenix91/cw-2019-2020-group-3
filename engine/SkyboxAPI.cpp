#include "engine_pch.h"
#include "resources/SkyboxAPI.h"
namespace Engine
{
	// Currently only supporting OpenGL.
	SkyboxAPI::API SkyboxAPI::s_api = SkyboxAPI::API::OpenGL;
}