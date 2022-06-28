//＝＝＝＝＝＝＝＝＝＝＝＝＝
//time.cpp
//＝＝＝＝＝＝＝＝＝＝＝＝＝
#include "main.h"
#include"time.h"

static LPDIRECT3DTEXTURE9 s_pTextureTime = NULL; //テクスチャのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffTime = NULL; //頂点バッファの設定
static D3DXVECTOR3 s_posTime; //スコアの位置
static int s_nTime;

//===================
//初期化処理
//===================
void InitTime(void)
{
	LPDIRECT3DDEVICE9  pDevice;
	int nCntTime;
	s_nTime = 0;

	s_posTime = D3DXVECTOR3(550.0f, 50.0f, 0.0f);		//位置設定

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number000.png",
		&s_pTextureTime);

	//頂点バッファ
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffTime,
		NULL);

	VERTEX_2D*pVtx;		//頂点へのポインタ

	//頂点バッファをロックし頂点情報へのポインタを取得
	s_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(s_posTime.x - 30.0f, s_posTime.y - 30.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(s_posTime.x + 30.0f, s_posTime.y - 30.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(s_posTime.x - 30.0f, s_posTime.y + 30.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(s_posTime.x + 30.0f, s_posTime.y + 30.0f, 0.0f);

		s_posTime += D3DXVECTOR3(50.0f, 0.0f, 0.0f);

		if (nCntTime == 2)
		{
			s_posTime += D3DXVECTOR3(20.0f, 0.0f, 0.0f);
		}

		//RHWの設定
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

		pVtx += 4; //頂点ポイントを四つ進む
	}

	//頂点バッファをアンロック
	s_pVtxBuffTime->Unlock();

	SetTime(6000);
}

//===================
//破棄処理
//===================
void UninitTime(void)
{
	//テクスチャの破棄
	if (s_pTextureTime != NULL)
	{
		s_pTextureTime->Release();
		s_pTextureTime = NULL;
	}

	//頂点バッファの破棄
	if (s_pVtxBuffTime != NULL)
	{
		s_pVtxBuffTime->Release();
		s_pVtxBuffTime = NULL;
	}

}

//===================
//更新処理
//===================
void UpdateTime(void)
{
	if (s_nTime != 0)
	{
		AddTime(-1);
	}
}

//===================
//描画処理
//===================
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスのポインタ
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTextureTime);

	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			nCntTime * 4,
			2);
	}
}

//===================
//セット
//===================
void SetTime(int nTime)
{
	int nCntTime;
	int aPosTexU[MAX_TIME]; //各桁の数字を格納

	s_nTime = nTime;

	aPosTexU[0] = (s_nTime % 100000) / 10000;
	aPosTexU[1] = (s_nTime % 10000) / 1000;
	aPosTexU[2] = (s_nTime % 1000) / 100;

	VERTEX_2D*pVtx; //頂点へのポインタ

	//頂点バッファをロックし頂点情報へのポインタを取得
	s_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		//テクスチャの座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + aPosTexU[nCntTime] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aPosTexU[nCntTime] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aPosTexU[nCntTime] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aPosTexU[nCntTime] * 0.1f, 1.0f);

		pVtx += 4; //頂点ポイントを四つ進む
	}

	//頂点バッファをアンロック
	s_pVtxBuffTime->Unlock();
}

//===================
//加算
//===================
void AddTime(int nValue)
{
	int nCntTime;
	int aPosTexU[MAX_TIME]; //各桁の数字を格納

	s_nTime += nValue;

	aPosTexU[0] = (s_nTime % 100000) / 10000;
	aPosTexU[1] = (s_nTime % 10000) / 1000;
	aPosTexU[2] = (s_nTime % 1000) / 100;

	VERTEX_2D*pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	s_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		//テクスチャの座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + aPosTexU[nCntTime] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + aPosTexU[nCntTime] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + aPosTexU[nCntTime] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + aPosTexU[nCntTime] * 0.1f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロック
	s_pVtxBuffTime->Unlock();
}

int GetTime(void)
{
	return s_nTime;
}