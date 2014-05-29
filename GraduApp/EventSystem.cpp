#include "pch.h"
#include "EventSystem.h"
#include <boost/bind.hpp>
#include "Player.h"
#include "GameManager.h"
#include "Blackboard.h"

namespace SST
{
	EventSystem::EventSystem()
	{
		std::shared_ptr<GameObject> m_player = GameManager::GetObjectManager()->GetByType("Player").front();
		SigA.connect(boost::bind(&GameObject::EventJump, m_player));
		SigB.connect(boost::bind(&GameObject::EventDown, m_player));
		GameManager::GetBlackboard()->set<std::string>("event", "template");
	}

	void EventSystem::ExecuteSignal(unsigned int id)
	{
		switch(id)
		{
		case 1:
			SigA();
			break;
		case 2:
			SigB();
			break;
		default:
			break;
		}
	}
}