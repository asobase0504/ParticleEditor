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

#include <assert.h>

//-----------------------------------------
// �R���X�g���N�^
//-----------------------------------------
CParticleManager::CParticleManager() :
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
	ReleaseEmitter();
}

//-----------------------------------------
// ����
//-----------------------------------------
CParticleEmitter* CParticleManager::Create(const D3DXVECTOR3& pos, const int& index)
{
	CParticleEmitter* emitter = new CParticleEmitter();

	emitter->Init();		// ������
	emitter->SetPos(pos);	// �ʒu�̍X�V

	// �w�肳�ꂽ�ԍ����Ƀf�[�^�������Ă��邩�ۂ�
	if (0 > index || m_bundledData.size() <= index)
	{
		assert(false);
		return 0;
	}

	emitter->SetParticle(m_bundledData.at(index).particleData);	// �w�肳��Ă��p�[�e�B�N���f�[�^�̑}��
	emitter->SetEmitter(m_bundledData.at(index).emitterData);	// �w�肳��Ă��G�~�b�^�\�f�[�^�̑}��

	m_particleEmitter.push_back(emitter);

	return emitter;
}

//-----------------------------------------
// �w�肵���G�~�b�^�\���폜����
//-----------------------------------------
void CParticleManager::ReleaseEmitter()
{
	m_particleEmitter.remove_if([](CParticleEmitter* inEmitter)
	{
		bool needsDelete = inEmitter->GetNeedsDelete();

		if (needsDelete)
		{
			inEmitter->Uninit();
			delete inEmitter;
			inEmitter = nullptr;
		}

		return needsDelete;
	});
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
