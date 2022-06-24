//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//particle.h
// Author : Tanaka Kouta
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
#ifndef _PARTICLE_H_		//このマクロが定義されてなかったら
#define _PARTICLE_H_		//2重インクルード防止のマクロ定義

#include "main.h"

//マクロ定義
#define FVF_VERTEX_PARTICLE	(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_PSIZE)

//-------------------------------------------------
// パーティクルクラス
// Author : Tanaka Kouta
// Author : Yuda Kaito
// 概要 : パーティクルの処理を担当するクラス
//-------------------------------------------------
class CParticle
{
public:	// 定数
	static const int maxNumber = 21000;
	static const int numType = 10;
public:	// 静的変数
	static CParticle* g_aParticle[maxNumber];
	static float g_fAngle;	// 発射角度
public:	// 列挙型
	//エフェクトの種別
	enum PARTICLETYPE
	{
		PARTICLETYPE_NONE = 0,
		PARTICLETYPE_NORMAL,
		PARTICLETYPE_PLAYER,
		PARTICLETYPE_MAX
	};
public: // 構造体

	//-------------------------------------------------
	// テクスチャ情報
	// Author : 
	// 概要 : テクスチャに関するデータ構造体
	//-------------------------------------------------
	struct ParticleTex
	{
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
		float size;
	};

	//-------------------------------------------------
	// エフェクトのカラー構造体
	// Author : 唐﨑結斗
	// 概要 : エフェクトの色関係の構造体
	//-------------------------------------------------
	struct Color	// ※新規追加構造体(ファイルに追加後消してください)
	{
		D3DXCOLOR col;				// 色
		D3DXCOLOR colRandamMax;		// ランダムな色の範囲(最大)
		D3DXCOLOR colRandamMin;		// ランダムな色の範囲(最小)
		D3DXCOLOR colTransition;	// 色の遷移量
		D3DXCOLOR destCol;			// 目的の色
		int nEndTime;				// 遷移完了までの時間			※新規追加変数(ファイルに追加後消してください)
		int nCntTransitionTime;		// 遷移カウント					※新規追加変数(ファイルに追加後消してください)
		bool bColTransition;		// カラー遷移					※新規追加変数(ファイルに追加後消してください)
		bool bColRandom;			// ランダムカラー				※新規追加変数(ファイルに追加後消してください)
		bool bRandomTransitionTime;	// 遷移時間の乱数				※新規追加変数(ファイルに追加後消してください)
	};

	//-------------------------------------------------
	// エフェクトのαブレンドの種別の列挙型
	// Author : 唐﨑結斗
	// 概要 : 乗算、加算、減算
	//-------------------------------------------------
	enum ALPHABLENDTYPE
	{
		TYPE_ADD = 0,		// 加算
		TYPE_SUB,			// 減算
		TYPE_NONE,			// 乗算
		MAX_TYPE,			// 種別の最大数
	};

	//-------------------------------------------------
	//エフェクトの構造体の定義
	// Author : Tanaka Kouta
	// Author : Yuda Kaito
	// Author : Hamada Ryuga
	// Author : 唐﨑結斗
	//-------------------------------------------------
	struct Particle
	{
		D3DXVECTOR3 pos;			// 位置
		D3DXVECTOR3 maxPopPos;		// 発生位置(最大)			※新規追加変数(ファイルに追加後消してください)
		D3DXVECTOR3 minPopPos;		// 発生位置(最小)			※新規追加変数(ファイルに追加後消してください)
		D3DXVECTOR3 move;			// 移動量
		D3DXVECTOR3 moveTransition;	// 移動量の推移量
		D3DXVECTOR3 rot;			// 向き
		PARTICLETYPE type;			// エフェクトの種類
		Color color;				// カラー					※新規追加変数(ファイルに追加後消してください)
		ALPHABLENDTYPE alphaBlend;	// αブレンディングの種別
		float fWidth;				// 幅
		float fHeight;				// 高さ
		float fAngle;				// 角度
		float fAttenuation;			// 減衰値
		float fRadius;				// 半径
		float fWeight;				// 重さ
		float fWeightTransition;	// 重さの推移量
		float fScale;				// 全体的な大きさ
		int nLife;					// 寿命
		int nMaxLife;
    bool bUseWeight;			// 重さを使用するかどうか
		bool bBackrot;				// 逆回転させるかどうか
		bool bTexturerot;			// テクスチャ回転させるかどうか
		bool bUse;					// 使用しているか
	};

public:
	CParticle();
	~CParticle();

	HRESULT Init();
	void Uninit();
	void Update();
	static void AllUpdate();
	void Draw();
	static void AllDraw();
	static CParticle* Create(const Particle& inParticle, const D3DXVECTOR3& inPos);
	void Set(const Particle& inParticle, const D3DXVECTOR3& inPos);
	void LoadTex();
	void Delete(const int data);
	void DeleteAll();
	void RemoveAngle(void);
	DWORD FloattoDword(float fVal);

private:	// メンバー変数
	LPDIRECT3DTEXTURE9 m_pTexture[numType] = {};	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
	Particle m_data;
	int m_idx;
};
#endif