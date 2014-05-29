#include "pch.h"
#include "GameManager.h"
#include <sstream>

namespace SST
{
	std::shared_ptr<GameObjectManager> GameManager::g_object_manager;
	std::shared_ptr<Blackboard> GameManager::m_blackboard;
	std::shared_ptr<EventSystem> GameManager::m_event_system;
	std::shared_ptr<GameManager> GameManager::ptr_gameManager;
	bool GameManager::instanceFlag;

	std::shared_ptr<GameManager> GameManager::GetInstance()
	{
		if(!ptr_gameManager) {
			ptr_gameManager = std::shared_ptr<GameManager>(new GameManager());
			instanceFlag = true;
		}

		return ptr_gameManager;
	}


	void GameManager::Initialize()
	{
		g_object_manager = std::make_shared<GameObjectManager>();
		m_blackboard = std::make_shared<Blackboard>();
		m_event_system = std::make_shared<EventSystem>();
	}

	std::shared_ptr<GameObjectManager> GameManager::GetObjectManager()
	{
		return g_object_manager;
	}


	std::shared_ptr<Blackboard> GameManager::GetBlackboard()
	{
		return m_blackboard;
	}

	std::shared_ptr<EventSystem> GameManager::GetEventSystem()
	{
		return m_event_system;
	}

	void GameManager::Update(float timeDelta)
	{
		double acc_x = 0, acc_y = 0, acc_z = 0, h_angle = 0, v_angle = 0, gyro_x = 0, gyro_y = 0, gyro_z = 0;

		if(m_blackboard->get<bool>("accelerometer"))
		{
			acc_x = m_blackboard->get<double>("acc_x");
			acc_y = m_blackboard->get<double>("acc_y");
			acc_z = m_blackboard->get<double>("acc_z");
			h_angle = m_blackboard->get<double>("h_angle");
			v_angle = m_blackboard->get<double>("v_angle");
		}

		if(m_blackboard->get<bool>("gyrometer"))
		{
			gyro_x = m_blackboard->get<double>("gyro_x");
			gyro_y = m_blackboard->get<double>("gyro_y");
			gyro_z = m_blackboard->get<double>("gyro_z");
		}

		// DEBUG PRINT
		/*std::ostringstream sstream2;
			sstream2 << "<GameManager::Update> h_angle=" << h_angle << " v_angle=" << v_angle << "\n";
		std::string s2 = sstream2.str();
		OutputDebugStringA(s2.c_str());
		*/
		// Update player  position
		DirectX::XMFLOAT2 p_pos = g_object_manager->GetByType("Player")[0]->GetPosition();
		DirectX::XMFLOAT2 p_vel;

		// Left/Right tilt
		p_vel.x = float(h_angle * 10);

		// Back/Front tilt
		p_vel.y = float(-v_angle * 10);

		// DEBUG PRINT
		/*std::ostringstream sstream;
			sstream << "<GameManager::Update> vel.x=" << p_vel.x << " vel.y=" << p_vel.y << "\n";
		std::string s = sstream.str();
		OutputDebugStringA(s.c_str());
		*/
		p_pos.x += p_vel.x * timeDelta;
		p_pos.y += p_vel.y * timeDelta;

		if(p_pos.x < 0.0f)
			p_pos.x = 0.0f;
		else if(p_pos.x > 370.0f)
			p_pos.x = 370.0f;
		if(p_pos.y < 0.0f)
			p_pos.y = 0.0f;
		else if(p_pos.y > 700.0f)
			p_pos.y = 700.0f;

		g_object_manager->GetByType("Player")[0]->SetVelocity(p_vel);
		g_object_manager->GetByType("Player")[0]->SetPosition(p_pos);

		// DEBUG PRINT
		/*
		std::ostringstream sstream;
			sstream << "<GameManager::Update> vel.x=" << p_vel.x << " vel.y=" << p_vel.y << "\n";
			sstream << "pvel.x = " << g_object_manager->GetByType("Player")[0]->GetVelocity().x << "\n";
		std::string s = sstream.str();
		OutputDebugStringA(s.c_str());*/

	}

	void GameManager::PrintEvent(std::string text)
	{
		m_blackboard->set<std::string>("event", text);
	}
}