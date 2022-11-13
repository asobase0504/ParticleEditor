//=============================================================================
// 
// アプリケーションクラス
// Author YudaKaito
// 
//=============================================================================
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include <Windows.h>

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CRenderer;
class CObject;
class CPlayer;
class CTexture;
class CParticleManager;
class CBg;
class CImguiProperty;
class CParticleEdit;

//-----------------------------------------------------------------------------
// アプリケーションクラス
//-----------------------------------------------------------------------------
class CApplication
{
public: // 定数
	static const int SCREEN_WIDTH = 1280;	// スクリーンの幅
	static const int SCREEN_HEIGHT = 720;	// スクリーンの高さ

public: // シングルトンのインスタンス取得
	static CApplication* GetInstance();

private:
	static CApplication* application;	// アプリケーション
	CApplication();

public:
	~CApplication();

	HRESULT Init(HWND hWnd, HINSTANCE hInstance);
	void Uninit();
	void Update();
	void Draw();

	// Getter
	CRenderer* GetRenderer() { return renderer; }
	CObject** GetMyObject() { return &object; }
	CTexture* GetTextureClass() { return texture; }
	CParticleManager* GetParticleManager() { return paticleManager; }
	CParticleEdit* GetParticleEdit() { return particleEdit; }
	CImguiProperty* GetImguiProperty();

private:
	CObject* object;					// オブジェクト
	CRenderer* renderer;				// レンダラ―
	CTexture* texture;					// テクスチャ
	CParticleManager* paticleManager;	// パーティクルマネジャー
	CParticleEdit* particleEdit;		// パーティクルエディタ
	CBg* bg;							// 背景
};
#endif // !_APPLICATION_H_
