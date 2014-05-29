#pragma once
#include "GameObject.h"
#include <list>
#include <vector>
#include <map>

namespace SST
{
	class GameObjectManager
	{
	public:
		GameObjectManager();
		~GameObjectManager();

		unsigned int Add(std::string _name, std::shared_ptr<GameObject> _object); // Add game object into game object map (all objects in the game world) and returns the id given to the object
		void Insert(std::string _name, std::shared_ptr<GameObject> _object); // Insert game object into game object map with predefined id (not using increasing counter)

		std::vector<std::shared_ptr<GameObject>> GetByType(std::string _type); // Return a list of handles to all objects with certain type

		private:
		unsigned int object_id; // Increasing counter for the object IDs that are saved in the map
		std::map<std::string, std::shared_ptr<GameObject>> m_objects; // Maping all the game objects in the game world. Using shared pointers.
	}; 
}

/*
namespace SST
{
	public ref class GameObjectManager sealed
	{
	internal:
		GameObjectManager();

		unsigned int Add(std::wstring _name, GameObject^ _object); // Add game object into game object map (all objects in the game world) and returns the id given to the object
		
		void Insert(std::wstring _name, GameObject^ _object); // Insert game object into game object map with predefined id (not using increasing counter)
		
		std::vector<GameObject^> GetByType(std::wstring _type); // Return a list of handles to all objects with certain type
		
	protected private:
		unsigned int object_id; // Increasing counter for the object IDs that are saved in the map
		std::map<std::wstring, GameObject^> m_objects;
		//std::map<std::string, std::shared_ptr<GameObject>> m_objects; // Maping all the game objects in the game world. Using shared pointers.
	}; 
}
*/