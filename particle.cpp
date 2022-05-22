//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//effect.cpp
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
#include "main.h"
#include "particle.h"

//グローバル変数
static LPDIRECT3DTEXTURE9		s_pTexture[NUM_PARTICLE] = {};	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;	//頂点バッファへのポインタ
Particle g_aParticle[MAX_PARTICLE];
float g_fAngle = 0.0f;

//初期化処理
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\flare.png",
		&s_pTexture[PARTICLETYPE_NORMAL]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bright.jpg",
		&s_pTexture[PARTICLETYPE_PLAYER]);

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		g_aParticle[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle[i].fWidth = 0.0f;
		g_aParticle[i].fHeight = 0.0f;
		g_aParticle[i].fRadius = 0.0f;
		g_aParticle[i].nLife = 0;
		g_aParticle[i].bUse = false;		//使用していない状態にする
	}

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

		pVtx[0].tex2 = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex2 = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex2 = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex2 = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;		//頂点データのポインタを4つ分集める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//終了処理
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

//更新処理
void UpdateParticle(void)
{
	VERTEX_2D *pVtx = nullptr;		//頂点情報へのポインタ

	//頂点バッファをロック
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//(ImGui)
	D3DXVECTOR3 ImPos = GetPos();
	D3DXVECTOR3 ImMove = GetMove();
	D3DXVECTOR3 ImRot = GetRot();
	D3DXCOLOR ImColor = GetColor();
	int ImSelect = GetType();
	float ImRadius = GetRadius();
	bool bEnable = bSetEffect();
	bool bBackRot = BackRot();

	float fRad = 0;
	float fGRad = 0;
	g_fAngle = 0;

	if (bEnable)
	{
		SetParticle(D3DXVECTOR3(ImPos.x, ImPos.y, ImPos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(ImColor.r, ImColor.g, ImColor.b, ImColor.a), 0, 50.0f, 50.0f, PARTICLETYPE_NORMAL);
	}

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		if (g_aParticle[i].bUse)
		{//エフェクトが使用されているなら
		 //エフェクトの移動
			g_aParticle[i].pos += g_aParticle[i].move;
			g_aParticle[i].pos += ImMove;

			g_aParticle[i].fRadius = ImRadius;

			if (bBackRot)
			{
				//float fRad = (g_aParticle[i].fAngle) * (D3DX_PI / 180);
				fGRad = (ImRot.z - g_fAngle);
			}

			else if (!bBackRot)
			{
				fRad = (g_aParticle[i].fAngle) * (D3DX_PI / 180);
				fGRad = (ImRot.z + g_fAngle);
			}

			//色変更（ImGui）
			switch (ImSelect)
			{
			case 1:
				g_aParticle[i].col.g -= 0.01f;
				g_aParticle[i].col.r++;
				break;

			case 2:
				g_aParticle[i].col.b -= 0.01f;
				g_aParticle[i].col.g++;
				break;

			case 3:
				g_aParticle[i].col.r -= 0.01f;
				g_aParticle[i].col.b++;
				break;

			case 0:
				break;

			default:
				break;
			}

			/*g_aParticle[i].col.r -= 0.01f;
			g_aParticle[i].col.b++;*/

			//
			//エフェクトの種類
			//
			switch (g_aParticle[i].type)
			{
			case PARTICLETYPE_NORMAL:
				//*
				/*g_fAngle += 30.0f * i;
				g_aParticle[i].move.x = sinf(fGRad) * 1.3f;
				g_aParticle[i].move.y = cosf(fGRad) * 1.3f;*/
				break;

			case PARTICLETYPE_PLAYER:
				break;

			default:
				break;
			}

			//∞
		/*	g_fAngle += 0.3f;
			g_aParticle[i].move.x = sinf((D3DX_PI / 180) * 17 * g_fAngle) * 3.0f;
			g_aParticle[i].move.y = sinf((D3DX_PI / 180) * 8 * g_fAngle) * 3.0f;*/

			//ふにゃふにゃ～～～
		/*	g_fAngle += 0.0135f;
			g_aParticle[i].move.x = 9 * powf(cosf(D3DX_PI * g_fAngle), 2.0f);
			g_aParticle[i].move.y = 9 * powf(sinf(D3DX_PI * g_fAngle), 2.0f);*/

			//螺旋だったり
			for (int j = 0; j < 1024; j++)
			{
				g_fAngle += 21.0f;
				//g_aParticle[i].fRadius = 10.0f;
				g_aParticle[i].move.x = (g_aParticle[i].fRadius * sinf(fGRad));
				g_aParticle[i].move.y = (g_aParticle[i].fRadius * cosf(fGRad));
			}

			g_aParticle[i].nLife--;

			if (g_aParticle[i].nLife <= 0)
			{//エフェクトの寿命
				DeleteParticle(i);
			}

			//======================
			//正規化
			//======================
			if (g_aParticle[i].fRadius > D3DX_PI)
			{
				g_aParticle[i].fRadius -= D3DX_PI * 2;
			}

			else if (g_aParticle[i].fRadius < -D3DX_PI)
			{
				g_aParticle[i].fRadius += D3DX_PI * 2;
			}

			//頂点座標の設定
			pVtx[0].pos = g_aParticle[i].pos + D3DXVECTOR3(-g_aParticle[i].fWidth, -g_aParticle[i].fHeight, 0.0f);
			pVtx[1].pos = g_aParticle[i].pos + D3DXVECTOR3(g_aParticle[i].fWidth, -g_aParticle[i].fHeight, 0.0f);
			pVtx[2].pos = g_aParticle[i].pos + D3DXVECTOR3(-g_aParticle[i].fWidth, g_aParticle[i].fHeight, 0.0f);
			pVtx[3].pos = g_aParticle[i].pos + D3DXVECTOR3(g_aParticle[i].fWidth, g_aParticle[i].fHeight, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = g_aParticle[i].col;
			pVtx[1].col = g_aParticle[i].col;
			pVtx[2].col = g_aParticle[i].col;
			pVtx[3].col = g_aParticle[i].col;
		}
		pVtx += 4;		//頂点データのポインタを4つ分集める
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//描画処理
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得

	//アルファブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
	{
		if (g_aParticle[nCnt].bUse)
		{//エフェクトが使用されている
		 //頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			//テクスチャの設定
			pDevice->SetTexture(0, s_pTexture[g_aParticle[nCnt].type]);

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
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fWidth, float fHeight, PARTICLETYPE type)
{
	VERTEX_2D*pVtx;		//頂点情報へのポインタ
	int ImLife = GetLife();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		if (!g_aParticle[i].bUse)
		{//エフェクトが使用されている
			g_aParticle[i].pos = pos;
			g_aParticle[i].move = move;
			g_aParticle[i].col = col;
			g_aParticle[i].type = type;
			g_aParticle[i].fWidth = fWidth;
			g_aParticle[i].fHeight = fHeight;

			//(ImGui)
			g_aParticle[i].nLife = ImLife;
			//g_aParticle[i].nLife = nLife;

			//頂点座標の設定
			pVtx[0].pos = g_aParticle[i].pos + D3DXVECTOR3(-g_aParticle[i].fWidth, -g_aParticle[i].fHeight, 0.0f);
			pVtx[1].pos = g_aParticle[i].pos + D3DXVECTOR3(g_aParticle[i].fWidth, -g_aParticle[i].fHeight, 0.0f);
			pVtx[2].pos = g_aParticle[i].pos + D3DXVECTOR3(-g_aParticle[i].fWidth, g_aParticle[i].fHeight, 0.0f);
			pVtx[3].pos = g_aParticle[i].pos + D3DXVECTOR3(g_aParticle[i].fWidth, g_aParticle[i].fHeight, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aParticle[i].bUse = true;

			break;
		}
		pVtx += 4;		//頂点データのポインタを4つ分集める
	}
	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//削除処理
void DeleteParticle(int nDelete)
{
	g_aParticle[nDelete].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aParticle[nDelete].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aParticle[nDelete].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aParticle[nDelete].type = PARTICLETYPE_NONE;
	g_aParticle[nDelete].fWidth = 0.0f;
	g_aParticle[nDelete].fHeight = 0.0f;
	g_aParticle[nDelete].fAngle = 0.0f;
	g_aParticle[nDelete].fRadius = 0.0f;
	g_aParticle[nDelete].nLife = 0;
	g_aParticle[nDelete].bUse = false;
}

void RemoveAngle(void)
{
	g_fAngle = 0;
}

Particle *GetParticle(void)
{
	return &g_aParticle[0];
}