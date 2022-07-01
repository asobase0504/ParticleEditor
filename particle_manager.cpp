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
	for (CParticleEmitter* i : m_particleEmitter)
	{
		i->Uninit();
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

	emitter->Init();
	emitter->SetPos(pos);
	emitter->SetParticle(&m_bundledData[inType].particleData);

	m_numAll++;
	m_particleEmitter.push_back(emitter);

	return idx;
}

//-----------------------------------------
// �ݒ�
//-----------------------------------------
void CParticleManager::SetBundledData(const BundledData&inData)
{
	m_bundledData.push_back(inData);
}

void CParticleManager::SetEmitterPos(const int idx, const D3DXVECTOR3 & inPos)
{
	m_particleEmitter[idx]->SetPos(inPos);
}
