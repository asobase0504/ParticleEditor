//=============================================================================
// 
// �A�v���P�[�V�����N���X
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "application.h"
#include "input.h"
#include "renderer.h"
#include "texture.h"
#include "object2d.h"
#include "file.h"

#include "particle.h"
#include "BG.h"
#include "imgui_property.h"

//-----------------------------------------------------------------------------
// �ÓI�����o�[�ϐ��̏�����
//-----------------------------------------------------------------------------
CApplication* CApplication::application = nullptr;

//=============================================================================
// �V���O���g���ł̃C���X�^���X�̎擾
//=============================================================================
CApplication* CApplication::GetInstance()
{
	if (application == nullptr)
	{
		application = new CApplication;
	}

	return application;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CApplication::CApplication() :
	renderer(nullptr),
	object(nullptr)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CApplication::~CApplication()
{
}

//=============================================================================
// ������
// Author YudaKaito
// �T�v : �v���O�����S�̂̏�����
//=============================================================================
HRESULT CApplication::Init(HWND hWnd, HINSTANCE hInstance)
{
	//�����̏�����
	srand((unsigned int)time(0));

	// �����_�����O�N���X
	renderer = new CRenderer;
	if (FAILED(renderer->Init(hWnd, true)))
	{
		return E_FAIL;
	}

	//���͏����̏���������
	if (FAILED(InitInput(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// �e�N�X�`��
	texture = new CTexture;

	// �p�[�e�B�N��
	LoadJson(L"data/FILE/Effect.json");

	SetTex();

	// �w�i
	bg = new CBg;
	if (FAILED(bg->Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// �I��
// Author YudaKaito
// �T�v : �v���O�����S�̂̏I��
//=============================================================================
void CApplication::Uninit()
{
	CObject::ReleaseAll();
	texture->UnloadAll();

	if (texture != nullptr)
	{
		texture->SavePath();
		delete texture;
		texture = nullptr;
	}

	if (renderer != nullptr)
	{
		renderer->Uninit();

		delete renderer;
		renderer = nullptr;
	}

	if (application != nullptr)
	{
		delete application;
		application = nullptr;
	}
}

//=============================================================================
// �X�V
// Author YudaKaito
// �T�v : �v���O�����S�̂̍X�V
//=============================================================================
void CApplication::Update()
{
	UpdateInput();

	renderer->Update();

	if (bSetImguiParticle())
	{
		CParticle::Create(GetImguiParticle(), GetPopPos());
	}
}

//=============================================================================
// �`��
// Author YudaKaito
// �T�v : �v���O�����S�̂̕`��
//=============================================================================
void CApplication::Draw()
{
	renderer->Draw();
}
