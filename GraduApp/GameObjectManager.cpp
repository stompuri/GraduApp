#include "pch.h"
#include "GameObjectManager.h"
#include "Player.h"

#include <string>


namespace SST
{
	GameObjectManager::GameObjectManager() : object_id(0)
	{
		// Create player object and add it into object managers list
		std::shared_ptr<Player> _player(new Player());
		_player->SetObjectId(Add("Player", _player));
	}

	GameObjectManager::~GameObjectManager()
	{
	}

	unsigned int GameObjectManager::Add(std::string _name, std::shared_ptr<GameObject> _ptr_gameObject)
	{
		
		std::string _idname = _name.append("_" + std::to_string(object_id) + "_");
		m_objects.insert(std::pair<std::string, std::shared_ptr<GameObject>>(_idname, _ptr_gameObject));
		object_id++;
		
		return object_id-1;
	}

	void GameObjectManager::Insert(std::string _idname, std::shared_ptr<GameObject> _object)
	{
		m_objects.insert(std::pair<std::string,std::shared_ptr<GameObject>>(_idname, _object));
	}
	
	std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetByType(std::string type)
	{
		std::vector<std::shared_ptr<GameObject>> _retu;
		
		std::map<std::string, std::shared_ptr<GameObject>>::iterator itr = m_objects.begin();
		while(itr != m_objects.end() ) {
			if(!itr->second->GetDeleteStatus()) {
				unsigned int found = itr->first.find(type);
				if (found != std::string::npos)
				{
					_retu.push_back(itr->second);
				}
			}
			itr++;
		}
		
		return _retu;
	}
}

/*
namespace SST
{
	GameObjectManager::GameObjectManager() : object_id(0)
	{
		//std::shared_ptr<Player> _player(new Player());
		//_player->SetObjectId(Add("Player", _player));
	}
	
	unsigned int GameObjectManager::Add(std::wstring _name, GameObject^ _object)
	{
		std::wstring _idname = L"_" + std::to_wstring(object_id) + L"_";
		m_objects.insert(std::pair<std::wstring,GameObject^>(_idname, _object));
		object_id++;
		return object_id-1;
	}
	
	void GameObjectManager::Insert(std::wstring _idname, GameObject^ _object)
	{
		m_objects.insert(std::pair<std::wstring,GameObject^>(_idname, _object));
	}
	
	std::vector<GameObject^> GameObjectManager::GetByType(std::wstring type)
	{
		std::vector<GameObject^> _retu;
		
		std::map<std::wstring, GameObject^>::iterator itr = m_objects.begin();
		while(itr != m_objects.end() ) {
			if(!itr->second->GetDeleteStatus()) {
				unsigned int found = itr->first.find(type);
				if (found != std::string::npos)
				{
					_retu.push_back(itr->second);
				}
			}
			itr++;
		}
		
		return _retu;
	}
}
*/