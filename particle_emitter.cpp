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

//=========================================
// 静的メンバー変数
//=========================================

//-----------------------------------------
// コンストラクタ
//-----------------------------------------
CParticleEmitter::CParticleEmitter() :
	m_info({}),
	m_particleInfo({})
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
	int popNumber = 5;
	for (int i = 0; i < popNumber; i++)
	{
		PopParticle();
	}
}

//-----------------------------------------
// 設定
//-----------------------------------------
void CParticleEmitter::SetPos(const D3DXVECTOR3 & inPos)
{
	m_pos = inPos;
}

//-----------------------------------------
// 出現
//-----------------------------------------
void CParticleEmitter::PopParticle(void)
{
	CParticle::Info popInfo = m_particleInfo;
	popInfo.nMaxLife = popInfo.nLife;
	popInfo.fWidth = popInfo.fScale;
	popInfo.fHeight = popInfo.fScale;
	popInfo.type = CParticle::PARTICLETYPE_NORMAL;

	D3DXVECTOR3 myPos = m_pos;
	// 生成位置の算出
	myPos.x += FloatRandam(popInfo.maxPopPos.x, -popInfo.minPopPos.x);
	myPos.y += FloatRandam(popInfo.maxPopPos.y, -popInfo.minPopPos.y);
	myPos.z += FloatRandam(popInfo.maxPopPos.z, -popInfo.minPopPos.z);

	// 色の算出
	CParticle::Color& popColor = popInfo.color;
	popColor.nCntTransitionTime = 0;
	if (popColor.bColRandom)
	{// ランダムカラーを使用
		popColor.colBigin.r = FloatRandam(popColor.colRandamMax.r, popColor.colRandamMin.r);
		popColor.colBigin.g = FloatRandam(popColor.colRandamMax.g, popColor.colRandamMin.g);
		popColor.colBigin.b = FloatRandam(popColor.colRandamMax.b, popColor.colRandamMin.b);

		if (popColor.bColTransition)
		{// 目的の色の設定
			if (popColor.bRandomTransitionTime)
			{
				popColor.nEndTime = rand() % popInfo.nLife + 1;
			}

			popColor.destCol.r = FloatRandam(popColor.colRandamMax.r, popColor.colRandamMin.r);
			popColor.destCol.g = FloatRandam(popColor.colRandamMax.g, popColor.colRandamMin.g);
			popColor.destCol.b = FloatRandam(popColor.colRandamMax.b, popColor.colRandamMin.b);
		}
	}

	if (popColor.bColTransition)
	{// トラディシオンカラーを使用
		if (popColor.bRandomTransitionTime)
		{
			popColor.nEndTime = rand() % popInfo.nLife + 1;
		}

		popColor.colTransition.r = (popColor.destCol.r - popColor.colBigin.r) / popColor.nEndTime;
		popColor.colTransition.g = (popColor.destCol.g - popColor.colBigin.g) / popColor.nEndTime;
		popColor.colTransition.b = (popColor.destCol.b - popColor.colBigin.b) / popColor.nEndTime;
	}

	static float ImAngle = 30.0f;
	float fRad = 0.0f;
	float fGRad = 0.0f;

	if (popInfo.bBackrot)
	{
		// float fRad = (popData.fAngle) * (D3DX_PI / 180);
		fGRad = (popInfo.rot.z - m_info.fAngle);
	}
	else
	{
		fRad = (popInfo.fAngle) * (D3DX_PI / 180);
		fGRad = (popInfo.rot.z + m_info.fAngle);
	}

	// 挙動
	{
		// 螺旋だったり
		m_info.fAngle += ImAngle;
		popInfo.move.x += (popInfo.fRadius * sinf(fGRad)) * popInfo.fAttenuation;
		popInfo.move.y += (popInfo.fRadius * cosf(fGRad)) * popInfo.fAttenuation;
	}

	// ======================
	// 正規化
	// ======================
	if (popInfo.fRadius > D3DX_PI)
	{
		popInfo.fRadius -= D3DX_PI * 2;
	}
	else if (popInfo.fRadius < -D3DX_PI)
	{
		popInfo.fRadius += D3DX_PI * 2;
	}

	if (m_info.fAngle > D3DX_PI)
	{
		m_info.fAngle -= D3DX_PI * 2;
	}
	else if (m_info.fAngle < -D3DX_PI)
	{
		m_info.fAngle += D3DX_PI * 2;
	}

	CParticle::Create(popInfo, myPos);
}

//-----------------------------------------
// 設定
//-----------------------------------------
void CParticleEmitter::SetParticle(const CParticle::Info& inParticle)
{
	m_particleInfo = inParticle;
}
