#pragma once
#include <boost/signals2/signal.hpp>

namespace SST
{
	class EventSystem
	{
	public:
		EventSystem();

		void RegisterToSignal();
		void ExecuteSignal(unsigned int id);

	private:
		boost::signals2::signal<void ()> SigA;
		boost::signals2::signal<void ()> SigB;
	};
}