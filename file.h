//============================
//
// ファイル設定ヘッター
// Author:hamada ryuuga
//
//============================
#ifndef _FILE_H_	// このマクロ定義がされてなかったら
#define _FILE_H_	// 二重インクルード防止のマクロ定義

#include "nlohmann/json.hpp"

//=====================================
// プロトタイプ宣言
//=====================================
// 制御関数
void SaveJson(nlohmann::json inJson, const std::string cUrl);
void LoadJson(const wchar_t* cUrl);
void LoadJsonTex(const char* cUrl);
void OutputStatusTex();
void SetTex();
#endif
