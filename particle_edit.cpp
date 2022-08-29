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
#include <vector>

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
	//D3DXVECTOR3��vector<float>�ɕύX����
	auto Vector3ToVectorFloat = [](D3DXVECTOR3 inVector)->std::vector<float>
	{
		return{ inVector.x, inVector.y, inVector.z };
	};

	nlohmann::json saveEffect;//���X�g�̐���
	CParticle::Info& particleInfo = *m_particleEditing->GetParticle();
	CParticleEmitter::Info& emitterInfo = *m_particleEditing->GetEmitterInfo();

	saveEffect["ver.01"];
	saveEffect["POSMAX"] = Vector3ToVectorFloat(emitterInfo.maxPopPos);
	saveEffect["POSMIN"] = Vector3ToVectorFloat(emitterInfo.minPopPos);
	saveEffect["MOVE"] = Vector3ToVectorFloat(particleInfo.move);
	saveEffect["MOVE_TRANSITION"] = Vector3ToVectorFloat(particleInfo.moveTransition);
	saveEffect["ROT"] = Vector3ToVectorFloat(particleInfo.rot);

	{ // �F�̐ݒ�

		//D3DXCOLOR��vector<float>�ɕύX����
		auto ColorToVectorFloat = [](D3DXCOLOR inColor)->std::vector<float>
		{
			return{ inColor.r, inColor.g, inColor.b, inColor.a };
		};

		CParticle::Color saveColor = particleInfo.color;
		D3DXCOLOR transition = saveColor.colTransition;
		D3DXCOLOR dest = saveColor.destCol;

		saveEffect["COLOR"]["BIGIN"] = ColorToVectorFloat(saveColor.colBigin);
		saveEffect["COLOR"]["DEST"] = ColorToVectorFloat(saveColor.destCol);
		saveEffect["COLOR"]["ENDTIME"] = saveColor.nEndTime;

		// �J���[�Ƀ����_��������������ꍇ
		if (particleInfo.color.bColRandom)
		{
			saveEffect["COLOR"]["RANDAM"]["MAX"] = ColorToVectorFloat(saveColor.colRandamMax);
			saveEffect["COLOR"]["RANDAM"]["MIN"] = ColorToVectorFloat(saveColor.colRandamMin);
		}
		if (particleInfo.color.bColTransition)
		{
			saveEffect["COLOR"]["TRANSITION"]["VALUE"] = ColorToVectorFloat(saveColor.colTransition);
			saveEffect["COLOR"]["TRANSITION"]["TIME"] = saveColor.nCntTransitionTime;
		}
		if (saveColor.bRandomTransitionTime)
		{
			saveEffect["COLOR"]["RANDOM_TRANSITION_TIME"] = saveColor.bRandomTransitionTime;
		}
	}

	{ // �傫���̐ݒ�
		saveEffect["SCALE"]["BASIC_VALUE"] = particleInfo.fScale;
		saveEffect["SCALE"]["TRANSITION"] = Vector3ToVectorFloat(particleInfo.scaleTransition);
		saveEffect["SCALE"]["PLUS_WIDTH"] = particleInfo.fWidth;
		saveEffect["SCALE"]["PLUS_HEIGHT"] = particleInfo.fHeight;
	}

	saveEffect["TYPE"] = particleInfo.type;
	saveEffect["RADIUS"] = particleInfo.fRadius;
	saveEffect["ANGLE"] = particleInfo.fAngle;
	saveEffect["ATTENUATION"] = particleInfo.fAttenuation;
	saveEffect["WEIGHT"] = particleInfo.fWeight;
	saveEffect["WEIGHT_TRANSITION"] = particleInfo.fWeightTransition;
	saveEffect["LIFE"] = particleInfo.nLife;
	saveEffect["BACKROT"] = particleInfo.bBackrot;

	SaveJson(saveEffect, "data/FILE/DataEffectOutput2.json");
}
