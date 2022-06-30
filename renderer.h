//=========================================
// 
// レンダーステートクラス
// Author YudaKaito
// 
//=========================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <d3dx9.h>

class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit();
	void Update();
	void Draw();

	// デバイスの取得
	LPDIRECT3DDEVICE9 GetDevice() { return pD3DDevice; }
private:
	LPDIRECT3D9 pD3D;				// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9 pD3DDevice;	// Deviceオブジェクト

#ifdef _DEBUG
	// フォント
	LPD3DXFONT pFont;
#endif // _DEBUG
};

#endif // !_RENDERER_H_
