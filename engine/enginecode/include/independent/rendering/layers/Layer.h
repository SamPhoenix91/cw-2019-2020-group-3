#pragma once

/*! \file Layer.h
\brief Implementation of a layer
*/

#include <string>
#include <vector>
#include "rendering/renderer/Renderer.h"
#include "rendering/renderer/PPRenderer.h"
#include "rendering/cameras/CameraController.h"
#include "resources/ResourceManager.h"

namespace Engine
{
	/*! \class Layer
	\brief Base class for all layers
	*/
	class Layer
	{
	protected:
		std::string m_name; //!< Name of a layer
		std::shared_ptr<Renderer> m_renderer; //!< Pointer to a renderer
		std::shared_ptr<PPRenderer> m_PPRenderer; //!< Pointer to a PPRenderer
		std::shared_ptr<CameraController> m_cameraController; //!< Pointer to a camera controller

	public:
		//! Default constructor
		Layer() {};
		//! Custom constructor \param name name of a layer
		Layer(const std::string& name);
		//! Runs when layer is attached
		virtual void onAttach() = 0;
		//! Runs when layer is detached
		virtual void onDetach() = 0;
		//! Runs every frame \param timestep delta time
		virtual void onUpdate(float timestep) = 0;
		//! Runs when an event occurs \param e event occurred
		virtual void onEvent(Event& e) = 0;
		//! Gets name of a layer \return layer's name
		const std::string& getName();

		//! Gets camera controller associated with a layer \return camera controller
		std::shared_ptr<CameraController>& getCamera() { return m_cameraController; }
		//! Gets renderer associated with a layer \return renderer
		std::shared_ptr<Renderer>& getRenderer() { return m_renderer; }
		std::shared_ptr<PPRenderer>& getPPRenderer() { return m_PPRenderer; }

		//! Virtual destructor
		virtual ~Layer() {};
	};
}