//==================================================
//
// �Q�[�����[�h
// Author : Yuda Kaito
//
//==================================================

//==================================================
// �C���N���[�h
//==================================================
// ��v�n��
#include "main.h"
#include "game.h"
#include "input.h"
#include <assert.h>
// �����n��
#include "BG.h"
#include "particle.h"
#include "imgui_property.h"

//==================================================
// �ÓI�ϐ�
//==================================================
CParticle* particle;

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitGame(void)
{
	particle = new CParticle;
	InitBg();		// �w�i
	particle->Init();	// �p�[�e�B�N��
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitGame(void)
{
	UninitBg();			// �w�i
	particle->Uninit();	// �p�[�e�B�N��
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateGame(void)
{
	UpdateBg();			// �w�i
	particle->Update();	// �p�[�e�B�N��

	if (bSetImguiParticle())
	{
		particle->Create(GetImguiParticle(), D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawGame(void)
{
	DrawBg();		// �w�i
	particle->Draw();	// �p�[�e�B�N��
}
