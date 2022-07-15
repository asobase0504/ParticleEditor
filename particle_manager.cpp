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

	emitter->Init();		// 初期化
	emitter->SetPos(pos);	// 位置の更新

	emitter->SetParticle(m_bundledData.at(inType).particleData);	// 指定されてたパーティクルデータの挿入
	emitter->SetEmitter(m_bundledData.at(inType).emitterData);

	m_numAll++;
	m_particleEmitter.push_back(emitter);

	return idx;
}

//-----------------------------------------
// 設定
//-----------------------------------------
void CParticleManager::SetBundledData(const BundledData& inData)
{
	m_bundledData.push_back(inData);
}

//-----------------------------------------
// チェンジ機能
//-----------------------------------------
void CParticleManager::ChangeBundledData(const int idx, const BundledData& inData)
{
	m_bundledData.at(idx) = inData;
}

//-----------------------------------------
// 指定された番号の位置を変更する
//-----------------------------------------
void CParticleManager::SetEmitterPos(const int idx, const D3DXVECTOR3 & inPos)
{
	m_particleEmitter.at(idx)->SetPos(inPos);
}
