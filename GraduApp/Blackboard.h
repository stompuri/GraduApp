// Blackboard-luokka yleiseen datan jakoon
// Syötä data: GameManager::GetBlackboard()->set<TYYPPI>("ID-nimi(string)", muuttuja);
// Lue data: GameManager::GetBlackboard()->get<TYYPPI>("ID-nimi(string)");

#pragma once
#include "ParameterBase.h"
#include <string>
#include <map>
#include <sstream>

namespace SST
{
	class Blackboard
	{
	public:
		Blackboard() { };

		template <typename T>
		void set(std::string id, T value);
		template <typename T>
		T& get(std::string searched);

	private:
		std::map<std::string, ParameterBase*> blackboard_map;
	};

	template <typename T>
	T& Blackboard::get(std::string searched) {
		if(blackboard_map.count(searched) == 0)
		{
			// Not found, add and return the added
			set<T>(searched, T(0));
		}

		std::map<std::string, ParameterBase*>::iterator it = blackboard_map.find(searched);
		
		/*// DEBUG PRINT
		std::ostringstream sstream;
			sstream << "<Blackboard::get> " << it->first << ": " << it->second->get<T>() << "\n";
		std::string s = sstream.str();
		OutputDebugStringA(s.c_str());*/
		
		return it->second->get<T>();
	}

	template <typename T>
	void Blackboard::set(std::string id, T value)
	{
		/*// DEBUG PRINT
		std::ostringstream sstream;
			sstream << "<Blackboard::set> " << id << " " << value << "\n";
		std::string s = sstream.str();
		OutputDebugStringA(s.c_str());*/

		// if object for id is found, update the value, otherwise insert new object int blackboard
		if(blackboard_map.count(id) > 0)
		{
			std::map<std::string, ParameterBase*>::iterator it = blackboard_map.find(id);
			it->second->setValue<T>(value);
		} else {
			ParameterBase *object = new Parameter<T>(value);
			blackboard_map.insert(std::pair<std::string, ParameterBase*>(id, object));
		}

	}
}
