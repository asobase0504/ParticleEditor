//==================================================
//
// ゲームモード
// Author : Yuda Kaito
//
//==================================================

//==================================================
// インクルード
//==================================================
// 主要系統
#include "main.h"
#include "game.h"
#include "input.h"
#include <assert.h>
// 処理系統
#include "BG.h"
#include "particle.h"

//==================================================
// 静的変数
//==================================================

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitGame(void)
{
	InitBg();		// 背景
	InitParticle();	// パーティクル
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitGame(void)
{
	UninitBg();			// 背景
	UninitParticle();	// パーティクル
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateGame(void)
{
	UpdateBg();			// 背景
	UpdateParticle();	// パーティクル
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawGame(void)
{
	DrawBg();		// 背景
	DrawParticle();	// パーティクル
}
