//=============================================================================
// 
// パーティクルエミッタ
// Author YudaKaito
// 
//=============================================================================
#ifndef _PARTICLE_EMITTER_H_
#define _PARTICLE_EMITTER_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include <d3dx9.h>
#include "particle.h"

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CParticle;

//-----------------------------------------------------------------------------
// パーティクルエミッタ
//-----------------------------------------------------------------------------
class CParticleEmitter
{
public:	// 構造体
	struct Info
	{
		D3DXVECTOR3 maxPopPos;		// 発生位置(最大)
		D3DXVECTOR3 minPopPos;		// 発生位置(最小)
		float fAngle;				// 発射角度
		float fAddAngle;			// 発射角度を生成時に変化させる量
		int popNumber;				// 一度の出現数
	};
public:	// 静的メンバー変数
public:
	CParticleEmitter();
	~CParticleEmitter();

	HRESULT Init();
	void Uninit();
	void Update();

	// Setter
	void SetPos(const D3DXVECTOR3& inPos);
	void SetBundledDataIndex(const int inIndex) { m_bundledDataIndex = inIndex; }
	void SetParticle(const CParticle::Info& inParticle);
	void SetEmitter(const Info& inEmitter) { m_info = inEmitter; }
	void SetNeedsDelete(const bool inNeedsDelete) { m_needsDelete = inNeedsDelete; }

	// Getter
	const D3DXVECTOR3& GetPos() { return m_pos; }
	bool GetNeedsDelete() { return m_needsDelete; }
	const int GetBundledIndex() { return m_bundledDataIndex; }

private:
	void PopParticle(void);

private:
	D3DXVECTOR3 m_pos;	// 出現位置
	Info m_info;		// エミッターが管理する情報一覧
	CParticle::Info m_particleInfo;	// このエミッターから出るパーティクルのデータ
	int m_bundledDataIndex;
	bool m_needsDelete;	// デリートする必要があるか
};

#endif // !_PARTICLE_EMITTER_H_
