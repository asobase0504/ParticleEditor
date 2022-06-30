// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// effect.cpp
// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
#include "particle.h"
#include "main.h"
#include "application.h"
#include "utility.h"
#include "renderer.h"

#include <assert.h>

//==================================================
// 静的メンバー変数
//==================================================
float CParticle::m_fAngle = 0.0f;
int CParticle::m_nIdxTex = 0;

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CParticle::CParticle()
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CParticle::~CParticle()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CParticle::Init()
{
	CObject2D::Init();

	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CParticle::Uninit()
{
	CObject2D::Uninit();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CParticle::Update()
{
	// (ImGui)
	bool *bTex = TexUse();

	if (bTex)
	{
		LoadTex();
	}

	/* ↓使用しているなら↓ */

	// エフェクトの移動
	pos += m_data.move;

	// 推移
	m_data.nLife--;							// 体力の減少
	m_data.move.y += m_data.fWeight;			// 重力
	m_data.move *= m_data.fAttenuation;			// 移動量の推移
	m_data.fWeight += m_data.fWeightTransition;	// 重さの推移

	D3DXCOLOR myColor = GetColor();
	if (m_data.color.bColTransition)
	{// 色の推移
		if (m_data.color.nEndTime >= m_data.color.nCntTransitionTime)
		{
			m_data.color.nCntTransitionTime++;
			myColor += m_data.color.colTransition;
		}
	}
	myColor.a -= 1.0f / m_data.nMaxLife;

	SetPos(pos);
	SetColor(myColor);
	SetSize(D3DXVECTOR2(m_data.fWidth, m_data.fHeight));

	if (m_data.nLife <= 0)
	{// エフェクトの寿命
		Release();
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CParticle::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();	// デバイスの取得

	switch (m_data.alphaBlend)
	{
	case TYPE_NONE:	// 乗算
		break;

	case TYPE_ADD:	// 加算
					// アルファブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;

	case TYPE_SUB:	// 減算
					// αブレンディングを減算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;

	default:
		assert(false);
		break;
	}

	CObject2D::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// テクスチャを引き継がない
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// 生成
//--------------------------------------------------
CParticle* CParticle::Create(const Particle& inParticle, const D3DXVECTOR3& inPos)
{
	CParticle* particle = nullptr;
	if (particle == nullptr)
	{
		particle = new CParticle;
		particle->Init();
		particle->Set(inParticle, inPos);
		return particle;
	}
	return nullptr;
}

//--------------------------------------------------
// データの初期設定
//--------------------------------------------------
void CParticle::Set(const Particle& inParticle, const D3DXVECTOR3& inPos)
{
	m_data = inParticle;

	m_data.nMaxLife = m_data.nLife;
	m_data.fWidth = m_data.fScale;
	m_data.fHeight = m_data.fScale;
	m_data.type = PARTICLETYPE_NORMAL;

	//m_data.fWidth = g_aParticle[]->m_data->fScale;
	//m_data.fHeight = g_aParticle->m_data->fScale;
	m_data.color.nCntTransitionTime = 0;
	m_data.bUse = true;

	D3DXVECTOR3 myPos = inPos;
	// 生成位置の算出
	myPos.x += FloatRandam(m_data.maxPopPos.x, -m_data.minPopPos.x);
	myPos.y += FloatRandam(m_data.maxPopPos.y, -m_data.minPopPos.y);
	myPos.z += FloatRandam(m_data.maxPopPos.z, -m_data.minPopPos.z);

	// 色の算出
	if (m_data.color.bColRandom)
	{// ランダムカラーを使用
		m_data.color.colBigin.r = FloatRandam(m_data.color.colRandamMax.r, m_data.color.colRandamMin.r);
		m_data.color.colBigin.g = FloatRandam(m_data.color.colRandamMax.g, m_data.color.colRandamMin.g);
		m_data.color.colBigin.b = FloatRandam(m_data.color.colRandamMax.b, m_data.color.colRandamMin.b);

		if (m_data.color.bColTransition)
		{// 目的の色の設定
			if (m_data.color.bRandomTransitionTime)
			{
				m_data.color.nEndTime = rand() % m_data.nLife + 1;
			}

			m_data.color.destCol.r = FloatRandam(m_data.color.colRandamMax.r, m_data.color.colRandamMin.r);
			m_data.color.destCol.g = FloatRandam(m_data.color.colRandamMax.g, m_data.color.colRandamMin.g);
			m_data.color.destCol.b = FloatRandam(m_data.color.colRandamMax.b, m_data.color.colRandamMin.b);
		}
	}

	if (m_data.color.bColTransition)
	{// トラディシオンカラーを使用
		if (m_data.color.bRandomTransitionTime)
		{
			m_data.color.nEndTime = rand() % m_data.nLife + 1;
		}

		m_data.color.colTransition.r = (m_data.color.destCol.r - m_data.color.colBigin.r) / m_data.color.nEndTime;
		m_data.color.colTransition.g = (m_data.color.destCol.g - m_data.color.colBigin.g) / m_data.color.nEndTime;
		m_data.color.colTransition.b = (m_data.color.destCol.b - m_data.color.colBigin.b) / m_data.color.nEndTime;
	}

	SetPos(myPos);
	SetSize(D3DXVECTOR2(m_data.fWidth, m_data.fHeight));
	SetTexture(m_nIdxTex);
	SetColor(m_data.color.colBigin);

	static float ImAngle = 20.0f;
	float fRad = 0.0f;
	float fGRad = 0.0f;

	if (m_data.bBackrot)
	{
		// float fRad = (m_data.fAngle) * (D3DX_PI / 180);
		fGRad = (m_data.rot.z - m_fAngle);
	}
	else
	{
		fRad = (m_data.fAngle) * (D3DX_PI / 180);
		fGRad = (m_data.rot.z + m_fAngle);
	}

	// 挙動
	{
		/*
		m_fAngle += 30.0f * i;
		m_data.move.x = sinf(fGRad) * 1.3f;
		m_data.move.y = cosf(fGRad) * 1.3f;

		// ∞
		m_fAngle += 0.7f;
		m_data.move.x = sinf((D3DX_PI / 180) * 17 * m_fAngle) * m_data.fAttenuation;
		m_data.move.y = sinf((D3DX_PI / 180) * 8 * m_fAngle) * m_data.fAttenuation;
		*/

		// 螺旋だったり
		m_fAngle += ImAngle;
		m_data.move.x += (m_data.fRadius * sinf(fGRad)) * m_data.fAttenuation;
		m_data.move.y += (m_data.fRadius * cosf(fGRad)) * m_data.fAttenuation;
	}

	// ======================
	// 正規化
	// ======================
	if (m_data.fRadius > D3DX_PI)
	{
		m_data.fRadius -= D3DX_PI * 2;
	}
	else if (m_data.fRadius < -D3DX_PI)
	{
		m_data.fRadius += D3DX_PI * 2;
	}

	if (m_fAngle > D3DX_PI)
	{
		m_fAngle -= D3DX_PI * 2;
	}
	else if (m_fAngle < -D3DX_PI)
	{
		m_fAngle += D3DX_PI * 2;
	}
}

//--------------------------------------------------
// テクスチャの読込み
//--------------------------------------------------
void CParticle::LoadTex()
{
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();
	char ImFile[512];
	bool *ImTex = TexUse();

	memset(ImFile, 0, sizeof(ImFile));

	if (ImTex)
	{
		// テクスチャの読み込み
		//D3DXCreateTextureFromFile(pDevice, , &m_pTexture[PARTICLETYPE_NORMAL]);

		ImTex = false;
	}
}

//--------------------------------------------------
// 角度の初期化
//--------------------------------------------------
void CParticle::RemoveAngle(void)
{
	m_fAngle = 0;
}

//--------------------------------------------------
// float を DWORD に変換
//--------------------------------------------------
DWORD CParticle::FloattoDword(float fVal)
{
	return *((DWORD*)&fVal);
}

//--------------------------------------------------
// テクスチャの設定
//--------------------------------------------------
void CParticle::SetIdxTex(int idxTex)
{
	m_nIdxTex = idxTex;
}

//--------------------------------------------------
// テクスチャの設定
//--------------------------------------------------
int CParticle::GetIdxTex()
{
	return m_nIdxTex;
}