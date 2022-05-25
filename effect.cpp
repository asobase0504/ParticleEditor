//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//effect.cpp
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
#include "main.h"
#include "effect.h"

//グローバル変数
static LPDIRECT3DTEXTURE9		s_pTexture[NUM_EFFECT] = {};	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;	//頂点バッファへのポインタ
Effect g_aEffect[MAX_EFFECT];

//初期化処理
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\flare.png",
		&s_pTexture[EFFECTTYPE_NORMAL]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bright.jpg",
		&s_pTexture[EFFECTTYPE_PLAYER]);

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		g_aEffect[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[i].fWidth = 0.0f;
		g_aEffect[i].fHeight = 0.0f;
		g_aEffect[i].fRadius = 0.0f;
		g_aEffect[i].nLife = 0;
		g_aEffect[i].bUse = false;		//使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,	//確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			//頂点フォーマット
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
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

//終了処理
void UninitEffect(void)
{
	for (int i = 0; i < NUM_EFFECT; i++)
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

//更新処理
void UpdateEffect(void)
{
	VERTEX_2D *pVtx = nullptr;		//頂点情報へのポインタ

	//頂点バッファをロック
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (g_aEffect[i].bUse)
		{//エフェクトが使用されているなら
		 //エフェクトの移動
			g_aEffect[i].pos += g_aEffect[i].move;

			//色変更
			g_aEffect[i].col.r -= 0.01f;
			g_aEffect[i].col.b++;

			g_aEffect[i].nLife--;

			if (g_aEffect[i].nLife == 0)
			{//エフェクトの寿命
				DeleteEffect(i);
			}

			//======================
			//正規化
			//======================
			if (g_aEffect[i].fRadius > D3DX_PI)
			{
				g_aEffect[i].fRadius -= D3DX_PI * 2;
			}

			else if (g_aEffect[i].fRadius < -D3DX_PI)
			{
				g_aEffect[i].fRadius += D3DX_PI * 2;
			}

			//頂点座標の設定
			pVtx[0].pos = g_aEffect[i].pos + D3DXVECTOR3(-g_aEffect[i].fWidth, -g_aEffect[i].fHeight, 0.0f);
			pVtx[1].pos = g_aEffect[i].pos + D3DXVECTOR3(g_aEffect[i].fWidth, -g_aEffect[i].fHeight, 0.0f);
			pVtx[2].pos = g_aEffect[i].pos + D3DXVECTOR3(-g_aEffect[i].fWidth, g_aEffect[i].fHeight, 0.0f);
			pVtx[3].pos = g_aEffect[i].pos + D3DXVECTOR3(g_aEffect[i].fWidth, g_aEffect[i].fHeight, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = g_aEffect[i].col;
			pVtx[1].col = g_aEffect[i].col;
			pVtx[2].col = g_aEffect[i].col;
			pVtx[3].col = g_aEffect[i].col;
		}
		pVtx += 4;		//頂点データのポインタを4つ分集める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//描画処理
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得

	//アルファブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		if (g_aEffect[nCnt].bUse)
		{//エフェクトが使用されている
		 //頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

		 //テクスチャの設定
			pDevice->SetTexture(0, s_pTexture[g_aEffect[nCnt].type]);

		 //ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				nCnt * 4,				//描画する最初の頂点インデックス
				2);						//描画するプリミティブ数
		}
	}

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//テクスチャを引き継がない
	pDevice->SetTexture(0, NULL);
}

//設定処理
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fWidth, float fHeight, EFFECTTYPE type)
{
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (!g_aEffect[i].bUse)
		{//エフェクトが使用されている
			g_aEffect[i].pos = pos;
			g_aEffect[i].move = move;
			g_aEffect[i].col = col;
			g_aEffect[i].type = type;
			g_aEffect[i].fWidth = fWidth;
			g_aEffect[i].fHeight = fHeight;
			g_aEffect[i].nLife = nLife;

			//頂点座標の設定
			pVtx[0].pos = g_aEffect[i].pos + D3DXVECTOR3(-g_aEffect[i].fWidth, -g_aEffect[i].fHeight, 0.0f);
			pVtx[1].pos = g_aEffect[i].pos + D3DXVECTOR3(g_aEffect[i].fWidth, -g_aEffect[i].fHeight, 0.0f);
			pVtx[2].pos = g_aEffect[i].pos + D3DXVECTOR3(-g_aEffect[i].fWidth, g_aEffect[i].fHeight, 0.0f);
			pVtx[3].pos = g_aEffect[i].pos + D3DXVECTOR3(g_aEffect[i].fWidth, g_aEffect[i].fHeight, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aEffect[i].bUse = true;

			break;
		}
		pVtx += 4;		//頂点データのポインタを4つ分集める
	}
	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//削除処理
void DeleteEffect(int nDelete)
{
	g_aEffect[nDelete].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aEffect[nDelete].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aEffect[nDelete].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aEffect[nDelete].type = EFFECTTYPE_NONE;
	g_aEffect[nDelete].fWidth = 0.0f;
	g_aEffect[nDelete].fHeight = 0.0f;
	g_aEffect[nDelete].fRadius = 0.0f;
	g_aEffect[nDelete].nLife = 0;
	g_aEffect[nDelete].bUse = false;
}