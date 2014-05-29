#pragma once

namespace SST
{
	class GameObject
	{
	public:
		GameObject();

		void SetObjectId(unsigned int _id); // Sets the objectID
		bool GetDeleteStatus(); // Return true/false telling if the object has been flagged for deletion
		DirectX::XMFLOAT2 GetPosition() { return m_position; }
		DirectX::XMFLOAT2 GetVelocity() { return m_velocity; }
		void SetPosition(DirectX::XMFLOAT2 _pos) { m_position = _pos; }
		void SetVelocity(DirectX::XMFLOAT2 _vel) { m_velocity = _vel; }

		void EventJump();
		void EventDown();
		
	private:
		unsigned int			i_id; // id that gameObjectManager gave to the object
		float					f_rotation; // What's the rotation status of this object
		DirectX::XMFLOAT2		m_velocity; // Speed (x,y) of this object
		DirectX::XMFLOAT2		m_position; // Position (x,y) of this object

		bool					b_to_be_deleted; // flag telling if the object should be deleted (in game loop)
	};
}