//=========================================
// 
// パーティクルエミッタ
// Author YudaKaito
// 
//=========================================
#ifndef _PARTICLE_MANAGER_H_
#define _PARTICLE_MANAGER_H_

//=========================================
// include
//=========================================
#include <d3dx9.h>
#include <vector>
#include "particle.h"
#include "particle_emitter.h"

//=========================================
// 前方宣言
//=========================================
class CParticleEmitter;

//=========================================
// パーティクルファクトリー
//=========================================
class CParticleManager
{
public:	// 列挙型
	enum TYPE
	{
		DEBUG_TYPE = 0,
		TYPE_01,
		TYPE_02,
		TYPE_04,
		TYPE_05,
		TYPE_06,
	};

public:	// 構造体
	struct BundledData
	{
		CParticle::Particle particleData;
		CParticleEmitter::Emitter emitterData;
	};
public:
	CParticleManager();
	~CParticleManager();

	HRESULT Init();
	void Uninit();
	void Update();
	int Create(const D3DXVECTOR3& pos, const TYPE& inType);
	void SetBundledData(const BundledData&inData);
	void SetEmitterPos(const int idx,const D3DXVECTOR3& inPos);
	std::vector<BundledData>& GetBundledData() { return m_bundledData; }
	std::vector<CParticleEmitter*> GetEmitter() { return m_particleEmitter; }
private:
	int m_numAll;	// 生成数
	std::vector<BundledData> m_bundledData;	// 情報体
	std::vector<CParticleEmitter*> m_particleEmitter;	// エミッタ―情報
};

#endif // !_PARTICLE_FACTORY_H_
