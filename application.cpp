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

#include "particle_manager.h"
#include "BG.h"

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

	paticleManager = new CParticleManager;
	// �p�[�e�B�N��
	if (FAILED(paticleManager->Init()))
	{
		return E_FAIL;
	}
	paticleManager->Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0);
	paticleManager->Create(D3DXVECTOR3(SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.75f, 0.0f), 0);
	paticleManager->Create(D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.25f, 0.0f), 0);

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
	// �I�u�W�F�N�g�̉��
	CObject::ReleaseAll();

	// �e�N�X�`���̉��
	texture->UnloadAll();
	if (texture != nullptr)
	{
		texture->SavePath();
		delete texture;
		texture = nullptr;
	}

	// �p�[�e�B�N���}�l�W���[�̉��
	if (paticleManager != nullptr)
	{
		paticleManager->Uninit();

		delete paticleManager;
		paticleManager = nullptr;
	}

	// �����_���[�̉��
	if (renderer != nullptr)
	{
		renderer->Uninit();

		delete renderer;
		renderer = nullptr;
	}

	// �A�v���P�[�V�����̉��
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

	static int a;

	if (GetKeyboardTrigger(DIK_F1))
	{
		a = paticleManager->Create(D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.5f, 0.0f), 0);
	}

	if (GetKeyboardTrigger(DIK_F2))
	{
		paticleManager->Release(a);
	}

	paticleManager->Update();
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
