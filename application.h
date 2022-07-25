//=========================================
// 
// アプリケーションクラス
// Author YudaKaito
// 
//=========================================
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <d3dx9.h>

//-----------------------------------------
// 前方宣言
//-----------------------------------------
class CRenderer;
class CObject;
class CPlayer;
class CTexture;
class CParticleManager;
class CBg;
class CImguiProperty;

//-----------------------------------------
// アプリケーションクラス
//-----------------------------------------
class CApplication
{
public: // 定数
	static const int SCREEN_WIDTH = 1280;	// スクリーンの幅
	static const int SCREEN_HEIGHT = 720;	// スクリーンの高さ

public: // シングルトンのインスタンス取得
	static CApplication* GetInstance();

private:
	CApplication();
public:
	~CApplication();

	HRESULT Init(HWND hWnd, HINSTANCE hInstance);
	void Uninit();
	void Update();
	void Draw();

	CRenderer* GetRenderer() { return renderer; }
	CObject** GetMyObject() { return &object; }
	CTexture* GetTextureClass() { return texture; }
	CParticleManager* GetParticleManager() { return paticleManager; }
	CImguiProperty* GetImguiProperty() { return imguiProperty; }
private:
	static CApplication* application;	// アプリケーション
	CObject* object;	// オブジェクト
	CRenderer* renderer;	// レンダラ―
	CTexture* texture;	// テクスチャ
	CParticleManager* paticleManager;	// パーティクルマネジャー
	CBg* bg;	// 背景
	CImguiProperty* imguiProperty;
};
#endif // !_APPLICATION_H_
