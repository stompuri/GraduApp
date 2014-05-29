#pragma once
#include "GameObjectManager.h"
#include "Blackboard.h"
#include "EventSystem.h"

namespace SST
{
	class GameManager
	{
	public:
		~GameManager() { instanceFlag = false; }
		static std::shared_ptr<GameManager> GetInstance();
		static void Initialize();

		static void Update(float timeDelta); // Update game state
		static void PrintEvent(std::string text); // Text for debug printing

		static std::shared_ptr<GameObjectManager> GetObjectManager();
		static std::shared_ptr<Blackboard> GetBlackboard();
		static std::shared_ptr<EventSystem> GetEventSystem();

	private:
		GameManager() { /* private constructor */ };
		static std::shared_ptr<GameManager> ptr_gameManager;
		static bool instanceFlag;

		static std::shared_ptr<GameObjectManager> g_object_manager;
		static std::shared_ptr<Blackboard> m_blackboard;
		static std::shared_ptr<EventSystem> m_event_system;

	};
}