//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//particle.h
// Author : Tanaka Kouta
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
#ifndef _PARTICLE_H_		//このマクロが定義されてなかったら
#define _PARTICLE_H_		//2重インクルード防止のマクロ定義

#include <d3dx9.h>
#include "object2d.h"

//-------------------------------------------------
// パーティクルクラス
// Author : Tanaka Kouta
// Author : Yuda Kaito
// 概要 : パーティクルの処理を担当するクラス
//-------------------------------------------------
class CParticle : public CObject2D
{
public:	// 定数
public:	// 静的変数
public:	// 列挙型
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
		D3DXCOLOR colBigin;			// 始まりの色
		D3DXCOLOR colRandamMax;		// ランダムな色の範囲(最大)
		D3DXCOLOR colRandamMin;		// ランダムな色の範囲(最小)
		D3DXCOLOR colTransition;	// 色の遷移量
		D3DXCOLOR destCol;			// 目的の色
		int nEndTime;				// 遷移完了までの時間
		int nCntTransitionTime;		// 遷移カウント
		bool bColTransition;		// 遷移を行うか
		bool bColRandom;			// ランダムで色を変更するか
		bool bRandomTransitionTime;	// 遷移完了時間の乱数
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
	struct Info
	{
		D3DXVECTOR3 move;				// 移動量
		D3DXVECTOR3 moveTransition;		// 移動量の推移量
		D3DXVECTOR3 rot;				// 向き
		Color color;					// カラー
		ALPHABLENDTYPE alphaBlend;		// αブレンディングの種別
		D3DXVECTOR3 scaleTransition;	// サイズの推移量			※新規追加変数(ファイルに追加後消してください)
		float fWidth;					// 幅
		float fHeight;					// 高さ
		float fAngle;					// 角度
		float fAttenuation;				// 減衰値
		float fRadius;					// 半径
		float fWeight;					// 重さ
		float fWeightTransition;		// 重さの推移量
		float fScale;					// 全体的な大きさ
		int nLife;						// 寿命
		int nMaxLife;					// 最大寿命
		int nMoveTime;					//							※未使用変数
		int nIdxTex;					// テクスチャの番号
		bool bUseWeight;				// 重さを使用するかどうか
		bool bBackrot;					// 逆回転させるかどうか
		bool bTexturerot;				// テクスチャ回転させるかどうか
	};

public:
	CParticle();
	~CParticle();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CParticle* Create(const Info& inParticle, const D3DXVECTOR3& inPos);
	void LoadTex();

private:	// プライベート関数
	void ColorTransition();

private:	// メンバー変数
	Info m_data;
	int m_idx;
};
#endif
