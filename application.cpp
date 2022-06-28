//=========================================
// 
// アプリケーションクラス
// Author YudaKaito
// 
//=========================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "application.h"
#include "input.h"
#include "renderer.h"
#include "texture.h"
#include "object2d.h"
#include "file.h"

#include "particle.h"
#include "BG.h"
#include "imgui_property.h"

//-----------------------------------------------------------------------------
// 静的メンバー変数の初期化
//-----------------------------------------------------------------------------
CApplication* CApplication::application = nullptr;

//=============================================================================
// シングルトンでのインスタンスの取得
//=============================================================================
CApplication* CApplication::GetInstance()
{
	if (application == nullptr)
	{
		application = new CApplication;
	}

	return application;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CApplication::CApplication() :
	renderer(nullptr),
	object(nullptr)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CApplication::~CApplication()
{
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CApplication::Init(HWND hWnd, HINSTANCE hInstance)
{
	//乱数の初期化
	srand((unsigned int)time(0));

	// レンダリングクラス
	renderer = new CRenderer;
	if (FAILED(renderer->Init(hWnd, true)))
	{
		return E_FAIL;
	}

	//入力処理の初期化処理
	if (FAILED(InitInput(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// テクスチャ
	texture = new CTexture;

	// パーティクル
	LoadJson(L"data/FILE/Effect.json");

	SetTex();

	// 背景
	bg = new CBg;
	if (FAILED(bg->Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CApplication::Uninit()
{
	CObject::ReleaseAll();
	texture->UnloadAll();

	if (texture != nullptr)
	{
		texture->SavePath();
		delete texture;
		texture = nullptr;
	}

	if (renderer != nullptr)
	{
		renderer->Uninit();

		delete renderer;
		renderer = nullptr;
	}

	if (application != nullptr)
	{
		delete application;
		application = nullptr;
	}
}

//=============================================================================
// 更新
//=============================================================================
void CApplication::Update()
{
	UpdateInput();

	renderer->Update();

	if (bSetImguiParticle())
	{
		CParticle::Create(GetImguiParticle().particle, GetImguiParticle().pos);
	}
}

//=============================================================================
// 描画
//=============================================================================
void CApplication::Draw()
{
	renderer->Draw();
}

//=============================================================================
// レンダリングのインスタンス取得
//=============================================================================
CRenderer* CApplication::GetRenderer()
{
	return renderer;
}

//=============================================================================
// オブジェクトのインスタンス取得
//=============================================================================
CObject** CApplication::GetMyObject()
{
	return &object;
}

//=============================================================================
// テクスチャクラスの取得
//=============================================================================
CTexture * CApplication::GetTextureClass()
{
	return texture;
}
