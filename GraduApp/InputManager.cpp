#include "pch.h"
#include "InputManager.h"
#include "GameManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace SST
{
	InputManager::InputManager()
	{
		m_orientation = Windows::Devices::Sensors::OrientationSensor::GetDefault();
		m_orientation->ReportInterval = 50;
		GameManager::GetBlackboard()->set<bool>("orientation", true);

		m_accelerometer = Windows::Devices::Sensors::Accelerometer::GetDefault();
		m_accelerometer->ReportInterval = 50;
		GameManager::GetBlackboard()->set<bool>("accelerometer", true);

		m_gyrometer = Windows::Devices::Sensors::Gyrometer::GetDefault();
		GameManager::GetBlackboard()->set<bool>("gyrometer", true);

		m_filtered_angle[0] = 0.0f;
		m_filtered_angle[1] = 0.0f;

		// Initial gravity
		m_accReading = m_accelerometer->GetCurrentReading();
		m_gravity.x = m_accReading->AccelerationX;
		m_gravity.y = m_accReading->AccelerationY;
		m_gravity.z = m_accReading->AccelerationZ;
	}

	void InputManager::Update(float delta)
	{
		double acc_x = 0, acc_y = 0, acc_z = 0;
		double old_acc_x = GameManager::GetBlackboard()->get<double>("acc_x");
		double old_acc_y = GameManager::GetBlackboard()->get<double>("acc_y");
		double old_acc_z = GameManager::GetBlackboard()->get<double>("acc_z");
		double gyro_x = 0, gyro_y = 0, gyro_z = 0;
		double h_angle = 0, v_angle = 0;
		float acc_moment = 0.0f;

		if (m_accelerometer != nullptr)
		{
			try
			{
				m_accReading = m_accelerometer->GetCurrentReading();
				
				acc_x = LowPassFilter(old_acc_x, m_accReading->AccelerationX);
				acc_y = LowPassFilter(old_acc_y, m_accReading->AccelerationY);
				acc_z = LowPassFilter(old_acc_z, m_accReading->AccelerationZ);
				
				acc_moment = sqrt(pow(acc_x-old_acc_x, 2)+pow(acc_y-old_acc_y, 2)+pow(acc_z-old_acc_z, 2));

				float alpha = 0.8;
				m_gravity.x = alpha * m_gravity.x + (1 - alpha) * m_accReading->AccelerationX;
				m_gravity.y = alpha * m_gravity.y + (1 - alpha) * m_accReading->AccelerationY;
				m_gravity.z = alpha * m_gravity.z + (1 - alpha) * m_accReading->AccelerationZ;

				double acc_x2 = (double)(acc_x*acc_x);
				double acc_y2 = (double)(acc_y*acc_y);
				double acc_z2 = (double)(acc_z*acc_z);
				h_angle = atan2(acc_x, (sqrt(acc_y2 + acc_z2))) * 180.0 / M_PI;
				v_angle = atan2(acc_y, (sqrt(acc_x2 + acc_z2))) * 180.0 / M_PI;
			}
			catch(Platform::Exception^ e)
			{
				// there is a bug tracking this issue already
				// we need to remove this try\catch once the bug # 158858 hits our branch
				// For now, to make this App work, catching the exception
				// The reverting is tracked by WP8 # 159660
			}
		}

		if (m_gyrometer != nullptr)
		{
			try
			{
				m_gyroReading = m_gyrometer->GetCurrentReading();

				gyro_x = m_gyroReading->AngularVelocityX * delta;
				gyro_y = m_gyroReading->AngularVelocityY * delta;
				gyro_z = m_gyroReading->AngularVelocityZ * delta;
				
				// Filtered pitch angle: 98% gyro_muutos+vanha kulma + 2% acc_x kulma
				m_filtered_angle[0] = 0.8 * (m_filtered_angle[0] - gyro_x * delta) + 0.2 * v_angle;
				// Filtered yaw angle: 98% gyro_muutos+vanha kulma + 2% acc_x kulma
				m_filtered_angle[1] = 0.8 * (m_filtered_angle[1] - gyro_y * delta) + 0.2 * h_angle;
			}
			catch(Platform::Exception^ e)
			{
				// there is a bug tracking this issue already
				// we need to remove this try\catch once the bug # 158858 hits our branch
				// For now, to make this App work, catching the exception
				// The reverting is tracked by WP8 # 159660
			}
		}

		if (m_orientation != nullptr)
		{
			//m_orientationReading = m_orientation->GetCurrentReading();
			//double(m_orientationReading->Quaternion->X);
		}
		
		GameManager::GetBlackboard()->set<double>("acc_x", acc_x);
		GameManager::GetBlackboard()->set<double>("acc_y", acc_y);
		GameManager::GetBlackboard()->set<double>("acc_z", acc_z);
		GameManager::GetBlackboard()->set<double>("acc_moment", acc_moment);
		GameManager::GetBlackboard()->set<double>("gyro_x", acc_x - gyro_x);
		GameManager::GetBlackboard()->set<double>("gyro_y", acc_y - gyro_y);
		GameManager::GetBlackboard()->set<double>("gyro_z", acc_z - gyro_z);
		GameManager::GetBlackboard()->set<double>("linear_x", acc_x - m_gravity.x);
		GameManager::GetBlackboard()->set<double>("linear_y", acc_y - m_gravity.y);
		GameManager::GetBlackboard()->set<double>("linear_z", acc_z - m_gravity.z);
		GameManager::GetBlackboard()->set<double>("h_angle", m_filtered_angle[1]); //h_angle);
		GameManager::GetBlackboard()->set<double>("v_angle", m_filtered_angle[0]); //v_angle);
	}
	
	double InputManager::DoubleRound(double _number)
	{

		_number = _number < 0.0 ? (ceil(_number*10 - 0.5) / 10) : (floor(_number*10 + 0.5) / 10);
		return _number;
	}

	double InputManager::LowPassFilter(double from, double to, float weight)
	{
		double filtered = from * (1.0f - weight) + to * weight;
		return filtered;
	}
}
