#pragma once

/*! \file AIComponent.h
\brief Defines an AI component
*/

extern "C"
{
	#include "lua.h" 
	#include "lualib.h"
	#include "lauxlib.h"
}
#include <../LuaBridge/Source/LuaBridge/LuaBridge.h>

#include <deque>
#include "Component.h"
#include "systems/Log.h"
#include "core/application.h"

namespace Engine
{
	//! \enum PathType \brief AI path types
	enum class PathType
	{
		Single, Constant, Reversing
	};

	/*! \class AIComponent
	\brief Defins an AI component
	
		Inherits from component
	*/
	class AIComponent : public Component
	{
	private:
		std::deque<glm::vec3> m_waypoints; //!< Waypoints
		std::shared_ptr<luabridge::LuaRef> m_lua; //!< Lua reference
		
		int m_currentPathNum; //!< Current path index
		glm::vec3 m_currentPath; //!< Current path waypoint

		glm::vec3 m_currentPosition; //!< Current position
		glm::vec3 m_desiredPosition; //!< Position to go to

		glm::vec3 m_currentRotation; //!< Current rotation
		glm::vec3 m_desiredRotation; //!< Rotation to rotate towards

		// These values below should be saved as they are required for the component
		float m_stopSize; //!< Stop distance
		std::string m_aiType; //!< AI type
		std::string m_scriptName; //!< Script name
		std::string m_pathTypeName; //!< Path type name
		std::vector<glm::vec3> m_path; //!< Path waypoints
		int m_pathType; //!< Path index
	public:
		/*! Custom constructor
		\param stop stop distance
		\param type AI type
		\param scriptName Lua script name
		\param pathType type of the path
		*/
		AIComponent(float stop, std::string type, std::string scriptName, std::string pathType)
			: m_stopSize(stop), m_aiType(type), m_scriptName(scriptName), m_pathTypeName(pathType)
		{
			if (pathType == "Single") { m_pathType = (int)PathType::Single; }
			else if (pathType == "Constant") { m_pathType = (int)PathType::Constant; }
			else if (pathType == "Reversing") { m_pathType = (int)PathType::Reversing; }
		};

		void onAttach(GameObject* owner) override 
		{ 
			m_owner = owner;	

			m_currentPosition = owner->getComponent<PositionComponent>()->getCurrentPosition();
			m_currentRotation = owner->getComponent<PositionComponent>()->getCurrentRotation();

			if (m_path.empty())
			{
				addPath(m_currentPosition.x, m_currentPosition.y, m_currentPosition.z);
				addPath(-m_currentPosition.x, -m_currentPosition.y, -m_currentPosition.z); // Needs second path on attach by default or velocity component takes priority
			}

			m_currentPath = m_path[0];

			luaUpdate();

			m_desiredPosition = m_waypoints.front();
		}

		//! Gets an owner \return owner
		GameObject* getOwner() override { return m_owner; }

		void onDetach() override
		{ 
			auto iter = m_owner->getMap().begin();
			while (iter != m_owner->getMap().end())
			{
				if ((*iter).second == this)
				{
					iter = m_owner->getMap().erase(iter);
					--iter;
				}
				else
				{
					++iter;
				}
			}

			m_lua.reset(); 
		};

		void onUpdate(float timestep) override 
		{
			if (glm::distance(m_desiredPosition, m_currentPosition) < m_stopSize)
			{
				if (m_waypoints.empty() == false)
				{
					m_waypoints.pop_front();
				}

				luaUpdate(); // check for lua stuff

				if (m_waypoints.empty() == false)
				{
					m_desiredPosition = m_waypoints.front();
					m_currentPath = m_desiredPosition;

					// find orientation
						// calculate acos of the dot product in each component

					//float angleBetween = glm::dot(m_currentPosition, m_desiredPosition);
					//glm::vec3 rotationAxis = glm::cross(m_currentPosition, m_desiredPosition); // (m_currentRotation = get orientation and convert to unit vectors  , m_desiredPosition - m_currentPosition <-normalise this)
					
					float angleBetween = glm::dot(glm::normalize(m_currentRotation), glm::normalize(m_desiredPosition - m_currentPosition));
					glm::vec3 rotationAxis = glm::cross(glm::normalize(m_currentRotation), glm::normalize(m_desiredPosition - m_currentPosition));
					
					float s = glm::sqrt((1 + angleBetween) * 2);
					float invs = 1 / s;
					glm::quat quaternion = glm::quat(s * 0.5f, glm::vec3(rotationAxis) * invs);
					glm::vec3 euler = glm::degrees(glm::eulerAngles(glm::normalize(quaternion)));

					m_desiredRotation = euler;

					/*float dot = glm::dot(m_currentPosition, m_desiredPosition);
					float magnitude = glm::sqrt((m_currentPosition.x * m_currentPosition.x) + (m_currentPosition.y * m_currentPosition.y) + (m_currentPosition.z * m_currentPosition.z));
					float angle = glm::acos(dot / magnitude);

					m_desiredRotation = m_currentRotation * angle;*/

					//LogInfo("Going to: {0}, {1}, {2}", m_desiredPosition.x, m_desiredPosition.y, m_desiredPosition.z);
					//LogInfo("Rotating to: {0}, {1}, {2}", m_desiredRotation.x, m_desiredRotation.y, m_desiredRotation.z);
				}
				else 
				{
					m_desiredPosition = m_currentPosition;
					m_desiredRotation = m_currentRotation;
				}			
			}

			// Send messages to other (velocity/rigidbody if physics implemented) components
			ComponentMessage posMsg(ComponentMessageType::AIPositionSet, m_desiredPosition);
			//ComponentMessage rotMsg(ComponentMessageType::AIRotationSet, m_desiredRotation); // Never got rotation working

			sendMessage(posMsg);
			//endMessage(rotMsg); // 

			m_currentPosition = m_owner->getComponent<PositionComponent>()->getCurrentPosition();
			m_currentRotation = m_owner->getComponent<PositionComponent>()->getCurrentRotation();
		};

