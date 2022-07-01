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

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CParticle::CParticle() : 
	m_data({})
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
		// αブレンディングを加算合成に設定
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
		particle->m_data = inParticle;
		particle->SetPos(inPos);
		particle->SetSize(D3DXVECTOR2(particle->m_data.fWidth, particle->m_data.fHeight));
		particle->SetTexture(particle->m_data.nIdxTex);
		particle->SetColor(particle->m_data.color.colBigin);
		return particle;
	}
	return nullptr;
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
// float を DWORD に変換
//--------------------------------------------------
DWORD CParticle::FloattoDword(float fVal)
{
	return *((DWORD*)&fVal);
}
