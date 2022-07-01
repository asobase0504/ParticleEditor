//=========================================
// 
// パーティクルエミッタ
// Author YudaKaito
// 
//=========================================
//=========================================
// include
//=========================================
#include "particle_manager.h"
#include "file.h"

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CParticleManager::CParticleManager() :
	m_numAll(0),
	m_bundledData({}),
	m_particleEmitter({})
{
}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
CParticleManager::~CParticleManager()
{
}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CParticleManager::Init()
{
	LoadJson(L"data/FILE/Effect.json");
	return S_OK;
}

//-----------------------------------------
// 終了
//-----------------------------------------
void CParticleManager::Uninit()
{
	for (CParticleEmitter* i : m_particleEmitter)
	{
		i->Uninit();
	}
}

//-----------------------------------------
// 更新
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
// 生成
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
// 設定
//-----------------------------------------
void CParticleManager::SetBundledData(const BundledData&inData)
{
	m_bundledData.push_back(inData);
}

void CParticleManager::SetEmitterPos(const int idx, const D3DXVECTOR3 & inPos)
{
	m_particleEmitter[idx]->SetPos(inPos);
}
