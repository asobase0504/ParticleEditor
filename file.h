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
#include "particle.h"


//=====================================
// プロトタイプ宣言
//=====================================
// 制御関数

FileParticleData GetStatus(void);//ゲット
void OutputStatus();
void LoodJson(const wchar_t* cUrl);

#endif
