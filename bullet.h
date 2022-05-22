#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//マクロ定義
#define MAX_BULLET	(512)	//弾の最大数

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXCOLOR col;		//弾の色
	float fBulletSize;	//弾の大きさ
	int nLife;			//寿命
	bool bUse;			//使用しているかどうか
}BULLET;

//プロトタイプ宣言
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move);
bool CollisionBullet(D3DXVECTOR3 *ppos, float fRadius);
BULLET *GetBullet(void);

#endif

