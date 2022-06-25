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
		&pTexture[PARTICLETYPE_NORMAL]);

	// データの初期化
	memset(&data, 0, sizeof(data));

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	// 確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

								// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtxBuff->Unlock();



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
	data.pos += data.move;

	// 推移
	data.nLife--;							// 体力の減少
	data.move.y += data.fWeight;			// 重力
	data.move *= data.fAttenuation;			// 移動量の推移
	data.fWeight += data.fWeightTransition;	// 重さの推移

	if (data.color.bColTransition)
	{// 色の推移
		if (data.color.nEndTime >= data.color.nCntTransitionTime)
		{
			data.color.nCntTransitionTime++;
			data.color.col += data.color.colTransition;
		}
	}
	//pParticle->color.col.a -= 1.0f / pParticle->nMaxLife;

	VERTEX_2D *pVtx = nullptr;		// 頂点情報へのポインタ

									// 頂点バッファをロック
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = data.pos + D3DXVECTOR3(-data.fWidth, -data.fHeight, 0.0f);
	pVtx[1].pos = data.pos + D3DXVECTOR3(data.fWidth, -data.fHeight, 0.0f);
	pVtx[2].pos = data.pos + D3DXVECTOR3(-data.fWidth, data.fHeight, 0.0f);
	pVtx[3].pos = data.pos + D3DXVECTOR3(data.fWidth, data.fHeight, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = data.color.col;
	pVtx[1].col = data.color.col;
	pVtx[2].col = data.color.col;
	pVtx[3].col = data.color.col;

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

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

		if (g_aParticle[i]->data.nLife <= 0)
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

	switch (data.alphaBlend)
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
	pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture[data.type]);

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

			g_aParticle[i]->idx = i;
			return g_aParticle[i];
		}
	}
}

//--------------------------------------------------
// データの初期設定
//--------------------------------------------------
void CParticle::Set(const Particle& inParticle, const D3DXVECTOR3 & inPos)
{
	data = inParticle;

	data.fWidth = data.fScale;
	data.fHeight = data.fScale;
	data.pos = inPos;
	data.type = PARTICLETYPE_NORMAL;

	//data.fWidth = g_aParticle[]->data->fScale;
	//data.fHeight = g_aParticle->data->fScale;
	data.color.nCntTransitionTime = 0;
	data.bUse = true;

	// 生成位置の算出
	data.pos.x += FloatRandam(data.maxPopPos.x, -data.minPopPos.x);
	data.pos.y += FloatRandam(data.maxPopPos.y, -data.minPopPos.y);
	data.pos.z += FloatRandam(data.maxPopPos.z, -data.minPopPos.z);

	// 色の算出
	if (data.color.bColRandom)
	{// ランダムカラーを使用
		data.color.col.r = FloatRandam(data.color.colRandamMax.r, data.color.colRandamMin.r);
		data.color.col.g = FloatRandam(data.color.colRandamMax.g, data.color.colRandamMin.g);
		data.color.col.b = FloatRandam(data.color.colRandamMax.b, data.color.colRandamMin.b);

		if (data.color.bColTransition)
		{// 目的の色の設定
			if (data.color.bRandomTransitionTime)
			{
				data.color.nEndTime = rand() % data.nLife + 1;
			}

			data.color.destCol.r = FloatRandam(data.color.colRandamMax.r, data.color.colRandamMin.r);
			data.color.destCol.g = FloatRandam(data.color.colRandamMax.g, data.color.colRandamMin.g);
			data.color.destCol.b = FloatRandam(data.color.colRandamMax.b, data.color.colRandamMin.b);
		}
	}

	if (data.color.bColTransition)
	{// トラディシオンカラーを使用
		if (data.color.bRandomTransitionTime)
		{
			data.color.nEndTime = rand() % data.nLife + 1;
		}

		data.color.colTransition.r = (data.color.destCol.r - data.color.col.r) / data.color.nEndTime;
		data.color.colTransition.g = (data.color.destCol.g - data.color.col.g) / data.color.nEndTime;
		data.color.colTransition.b = (data.color.destCol.b - data.color.col.b) / data.color.nEndTime;
	}

	VERTEX_2D*pVtx;	// 頂点情報へのポインタ

					// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = data.pos + D3DXVECTOR3(-data.fWidth, -data.fHeight, 0.0f);
	pVtx[1].pos = data.pos + D3DXVECTOR3(data.fWidth, -data.fHeight, 0.0f);
	pVtx[2].pos = data.pos + D3DXVECTOR3(-data.fWidth, data.fHeight, 0.0f);
	pVtx[3].pos = data.pos + D3DXVECTOR3(data.fWidth, data.fHeight, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = data.color.col;
	pVtx[1].col = data.color.col;
	pVtx[2].col = data.color.col;
	pVtx[3].col = data.color.col;

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	float ImAngle = GetAngle();
	float fRad = 0.0f;
	float fGRad = 0.0f;

	if (data.bBackrot)
	{
		// float fRad = (data.fAngle) * (D3DX_PI / 180);
		fGRad = (data.rot.z - g_fAngle);
	}
	else
	{
		fRad = (data.fAngle) * (D3DX_PI / 180);
		fGRad = (data.rot.z + g_fAngle);
	}

	// 挙動
	{
		/*
		g_fAngle += 30.0f * i;
		data.move.x = sinf(fGRad) * 1.3f;
		data.move.y = cosf(fGRad) * 1.3f;

		// ∞
		g_fAngle += 0.7f;
		data.move.x = sinf((D3DX_PI / 180) * 17 * g_fAngle) * data.fAttenuation;
		data.move.y = sinf((D3DX_PI / 180) * 8 * g_fAngle) * data.fAttenuation;
		*/

		// 螺旋だったり
		g_fAngle += ImAngle;
		data.move.x += (data.fRadius * sinf(fGRad)) * data.fAttenuation;
		data.move.y += (data.fRadius * cosf(fGRad)) * data.fAttenuation;
	}

	// ======================
	// 正規化
	// ======================
	if (data.fRadius > D3DX_PI)
	{
		data.fRadius -= D3DX_PI * 2;
	}
	else if (data.fRadius < -D3DX_PI)
	{
		data.fRadius += D3DX_PI * 2;
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
	bool *ImTex = TexUse();

	memset(ImFile, 0, sizeof(ImFile));

	if (ImTex)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, GetFileName(), &pTexture[PARTICLETYPE_NORMAL]);

		ImTex = false;
	}
}

//--------------------------------------------------
// 情報の削除
//--------------------------------------------------
void CParticle::Delete(const int data)
{
	// データのリセット
	memset(&this->data, 0, sizeof(this->data));
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
