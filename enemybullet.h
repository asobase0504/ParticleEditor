#ifndef _ENEMYBULLET_H_
#define _ENEMYBULLET_H_

#include "main.h"

//マクロ定義
#define MAX_ENEMYBULLET		(512)	//弾の最大数

//弾構造体の定義
struct ENEMYBULLET
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 rotDest;	//目的の向き
	D3DXVECTOR3 vec;
	D3DXCOLOR col;			//弾の色
	float fBulletSize;		//弾の大きさ
	float fAngle;			//角度
	int nLife;				//寿命
	bool bUse;				//使用しているかどうか
};

//プロトタイプ宣言
void InitEnemyBullet(void);
void UninitEnemyBullet(void);
void UpdateEnemyBullet(void);
void DrawEnemyBullet(void);
void SetEnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move);
bool CollisionEnemyBullet(D3DXVECTOR3 *ppos, float fRadius);
ENEMYBULLET *GetEnemyBullet(void);

#endif

