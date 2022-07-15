//=========================================
// 
// �p�[�e�B�N���G�~�b�^
// Author YudaKaito
// 
//=========================================
//=========================================
// include
//=========================================
#include "particle_manager.h"
#include "file.h"

//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
CParticleManager::CParticleManager() :
	m_numAll(0),
	m_bundledData({}),
	m_particleEmitter({})
{
}

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
CParticleManager::~CParticleManager()
{
}

//-----------------------------------------
// ������
//-----------------------------------------
HRESULT CParticleManager::Init()
{
	LoadJson(L"data/FILE/Effect.json");
	return S_OK;
}

//-----------------------------------------
// �I��
//-----------------------------------------
void CParticleManager::Uninit()
{
	for (CParticleEmitter* emitter : m_particleEmitter)
	{
		if (emitter != nullptr)
		{
			emitter->Uninit();
			delete emitter;
			emitter = nullptr;
		}
	}
}

//-----------------------------------------
// �X�V
//-----------------------------------------
void CParticleManager::Update()
{
	for (CParticleEmitter* i : m_particleEmitter)
	{
		if (i == nullptr)
		{
			continue;
		}

		i->Update();
	}
}

//-----------------------------------------
// ����
//-----------------------------------------
int CParticleManager::Create(const D3DXVECTOR3& pos, const TYPE& inType)
{
	int idx = m_numAll;
	CParticleEmitter* emitter = new CParticleEmitter();

	emitter->Init();		// ������
	emitter->SetPos(pos);	// �ʒu�̍X�V

	emitter->SetParticle(m_bundledData.at(inType).particleData);	// �w�肳��Ă��p�[�e�B�N���f�[�^�̑}��
	emitter->SetEmitter(m_bundledData.at(inType).emitterData);

	m_numAll++;
	m_particleEmitter.push_back(emitter);

	return idx;
}

//-----------------------------------------
// �ݒ�
//-----------------------------------------
void CParticleManager::SetBundledData(const BundledData& inData)
{
	m_bundledData.push_back(inData);
}

//-----------------------------------------
// �`�F���W�@�\
//-----------------------------------------
void CParticleManager::ChangeBundledData(const int idx, const BundledData& inData)
{
	m_bundledData.at(idx) = inData;
}

//-----------------------------------------
// �w�肳�ꂽ�ԍ��̈ʒu��ύX����
//-----------------------------------------
void CParticleManager::SetEmitterPos(const int idx, const D3DXVECTOR3 & inPos)
{
	m_particleEmitter.at(idx)->SetPos(inPos);
}
