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

#include <assert.h>

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CParticleManager::CParticleManager() :
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
	ReleaseEmitter();
}

//-----------------------------------------
// 生成
//-----------------------------------------
CParticleEmitter* CParticleManager::Create(const D3DXVECTOR3& pos, const int& index)
{
	CParticleEmitter* emitter = new CParticleEmitter();

	emitter->Init();		// 初期化
	emitter->SetPos(pos);	// 位置の更新

	// 指定された番号内にデータが入っているか否か
	if (0 > index || m_bundledData.size() <= index)
	{
		assert(false);
		return 0;
	}

	emitter->SetParticle(m_bundledData.at(index).particleData);	// 指定されてたパーティクルデータの挿入
	emitter->SetEmitter(m_bundledData.at(index).emitterData);	// 指定されてたエミッタ―データの挿入

	m_particleEmitter.push_back(emitter);

	return emitter;
}

//-----------------------------------------
// 指定したエミッタ―を削除する
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
