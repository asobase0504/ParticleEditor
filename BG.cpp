#include "main.h"
#include "input.h"
#include "BG.h"

//マクロ定義
#define NUM_BG (2)

//グローバル変数
LPDIRECT3DTEXTURE9	g_pTextureBg = NULL;
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffBg = NULL;

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//BGの初期化処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void InitBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	//無地
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\aquaBG.jpg",
		&g_pTextureBg);

	//for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	//{
	//	g_aBg.nCounterAnim = 0;		//カウンターを初期化させる
	//	g_aBg.nPatternAnim = 0;		//アニメーションパターンNo.
	//}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,    //確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg,
		NULL);

	VERTEX_2D*pVtx;				 //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

	//rhw
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	pVtx += 4;

	//頂点バッファのアンロック
	g_pVtxBuffBg->Unlock();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//BGの終了処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void UninitBg(void)
{
	//テクスチャの破棄
	if (g_pTextureBg != NULL)
	{
		g_pTextureBg->Release();
		g_pTextureBg = NULL;
	}

	//頂点バッファの破壊
	if (g_pVtxBuffBg != NULL)
	{
		g_pVtxBuffBg->Release();
		g_pVtxBuffBg = NULL;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//BGの更新処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void UpdateBg(void)
{
	//テクスチャ座標の更新
	VERTEX_2D*pVtx;

	//g_aBg.nCounterAnim++;		//カウンターを加算

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	//for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	//{
	//	if ((g_aBg.nCounterAnim % 60) == 0)
	//	{//一定時間経過
	//	 //パターンNo.を更新
	//		g_aBg.nPatternAnim = (g_aBg.nPatternAnim + 1) % 2;
	//		g_aBg.nPatternAnim++;

	//		if (g_aBg.nPatternAnim == 8)
	//		{
	//			g_aBg.nPatternAnim = 0;
	//		}
	//	}
	//	pVtx += 4;
	//}

	//頂点バッファのアンロック
	g_pVtxBuffBg->Unlock();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//BGの描画処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void DrawBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;        //デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBg, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBg);

	//ポリゴンの描画
	for (int nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			nCntBG * 4,
			2);						//プリミティブ(ポリゴン)数
	}
}
