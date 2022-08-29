//============================
//
// ファイル設定ヘッター
// Author:hamada ryuuga
//
//============================
#ifndef _FILE_H_	// このマクロ定義がされてなかったら
#define _FILE_H_	// 二重インクルード防止のマクロ定義

#include "nlohmann/json.hpp"

//--------------------------------------------------
// 代入する変数の型
//--------------------------------------------------
enum Type {
	UseChack,	// 配列
	Vector3,	// D3DXVECTOR3型
	Color,		// D3DXCOLOR型
	Float,		// float型
	Int,		// int型
	Bool		// bool型
};

//--------------------------------------------------
// 代入時に必要な情報
//--------------------------------------------------
struct ConvTable {
	Type type;			// 代入形態
	const char *name;	// Key
	void *store;		// 代入したい値
};

//--------------------------------------------------
// 代入する値を複数に対応する。
//--------------------------------------------------
struct DeepTable {
	std::vector<ConvTable> table;
	bool* use;
};

//=====================================
// プロトタイプ宣言
//=====================================
// 制御関数
void SaveJson(nlohmann::json inJson, const std::string cUrl);
nlohmann::json LoadJson(const wchar_t* cUrl);
void LoadJsonTex(const char* cUrl);
void OutputStatusTex();
void SetTex();
void LoadValueBundledData(nlohmann::json*, DeepTable&);
#endif
