#include "pch.h"
#include "GameObject.h"
#include "GameManager.h"

namespace SST
{
	GameObject::GameObject() : i_id(0), f_rotation(0.0f), m_velocity(0.0f, 0.0f), m_position(1.0f, 1.0f), b_to_be_deleted(false)
	{
	}

	void GameObject::SetObjectId(unsigned int _id)
	{
		i_id = _id;
	}

	bool GameObject::GetDeleteStatus()
	{
		return b_to_be_deleted;
	}

	void GameObject::EventJump()
	{
		GameManager::PrintEvent("Jump");
	}

	void GameObject::EventDown()
	{
		GameManager::PrintEvent("Down");
	}
}
