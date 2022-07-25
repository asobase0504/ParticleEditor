//=============================================================================
// 
// アプリケーションクラス
// Author YudaKaito
// 
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "application.h"
#include "input.h"
#include "renderer.h"
#include "texture.h"
#include "object2d.h"
#include "imgui_property.h"
#include "file.h"

#include "particle_manager.h"
#include "BG.h"

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
// Author YudaKaito
// 概要 : プログラム全体の初期化
//=============================================================================
HRESULT CApplication::Init(HWND hWnd, HINSTANCE hInstance)
{
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

	paticleManager = new CParticleManager;
	// パーティクル
	if (FAILED(paticleManager->Init()))
	{
		return E_FAIL;
	}
	paticleManager->Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0);
	paticleManager->Create(D3DXVECTOR3(SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.75f, 0.0f), 0);
	paticleManager->Create(D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.25f, 0.0f), 0);

	SetTex();

	// 背景
	bg = new CBg;
	if (FAILED(bg->Init()))
	{
		return E_FAIL;
	}

	imguiProperty = new CImguiProperty;

	imguiProperty->Init(hWnd, GetRenderer()->GetDevice());

	return S_OK;
}

//=============================================================================
// 終了
// Author YudaKaito
// 概要 : プログラム全体の終了
//=============================================================================
void CApplication::Uninit()
{
	// オブジェクトの解放
	CObject::ReleaseAll();

	// テクスチャの解放
	texture->UnloadAll();
	if (texture != nullptr)
	{
		texture->SavePath();
		delete texture;
		texture = nullptr;
	}

	// パーティクルマネジャーの解放
	if (paticleManager != nullptr)
	{
		paticleManager->Uninit();

		delete paticleManager;
		paticleManager = nullptr;
	}

	// レンダラーの解放
	if (renderer != nullptr)
	{
		renderer->Uninit();

		delete renderer;
		renderer = nullptr;
	}

	// imguiの解放
	if (imguiProperty != nullptr)
	{
		//imguiProperty->Uninit();

		delete imguiProperty;
		imguiProperty = nullptr;
	}

	// アプリケーションの解放
	if (application != nullptr)
	{
		delete application;
		application = nullptr;
	}
}

//=============================================================================
// 更新
// Author YudaKaito
// 概要 : プログラム全体の更新
//=============================================================================
void CApplication::Update()
{
	UpdateInput();

	renderer->Update();

	static int a;

	if (GetKeyboardTrigger(DIK_F1))
	{
		a = paticleManager->Create(D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.5f, 0.0f), 0);
	}

	if (GetKeyboardTrigger(DIK_F2))
	{
		paticleManager->Release(a);
	}

	paticleManager->Update();

	imguiProperty->Update();	// imguiの更新
}

//=============================================================================
// 描画
// Author YudaKaito
// 概要 : プログラム全体の描画
//=============================================================================
void CApplication::Draw()
{
	renderer->Draw();
}
