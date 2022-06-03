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

//-------------------------------------------------
// エフェクトのカラー構造体
// Auther : 唐﨑結斗
// 概要 : エフェクトの色関係の構造体
//-------------------------------------------------
struct ParticleColor			// ※新規追加構造体(ファイルに追加後消してください)
{
	D3DXCOLOR			col;					// 色
	D3DXCOLOR			colRandamMax;			// ランダムな色の範囲(最大)
	D3DXCOLOR			colRandamMin;			// ランダムな色の範囲(最小)
	D3DXCOLOR			colTransition;			// 色の推移量
	D3DXCOLOR			destCol;				// 目的の色
	int					nEndTime;				// 遷移完了までの時間			※新規追加変数(ファイルに追加後消してください)
	int					nCntTransitionTime;		// 遷移カウント					※新規追加変数(ファイルに追加後消してください)
	bool				bColTransition;			// カラー遷移					※新規追加変数(ファイルに追加後消してください)
	bool				bColRandom;				// ランダムカラー				※新規追加変数(ファイルに追加後消してください)
};

//-------------------------------------------------
// エフェクトのαブレンドの種別の列挙型
// Auther : 唐﨑結斗
// 概要 : 乗算、加算、減算
//-------------------------------------------------
enum ALPHABLENDTYPE 
{
	TYPE_ADD = 0,		// 加算
	TYPE_SUB,			// 減算
	TYPE_NONE,			// 乗算
	MAX_TYPE,			// 種別の最大数
};

//エフェクトの構造体の定義
struct Particle
{
	D3DXVECTOR3			pos;					// 位置
	D3DXVECTOR3			maxPopPos;				// 発生位置(最大)				※新規追加変数(ファイルに追加後消してください)
	D3DXVECTOR3			minPopPos;				// 発生位置(最小)				※新規追加変数(ファイルに追加後消してください)
	D3DXVECTOR3			move;					// 移動量
	D3DXVECTOR3			moveTransition;			// 移動量の推移量
	D3DXVECTOR3			rot;					// 向き
	PARTICLETYPE		type;					// エフェクトの種類
	ParticleColor		color;					// カラー						※新規追加変数(ファイルに追加後消してください)
	ALPHABLENDTYPE		alphaBlend;				// αブレンディングの種別
	float				fWidth;					// 幅
	float				fHeight;				// 高さ
	float				fAngle;					// 角度
	float				fAttenuation;			// 減衰値
	float				fRadius;				// 半径
	float				fWeight;				// 重さ
	float				fWeightTransition;		// 重さの推移量
	float				fScale;
	int					nLife;					// 寿命
	bool				bUseWeight;				// 重さを使用するかどうか
	bool				bBackrot;				// 逆回転させるかどうか
	bool				bTexturerot;			// テクスチャ回転させるかどうか
	bool				bUse;					// 使用しているか
};

//プロトタイプ宣言
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticleImgui(Particle& inParticle);
void LoadTex(void);
void DeleteParticle(int nDelete);
void DeleteParticleAll();
void RemoveAngle(void);
Particle *GetParticle(void);
DWORD FloattoDword(float f);
#endif