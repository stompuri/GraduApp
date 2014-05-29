#include "pch.h"
#include "GestureRecognizer.h"
#include "GameManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace SST
{
	GestureRecognizer::GestureRecognizer()
	{
		ptr_gesture = std::shared_ptr<Gesture>(new Gesture());
		m_released = true;
		m_recognizing = false;
	}


	GestureRecognizer::~GestureRecognizer()
	{
	}

	void GestureRecognizer::Update()
	{
		double testi = GameManager::GetBlackboard()->get<double>("v_angle");
		// Check if gesture A is noticed
		if(GameManager::GetBlackboard()->get<double>("v_angle") > 80.0)
		{
			//GameManager::GetEventSystem()->ExecuteSignal(1);
		}


		// Add gesture if touch noticed
		if(GameManager::GetBlackboard()->get<bool>("touch"))
		{	
			m_released = false;
			float acc_x = float(GameManager::GetBlackboard()->get<double>("linear_x"));
			float acc_y = float(GameManager::GetBlackboard()->get<double>("linear_y"));
			float acc_z = float(GameManager::GetBlackboard()->get<double>("linear_z"));
			DirectX::XMFLOAT3 vec(RoundFloat(acc_x), RoundFloat(acc_y), RoundFloat(acc_z));
			
			// Filter
			float vec_size = sqrt(acc_x*acc_x+acc_y*acc_y+acc_z*acc_z);
			if(vec_size > 0.2)
				ptr_gesture->AddPoint(vec);
		}

		if(!GameManager::GetBlackboard()->get<bool>("touch"))
		{
			if(!m_released)
			{
				ptr_gesture->kMeanCluster();
				m_gestures.push_back(ptr_gesture);
				ptr_gesture = std::shared_ptr<Gesture>(new Gesture());
				m_released = true;

				// DEBUG PRINT
				std::ostringstream sstream2;
					sstream2 << "****************************\n";
				std::string s2 = sstream2.str();
				OutputDebugStringA(s2.c_str());
			} else {
				
				// Recognition going on...
				if(m_recognizing)
				{
					/*
					// Check if the recorded gesture finished -> try to recognize it
					if(GameManager::GetBlackboard()->get<double>("acc_moment") < 0.1)
					{
						m_recognizing = false;
						// Tarkista tunnistetaanko nauhoitettua elettä
						bool result = Compare();

						// Clear the recorded gesture
						ptr_gesture = std::shared_ptr<Gesture>(new Gesture());
						return;
					}

					// Save acc vector to ongoing recognition pattern
					float acc_x = float(GameManager::GetBlackboard()->get<double>("acc_x"));
					float acc_y = float(GameManager::GetBlackboard()->get<double>("acc_y"));
					float acc_z = float(GameManager::GetBlackboard()->get<double>("acc_z"));

					DirectX::XMFLOAT3 vec(RoundFloat(acc_x), RoundFloat(acc_y), RoundFloat(acc_z));
					ptr_gesture->AddPoint(vec);
					*/
				} else {
				// Not recognizing...
					// Check the accelerometer moments change amount to see if gesture started
					if(GameManager::GetBlackboard()->get<double>("acc_moment") > 0.3)
					{
						m_recognizing = true;
					}
				}
			}
		}
		// 1) Check if there's a pause in a movement to detect start

		// 2) If there was a pause, start recording the movement

		// 2.1) Check if the recorded move pattern matches with any pattern in database
	}

	float GestureRecognizer::RoundFloat(float _value)
	{
		if(_value < 0)
		{
			_value = ceilf(_value*1000 - 0.5f) / 1000;
		} else {
			_value = floorf(_value*1000 + 0.5f) / 1000;
		}
		//_value = _value < 0.0f ? (ceil(_value*10 - 0.5f) / 10) : (floor(_value*10 + 0.5f) / 10);
		return _value;
	}

	bool GestureRecognizer::Compare()
	{
		// Go through saved gestures
		for(std::list<std::shared_ptr<Gesture>>::iterator it = m_gestures.begin(); it != m_gestures.end(); ++it)
		{
			// Compare ongoing gesture to this saved one
			bool test = CompareHelper(ptr_gesture, *it);
			if(test)
			{
				ptr_gesture = std::shared_ptr<Gesture>(new Gesture());
				return true;
			}
		}
		return false;
	}
	bool GestureRecognizer::CompareHelper(std::shared_ptr<Gesture> gesture1, std::shared_ptr<Gesture> gesture2)
	{
		for(std::list<DirectX::XMFLOAT3>::iterator it1 = gesture1->GetGesture().begin(); it1 != gesture1->GetGesture().end(); ++it1)
		{
			for(std::list<DirectX::XMFLOAT3>::iterator it2 = gesture2->GetGesture().begin(); it2 != gesture2->GetGesture().end(); ++it2)
			{
				float _x = it1->x;
				float _y = it1->y;
				float _z = it1->z;

				float _x2 = it2->x;
				float _y2 = it2->y;
				float _z2 = it2->z;
			}
		}

		return true;
	}
}
