//=============================================================================
// 
// �p�[�e�B�N���G�f�B�^
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
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CParticleEdit::CParticleEdit()
{
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CParticleEdit::~CParticleEdit()
{
}

//-----------------------------------------------------------------------------
// ������
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
// �I��
//-----------------------------------------------------------------------------
void CParticleEdit::Uninit()
{
	// imgui�̉��
	if (m_imgui != nullptr)
	{
		//imguiProperty->Uninit();

		delete m_imgui;
		m_imgui = nullptr;
	}
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CParticleEdit::Update()
{
	m_imgui->Update();	// imgui�̍X�V
}
