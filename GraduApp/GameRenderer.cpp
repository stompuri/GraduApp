#include "pch.h"
#include "GameRenderer.h"
#include "GameManager.h"
#include "ParameterBase.h"

#include "DDSTextureLoader.h"

#include <sstream>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

namespace SST
{
	GameRenderer::GameRenderer()
	{
	}

	void GameRenderer::CreateDeviceResources()
	{
		Direct3DBase::CreateDeviceResources();

		// Create DirectXTK objects
		auto device = m_d3dDevice.Get();

		auto context = m_d3dContext.Get();
		m_sprites.reset( new SpriteBatch( context ) );

		// Load fonts
		m_font.reset( new SpriteFont( device, L"assets\\italic.spritefont" ) );

		// Load textures
		DX::ThrowIfFailed(
			CreateDDSTextureFromFile( device, L"assets\\player.dds", nullptr, m_texture1.ReleaseAndGetAddressOf() )
						 );
	}

	void GameRenderer::CreateWindowSizeDependentResources()
	{
		Direct3DBase::CreateWindowSizeDependentResources();
	}

	void GameRenderer::Update(float timeTotal, float timeDelta)
	{
		UNREFERENCED_PARAMETER( timeTotal );
		UNREFERENCED_PARAMETER( timeDelta );
	}

	void GameRenderer::Render(float alpha, float timeDelta)
	{
		int acc_x = 0, acc_y = 0, acc_z = 0;
		double h_angle = 0, v_angle = 0;
		int gyro_x = 0, gyro_y = 0, gyro_z = 0;
		int linear_x = 0, linear_y = 0, linear_z = 0;

		if(GameManager::GetBlackboard()->get<bool>("accelerometer"))
		{
			acc_x = int(100 * GameManager::GetBlackboard()->get<double>("acc_x"));
			acc_y = int(100 * GameManager::GetBlackboard()->get<double>("acc_y"));
			acc_z = int(100 * GameManager::GetBlackboard()->get<double>("acc_z"));
			h_angle = GameManager::GetBlackboard()->get<double>("h_angle");
			v_angle = GameManager::GetBlackboard()->get<double>("v_angle");
			linear_x = int(100 * GameManager::GetBlackboard()->get<double>("linear_x"));
			linear_y = int(100 * GameManager::GetBlackboard()->get<double>("linear_y"));
			linear_z = int(100 * GameManager::GetBlackboard()->get<double>("linear_z"));
		}

		if(GameManager::GetBlackboard()->get<bool>("gyrometer"))
		{
			gyro_x = int(10 * GameManager::GetBlackboard()->get<double>("gyro_x"));
			gyro_y = int(10 * GameManager::GetBlackboard()->get<double>("gyro_y"));
			gyro_z = int(10 * GameManager::GetBlackboard()->get<double>("gyro_z"));
		}

		// Clear the screen
		m_d3dContext->ClearRenderTargetView(
			m_renderTargetView.Get(),
			Colors::MidnightBlue
			);

		m_d3dContext->ClearDepthStencilView(
			m_depthStencilView.Get(),
			D3D11_CLEAR_DEPTH,
			1.0f,
			0
			);

		m_d3dContext->OMSetRenderTargets(
			1,
			m_renderTargetView.GetAddressOf(),
			m_depthStencilView.Get()
			);

		// Get handle to object manager
		DirectX::XMFLOAT2 _player_pos = GameManager::GetObjectManager()->GetByType("Player")[0]->GetPosition();
		DirectX::XMFLOAT2 _player_vel = GameManager::GetObjectManager()->GetByType("Player")[0]->GetVelocity();
		DirectX::XMFLOAT2 draw_player_pos;
		draw_player_pos.x = _player_pos.x + alpha * _player_vel.x * timeDelta;
		draw_player_pos.y = _player_pos.y + alpha * _player_vel.y * timeDelta;

		// DEBUG PRINT
		/*std::ostringstream sstream;
			sstream << "<GameRenderer::Render> alpha=" << alpha << " pos.x = " << _player_pos.x << " vel.x = " << _player_vel.x << "\n";
			sstream << "draw.x =" << draw_player_pos.x << "\n";
		std::string s = sstream.str();
		OutputDebugStringA(s.c_str());
		*/

		std::wostringstream ws;

		// Print FPS
		ws << "FPS: " << int(GameManager::GetBlackboard()->get<float>("fps"));
		std::wstring _wstring4(ws.str());
		const wchar_t* fps = _wstring4.c_str();

		// Clear stream
		ws.str(L"");
		ws.clear();
		
		// Print Event info
		ws << GameManager::GetBlackboard()->get<std::string>("event").c_str();
		std::wstring _wstring5(ws.str());
		const wchar_t* event_text = _wstring5.c_str();

		// Clear stream
		ws.str(L"");
		ws.clear();
		

		// Print player position
		ws << int(_player_pos.x) << "/" << int(_player_pos.y);
		std::wstring _wstring(ws.str());
		const wchar_t* player_pos = _wstring.c_str();

		// Clear stream
		ws.str(L"");
		ws.clear();
		
		// Print accelerometer data
		ws << acc_x << "/" << acc_y << "/" << acc_z << "\n [" << h_angle << "/" << v_angle << "]";
		std::wstring _wstring2(ws.str());
		const wchar_t* acc_values = _wstring2.c_str();

		// Clear stream
		ws.str(L"");
		ws.clear();
		
		// Print linear acceleration data
		ws << linear_x << "/" << linear_y << "/" << linear_z;
		std::wstring _wstring3(ws.str());
		const wchar_t* linear_values = _wstring3.c_str();
		
		// Draw sprite
		m_sprites->Begin();
		m_sprites->Draw( m_texture1.Get(), XMFLOAT2(draw_player_pos.x, draw_player_pos.y), nullptr, Colors::White );

		// Draw text
		m_font->DrawString( m_sprites.get(), L"GraduApp", XMFLOAT2( 10, 10 ), Colors::Yellow );
		m_font->DrawString( m_sprites.get(), L"Position", XMFLOAT2( 10, 400 ), Colors::Yellow );
		m_font->DrawString( m_sprites.get(), player_pos, XMFLOAT2( 10, 450 ), Colors::Yellow );
		m_font->DrawString( m_sprites.get(), fps, XMFLOAT2( 10, 300 ), Colors::Yellow );
		m_font->DrawString( m_sprites.get(), event_text, XMFLOAT2( 10, 350 ), Colors::Yellow );
		
		if(GameManager::GetBlackboard()->get<bool>("accelerometer"))
		{
			m_font->DrawString( m_sprites.get(), L"Accelerometer", XMFLOAT2( 10, 500 ), Colors::Yellow );
			m_font->DrawString( m_sprites.get(), acc_values, XMFLOAT2( 10, 550 ), Colors::Yellow );

			m_font->DrawString( m_sprites.get(), L"Linear acceleration", XMFLOAT2( 10, 650 ), Colors::Yellow );
			m_font->DrawString( m_sprites.get(), linear_values, XMFLOAT2( 10, 690 ), Colors::Yellow );
		}
		
		m_sprites->End();
	}

}