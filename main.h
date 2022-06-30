//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//Main.h
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

#ifndef _MAIN_H_		//二重ガードインクルード
#define _MAIN_H_

#include <d3dx9.h>

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------
#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
BOOL GetFile(HWND hWnd, TCHAR* fname, int nsize, TCHAR* initDir);
float GetAngle(void);
bool *TexUse(void);
char *GetBuffer(void);
HWND GetWnd(void);
void funcFileSave(HWND hWnd);
#endif
