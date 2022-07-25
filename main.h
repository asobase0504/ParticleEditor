//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//Main.h
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

#ifndef _MAIN_H_		//二重ガードインクルード
#define _MAIN_H_

#include <d3dx9.h>

//--------------------------------------------------
//マクロ定義
//--------------------------------------------------

//--------------------------------------------------
//プロトタイプ宣言
//--------------------------------------------------
BOOL GetFile(HWND hWnd, TCHAR* fname, int nsize, TCHAR* initDir);
bool *TexUse(void);
char *GetBuffer(void);
HWND GetWnd(void);
void funcFileSave(HWND hWnd);
#endif
