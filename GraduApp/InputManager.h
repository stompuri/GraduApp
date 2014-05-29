// InputManager-luokka
// Vastaa syötteen lukemisesta, käsittelystä ja tallentamisesta Blackboardille

#pragma once

namespace SST
{
	ref class InputManager sealed
	{
	public:
		InputManager();

		// InputManagers update collects the input from all the input sources
		void Update(float delta);

	private:
		Windows::Devices::Sensors::Accelerometer^ m_accelerometer;
		Windows::Devices::Sensors::AccelerometerReading^ m_accReading;
		
		Windows::Devices::Sensors::Gyrometer^ m_gyrometer;
		Windows::Devices::Sensors::GyrometerReading^ m_gyroReading;

		Windows::Devices::Sensors::OrientationSensor^ m_orientation;
		Windows::Devices::Sensors::OrientationSensorReading^ m_orientationReading;

		double DoubleRound(double _number);
		double LowPassFilter(double from, double to, float weight = 0.4f);
		float CompFilter(float acc, float gyr_rate, float delta);

		float m_filtered_angle[2];
		DirectX::XMFLOAT3 m_gravity;
	};
}