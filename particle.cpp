//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//effect.cpp
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
#include "main.h"
#include "particle.h"
#include <time.h>
#include "imgui_property.h"
#include "utility.h"

//==================================================
// グローバル変数
//==================================================
static LPDIRECT3DTEXTURE9		s_pTexture[NUM_PARTICLE] = {};	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;	//頂点バッファへのポインタ
Particle g_aParticle[MAX_PARTICLE];
float g_fAngle = 0.0f;

//--------------------------------------------------
// 初期化処理
//--------------------------------------------------
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\flare.png",
		&s_pTexture[PARTICLETYPE_NORMAL]);

	//テクスチャの読み込み
	memset(g_aParticle, 0, sizeof(g_aParticle));

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTICLE,	//確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			//頂点フォーマット
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;		//頂点データのポインタを4つ分集める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//--------------------------------------------------
// 終了処理
//--------------------------------------------------
void UninitParticle(void)
{
	for (int i = 0; i < NUM_PARTICLE; i++)
	{
		//テクスチャの破棄
		if (s_pTexture[i] != NULL)
		{
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}

	//頂点バッファの破壊
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// 更新処理
//--------------------------------------------------
void UpdateParticle(void)
{
	//(ImGui)
	bool bTex = TexUse();

	if (bTex)
	{
		LoadTex();
	}

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		Particle* pParticle = &g_aParticle[i];

		if (!pParticle->bUse)
		{//エフェクトが使用されているなら
			continue;
		}

		/* ↓使用しているなら↓ */

		//エフェクトの移動
		pParticle->pos += pParticle->move;

		// 推移
		pParticle->nLife--;											// 体力の減少
		pParticle->move.y += pParticle->fWeight;					// 重力
		pParticle->color.col += pParticle->color.colTransition;	// 色の推移
		pParticle->move *= pParticle->fAttenuation;					// 移動量の推移
		pParticle->fWeight += pParticle->fWeightTransition;			// 重さの推移

		if (pParticle->nLife <= 0)
		{//エフェクトの寿命
			DeleteParticle(i);
		}

		VERTEX_2D *pVtx = nullptr;		//頂点情報へのポインタ

		//頂点バッファをロック
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;		//頂点データのポインタを4つ分集める

		//頂点座標の設定
		pVtx[0].pos = pParticle->pos + D3DXVECTOR3(-pParticle->fWidth, -pParticle->fHeight, 0.0f);
		pVtx[1].pos = pParticle->pos + D3DXVECTOR3(pParticle->fWidth, -pParticle->fHeight, 0.0f);
		pVtx[2].pos = pParticle->pos + D3DXVECTOR3(-pParticle->fWidth, pParticle->fHeight, 0.0f);
		pVtx[3].pos = pParticle->pos + D3DXVECTOR3(pParticle->fWidth, pParticle->fHeight, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = pParticle->color.col;
		pVtx[1].col = pParticle->color.col;
		pVtx[2].col = pParticle->color.col;
		pVtx[3].col = pParticle->color.col;
		
		//頂点バッファをアンロックする
		s_pVtxBuff->Unlock();
	}
}

//--------------------------------------------------
// 描画処理
//--------------------------------------------------
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得

	//点に貼る(true)、ポリゴンに貼る(false)
	//pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE,true);

	//カメラの位置(true)、スクリーンの位置(false)
	//pDevice->SetRenderState(D3DRS_POINTSCALEENABLE,false);

	//サイズを設定
	//pDevice->SetRenderState(D3DRS_POINTSIZE,FloattoDword(1.0f));
	//pDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FloattoDword(0.0f));

	//ポイントサイズの計算
	//pDevice->SetRenderState(D3DRS_POINTSCALE_A, FloattoDword(0.0f));
	//pDevice->SetRenderState(D3DRS_POINTSCALE_B, FloattoDword(0.0f));
	//pDevice->SetRenderState(D3DRS_POINTSCALE_C, FloattoDword(1.0f));

	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
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
			//アルファブレンディングを加算合成に設定
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
		
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture[g_aParticle[nCnt].type]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		//pDevice->DrawPrimitive(D3DPT_POINTLIST,0, nCnt);

		//αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	//ポイントスプライトを解除する
	//pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);

	//テクスチャを引き継がない
	pDevice->SetTexture(0, NULL);
}

//--------------------------------------------------
// 設定処理
//--------------------------------------------------
void SetParticleImgui(Particle& inParticle)
{
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		Particle* pParticle = &g_aParticle[i];

		if (pParticle->bUse)
		{
			continue;
		}

		/* ↓使用されてないなら↓ */

		// データのリセット
		DeleteParticle(i);

		*pParticle = inParticle;
		pParticle->type = PARTICLETYPE_NORMAL;

		// αブレンディングの種別
		pParticle->alphaBlend = g_aParticle->alphaBlend;

		pParticle->fWidth = g_aParticle->fScale;
		pParticle->fHeight = g_aParticle->fScale;
		pParticle->bUse = true;

		// 生成位置の算出
		pParticle->pos.x += FloatRandam(pParticle->maxPopPos.x, -pParticle->minPopPos.x);
		pParticle->pos.y += FloatRandam(pParticle->maxPopPos.y, -pParticle->minPopPos.y);
		pParticle->pos.z += FloatRandam(pParticle->maxPopPos.z, -pParticle->minPopPos.z);

		VERTEX_2D*pVtx;	// 頂点情報へのポインタ

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
		s_pVtxBuff->Unlock();

		float ImAngle = GetAngle();
		float fRad = 0.0f;
		float fGRad = 0.0f;

		if (pParticle->bBackrot)
		{
			//float fRad = (pParticle->fAngle) * (D3DX_PI / 180);
			fGRad = (pParticle->rot.z - g_fAngle);
		}
		else
		{
			fRad = (pParticle->fAngle) * (D3DX_PI / 180);
			fGRad = (pParticle->rot.z + g_fAngle);
		}

		//挙動
		{
			/*
			g_fAngle += 30.0f * i;
			pParticle->move.x = sinf(fGRad) * 1.3f;
			pParticle->move.y = cosf(fGRad) * 1.3f;

			//∞
			g_fAngle += 0.7f;
			pParticle->move.x = sinf((D3DX_PI / 180) * 17 * g_fAngle) * pParticle->fAttenuation;
			pParticle->move.y = sinf((D3DX_PI / 180) * 8 * g_fAngle) * pParticle->fAttenuation;
			*/

			// 螺旋だったり
			g_fAngle += ImAngle;
			pParticle->move.x += (pParticle->fRadius * sinf(fGRad)) * pParticle->fAttenuation;
			pParticle->move.y += (pParticle->fRadius * cosf(fGRad)) * pParticle->fAttenuation;
		}

		//======================
		// 正規化
		//======================
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

// テクスチャの読込み
void LoadTex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	char ImFile[512];
	bool ImTex = TexUse();

	memset(ImFile, 0, sizeof(ImFile));

	if (ImTex)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			GetFileName(),
			&s_pTexture[PARTICLETYPE_NORMAL]);

		ImTex = false;
	}
}

// 削除処理
void DeleteParticle(int nDelete)
{
	// データのリセット
	memset(&g_aParticle[nDelete], 0, sizeof(g_aParticle[nDelete]));
}

void DeleteParticleAll()
{
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		DeleteParticle(i);
	}
}

// 角度の初期化処理
void RemoveAngle(void)
{
	g_fAngle = 0;
}

Particle *GetParticle(void)
{
	return &g_aParticle[0];
}

//FloatをDWORDに変換する
DWORD FloattoDword(float fVal)
{
	return *((DWORD*)&fVal);
}