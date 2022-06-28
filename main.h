//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//Main.h
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

#ifndef _MAIN_H_		//二重ガードインクルード
#define _MAIN_H_

//--------------------------------------------------
//インクルードファイル
//--------------------------------------------------
#include <windows.h>
#include "d3dx9.h"		//描画処理
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"
#include <d3d9.h>
#include <tchar.h>
#include <time.h>
// imgui系統
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <imgui_internal.h>

//--------------------------------------------------
//ライブラリのリンク
//--------------------------------------------------
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)
//#define FVF_VERTEX_2D			(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
//#define FVF_VERTEX_2D_TEX2		(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1|D3DFVF_TEX2)					// マルチテクスチャ用の頂点フォーマット[2D]

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void);
BOOL GetFile(HWND hWnd, TCHAR* fname, int nsize, TCHAR* initDir);
float GetAngle(void);
char GetFileName(int nNum);
bool TexUse(void);

#endif
