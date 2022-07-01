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

//-----------------------------------------
// アプリケーションクラス
//-----------------------------------------
class CApplication
{
public: // 定数
	static const int SCREEN_WIDTH = 1280;	// スクリーンの高さ
	static const int SCREEN_HEIGHT = 720;

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
private:
	static CApplication* application;
	CObject* object;
	CRenderer* renderer;
	CTexture* texture;
	CParticleManager* paticleManager;
	CBg* bg;
};

#endif // !_APPLICATION_H_
