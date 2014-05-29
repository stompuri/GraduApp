#include "pch.h"
#include "GraduApp.h"
#include "BasicTimer.h"
#include "Blackboard.h"

#include <sstream>

using namespace Windows::System::Threading;

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::System;
using namespace Windows::System::Threading;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace concurrency;

	GraduApp::GraduApp() :
		m_windowClosed(false),
		m_windowVisible(true)
	{
	}

	void GraduApp::Initialize(CoreApplicationView^ applicationView)
	{
		applicationView->Activated +=
			ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &GraduApp::OnActivated);

		CoreApplication::Suspending +=
			ref new EventHandler<SuspendingEventArgs^>(this, &GraduApp::OnSuspending);

		CoreApplication::Resuming +=
			ref new EventHandler<Platform::Object^>(this, &GraduApp::OnResuming);

		m_game = SST::GameManager::GetInstance();
		m_game->Initialize();
		m_renderer = ref new SST::GameRenderer();
		m_input_manager = ref new SST::InputManager();
		m_gesture_recognizer = std::make_shared<SST::GestureRecognizer>();

		// Disable auto screen lock
		m_displayRequest = ref new Display::DisplayRequest();
		m_displayRequest->RequestActive();
	}

	void GraduApp::SetWindow(CoreWindow^ window)
	{
		window->VisibilityChanged +=
			ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &GraduApp::OnVisibilityChanged);

		window->Closed += 
			ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &GraduApp::OnWindowClosed);

		window->PointerPressed +=
			ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &GraduApp::OnPointerPressed);

		window->PointerMoved +=
			ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &GraduApp::OnPointerMoved);

		window->PointerReleased +=
			ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &GraduApp::OnPointerReleased);

		m_renderer->Initialize(CoreWindow::GetForCurrentThread());
	}

	void GraduApp::Load(Platform::String^ entryPoint)
	{
	}

	void GraduApp::Run()
	{
		m_game->GetBlackboard()->set<bool>("game_running", true);

		BasicTimer^ timer = ref new BasicTimer();
		BasicTimer^ timer_update = ref new BasicTimer();
		float render_time = float(1.0f / 60.0f); // forcing the main loop render-task into this execution interval (60fps)
		float update_time = float(1.0f / 30.0f); // forcing the main loop update-tasks into this execution interval (30fps)
		float execution_time = 0.0f;

		// Timer for debugging
		BasicTimer^ timer_debug = ref new BasicTimer();

		// Thread for handling user input
		auto thread_input = ref new WorkItemHandler(
			[this](IAsyncAction^ thread_input)
		{
			BasicTimer^ timer = ref new BasicTimer();
			BasicTimer^ timer_update = ref new BasicTimer();
			float update_time = 0.05f; // forcing the main loop into this execution interval (20Hz)

			while (!m_windowClosed)
			{
				// Check how long one round took and force the wanted execution interval
				// (wait if needed and reset the timer)
				timer->Update();
				if(timer->Delta < update_time)
				{
					wait(int(1000*(update_time - timer->Delta)));
					timer->Update();
				}
				timer_update->Update();
				// Process input
				m_input_manager->Update(timer_update->Delta);
			}
		});
		m_thread_input = ThreadPool::RunAsync(thread_input, WorkItemPriority::Low); // A reference to the work item is cached so that we can trigger a cancellation if needed

		// Thread for handling gesture recognition
		auto thread_gestures = ref new WorkItemHandler(
			[this](IAsyncAction^ thread_gestures)
		{
			BasicTimer^ timer = ref new BasicTimer();
			float update_time = 0.1f; // forcing the main loop into this execution interval (10Hz)

			while (!m_windowClosed)
			{
				// Check how long one round took and force the wanted execution interval
				// (wait if needed and reset the timer)
				timer->Update();
				if(timer->Delta < update_time)
				{
					wait(int(1000*(update_time - timer->Delta)));
					timer->Update();
				}

				// Do gesture recognition
				m_gesture_recognizer->Update();
			}
		});
		m_thread_gestures = ThreadPool::RunAsync(thread_gestures, WorkItemPriority::Low); // A reference to the work item is cached so that we can trigger a cancellation if needed
		
		while (!m_windowClosed)
		{
			if (m_windowVisible)
			{
				// Process system events
				CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

				// Update game state - Try to keep constant
				timer_update->Update();
				execution_time += timer_update->Delta; 
				while(execution_time >= update_time)
				{
					// FPS PRINT
					timer_debug->Update();
					m_game->GetBlackboard()->set<float>("fps", std::floor(1.0f / timer_debug->Delta + 0.5f));
					
					//testi_fps = int(std::floor(1.0f / timer_debug->Delta + 0.5f));
					m_game->Update(update_time);
					execution_time -= update_time;
				}
				
				// Check how long one round took and force the wanted execution interval
				// (wait if needed and reset the timer)
				timer->Update();
				if(timer->Delta < render_time)
				{
					wait(int(std::floor((render_time - timer->Delta)*1000 + 0.5f)));
					timer->Update();
				}

				// Debug print for FPS
				//timer_debug->Update();
				//m_game->GetBlackboard()->set<float>("fps", 1000.0f * timer_debug->Delta);

				// Render & present
				float interpolation = execution_time / update_time;
				m_renderer->Render(interpolation, render_time);
				m_renderer->Present(); // This call is synchronized to the display frame rate.	
			}
			else
			{
				CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
			}
		}
		
		if (m_thread_input != nullptr)
		{
			m_thread_input->Cancel();
		}
		if (m_thread_gestures != nullptr)
		{
			m_thread_gestures->Cancel();
		}

		m_game->GetBlackboard()->set<bool>("game_running", false);
	}

	void GraduApp::Uninitialize()
	{
		m_displayRequest->RequestRelease();
	}

	void GraduApp::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
	{
		m_windowVisible = args->Visible;
	}

	void GraduApp::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
	{
		m_windowClosed = true;
	}

	void GraduApp::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
	{
		// Insert your code here.
		m_game->GetBlackboard()->set<bool>("touch", true);
	}

	void GraduApp::OnPointerMoved(CoreWindow^ sender, PointerEventArgs^ args)
	{
		// Insert your code here.
	}

	void GraduApp::OnPointerReleased(CoreWindow^ sender, PointerEventArgs^ args)
	{
		// Insert your code here.
		m_game->GetBlackboard()->set<bool>("touch", false);
	}

	void GraduApp::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
	{
		CoreWindow::GetForCurrentThread()->Activate();
	}

	void GraduApp::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
	{
		// Save app state asynchronously after requesting a deferral. Holding a deferral
		// indicates that the application is busy performing suspending operations. Be
		// aware that a deferral may not be held indefinitely. After about five seconds,
		// the app will be forced to exit.
		SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();
		m_renderer->ReleaseResourcesForSuspending();

		create_task([this, deferral]()
		{
			// Insert your code here.

			deferral->Complete();
		});
	}
 
	void GraduApp::OnResuming(Platform::Object^ sender, Platform::Object^ args)
	{
		// Restore any data or state that was unloaded on suspend. By default, data
		// and state are persisted when resuming from suspend. Note that this event
		// does not occur if the app was previously terminated.
		 m_renderer->CreateWindowSizeDependentResources();
	}

	IFrameworkView^ Direct3DApplicationSource::CreateView()
	{
		return ref new GraduApp();
	}

	[Platform::MTAThread]
	int main(Platform::Array<Platform::String^>^)
	{
		auto direct3DApplicationSource = ref new Direct3DApplicationSource();
		CoreApplication::Run(direct3DApplicationSource);
		return 0;
	}
