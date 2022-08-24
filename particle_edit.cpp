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
#include "input.h"
#include "file.h"

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
	m_defaultData = CApplication::GetInstance()->GetParticleManager()->GetBundledData()[0];
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

	if (GetKeyboardTrigger(DIK_J))
	{
		CApplication::GetInstance()->GetParticleManager()->SetBundledData(m_defaultData);
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
// �ҏW����G�~�b�^�\��i�߂�
//-----------------------------------------------------------------------------
void CParticleEdit::DoNextEditingEmitter(int inIndex)
{
	for (auto it = m_particleEmitter.begin(); it != m_particleEmitter.end(); it++)
	{
		if (*it != m_particleEditing)
		{
			continue;
		}

		/* ��list�ɕҏW���̃G�~�b�^�\�Ɠ����f�[�^���������ꍇ�� */

		if (it == m_particleEmitter.begin() && inIndex < 0)
		{ // list�̍ŏ��̗v�f�������f�[�^�������ꍇ
			it = m_particleEmitter.end();
		}

		// �w�肵�����A�C�e���[�^�����炷
		std::advance(it, inIndex);

		if (it == m_particleEmitter.end() && inIndex > 0)
		{ // ���炵���ꏊ��end�������ꍇ
			it = m_particleEmitter.begin();
		}

		// �ҏW����C�e���[�^���X�V����
		m_particleEditing = *it;

		return;
	}
}

//-----------------------------------------------------------------------------
// �ҏW���Ă�G�~�b�^�\���Z�[�u����
//-----------------------------------------------------------------------------
void CParticleEdit::SaveEffect()
{
	nlohmann::json saveEffect;//���X�g�̐���

	CParticleEmitter* loadData = m_particleEditing;

	CParticle::Info& particleInfo = *loadData->GetParticle();
	CParticleEmitter::Info& emitterInfo = *loadData->GetEmitterInfo();

	saveEffect["POSMAX"] = { { "X", emitterInfo.maxPopPos.x } ,{ "Y", emitterInfo.maxPopPos.y } ,{ "Z", emitterInfo.maxPopPos.z } };
	saveEffect["POSMIN"] = { { "X", emitterInfo.minPopPos.x } ,{ "Y", emitterInfo.minPopPos.y } ,{ "Z", emitterInfo.minPopPos.z } };
	saveEffect["MOVE"] = { { "X", particleInfo.move.x } ,{ "Y", particleInfo.move.y } ,{ "Z", particleInfo.move.z } };
	saveEffect["MOVETRANSITION"] = { { "X", particleInfo.moveTransition.x } ,{ "Y", particleInfo.moveTransition.y } ,{ "Z", particleInfo.moveTransition.z } };
	saveEffect["ROT"] = { { "X", particleInfo.rot.x } ,{ "Y", particleInfo.rot.y },{ "Z", particleInfo.rot.z } };

	saveEffect["COL"] = { { "R", particleInfo.color.colBigin.r },{ "G" ,particleInfo.color.colBigin.g } ,{ "B", particleInfo.color.colBigin.b } ,{ "A", particleInfo.color.colBigin.a } };
	saveEffect["COLRANDAMMAX"] = { { "R", particleInfo.color.colRandamMax.r },{ "G" ,particleInfo.color.colRandamMax.g } ,{ "B", particleInfo.color.colRandamMax.b } ,{ "A", particleInfo.color.colRandamMax.a } };
	saveEffect["COLRANDAMMIN"] = { { "R", particleInfo.color.colRandamMin.r },{ "G" ,particleInfo.color.colRandamMin.g } ,{ "B", particleInfo.color.colRandamMin.b } ,{ "A", particleInfo.color.colRandamMin.a } };
	saveEffect["COLTRANSITION"] = { { "R", particleInfo.color.colTransition.r },{ "G" ,particleInfo.color.colTransition.g } ,{ "B", particleInfo.color.colTransition.b } ,{ "A", particleInfo.color.colTransition.a } };
	saveEffect["DESTCOL"] = { { "R", particleInfo.color.destCol.r },{ "G" ,particleInfo.color.destCol.g } ,{ "B", particleInfo.color.destCol.b } ,{ "A", particleInfo.color.destCol.a } };
	saveEffect["ENDTIME"] = particleInfo.color.nEndTime;
	saveEffect["CNTTRANSITIONTIME"] = particleInfo.color.nCntTransitionTime;
	saveEffect["BCOLTRANSITION"] = particleInfo.color.bColTransition;
	saveEffect["COLRANDOM"] = particleInfo.color.bColRandom;
	saveEffect["RANDOMTRANSITIONTIME"] = particleInfo.color.bRandomTransitionTime;

	saveEffect["TYPE"] = particleInfo.type;
	saveEffect["SCALE_TRANSITION"] = { { "X", particleInfo.scaleTransition.x } ,{ "Y", particleInfo.scaleTransition.y } ,{ "Z", particleInfo.scaleTransition.y } };
	saveEffect["WIDTH"] = particleInfo.fWidth;
	saveEffect["HEIGHT"] = particleInfo.fHeight;
	saveEffect["ANGLE"] = particleInfo.fAngle;
	saveEffect["ATTENUATION"] = particleInfo.fAttenuation;
	saveEffect["RADIUS"] = particleInfo.fRadius;
	saveEffect["WEIGHT"] = particleInfo.fWeight;
	saveEffect["WEIGHTTRANSITION"] = particleInfo.fWeightTransition;
	saveEffect["LIFE"] = particleInfo.nLife;
	saveEffect["BACKROT"] = particleInfo.bBackrot;
	saveEffect["SCALE"] = particleInfo.fScale;

	SaveJson(saveEffect, "data/FILE/DataEffectOutput.json");
}
