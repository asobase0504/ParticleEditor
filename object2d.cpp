//=========================================
// 
// オブジェクトクラス
// Author YudaKaito
// 
//=========================================
#include "object2d.h"
#include "application.h"
#include "renderer.h"
#include "texture.h"
#include "main.h"
#include <assert.h>

//=============================================================================
// コンストラクタ
//=============================================================================
CObject2D::CObject2D() :
	m_pVtxBuff(nullptr),
	m_nTexture(-1),
	m_rotY(0.0f),
	m_fLength(0.0f),
	fAngle(0.0f),
	m_col(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CObject2D::~CObject2D()
{
}

//=============================================================================
// ポリゴンの初期化
//=============================================================================
HRESULT CObject2D::Init()
{
	// 頂点バッファの生成
	CApplication::GetInstance()->GetRenderer()->GetDevice()->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	m_fLength = sqrtf(size.x * size.x + size.y * size.y) * 0.5f;
	fAngle = atan2f(size.x, size.y);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = SCREEN_WIDTH * 0.5f + sinf(m_rotY + fAngle + -D3DX_PI) * m_fLength;
	pVtx[0].pos.y = SCREEN_HEIGHT * 0.5f + cosf(m_rotY + fAngle + -D3DX_PI) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = SCREEN_WIDTH * 0.5f + sinf(m_rotY + -fAngle + D3DX_PI) * m_fLength;
	pVtx[1].pos.y = SCREEN_HEIGHT * 0.5f + cosf(m_rotY + -fAngle + D3DX_PI) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = SCREEN_WIDTH * 0.5f + sinf(m_rotY + fAngle * -1.0f) * m_fLength;
	pVtx[2].pos.y = SCREEN_HEIGHT * 0.5f + cosf(m_rotY + fAngle * -1.0f) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = SCREEN_WIDTH * 0.5f + sinf(m_rotY + fAngle) * m_fLength;
	pVtx[3].pos.y = SCREEN_HEIGHT * 0.5f + cosf(m_rotY + fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// texの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// ポリゴンの終了
//=============================================================================
void CObject2D::Uninit()
{
	// 頂点バッファの破壊
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//=============================================================================
// ポリゴンの更新
//=============================================================================
void CObject2D::Update()
{
	m_rotY += -0.01f;
	size.x += 5.0f * sinf(m_rotY);
	size.y += 5.0f * sinf(m_rotY);

	SetSize(size);
	SetPos(pos);
}

//=============================================================================
// ポリゴンの描画
//=============================================================================
void CObject2D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	LPDIRECT3DTEXTURE9 texture = CApplication::GetInstance()->GetTextureClass()->GetTexture(m_nTexture);

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// テクスチャの破棄
	pDevice->SetTexture(0, NULL);
}

//=============================================================================
// 位置の設定
//=============================================================================
void CObject2D::SetPos(const D3DXVECTOR3 & inPos)
{
	CObject::SetPos(inPos);

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = pos.x + sinf(m_rotY + fAngle + -D3DX_PI) * m_fLength;
	pVtx[0].pos.y = pos.y + cosf(m_rotY + fAngle + -D3DX_PI) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + sinf(m_rotY + -fAngle + D3DX_PI) * m_fLength;
	pVtx[1].pos.y = pos.y + cosf(m_rotY + -fAngle + D3DX_PI) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x + sinf(m_rotY + fAngle * -1.0f) * m_fLength;
	pVtx[2].pos.y = pos.y + cosf(m_rotY + fAngle * -1.0f) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + sinf(m_rotY + fAngle) * m_fLength;
	pVtx[3].pos.y = pos.y + cosf(m_rotY + fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 大きさの設定
//=============================================================================
void CObject2D::SetSize(const D3DXVECTOR2 & inSize)
{
	size = inSize;
	m_fLength = sqrtf(size.x * size.x + size.y * size.y) * 0.5f;
	fAngle = atan2f(size.x, size.y);

	SetPos(pos);
}

void CObject2D::SetColor(const D3DXCOLOR & inColor)
{
	m_col = inColor;

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ポリゴンクラスの作成
//=============================================================================
CObject2D* CObject2D::Create()
{
	if (numAll >= NUM_MAX)
	{
		return nullptr;
	}

	int idx = numAll;
	CObject2D* objectCreate = new CObject2D;
	objectCreate->createIdx = idx;

	if (objectCreate != nullptr)
	{
		if (FAILED(objectCreate->Init()))
		{
			assert(false);
		}
	}

	return objectCreate;
}
