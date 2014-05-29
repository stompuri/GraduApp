#pragma once
#include "Gesture.h"
#include <list>

namespace SST
{
	class GestureRecognizer
	{
	public:
		GestureRecognizer();
		~GestureRecognizer();

		void Update();

	private:
		float RoundFloat(float _value);
		bool Compare();
		bool CompareHelper(std::shared_ptr<Gesture> gesture1, std::shared_ptr<Gesture> gesture2);

		std::shared_ptr<Gesture> ptr_gesture;
		std::list<std::shared_ptr<Gesture>> m_gestures;

		bool m_released;
		bool m_recognizing; // Recognition going on
	};
}
