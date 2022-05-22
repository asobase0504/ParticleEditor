//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//particle.h
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
#ifndef _PARTICLE_H_		//このマクロが定義されてなかったら
#define _PARTICLE_H_		//2重インクルード防止のマクロ定義

#include "main.h"

//マクロ定義
#define MAX_PARTICLE	(4096)	//エフェクトの最大数
#define NUM_PARTICLE	(10)		//エフェクトの種類

//エフェクトの列挙型
enum PARTICLETYPE
{
	PARTICLETYPE_NONE = 0,
	PARTICLETYPE_NORMAL,
	PARTICLETYPE_PLAYER,
	PARTICLETYPE_MAX
};

//エフェクトの構造体の定義
struct Particle
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXCOLOR col;		//色
	PARTICLETYPE type;	//エフェクトの種類
	float fWidth;		//幅
	float fHeight;		//高さ
	float fAngle;		//角度
	float fRadius;		//半径
	int nLife;			//寿命
	bool bUse;			//使用しているか
};

//プロトタイプ宣言
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fWidth, float fHeight, PARTICLETYPE type);
void DeleteParticle(int nDelete);
void RemoveAngle(void);
Particle *GetParticle(void);

#endif