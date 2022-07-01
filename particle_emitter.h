//=========================================
// 
// パーティクルエミッタ
// Author YudaKaito
// 
//=========================================
#ifndef _PARTICLE_EMITTER_H_
#define _PARTICLE_EMITTER_H_

//=========================================
// include
//=========================================
#include <d3dx9.h>
#include "particle.h"

//=========================================
// 前方宣言
//=========================================
class CParticle;

//=========================================
// パーティクルエミッタ
//=========================================
class CParticleEmitter
{
public:	// 構造体
	struct Emitter
	{
	};
public:	// 静的メンバー変数
	static float m_fAngle;	// 発射角度
public:
	CParticleEmitter();
	~CParticleEmitter();

	HRESULT Init();
	void Uninit();
	void Update();
	void SetPos(const D3DXVECTOR3& inPos);
	const D3DXVECTOR3& GetPos() { return pos; }
	void Pop(void);
	void SetParticle(CParticle::Particle* inParticle);
	CParticle::Particle* GetParticle() { return m_data; }
private:
	D3DXVECTOR3 pos;	// 出現位置
	Emitter emitterInfo;
	CParticle::Particle* m_data;	// このエミッターから出るパーティクルのデータ
};

#endif // !_PARTICLE_EMITTER_H_
