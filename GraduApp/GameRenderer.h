#pragma once

#include "Direct3DBase.h"

#include "SpriteBatch.h"
#include "SpriteFont.h"

namespace SST
{
	ref class GameRenderer sealed : public Direct3DBase
	{
	public:
		GameRenderer();

		// Direct3DBase methods.
		virtual void CreateDeviceResources() override;
		virtual void CreateWindowSizeDependentResources() override;
		virtual void Render(float alpha, float timeDelta) override;
	
		// Method for updating time-dependent objects.
		void Update(float timeTotal, float timeDelta);

	private:
		std::unique_ptr<DirectX::SpriteBatch>                                   m_sprites;
		std::unique_ptr<DirectX::SpriteFont>                                    m_font;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_texture1;

		
	};
}