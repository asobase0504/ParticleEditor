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
CParticle::Particle CParticle::g_aParticle[] = {};

// --------------------------------------------------
// コンストラクタ
// --------------------------------------------------
CParticle::CParticle()
{
}

// --------------------------------------------------
// デストラクタ
// --------------------------------------------------
CParticle::~CParticle()
{
}

// --------------------------------------------------
// 初期化
// --------------------------------------------------
HRESULT CParticle::Init()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\flare.png",
		&pTexture[PARTICLETYPE_NORMAL]);

	// テクスチャの読み込み
	memset(g_aParticle, 0, sizeof(g_aParticle));

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * maxNumber,	// 確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < maxNumber; nCnt++)
	{
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
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	return S_OK;
}

// --------------------------------------------------
// 終了
// --------------------------------------------------
void CParticle::Uninit()
{
	for (int i = 0; i < numType; i++)
	{
		// テクスチャの破棄
		if (pTexture[i] != NULL)
		{
			pTexture[i]->Release();
			pTexture[i] = NULL;
		}
	}

	// 頂点バッファの破壊
	if (pVtxBuff != NULL)
	{
		pVtxBuff->Release();
		pVtxBuff = NULL;
	}
}

// --------------------------------------------------
// 更新
// --------------------------------------------------
void CParticle::Update()
{
	// (ImGui)
	bool bTex = TexUse();

	if (bTex)
	{
		LoadTex();
	}

	for (int i = 0; i < maxNumber; i++)
	{
		Particle* pParticle = &g_aParticle[i];

		if (!pParticle->bUse)
		{// エフェクトが使用されているなら
			continue;
		}

		/* ↓使用しているなら↓ */

		// エフェクトの移動
		pParticle->pos += pParticle->move;

		// 推移
		pParticle->nLife--;									// 体力の減少
		pParticle->move.y += pParticle->fWeight;			// 重力
		pParticle->move *= pParticle->fAttenuation;			// 移動量の推移
		pParticle->fWeight += pParticle->fWeightTransition;	// 重さの推移

		if (pParticle->color.bColTransition)
		{// 色の推移
			if (pParticle->color.nEndTime >= pParticle->color.nCntTransitionTime)
			{
				pParticle->color.nCntTransitionTime++;
				pParticle->color.col += pParticle->color.colTransition;
			}
		}

		if (pParticle->nLife <= 0)
		{// エフェクトの寿命
			Delete(i);
		}

		VERTEX_2D *pVtx = nullptr;		// 頂点情報へのポインタ

		// 頂点バッファをロック
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;		// 頂点データのポインタを4つ分集める

		// 頂点座標の設定
		pVtx[0].pos = pParticle->pos + D3DXVECTOR3(-pParticle->fWidth, -pParticle->fHeight, 0.0f);
		pVtx[1].pos = pParticle->pos + D3DXVECTOR3(pParticle->fWidth, -pParticle->fHeight, 0.0f);
		pVtx[2].pos = pParticle->pos + D3DXVECTOR3(-pParticle->fWidth, pParticle->fHeight, 0.0f);
		pVtx[3].pos = pParticle->pos + D3DXVECTOR3(pParticle->fWidth, pParticle->fHeight, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = pParticle->color.col;
		pVtx[1].col = pParticle->color.col;
		pVtx[2].col = pParticle->color.col;
		pVtx[3].col = pParticle->color.col;

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

// --------------------------------------------------
// 描画
// --------------------------------------------------
void CParticle::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 点に貼る(true)、ポリゴンに貼る(false)
	// pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE,true);

	// カメラの位置(true)、スクリーンの位置(false)
	// pDevice->SetRenderState(D3DRS_POINTSCALEENABLE,false);

	// サイズを設定
	// pDevice->SetRenderState(D3DRS_POINTSIZE,FloattoDword(1.0f));
	// pDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FloattoDword(0.0f));

	// ポイントサイズの計算
	// pDevice->SetRenderState(D3DRS_POINTSCALE_A, FloattoDword(0.0f));
	// pDevice->SetRenderState(D3DRS_POINTSCALE_B, FloattoDword(0.0f));
	// pDevice->SetRenderState(D3DRS_POINTSCALE_C, FloattoDword(1.0f));

	for (int nCnt = 0; nCnt < maxNumber; nCnt++)
	{
		if (!g_aParticle[nCnt].bUse)
		{
			continue;
		}

		/* ↓使用しているなら↓ */

		switch (g_aParticle[nCnt].alphaBlend)
		{
		case TYPE_NONE:		// 乗算
			break;

		case TYPE_ADD:		// 加算
			// アルファブレンディングを加算合成に設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			break;

		case TYPE_SUB:			// 減算
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
		pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, pTexture[g_aParticle[nCnt].type]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		// pDevice->DrawPrimitive(D3DPT_POINTLIST,0, nCnt);

		// αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	// ポイントスプライトを解除する
	// pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);

	// テクスチャを引き継がない
	pDevice->SetTexture(0, NULL);
}

// --------------------------------------------------
// 生成
// --------------------------------------------------
void CParticle::Create(Particle& inParticle)
{
	for (int i = 0; i < maxNumber; i++)
	{
		Particle* pParticle = &g_aParticle[i];

		if (pParticle->bUse)
		{
			continue;
		}

		/* ↓使用されてないなら↓ */

		// データのリセット
		Delete(i);

		*pParticle = inParticle;
		pParticle->type = PARTICLETYPE_NORMAL;

		pParticle->fWidth = g_aParticle->fScale;
		pParticle->fHeight = g_aParticle->fScale;
		pParticle->color.nCntTransitionTime = 0;
		pParticle->bUse = true;

		// 生成位置の算出
		pParticle->pos.x += FloatRandam(pParticle->maxPopPos.x, -pParticle->minPopPos.x);
		pParticle->pos.y += FloatRandam(pParticle->maxPopPos.y, -pParticle->minPopPos.y);
		pParticle->pos.z += FloatRandam(pParticle->maxPopPos.z, -pParticle->minPopPos.z);

		// 色の算出
		if (pParticle->color.bColRandom)
		{// ランダムカラーを使用
			pParticle->color.col.r = FloatRandam(pParticle->color.colRandamMax.r, pParticle->color.colRandamMin.r);
			pParticle->color.col.g = FloatRandam(pParticle->color.colRandamMax.g, pParticle->color.colRandamMin.g);
			pParticle->color.col.b = FloatRandam(pParticle->color.colRandamMax.b, pParticle->color.colRandamMin.b);

			if (pParticle->color.bColTransition)
			{// 目的の色の設定
				if (pParticle->color.bRandomTransitionTime)
				{
					pParticle->color.nEndTime = rand() % pParticle->nLife + 1;
				}

				pParticle->color.destCol.r = FloatRandam(pParticle->color.colRandamMax.r, pParticle->color.colRandamMin.r);
				pParticle->color.destCol.g = FloatRandam(pParticle->color.colRandamMax.g, pParticle->color.colRandamMin.g);
				pParticle->color.destCol.b = FloatRandam(pParticle->color.colRandamMax.b, pParticle->color.colRandamMin.b);
			}
		}

		if (pParticle->color.bColTransition)
		{// トラディシオンカラーを使用
			if (pParticle->color.bRandomTransitionTime)
			{
				pParticle->color.nEndTime = rand() % pParticle->nLife + 1;
			}

			pParticle->color.colTransition.r = (pParticle->color.destCol.r - pParticle->color.col.r) / pParticle->color.nEndTime;
			pParticle->color.colTransition.g = (pParticle->color.destCol.g - pParticle->color.col.g) / pParticle->color.nEndTime;
			pParticle->color.colTransition.b = (pParticle->color.destCol.b - pParticle->color.col.b) / pParticle->color.nEndTime;
		}

		VERTEX_2D*pVtx;	// 頂点情報へのポインタ

						// 頂点バッファをロックし、頂点情報へのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;	// 頂点データのポインタを4つ分集める

						// 頂点座標の設定
		pVtx[0].pos = pParticle->pos + D3DXVECTOR3(-pParticle->fWidth, -pParticle->fHeight, 0.0f);
		pVtx[1].pos = pParticle->pos + D3DXVECTOR3(pParticle->fWidth, -pParticle->fHeight, 0.0f);
		pVtx[2].pos = pParticle->pos + D3DXVECTOR3(-pParticle->fWidth, pParticle->fHeight, 0.0f);
		pVtx[3].pos = pParticle->pos + D3DXVECTOR3(pParticle->fWidth, pParticle->fHeight, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = pParticle->color.col;
		pVtx[1].col = pParticle->color.col;
		pVtx[2].col = pParticle->color.col;
		pVtx[3].col = pParticle->color.col;

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();

		float ImAngle = GetAngle();
		float fRad = 0.0f;
		float fGRad = 0.0f;

		if (pParticle->bBackrot)
		{
			// float fRad = (pParticle->fAngle) * (D3DX_PI / 180);
			fGRad = (pParticle->rot.z - g_fAngle);
		}
		else
		{
			fRad = (pParticle->fAngle) * (D3DX_PI / 180);
			fGRad = (pParticle->rot.z + g_fAngle);
		}

		// 挙動
		{
			/*
			g_fAngle += 30.0f * i;
			pParticle->move.x = sinf(fGRad) * 1.3f;
			pParticle->move.y = cosf(fGRad) * 1.3f;

			// ∞
			g_fAngle += 0.7f;
			pParticle->move.x = sinf((D3DX_PI / 180) * 17 * g_fAngle) * pParticle->fAttenuation;
			pParticle->move.y = sinf((D3DX_PI / 180) * 8 * g_fAngle) * pParticle->fAttenuation;
			*/

			// 螺旋だったり
			g_fAngle += ImAngle;
			pParticle->move.x += (pParticle->fRadius * sinf(fGRad)) * pParticle->fAttenuation;
			pParticle->move.y += (pParticle->fRadius * cosf(fGRad)) * pParticle->fAttenuation;
		}

		// ======================
		// 正規化
		// ======================
		if (pParticle->fRadius > D3DX_PI)
		{
			pParticle->fRadius -= D3DX_PI * 2;
		}
		else if (pParticle->fRadius < -D3DX_PI)
		{
			pParticle->fRadius += D3DX_PI * 2;
		}

		if (g_fAngle > D3DX_PI)
		{
			g_fAngle -= D3DX_PI * 2;
		}
		else if (g_fAngle < -D3DX_PI)
		{
			g_fAngle += D3DX_PI * 2;
		}

		break;
	}
}

// --------------------------------------------------
// テクスチャの読込み
// --------------------------------------------------
void CParticle::LoadTex()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	char ImFile[512];
	bool ImTex = TexUse();

	memset(ImFile, 0, sizeof(ImFile));

	if (ImTex)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			GetFileName(),
			&pTexture[PARTICLETYPE_NORMAL]);

		ImTex = false;
	}
}

// --------------------------------------------------
// 情報の削除
// --------------------------------------------------
void CParticle::Delete(const int data)
{
	// データのリセット
	memset(&g_aParticle[data], 0, sizeof(g_aParticle[data]));
}

// --------------------------------------------------
// 情報を全て削除
// --------------------------------------------------
void CParticle::DeleteAll()
{
	for (int i = 0; i < maxNumber; i++)
	{
		Delete(i);
	}
}

// --------------------------------------------------
// 角度の初期化
// --------------------------------------------------
void CParticle::RemoveAngle(void)
{
	g_fAngle = 0;
}

// --------------------------------------------------
// float を DWORD に変換
// --------------------------------------------------
DWORD CParticle::FloattoDword(float fVal)
{
	return *((DWORD*)&fVal);
}
