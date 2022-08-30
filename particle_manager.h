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
#include <list>
#include <vector>
#include "particle.h"
#include "particle_emitter.h"
#include "file.h"

//=========================================
// 前方宣言
//=========================================
class CParticleEmitter;

//=========================================
// パーティクルファクトリー
//=========================================
class CParticleManager
{
public:	// 構造体
	struct BundledData
	{
		CParticle::Info particleData;
		CParticleEmitter::Info emitterData;
	};
public:
	CParticleManager();
	~CParticleManager();

	HRESULT Init();
	void Uninit();
	void Update();
	CParticleEmitter* Create(const D3DXVECTOR3& pos, const int& idx);
	void ChangeBundledData(const int idx, const BundledData&inData);

	void ChangeEmitterInfo(CParticleEmitter* inEmitter, const int& idx);

	// 外部読込み
	void LoadBundledData(const wchar_t* cUrl);

	// Setter
	int SetBundledData(const BundledData&inData);

	// Getter
	std::vector<BundledData>& GetBundledData() { return m_bundledData; }	// 情報体全体の取得
	std::list<CParticleEmitter*> GetEmitter() { return m_particleEmitter; }	// エミッタ―全体の取得

private:
	void ReleaseEmitter();
private:
	std::vector<BundledData> m_bundledData;	// 情報体
	std::list<CParticleEmitter*> m_particleEmitter;	// エミッタ―情報
};

#endif // !_PARTICLE_FACTORY_H_
