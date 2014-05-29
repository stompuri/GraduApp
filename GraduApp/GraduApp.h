#pragma once

#include "pch.h"
#include "GameRenderer.h"
#include "GameManager.h"
#include "InputManager.h"
#include "GestureRecognizer.h"

ref class GraduApp sealed : public Windows::ApplicationModel::Core::IFrameworkView
{
public:
	GraduApp();
	
	// IFrameworkView Methods.
	virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
	virtual void SetWindow(Windows::UI::Core::CoreWindow^ window);
	virtual void Load(Platform::String^ entryPoint);
	virtual void Run();
	virtual void Uninitialize();

protected:
	// Event Handlers.
	void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView, Windows::ApplicationModel::Activation::IActivatedEventArgs^ args);
	void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ args);
	void OnResuming(Platform::Object^ sender, Platform::Object^ args);
	void OnWindowClosed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::CoreWindowEventArgs^ args);
	void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
	void OnPointerPressed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args);
	void OnPointerMoved(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args);
	void OnPointerReleased(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args);

private:
	SST::GameRenderer^ m_renderer;
	SST::InputManager^ m_input_manager;
	std::shared_ptr<SST::GameManager> m_game;
	std::shared_ptr<SST::GestureRecognizer> m_gesture_recognizer;

	bool m_windowClosed;
	bool m_windowVisible;

	Windows::Foundation::IAsyncAction^ m_thread_input;
	Windows::Foundation::IAsyncAction^ m_thread_gestures;

	Windows::System::Display::DisplayRequest^ m_displayRequest;
};

ref class Direct3DApplicationSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource
{
public:
	virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
};