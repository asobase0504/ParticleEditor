#include "application.h"
#include "renderer.h"
#include "particle_edit.h"
#include "imgui_property.h"

CParticleEdit::CParticleEdit()
{
}

CParticleEdit::~CParticleEdit()
{
}

HRESULT CParticleEdit::Init(HWND hWnd)
{
	m_imguiProperty = new CImguiProperty;
	m_imguiProperty->Init(hWnd, CApplication::GetInstance()->GetRenderer()->GetDevice());

	return S_OK;
}

void CParticleEdit::Uninit()
{
	// imguiの解放
	if (m_imguiProperty != nullptr)
	{
		//imguiProperty->Uninit();

		delete m_imguiProperty;
		m_imguiProperty = nullptr;
	}
}

void CParticleEdit::Update()
{
	m_imguiProperty->Update();	// imguiの更新
}
