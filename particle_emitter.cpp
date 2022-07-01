//=========================================
// 
// パーティクルエミッタ
// Author YudaKaito
// 
//=========================================
//=========================================
// include
//=========================================
#include "particle_emitter.h"
#include "utility.h"

//==================================================
// 静的メンバー変数
//==================================================
float CParticleEmitter::m_fAngle = 0.0f;

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CParticleEmitter::CParticleEmitter() :
	emitterInfo({}),
	m_data(nullptr)
{

}

//-----------------------------------------
// デストラクタ
//-----------------------------------------
CParticleEmitter::~CParticleEmitter()
{

}

//-----------------------------------------
// 初期化
//-----------------------------------------
HRESULT CParticleEmitter::Init()
{
	return S_OK;
}

//-----------------------------------------
// 終了
//-----------------------------------------
void CParticleEmitter::Uninit()
{
}

//-----------------------------------------
// 更新
//-----------------------------------------
void CParticleEmitter::Update()
{
	Pop();
}

//-----------------------------------------
// 設定
//-----------------------------------------
void CParticleEmitter::SetPos(const D3DXVECTOR3 & inPos)
{
	pos = inPos;
}

//-----------------------------------------
// 出現
//-----------------------------------------
void CParticleEmitter::Pop(void)
{
	CParticle::Particle popData = *m_data;
	popData.nMaxLife = popData.nLife;
	popData.fWidth = popData.fScale;
	popData.fHeight = popData.fScale;
	popData.type = CParticle::PARTICLETYPE_NORMAL;

	popData.color.nCntTransitionTime = 0;
	popData.bUse = true;

	D3DXVECTOR3 myPos = pos;
	// 生成位置の算出
	myPos.x += FloatRandam(popData.maxPopPos.x, -popData.minPopPos.x);
	myPos.y += FloatRandam(popData.maxPopPos.y, -popData.minPopPos.y);
	myPos.z += FloatRandam(popData.maxPopPos.z, -popData.minPopPos.z);

	// 色の算出
	if (popData.color.bColRandom)
	{// ランダムカラーを使用
		popData.color.colBigin.r = FloatRandam(popData.color.colRandamMax.r, popData.color.colRandamMin.r);
		popData.color.colBigin.g = FloatRandam(popData.color.colRandamMax.g, popData.color.colRandamMin.g);
		popData.color.colBigin.b = FloatRandam(popData.color.colRandamMax.b, popData.color.colRandamMin.b);

		if (popData.color.bColTransition)
		{// 目的の色の設定
			if (popData.color.bRandomTransitionTime)
			{
				popData.color.nEndTime = rand() % popData.nLife + 1;
			}

			popData.color.destCol.r = FloatRandam(popData.color.colRandamMax.r, popData.color.colRandamMin.r);
			popData.color.destCol.g = FloatRandam(popData.color.colRandamMax.g, popData.color.colRandamMin.g);
			popData.color.destCol.b = FloatRandam(popData.color.colRandamMax.b, popData.color.colRandamMin.b);
		}
	}

	if (popData.color.bColTransition)
	{// トラディシオンカラーを使用
		if (popData.color.bRandomTransitionTime)
		{
			popData.color.nEndTime = rand() % popData.nLife + 1;
		}

		popData.color.colTransition.r = (popData.color.destCol.r - popData.color.colBigin.r) / popData.color.nEndTime;
		popData.color.colTransition.g = (popData.color.destCol.g - popData.color.colBigin.g) / popData.color.nEndTime;
		popData.color.colTransition.b = (popData.color.destCol.b - popData.color.colBigin.b) / popData.color.nEndTime;
	}

	static float ImAngle = 10.0f;
	float fRad = 0.0f;
	float fGRad = 0.0f;

	if (popData.bBackrot)
	{
		// float fRad = (popData.fAngle) * (D3DX_PI / 180);
		fGRad = (popData.rot.z - m_fAngle);
	}
	else
	{
		fRad = (popData.fAngle) * (D3DX_PI / 180);
		fGRad = (popData.rot.z + m_fAngle);
	}

	// 挙動
	{
		/*
		m_fAngle += 30.0f * i;
		popData.move.x = sinf(fGRad) * 1.3f;
		popData.move.y = cosf(fGRad) * 1.3f;

		// ∞
		m_fAngle += 0.7f;
		popData.move.x = sinf((D3DX_PI / 180) * 17 * m_fAngle) * popData.fAttenuation;
		popData.move.y = sinf((D3DX_PI / 180) * 8 * m_fAngle) * popData.fAttenuation;
		*/

		// 螺旋だったり
		m_fAngle += ImAngle;
		popData.move.x += (popData.fRadius * sinf(fGRad)) * popData.fAttenuation;
		popData.move.y += (popData.fRadius * cosf(fGRad)) * popData.fAttenuation;
	}

	// ======================
	// 正規化
	// ======================
	if (popData.fRadius > D3DX_PI)
	{
		popData.fRadius -= D3DX_PI * 2;
	}
	else if (popData.fRadius < -D3DX_PI)
	{
		popData.fRadius += D3DX_PI * 2;
	}

	if (m_fAngle > D3DX_PI)
	{
		m_fAngle -= D3DX_PI * 2;
	}
	else if (m_fAngle < -D3DX_PI)
	{
		m_fAngle += D3DX_PI * 2;
	}

	CParticle::Create(popData, myPos);
}

void CParticleEmitter::SetParticle(CParticle::Particle* inParticle)
{
	m_data = inParticle;
}
