//=============================================================================
// 
// パーティクルエディタ
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "application.h"
#include "renderer.h"
#include "particle_manager.h"
#include "particle_edit.h"
#include "particle_imgui.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CParticleEdit::CParticleEdit()
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CParticleEdit::~CParticleEdit()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
HRESULT CParticleEdit::Init(HWND hWnd)
{
	m_imgui = new CParticleImgui;
	m_imgui->Init(hWnd, CApplication::GetInstance()->GetRenderer()->GetDevice());

	m_particleEditing = CApplication::GetInstance()->GetParticleManager()->Create(D3DXVECTOR3(CApplication::SCREEN_WIDTH * 0.5f, CApplication::SCREEN_HEIGHT * 0.5f, 0.0f), 0);
	m_particleEmitter.push_back(m_particleEditing);

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CParticleEdit::Uninit()
{
	// imguiの解放
	if (m_imgui != nullptr)
	{
		//imguiProperty->Uninit();

		delete m_imgui;
		m_imgui = nullptr;
	}
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CParticleEdit::Update()
{
	m_imgui->Update();	// imguiの更新
}
