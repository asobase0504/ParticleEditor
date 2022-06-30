#include "input.h"
#include "BG.h"
#include "application.h"

//マクロ定義
#define NUM_BG (2)

//グローバル変数
LPDIRECT3DTEXTURE9	g_pTextureBg = NULL;
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffBg = NULL;

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CBg::CBg()
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CBg::~CBg()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CBg::Init()
{
	CObject2D::Init();
	SetPos(D3DXVECTOR3(CApplication::SCREEN_WIDTH * 0.5f, CApplication::SCREEN_HEIGHT * 0.5f, 0.0f));
	SetSize(D3DXVECTOR2((float)CApplication::SCREEN_WIDTH, (float)CApplication::SCREEN_HEIGHT));
	SetColor(D3DXCOLOR(0.2f, 0.2f, 0.22f, 1.0f));
	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CBg::Uninit()
{
	CObject2D::Uninit();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CBg::Update()
{
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CBg::Draw()
{
	CObject2D::Draw();
}
