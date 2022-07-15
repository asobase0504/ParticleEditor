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
	struct Info
	{
		float fAngle;
	};
public:	// 静的メンバー変数
public:
	CParticleEmitter();
	~CParticleEmitter();

	HRESULT Init();
	void Uninit();
	void Update();
	void SetPos(const D3DXVECTOR3& inPos);
	const D3DXVECTOR3& GetPos() { return m_pos; }
	void PopParticle(void);
	void SetParticle(const CParticle::Info& inParticle);
	void SetEmitter(const Info& inEmitter) { m_info = inEmitter; }
	CParticle::Info* GetParticle() { return &m_particleInfo; }
private:
	D3DXVECTOR3 m_pos;	// 出現位置
	Info m_info;		// エミッターが管理する情報一覧
	CParticle::Info m_particleInfo;	// このエミッターから出るパーティクルのデータ
};

#endif // !_PARTICLE_EMITTER_H_
