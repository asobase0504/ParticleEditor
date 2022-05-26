//============================
//
// ファイル設定ヘッター
// Author:hamada ryuuga
//
//============================
#ifndef _FILE_H_			// このマクロ定義がされてなかったら
#define _FILE_H_			// 二重インクルード防止のマクロ定義

//=====================================
// インクルード
//=====================================
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include "nlohmann/json.hpp"

#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <string>
#include <iostream>
#include <windows.h>
#include <sstream>

#include "main.h"

//構造体
struct EffectData
{
	//
	D3DXVECTOR3 nPopPos;
	D3DXVECTOR3 ROT;
	D3DXVECTOR2 DIVISION;
	D3DXCOLOR COL;
	int Effecttype;
	int Alphablend_TYPE;
	int	nLife;					// 寿命
	int TEXTURE;
	int TEXTURE2;
	int TIME;
	int NUMPARTICLE;
	int MULTITEX;
	float   Radius;
	float   Speed;
	float   Weight;
	float	fRadius;				// 半径
	float	fSpeed;					// 移動速度
	float	fWeight;				// 重さ
	float	fAttenuationWeight;		// 重さの減衰量
	bool	bMultiTex;				// マルチテクスチャの使用状況
	bool	bUse;					// 使用状況
};

//=====================================
// プロトタイプ宣言
//=====================================
// 制御関数

EffectData *GetStatus(void);//ゲット
void OutputStatus();
void LoodJson(const wchar_t* cUrl);

#endif
