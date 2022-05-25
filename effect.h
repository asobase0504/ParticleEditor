//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//effect.h
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
#ifndef _EFFECT_H_		//このマクロが定義されてなかったら
#define _EFFECT_H_		//2重インクルード防止のマクロ定義

#include "main.h"

//マクロ定義
#define MAX_EFFECT	(4096)	//エフェクトの最大数
#define NUM_EFFECT	(10)	//エフェクトの種類

//エフェクトの列挙型
enum EFFECTTYPE
{
	EFFECTTYPE_NONE = 0,
	EFFECTTYPE_NORMAL,
	EFFECTTYPE_PLAYER,
	EFFECTTYPE_MAX
};

//エフェクトの構造体の定義
struct Effect
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 move;			// 移動量
	D3DXCOLOR col;				// 色
	EFFECTTYPE type;			// エフェクトの種類
	float fWidth;				// 幅
	float fHeight;				// 高さ
	float fRadius;				// 半径
	int nLife;					// 寿命
	bool bUse;					// 使用しているか
};

//プロトタイプ宣言
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fWidth, float fHeight, EFFECTTYPE type);
void DeleteEffect(int nDelete);

#endif