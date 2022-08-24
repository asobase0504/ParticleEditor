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
#include "input.h"

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

	if (GetKeyboardTrigger(DIK_J))
	{
		m_particleEditing = CApplication::GetInstance()->GetParticleManager()->Create(D3DXVECTOR3(CApplication::SCREEN_WIDTH * 0.5f, CApplication::SCREEN_HEIGHT * 0.5f, 0.0f), 0);
		m_particleEmitter.push_back(m_particleEditing);
	}
	if (GetKeyboardTrigger(DIK_K))
	{
		DoNextEditingEmitter(1);
	}
	if (GetKeyboardTrigger(DIK_U))
	{
		m_particleEditing->SetNeedsDelete(true);
		DoNextEditingEmitter(1);
	}
}

//-----------------------------------------------------------------------------
// 編集するエミッタ―を進める
//-----------------------------------------------------------------------------
void CParticleEdit::DoNextEditingEmitter(int inIndex)
{
	for (auto it = m_particleEmitter.begin(); it != m_particleEmitter.end(); it++)
	{
		if (*it != m_particleEditing)
		{
			continue;
		}

		/* ↓listに編集中のエミッタ―と同じデータがあった場合↓ */

		if (it == m_particleEmitter.begin() && inIndex < 0)
		{ // listの最初の要素が同じデータだった場合
			it = m_particleEmitter.end();
		}

		// 指定した数、イテレータをずらす
		std::advance(it, inIndex);

		if (it == m_particleEmitter.end() && inIndex > 0)
		{ // ずらした場所がendだった場合
			it = m_particleEmitter.begin();
		}

		// 編集するイテレータを更新する
		m_particleEditing = *it;

		return;
	}
}
