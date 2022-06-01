//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//particle.h
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
#ifndef _PARTICLE_H_		//このマクロが定義されてなかったら
#define _PARTICLE_H_		//2重インクルード防止のマクロ定義

#include "main.h"

//マクロ定義
#define MAX_PARTICLE	(21000)	//エフェクトの最大数
#define NUM_PARTICLE	(10)		//エフェクトの種類
#define FVF_VERTEX_PARTICLE	(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_PSIZE)

//エフェクトの列挙型
enum PARTICLETYPE
{
	PARTICLETYPE_NONE = 0,
	PARTICLETYPE_NORMAL,
	PARTICLETYPE_PLAYER,
	PARTICLETYPE_MAX
};

struct ParticleTex
{
	D3DXVECTOR3 pos;
	D3DXCOLOR col;
	float size;
};

//エフェクトの構造体の定義
struct Particle
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 move;			// 移動量
	D3DXVECTOR3 moveTransition;	// 移動量の推移量
	D3DXVECTOR3 rot;			// 向き
	D3DXCOLOR col;				// 色
	D3DXCOLOR colRandamMax;		// ランダムな色の範囲(最大)
	D3DXCOLOR colRandamMin;		// ランダムな色の範囲(最小)
	D3DXCOLOR colTransition;	// 色の推移量
	PARTICLETYPE type;			// エフェクトの種類
	float fWidth;				// 幅
	float fHeight;				// 高さ
	float fAngle;				// 角度
	float fAttenuation;			// 減衰値
	float fRadius;				// 半径
	float fWeight;				// 重さ
	float fWeightTransition;	// 重さの推移量
	float fScale;
	int nLife;					// 寿命
	bool bUseWeight;			// 重さを使用するかどうか
	bool bBackrot;				// 逆回転させるかどうか
	bool bTexturerot;			// テクスチャ回転させるかどうか
	bool bUse;					// 使用しているか
};

//プロトタイプ宣言
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fWidth, float fHeight, PARTICLETYPE type);
void SetParticleImgui(Particle& inParticle);
void LoadTex(void);
void DeleteParticle(int nDelete);
void DeleteParticleAll();
void RemoveAngle(void);
Particle *GetParticle(void);
DWORD FloattoDword(float f);
#endif