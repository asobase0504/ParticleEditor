// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// effect.cpp
// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
#include "main.h"
#include "particle.h"
#include <time.h>
#include "imgui_property.h"
#include "utility.h"

// ==================================================
// 静的メンバー変数
// ==================================================
CParticle* CParticle::g_aParticle[] = {};
float CParticle::g_fAngle = 0.0f;

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
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\flare.png",
		&m_pTexture[PARTICLETYPE_NORMAL]);

	// データの初期化
	memset(&m_data, 0, sizeof(m_data));

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	// 確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;		// 頂点データのポインタを4つ分集める

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CParticle::Uninit()
{
	for (int i = 0; i < numType; i++)
	{
		// テクスチャの破棄
		if (m_pTexture[i] != NULL)
		{
			m_pTexture[i]->Release();
			m_pTexture[i] = NULL;
		}
	}

	// 頂点バッファの破壊
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CParticle::Update()
{
	// (ImGui)
	bool bTex = TexUse();

	if (bTex)
	{
		LoadTex();
	}

	/* ↓使用しているなら↓ */

	// エフェクトの移動
	m_data.pos += m_data.move;

	// 推移
	m_data.nLife--;							// 体力の減少
	m_data.move.y += m_data.fWeight;			// 重力
	m_data.move *= m_data.fAttenuation;			// 移動量の推移
	m_data.fWeight += m_data.fWeightTransition;	// 重さの推移

	if (m_data.color.bColTransition)
	{// 色の推移
		if (m_data.color.nEndTime >= m_data.color.nCntTransitionTime)
		{
			m_data.color.nCntTransitionTime++;
			m_data.color.col += m_data.color.colTransition;
		}
	}

	VERTEX_2D *pVtx = nullptr;		// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = m_data.pos + D3DXVECTOR3(-m_data.fWidth, -m_data.fHeight, 0.0f);
	pVtx[1].pos = m_data.pos + D3DXVECTOR3(m_data.fWidth, -m_data.fHeight, 0.0f);
	pVtx[2].pos = m_data.pos + D3DXVECTOR3(-m_data.fWidth, m_data.fHeight, 0.0f);
	pVtx[3].pos = m_data.pos + D3DXVECTOR3(m_data.fWidth, m_data.fHeight, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = m_data.color.col;
	pVtx[1].col = m_data.color.col;
	pVtx[2].col = m_data.color.col;
	pVtx[3].col = m_data.color.col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}

//--------------------------------------------------
// 全てを更新
//--------------------------------------------------
void CParticle::AllUpdate()
{
	for (int i = 0; i < maxNumber; i++)
	{
		if (g_aParticle[i] == nullptr)
		{
			continue;
		}

		g_aParticle[i]->Update();

		if (g_aParticle[i]->m_data.nLife <= 0)
		{// エフェクトの寿命
			g_aParticle[i]->Uninit();
			delete g_aParticle[i];
			g_aParticle[i] = nullptr;
		}
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CParticle::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

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

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture[m_data.type]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// テクスチャを引き継がない
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// 全ての描画
//--------------------------------------------------
void CParticle::AllDraw()
{
	for (int i = 0; i < maxNumber; i++)
	{
		if (g_aParticle[i] == nullptr)
		{
			continue;
		}

		g_aParticle[i]->Draw();
	}
}

//--------------------------------------------------
// 生成
//--------------------------------------------------
CParticle* CParticle::Create(const Particle& inParticle, const D3DXVECTOR3& inPos)
{
	for (int i = 0; i < maxNumber; i++)
	{
		if (g_aParticle[i] == nullptr)
		{
			g_aParticle[i] = new CParticle;
			g_aParticle[i]->Init();
			g_aParticle[i]->Set(inParticle, inPos);

			g_aParticle[i]->m_idx = i;
			return g_aParticle[i];
		}
	}
}

//--------------------------------------------------
// データの初期設定
//--------------------------------------------------
void CParticle::Set(const Particle& inParticle, const D3DXVECTOR3 & inPos)
{
	m_data = inParticle;

	m_data.fWidth = m_data.fScale;
	m_data.fHeight = m_data.fScale;
	m_data.pos = inPos;
	m_data.type = PARTICLETYPE_NORMAL;

	//m_data.fWidth = g_aParticle[]->m_data->fScale;
	//m_data.fHeight = g_aParticle->m_data->fScale;
	m_data.color.nCntTransitionTime = 0;
	m_data.bUse = true;

	// 生成位置の算出
	m_data.pos.x += FloatRandam(m_data.maxPopPos.x, -m_data.minPopPos.x);
	m_data.pos.y += FloatRandam(m_data.maxPopPos.y, -m_data.minPopPos.y);
	m_data.pos.z += FloatRandam(m_data.maxPopPos.z, -m_data.minPopPos.z);

	// 色の算出
	if (m_data.color.bColRandom)
	{// ランダムカラーを使用
		m_data.color.col.r = FloatRandam(m_data.color.colRandamMax.r, m_data.color.colRandamMin.r);
		m_data.color.col.g = FloatRandam(m_data.color.colRandamMax.g, m_data.color.colRandamMin.g);
		m_data.color.col.b = FloatRandam(m_data.color.colRandamMax.b, m_data.color.colRandamMin.b);

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

		m_data.color.colTransition.r = (m_data.color.destCol.r - m_data.color.col.r) / m_data.color.nEndTime;
		m_data.color.colTransition.g = (m_data.color.destCol.g - m_data.color.col.g) / m_data.color.nEndTime;
		m_data.color.colTransition.b = (m_data.color.destCol.b - m_data.color.col.b) / m_data.color.nEndTime;
	}

	VERTEX_2D*pVtx;	// 頂点情報へのポインタ

					// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = m_data.pos + D3DXVECTOR3(-m_data.fWidth, -m_data.fHeight, 0.0f);
	pVtx[1].pos = m_data.pos + D3DXVECTOR3(m_data.fWidth, -m_data.fHeight, 0.0f);
	pVtx[2].pos = m_data.pos + D3DXVECTOR3(-m_data.fWidth, m_data.fHeight, 0.0f);
	pVtx[3].pos = m_data.pos + D3DXVECTOR3(m_data.fWidth, m_data.fHeight, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = m_data.color.col;
	pVtx[1].col = m_data.color.col;
	pVtx[2].col = m_data.color.col;
	pVtx[3].col = m_data.color.col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	float ImAngle = GetAngle();
	float fRad = 0.0f;
	float fGRad = 0.0f;

	if (m_data.bBackrot)
	{
		// float fRad = (m_data.fAngle) * (D3DX_PI / 180);
		fGRad = (m_data.rot.z - g_fAngle);
	}
	else
	{
		fRad = (m_data.fAngle) * (D3DX_PI / 180);
		fGRad = (m_data.rot.z + g_fAngle);
	}

	// 挙動
	{
		/*
		g_fAngle += 30.0f * i;
		m_data.move.x = sinf(fGRad) * 1.3f;
		m_data.move.y = cosf(fGRad) * 1.3f;

		// ∞
		g_fAngle += 0.7f;
		m_data.move.x = sinf((D3DX_PI / 180) * 17 * g_fAngle) * m_data.fAttenuation;
		m_data.move.y = sinf((D3DX_PI / 180) * 8 * g_fAngle) * m_data.fAttenuation;
		*/

		// 螺旋だったり
		g_fAngle += ImAngle;
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

	if (g_fAngle > D3DX_PI)
	{
		g_fAngle -= D3DX_PI * 2;
	}
	else if (g_fAngle < -D3DX_PI)
	{
		g_fAngle += D3DX_PI * 2;
	}
}

//--------------------------------------------------
// テクスチャの読込み
//--------------------------------------------------
void CParticle::LoadTex()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	char ImFile[512];
	bool ImTex = TexUse();

	memset(ImFile, 0, sizeof(ImFile));

	if (ImTex)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, GetFileName(), &m_pTexture[PARTICLETYPE_NORMAL]);

		ImTex = false;
	}
}

//--------------------------------------------------
// 情報の削除
//--------------------------------------------------
void CParticle::Delete(const int data)
{
	// データのリセット
	memset(&this->m_data, 0, sizeof(this->m_data));
}

//--------------------------------------------------
// 情報を全て削除
//--------------------------------------------------
void CParticle::DeleteAll()
{
	for (int i = 0; i < maxNumber; i++)
	{
		Delete(i);
	}
}

//--------------------------------------------------
// 角度の初期化
//--------------------------------------------------
void CParticle::RemoveAngle(void)
{
	g_fAngle = 0;
}

//--------------------------------------------------
// float を DWORD に変換
//--------------------------------------------------
DWORD CParticle::FloattoDword(float fVal)
{
	return *((DWORD*)&fVal);
}