		void onEvent(Event& e) override {};

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}

		//! Adds a waypoint \param x x-axis coordinate \param y y-axis coordinate \param z z-axis coordinate
		void addWaypoint(float x, float y, float z)
		{
			m_waypoints.push_back(glm::vec3(x, y, z));
		}

		//! Adds a waypoint to the front \param x x-axis coordinate \param y y-axis coordinate \param z z-axis coordinate
		void addWaypointFront(float x, float y, float z)
		{
			m_waypoints.push_front(glm::vec3(x, y, z));
		}

		//! Adds a path \param x x-axis coordinate \param y y-axis coordinate \param z z-axis coordinate
		void addPath(float x, float y, float z)
		{
			m_path.push_back(glm::vec3(x, y, z));
		}

		//! Changes a path at given indes \param index path index \param x x-axis coordinate \param y y-axis coordinate \param z z-axis coordinate
		void changePath(int index, float x, float y, float z)
		{
			m_path[index] = glm::vec3(x, y, z);
		}

		// Removes last path
		void removePathBack()
		{
			m_path.pop_back();
		}

		//! Gets path X coordinate \param index path index
		float pathPosX(int index)
		{
			return m_path[index].x;
		}

		//! Gets path Y coordinate \param index path index
		float pathPosY(int index)
		{
			return m_path[index].y;
		}

		//! Gets path Z coordinate \param index path index
		float pathPosZ(int index)
		{
			return m_path[index].z;
		}

		//! Gets number of waypoints \return number of waypoints
		int numWaypoints() { return m_waypoints.size(); }

		//! Gets current path number \return path index
		int currentPathNum() 
		{ 
			int result;

			auto iter = std::find(m_path.begin(), m_path.end(), m_currentPath);

			if (iter != m_path.end())
			{
				result = std::distance(m_path.begin(), iter);
			}
		
			return result;
		}

		//! Gets total paths size \return number of paths
		int numPath() { return m_path.size(); }

		//! Gets path type \return path type
		int pathType() 
		{ 
			return (int)m_pathType; 
		}

		//! Gets path type name \return path type name
		std::string getPathTypeName()
		{
			return m_pathTypeName;
		}

		//! Gets path XYZ coordinates \return path XYZ
		std::vector<glm::vec3>& getPathWaypoints()
		{
			return m_path;
		}

		//! Sets lua function \param ref new LuaRef
		void setLuaFunction(const luabridge::LuaRef& ref)
		{
			m_lua = std::make_shared<luabridge::LuaRef>(ref);
		}
		
		//! Updates from Lua script
		void luaUpdate()
		{
			if (m_lua)
			{
				try
				{
					(*m_lua)(this);
				}
				catch(luabridge::LuaException const& e)
				{
					LogError("LUA EXCEPTION");
					std::cout << e.what() << std::endl;
				}
			}
		}

		//! Registers a class in Lua
		void registerClass()
		{
			luabridge::getGlobalNamespace(Application::getInstance().getLuaState())
				.beginClass<AIComponent>("AI")
				.addFunction("numWaypoints", &AIComponent::numWaypoints)
				.addFunction("addWaypoint", &AIComponent::addWaypoint)
				.addFunction("addWaypointFront", &AIComponent::addWaypointFront)
				.addFunction("currentPathNum", &AIComponent::currentPathNum)
				.addFunction("pathPosX", &AIComponent::pathPosX)
				.addFunction("pathPosY", &AIComponent::pathPosY)
				.addFunction("pathPosZ", &AIComponent::pathPosZ)
				.addFunction("numPath", &AIComponent::numPath)
				.addFunction("pathType", &AIComponent::pathType)
				.endClass();
		}

		//! Executes Lua file \param luaScriptPath directory \param tableName table name \param luaFuncName function name
		void doFile(std::string luaScriptPath, std::string tableName, std::string luaFuncName)
		{
			if (luaL_dofile(Application::getInstance().getLuaState(), luaScriptPath.c_str()) == 0)
			{
				luabridge::LuaRef table = luabridge::getGlobal(Application::getInstance().getLuaState(), tableName.c_str());

				if (table.isTable())
				{
					if (table[luaFuncName].isFunction())
					{
						this->setLuaFunction(table[luaFuncName]);
					}
				}
			}
			else
			{
				LogError("Couldn't load {0}", luaScriptPath);
			}
		}
		
		//! Sets stop distance \param dist new stop distance
		void setStopDist(float dist) { m_stopSize = dist; }

		//! Gets stop distance \return current stop distance
		float getStopDist()
		{
			return m_stopSize;
		}

		//! Set AI type \param type new AI type
		void setAiType(std::string type) { m_aiType = type; }

		//! Gets AI type \return current AI type
		std::string& getAiType() { return m_aiType; }

		//! Sets script name \param scr new script name
		void setScriptName(std::string scr) { m_scriptName = scr; }

		//! Gets script name \return current script name
		std::string& getScriptName() { return m_scriptName; }

		//! Destructor
		~AIComponent()
		{
			
		}
	};
}